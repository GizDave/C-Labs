#include <stdio.h>
#include <stdbool.h>

#define NORMAL_VERBOSE 1

int mm_init(void);
void *mm_malloc(size_t size);
void mm_free(void *ptr);
void *mm_realloc(void *ptr, size_t size);
void mm_checkheap(int verbose);

/* Textbook macros with some additions for explicit list traversal */
#define WSIZE      				8
#define DSIZE      				16
#define CHUNKSIZE 		 		(1 << 12)
#define MAX(x, y) 				((x) > (y) ? (x) : (y))
#define PACK(size, alloc)  		((size) | (alloc))
#define GET(p)       			(*(unsigned int *)(p))
#define PUT(p, val)  			(*(unsigned int *)(p) = (val))
#define GET_SIZE(p)   			(GET(p) & ~0x7)
#define GET_ALLOC(p)  			(GET(p) & 0x1)
//#define SET_ALLOC(p, alloc)		(*(unsigned int *)(p) &= ~alloc)
#define HDRP(bp)  				((char *)(bp) - WSIZE)
#define FTRP(bp)  				((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)
#define NEXT_BLKP(bp)  			((char *)(bp) + GET_SIZE(HDRP(bp)))
#define PREV_BLKP(bp)  			((char *)(bp) - GET_SIZE((char *)(bp) - DSIZE))
#define GET_NEXT(bp)  			(*(char **)(bp + WSIZE))
#define GET_PREV(bp)  			(*(char **)(bp))
#define SET_NEXT(bp, new_bp) 	(GET_NEXT(bp) = new_bp)
#define SET_PREV(bp, new_bp) 	(GET_PREV(bp) = new_bp)

void *coalesce(void *bp);
void *extend_heap(size_t words);
void *find_fit(size_t asize);
void place(void *bp, size_t asize);
void split(void *bp, size_t size, size_t difference);
void *add_chunk(void *bp); 
void remove_chunk(void *bp); 