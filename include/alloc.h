/* by Scarlett. public domain. */
#ifndef GC_ALLOC_H
#define GC_ALLOC_H
#include <gc.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

static inline size_t
z_mult_no_oflow_(size_t n, size_t size)
{
	if (size != 0 && n > SIZE_MAX / size) {
		fprintf(stderr,
		    "w3m: overflow in malloc, %zu*%zu\n", n, size);
		exit(1);
	}
	return n * size;
}

#define New(type) \
	(GC_MALLOC(sizeof(type)))

#define NewAtom(type) \
	(GC_MALLOC_ATOMIC(sizeof(type)))

#define New_N(type,n) \
	(GC_MALLOC(z_mult_no_oflow_((n), sizeof(type))))

#define NewAtom_N(type,n) \
	(GC_MALLOC_ATOMIC(z_mult_no_oflow_((n), sizeof(type))))

#define New_Reuse(type,ptr,n) \
	(GC_REALLOC((ptr), z_mult_no_oflow_((n), sizeof(type))))

#endif /* GC_ALLOC_H */
