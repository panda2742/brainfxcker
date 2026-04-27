#pragma once

#include <stddef.h>

#define PAGE_BITS 6
#define PAGE_SIZE (1 << PAGE_BITS)
#define PAGE_MASK (PAGE_SIZE - 1)

typedef struct {
	void	**pages;
	size_t	page_count;
	size_t	count;
	size_t	sib;
}	PagedVector;

PagedVector	*pv_create(size_t sib);
void		*pv_get(PagedVector *vec, size_t i);
void		*pv_push(PagedVector *vec, void *element);
void		pv_free(PagedVector *vec);
