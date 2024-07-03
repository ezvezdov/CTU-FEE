// https://arjunsreedharan.org/post/148675821737/memory-allocators-101-write-a-simple-memory
// https://osy.pages.fel.cvut.cz/docs/prednasky/pdf/lekce07.pdf

#include "mem_alloc.h"
#include <stdio.h>
#include <string.h>

#define DEBUG 0

static inline void *nbrk(void *address);

#ifdef NOVA

/**********************************/
/* nbrk() implementation for NOVA */
/**********************************/

static inline unsigned syscall2(unsigned w0, unsigned w1)
{
    asm volatile("   mov %%esp, %%ecx    ;"
                 "   mov $1f, %%edx      ;"
                 "   sysenter            ;"
                 "1:                     ;"
                 : "+a"(w0)
                 : "S"(w1)
                 : "ecx", "edx", "memory");
    return w0;
}

static void *nbrk(void *address)
{
    return (void *)syscall2(3, (unsigned)address);
}
#else

/***********************************/
/* nbrk() implementation for Linux */
/***********************************/

#include <unistd.h>

static void *nbrk(void *address)
{
    void *current_brk = sbrk(0);
    if (address != NULL) {
        int ret = brk(address);
        if (ret == -1)
            return NULL;
    }
    return current_brk;
}

#endif

union header {
	struct {
        unsigned long size;
        unsigned is_free;
        union header *next;
	} s;
    char align[16]; //force align to 16 bytes
};
typedef union header header_t;

header_t *head, *tail;

void *nbrk_min = 0;
unsigned long min_block_size = sizeof(header_t) + 4; // minimum block size

header_t *get_free_block(unsigned long size)
{   
	header_t *cur = head;
	while(cur) {
		if (cur->s.is_free && cur->s.size >= size){
            unsigned long remainder_size = cur->s.size - size;
            if(cur->s.size == size){
                if(DEBUG) printf("Find free block with same size\n");
                return cur;
            }
            else if(remainder_size < min_block_size){
                if(DEBUG) printf("Find free block (without splitting)\n");
                return cur;
            }
            
            else{
                //Split blocks
                if(DEBUG) printf("Start blocks splitting\n");
                void *remainder_address = (void*)(cur+1) + size;

                header_t *remainder = remainder_address;
                remainder->s.is_free = 1;
                remainder->s.size = remainder_size;
                remainder->s.next = cur->s.next;
                if(tail == cur){
                    tail = remainder;
                }

                cur->s.next = remainder_address;
                cur->s.size = size;

                return cur;
            }
        }
		cur = cur->s.next;
	}
	return 0;
}

void *my_malloc(unsigned long size)
{
    if(DEBUG) printf("Start Malloc with size %ld\n",size);

    // Get minimum heap address
    if(nbrk_min == 0){
        nbrk_min = nbrk(0);
        if(DEBUG) printf("set nbrk_min = %p",nbrk_min);
    }
    
    // Round size to word size
    while (size % 4 != 0){
        if(DEBUG) printf("Allocated size was increased by 1 to round to word size\n");
        size++;
    }

    if(!size){
        if (DEBUG) printf("Malloc ERROR: !size\n");
        return (void*)0;
    }

    unsigned long total_size = size + sizeof(header_t);
    header_t *header;

    // Get free header from linked list
    header = get_free_block(size);
    if (header) {
        // Return header from linked list if it exist
        if (DEBUG) printf("Free memory was founded in linked list. Address of header is %p\n",header);
		header->s.is_free = 0;
		return (void*)(header + 1);
	}
    
    // Get current brk
    void *old_address = nbrk(0);
    if(DEBUG) printf("Current brk is %p\n", old_address);

    // Alloc new memory
    if(DEBUG) printf("Allocating new memory\n");
    old_address = nbrk(old_address + total_size);

    // Exception if memory wasn't allocated
    if(old_address == 0 || old_address == nbrk(0)){
        if(DEBUG) printf("Malloc ERROR: old_address == 0 || old_address == nbrk(0)\n");
        return (void*)0;
    }
    
    // Create new header and add it to head,tail
    header = old_address;
	header->s.size = size;
	header->s.is_free = 0;
	header->s.next = 0;
    
	if (!head){
        head = header;
    }
	if (tail){
        tail->s.next = header;
    }
		
	tail = header;

    return (void*)(header + 1);
}



int my_free(void *address)
{
    printf("Start Free %p\n",address);

    // Get minimum heap address
    if(nbrk_min == 0){
        nbrk_min = nbrk(0);
        if(DEBUG) printf("set nbrk_min = %p",nbrk_min);
    }

    header_t *header;

    if(!address){
        return 0;
    }
    if (address < nbrk_min ){
        if(DEBUG) printf("ERROR: free\n");
        return 1;
    }

    header = (header_t*)address - 1;

    // Check is block free
    if(header->s.is_free == 1){
        if(DEBUG) printf("ERROR: Current block is free\n");
        return 3;
    }
    
    header->s.is_free = 1;

    // Join free blocks
    header_t *cur, *tmp;
    cur = head;


    // Joining two blocks
    if(DEBUG) printf("Searching blocks to join!\n");
    
    while (cur){
        if(cur->s.is_free == 1 && cur->s.next && cur->s.next->s.is_free == 1){
            tmp = cur->s.next;
            cur->s.next = tmp->s.next;
            if(tmp == tail){
                tail = cur;
            }

            if(DEBUG) printf("Joining blocks %p-%p and %p-%p\n",cur,(void*)(cur+1) + cur->s.size,tmp,(void*)(tmp+1) + tmp->s.size);
            cur->s.size = cur->s.size + tmp->s.size + sizeof(header_t);
            
            void *header_align = &(tmp->align);

            // Zero old header
            memset(header_align,0x0,sizeof(header_t));
            continue;
        }
        cur = cur->s.next;
    }
    cur = head;
    
    return 0;
    
}
