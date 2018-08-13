#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"

//Textbook implicit turned explicit

char *heap_listp, *free_list;

/* Initialize lab by allocating the initial heap area. Return -1 if anything is wrong. Otherwise, return 0. */
//same as textbook
int mm_init(void){
	if((heap_listp = mem_sbrk(64)) == (void *) - 1)
	return -1;

	PUT(heap_listp, 0);
	PUT(heap_listp + (1 * WSIZE), PACK(DSIZE, 1));
	PUT(heap_listp + (2 * WSIZE), PACK(DSIZE, 1));
	PUT(heap_listp + (3 * WSIZE), PACK(0, 1));
	free_list = heap_listp + 2*WSIZE; 

	if(extend_heap(4) == NULL){ 
		return -1;
	}
	//mm_checkheap(NORMAL_VERBOSE);
	return 0;
}

/* Return a pointer to an allocated block payload of at least size bytes. The entire allocated block should lie within the heap reigion and should not overlap with any other allocated chunk */
//same as textbook
void *mm_malloc(size_t size){
	if(size == 0)
		return NULL;
	
	size_t asize;
	size_t extendsize;
	void *bp;

	if (size <= DSIZE)
		asize = 2 * DSIZE;
	else
		asize = DSIZE * ((size + DSIZE + (DSIZE - 1)) / DSIZE);

	if((bp = find_fit(asize)) != NULL){
		place(bp, asize);
		return bp;
	}

	extendsize = MAX(asize, CHUNKSIZE);
	if((bp = extend_heap(extendsize / WSIZE)) == NULL)
		return NULL;
	
	place(bp, asize);
	//mm_checkheap(NORMAL_VERBOSE);
	return bp;
} 

/* Free the block pointed to by bp. It returns nothing. This routine is only guaranteed to work when the passed pointer bp was returned by an earlier call to mm_malloc or mm_realloc and has not yet been freed */
//Same as textbook
void mm_free(void *bp){
	assert(bp >= mem_heap_lo() && bp <= mem_heap_hi());

	size_t size = GET_SIZE(HDRP(bp));
	PUT(HDRP(bp), PACK(size, 0));
	PUT(FTRP(bp), PACK(size, 0));
	coalesce(bp);
	//mm_checkheap(NORMAL_VERBOSE);
}

/* Return a pointer to an allocated region of at least size bytes */
void *mm_realloc(void *bp, size_t size){
	if(size <= 0){
		mm_free(bp);
		return NULL; 
	}
	else {
		size_t oldsize = GET_SIZE(HDRP(bp)); 
		size_t newsize = size + 2 * WSIZE;

		if(oldsize >= newsize)
			return bp;
		else{
			if(!GET_ALLOC(HDRP(NEXT_BLKP(bp)))){
				oldsize += GET_SIZE(HDRP(NEXT_BLKP(bp)));
				if(oldsize >= newsize){
					remove_chunk(NEXT_BLKP(bp)); 
					PUT(HDRP(bp), PACK(oldsize, 1)); 
					PUT(FTRP(bp), PACK(oldsize, 1)); 
					//mm_checkheap(NORMAL_VERBOSE);
					return bp; 
				}
			}
			else {  
				void *new_ptr = mm_malloc(newsize);  
				place(new_ptr, newsize);
				memcpy(new_ptr, bp, newsize); 
				mm_free(bp); 
				//mm_checkheap(NORMAL_VERBOSE);
				return new_ptr; 
			}
		}
	}
	//mm_checkheap(NORMAL_VERBOSE);
	return bp;
} 

/* for debugging only */
void mm_checkheap(int verbose_level){
	/*
	char *free_temp = free_list;
	char *free_curr = heap_listp;
	bool in_list = false;
	int num_all, num_free, fail;
	num_free = fail = 0;

	for(num_all=0; free_curr <= mem_heap_hi(); num_all++){
		printf("safe\n");
		if(GET_ALLOC(HDRP(free_curr))){
			printf("1st conditional\n");
			printf("*|* chunk passes (not free)\n");
		}
		else{
			printf("2nd conditional\n");
			while(free_temp != NULL){
				if(free_temp == free_curr){
					in_list = true;
					break;
				}
				free_temp = GET_NEXT(free_temp);
			}
			printf("out of loop\n");
			if(in_list){
				printf("*|* chunk passes (free)\n");
				num_free++;
			}
			else {
				num_free++;
				printf("*|* free chunk is not in the free_list\n");
				if(verbose_level == NORMAL_VERBOSE){
					printf("-----> Detailed information:\n");
					if(GET_PREV(free_curr) != NULL)
						printf("       prev_header size: %d\n", GET_SIZE(HDRP(GET_PREV(free_curr))));
					printf("       curr_header size: %d\n", GET_SIZE(HDRP(free_curr)));
					if(GET_NEXT(free_curr) != NULL)
						printf("       next_header size: %d\n", GET_SIZE(HDRP(GET_NEXT(free_curr))));
				}
				fail++;
			}
		}
		free_curr = (char*)free_curr + GET_SIZE(HDRP(free_curr));
		in_list = false;
		free_temp = free_list;
	}

	printf("There are %d chunks in total. %d chunks are free. %d chunks fail in the test.\n", num_all, num_free, fail);
	*/
}

/****************** Helper Functions ******************/
/* Merge memory with chunks before and after bp, if they are free and not allocated. */
//Modfied textbook code
void *coalesce(void *bp){
	size_t NEXT_ALLOC = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
	size_t PREV_ALLOC = GET_ALLOC(FTRP(PREV_BLKP(bp))) || PREV_BLKP(bp) == bp;
	size_t size = GET_SIZE(HDRP(bp));

  	if(!PREV_ALLOC){
		size += GET_SIZE(HDRP(PREV_BLKP(bp)));

		if(!NEXT_ALLOC){
			size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
			remove_chunk(NEXT_BLKP(bp));
		}

		bp = PREV_BLKP(bp);
		PUT(HDRP((bp)), PACK(size, 0));
		PUT(FTRP((bp)), PACK(size, 0));
		remove_chunk(bp);
	}
	else if(!NEXT_ALLOC){
		size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
		remove_chunk(NEXT_BLKP(bp));
		PUT(HDRP(bp), PACK(size, 0));
		PUT(FTRP(bp), PACK(size, 0));
	}

	return add_chunk(bp);
}

/* Invoked when heap is initialized and when mm_malloc is unable to find a suitable fit */
/* Round up the requested size to the nearest mutilple of 2 words and then request addition heap space from the memory system */
//same as textbook
void *extend_heap(size_t words){
	char *bp;
	size_t size;

	size = (words % 2) ? (words+1) * WSIZE : words * WSIZE;
	
	if ((long)(bp = mem_sbrk(size)) == -1){ 
		return NULL;
	}

	PUT(HDRP(bp), PACK(size, 0));
	PUT(FTRP(bp), PACK(size, 0));
	PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1));
	return coalesce(bp);
}

//first fit
void *find_fit(size_t asize){
	void *bp = free_list;

	while(bp != NULL && GET_ALLOC(HDRP(bp)) == 0){
		if (asize <= GET_SIZE(HDRP(bp))) {
			return bp;
		}
		bp = GET_NEXT(bp);
	}
	return NULL;
}

/* Place the requested block at the beginning of the free block, splitting only if the size of the remainder would equal or exceed the minimum block size */
void place(void *bp, size_t asize){
	size_t oldsize = GET_SIZE(HDRP(bp));

	if ((oldsize - asize) >= 32) 
		split(bp, asize, oldsize - asize);
	else {
		PUT(HDRP(bp), PACK(oldsize, 1));
		PUT(FTRP(bp), PACK(oldsize, 1));
		remove_chunk(bp);
	}
}

/* Divide bp into two halves, one with the desired size and the other containing what's left of the original bp */
void split(void *bp, size_t asize, size_t difference){
	PUT(HDRP(bp), PACK(asize, 1));
    PUT(FTRP(bp), PACK(asize, 1));
    remove_chunk(bp); //keep coalescing from undoing split
    
    bp = NEXT_BLKP(bp);
    PUT(HDRP(bp), PACK(difference, 0));
    PUT(FTRP(bp), PACK(difference, 0));
    coalesce(bp);
}

/* Insert chunk right before free_list */
void *add_chunk(void *bp){
	SET_NEXT(bp, free_list); 
	SET_PREV(bp, NULL);
	SET_PREV(free_list, bp); 
	return free_list=bp;
}

/* Remove chunk from free_list*/
void remove_chunk(void *bp){
	if(GET_NEXT(bp) != NULL)
		SET_PREV(GET_NEXT(bp), GET_PREV(bp));

	if(GET_PREV(bp) != NULL)
		SET_NEXT(GET_PREV(bp), GET_NEXT(bp));
	else
		free_list = GET_NEXT(bp);
}