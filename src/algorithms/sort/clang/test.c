#include <stdbool.h>	// uses bool(ean) type
#include <stdlib.h>	// uses dynamic memory (de)allocation via malloc() and free()
#include <stdint.h>	// uses unsigned integers of 64-bits
#include <stdio.h>	// does logging to the console
#include <math.h>	// uses the rand() Pseudo Random Number Generator PRNG
#include <time.h>	// uses time() to seed the PRNG

#define RUNS 13
#define REPS 1024
#define iSIZE 16
#define SIZE 0x0000000000010000
#define EXEC_SANE_CHECKS false
#define WARNINGS false

void test_search();
void test_isearch();
void test_isort();
void complexity();

int main ()
{
  test_search();
  test_isearch();
  test_isort();
  complexity();
  return 0;
}


// returns true if the array is sorted (in ascending order), false otherwise
bool sorted (const uint64_t* x, int64_t const b, int64_t const e)
{
  bool sorted = true;
  int64_t const numel = (e - b);
  if ( (numel == 0) || (numel == 1) )
  {
    return sorted;
  }

  for (int64_t i = b; i != (e - 1); ++i)
  {
    if (x[i] > x[i + 1])
    {
      sorted = false;
      return sorted;
    }
  }
  return sorted;
}


// int64_t search(const uint64_t* x, int64_t b, int64_t e, uint64_t const target)
//
// Synopsis:
// Recursive implementation of the binary search algorithm.
// Returns the position of the target if it is present in the asymmetric range [b, e),
// otherwise returns the insertion position -(pos + 1).
//
// Inputs:
// x		the array
// b		beginning of the array
// e		end of the array (non-inclusive)
// target	the sought value
//
// Output:
// pos		if found the position of the target, otherwise the insertion position


int64_t search (const uint64_t* x, int64_t b, int64_t e, uint64_t const target)
{
  if (EXEC_SANE_CHECKS)
  {
    if ( !sorted(x, b, e) )
    {
      if (WARNINGS)
      {
	printf("search(): WARNING unsorted array\n");
      }
      return ( (int64_t) 0x8000000000000000);
    }
  }

  uint64_t const tgt = target;
  while (b < e)
  {
    int64_t const m = b + (e - b) / 2;
    if (x[m] == tgt)
    {
      int64_t const pos = m;
      return pos;
    }
    else if (x[m] < tgt)
    {
      b = (m + 1);
      return search(x, b, e, tgt);
    }
    else
    {
      e = m;
      return search(x, b, e, tgt);
    }
  }
  int64_t const pos = -(b + 1);
  return pos;
}


// int64_t isearch(const uint64_t* x, int64_t b, int64_t e, uint64_t const target)
//
// Synopsis:
// Iterative implementation of the binary search algorithm.
// Returns the position of the target if it is present in the asymmetric range [b, e),
// otherwise returns the insertion position -(pos + 1).
//
// Inputs:
// x		the array
// b		beginning of the array
// e		end of the array (non-inclusive)
// target	the sought value
//
// Output:
// pos		if found the position of the target, otherwise the insertion position


int64_t isearch (const uint64_t* x, int64_t b, int64_t e, uint64_t const target)
{
  if (EXEC_SANE_CHECKS)
  {
    if ( !sorted(x, b, e) )
    {
      if (WARNINGS)
      {
	printf("search(): WARNING unsorted array\n");
      }
      return ( (int64_t) 0x8000000000000000);
    }
  }

  uint64_t const tgt = target;
  while (b < e)
  {
    int64_t const m = b + (e - b) / 2;
    if (x[m] == tgt)
    {
      int64_t const pos = m;
      return pos;
    }
    else if (x[m] < tgt)
    {
      b = (m + 1);
    }
    else
    {
      e = m;
    }
  }
  int64_t const pos = -(b + 1);
  return pos;
}


// shifts `numel' elements to the right from `inloc' position
void shift (uint64_t* x, int64_t const numel, int64_t const inloc)
{
  for (int64_t j = 0; j != numel; ++j)
  {
    int64_t const k = inloc + ( numel - (j + 1) );
    x[k + 1] = x[k];
  }
}


// implements insertion sort (optimizations: gcc can vectorize the innermost for-loop)
void isort (uint64_t* x, size_t const size)
{
  // loop-invariant: elements in the asymmetric range [0, i) are sorted
  for (size_t i = 1; i != size; ++i)
  {
    int64_t const idx = i;				// casts index explicitly
    uint64_t const inelem = x[i];			// gets the insertion element
    int64_t const l = isearch(x, 0, idx, inelem);	// gets the target location
    int64_t const inloc = (l < 0)? -(l + 1) : (l + 1);	// sets the insertion location
    int64_t const numel = (idx - inloc);		// gets the #elements to shift
    shift(x, numel, inloc);				// shifts to make inloc available
    x[inloc] = inelem;					// inserts at designated location
  }
}


void iota (uint64_t* x, size_t const size, uint64_t const start, uint64_t const stride)
{
  for (size_t i = 0; i != size; ++i)
  {
    x[i] = start + stride * i;
  }
}


// fills the array with pseudo-random numbers
void prns (uint64_t* x, size_t const size)
{
  for (size_t i = 0; i != size; ++i)
  {
    x[i] = rand();
  }
}


// initializes the array `x'
uint64_t* create (size_t const size)
{
  if (size >= 0x7fffffffffffffff)
  {
    printf("create(): reserved values\n");
    return NULL;
  }

  size_t const bytes = size * sizeof(uint64_t);
  uint64_t* x = malloc(bytes);
  if (x == NULL)
  {
    return NULL;
  }

  uint64_t const start = 2;
  uint64_t const stride = 2;
  iota(x, size, start, stride);

  return x;
}


// destroys the array `x' (frees it from memory)
uint64_t* destroy (uint64_t* x)
{
  if (x == NULL)
  {
    return x;
  }

  free(x);
  x = NULL;
  return x;
}


// gets the elapsed-time from the time difference in nanoseconds
double getElapsedTime (const struct timespec* b, const struct timespec* e)
{
  double begin = ( (double) (b -> tv_nsec) ) + 1.0e9 * ( (double) (b -> tv_sec) );
  double end   = ( (double) (e -> tv_nsec) ) + 1.0e9 * ( (double) (e -> tv_sec) );
  return (end - begin);
}


// exports the average runtime of isort() as a function of the input size
void complexity ()
{
  struct timespec* begin = malloc( sizeof(struct timespec) );
  if (begin == NULL)
  {
    printf("complexity(): failed to allocate memory for the timespec struct\n");
    return;
  }

  struct timespec* end = malloc( sizeof(struct timespec) );
  if (end == NULL)
  {
    free(begin);
    begin = NULL;
    printf("complexity(): failed to allocate memory for the timespec struct\n");
    return;
  }

  bool failed = false;
  srand( time(NULL) );
  size_t size = iSIZE;
  double etimes[REPS];
  for (size_t run = 0; run != RUNS; ++run)
  {
    uint64_t* x = create(size);
    if (x == NULL)
    {
      free(begin);
      free(end);
      begin = NULL;
      end = NULL;
      printf("complexity(): failed to allocate memory for the array `x'\n");
      return;
    }

    double etime = 0;
    for (size_t rep = 0; rep != REPS; ++rep)
    {
      prns(x, size);

      clock_gettime(CLOCK_MONOTONIC_RAW, begin);
      isort(x, size);
      clock_gettime(CLOCK_MONOTONIC_RAW, end);

      if ( !sorted(x, 0, size) )
      {
	failed = true;
	printf("complexity(): isort() implementation failed\n");
	break;
      }

      etime += getElapsedTime(begin, end);
    }

    if (failed)
    {
      x = destroy(x);
      break;
    }

    etimes[run] = etime / ( (double) REPS );

    size *= 2;
    x = destroy(x);
  }

  printf("test-isort[2]: ");
  if (failed)
  {
    printf("FAIL\n");
  }
  else
  {
    printf("PASS\n");
  }

  if (failed)
  {
    free(begin);
    free(end);
    begin = NULL;
    end = NULL;
    return;
  }

  const char fname[] = "complexity.txt";
  FILE* file = fopen(fname, "w");
  if (file == NULL)
  {
    free(begin);
    free(end);
    begin = NULL;
    end = NULL;
    printf("complexity(): IO ERROR with file %s\n", fname);
    return;
  }

  size = iSIZE;
  for (size_t run = 0; run != RUNS; ++run)
  {
    fprintf(file, "%16lu %.16e\n", size, etimes[run]);
    size *= 2;
  }

  fclose(file);
}


void test_search ()
{
  size_t const size = 256;
  uint64_t* x = create(size);
  if (x == NULL)
  {
    return;
  }

  size_t fails = 0;
  int64_t const numel = size;
  for (int64_t i = 0; i != numel; ++i)
  {
    uint64_t const tgt = x[i];
    int64_t const pos = search(x, 0, numel, tgt);
    if (pos != i)
    {
      ++fails;
    }
  }

  printf("test-search[0]: ");
  if (fails != 0)
  {
    printf("FAIL\n");
  }
  else
  {
    printf("PASS\n");
  }

  for (int64_t i = 0; i != numel; ++i)
  {
    uint64_t const tgt = (2 * i + 1);
    int64_t const pos = search(x, 0, numel, tgt);
    if ( -(pos + 1) != i )
    {
      ++fails;
    }
  }
 
  printf("test-search[1]: ");
  if (fails != 0)
  {
    printf("FAIL\n");
  }
  else
  {
    printf("PASS\n");
  }

  if (EXEC_SANE_CHECKS)
  {
    iota(x, size, -1, -2);

    size_t fails = 0;
    for (int64_t i = 0; i != numel; ++i)
    {
      uint64_t const tgt = i;
      int64_t const pos = isearch(x, 0, numel, tgt);
      if (pos != ( (int64_t) 0x8000000000000000 ) )
      {
	++fails;
      }
    }

    printf("test-search[2]: ");
    if (fails != 0)
    {
      printf("FAIL\n");
    }
    else
    {
      printf("PASS\n");
    }
  }

  x = destroy(x);
}


void test_isearch ()
{
  size_t const size = 256;
  uint64_t* x = create(size);
  if (x == NULL)
  {
    return;
  }

  size_t fails = 0;
  int64_t const numel = size;
  for (int64_t i = 0; i != numel; ++i)
  {
    uint64_t const tgt = x[i];
    int64_t const pos = isearch(x, 0, numel, tgt);
    if (pos != i)
    {
      ++fails;
    }
  }

  printf("test-isearch[0]: ");
  if (fails != 0)
  {
    printf("FAIL\n");
  }
  else
  {
    printf("PASS\n");
  }

  for (int64_t i = 0; i != numel; ++i)
  {
    uint64_t const tgt = (2 * i + 1);
    int64_t const pos = isearch(x, 0, numel, tgt);
    if ( -(pos + 1) != i )
    {
      ++fails;
    }
  }
 
  printf("test-isearch[1]: ");
  if (fails != 0)
  {
    printf("FAIL\n");
  }
  else
  {
    printf("PASS\n");
  }

  if (EXEC_SANE_CHECKS)
  {
    iota(x, size, -1, -2);

    size_t fails = 0;
    int64_t const numel = size;
    for (int64_t i = 0; i != numel; ++i)
    {
      uint64_t const tgt = i;
      int64_t const pos = isearch(x, 0, numel, tgt);
      if (pos != ( (int64_t) 0x8000000000000000 ) )
      {
	++fails;
      }
    }

    printf("test-isearch[2]: ");
    if (fails != 0)
    {
      printf("FAIL\n");
    }
    else
    {
      printf("PASS\n");
    }
  }

  x = destroy(x);
}


void test_isort ()
{
  bool failed = false;
  for (size_t size = 16; size != SIZE; size *= 2)
  {
    uint64_t* x = create(size);
    if (x == NULL)
    {
      return;
    }
    isort(x, size);
    if ( !sorted(x, 0, size) )
    {
      failed = true;
      x = destroy(x);
      break;
    }
    x = destroy(x);
  }

  printf("test-isort[0]: ");
  if (failed)
  {
    printf("FAIL\n");
  }
  else
  {
    printf("PASS\n");
  }

  failed = false;
  for (size_t size = 16; size != SIZE; size *= 2)
  {
    uint64_t* x = create(size);
    if (x == NULL)
    {
      return;
    }
    srand( time(NULL) );
    prns(x, size);
    isort(x, size);
    if ( !sorted(x, 0, size) )
    {
      failed = true;
      x = destroy(x);
      break;
    }
    x = destroy(x);
  }

  printf("test-isort[1]: ");
  if (failed)
  {
    printf("FAIL\n");
  }
  else
  {
    printf("PASS\n");
  }
}


/*

Algorithms							August 04, 2023

source: test.c
author: @misael-diaz

Synopsis:
Implements (some) classic algorithms.

Copyright (c) 2023 Misael Diaz-Maldonado
This file is released under the GNU General Public License as published
by the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

References:
[0] A Koenig and B Moo, Accelerated C++ Practical Programming by Example.
[1] JJ McConnell, Analysis of Algorithms, second edition.

*/
