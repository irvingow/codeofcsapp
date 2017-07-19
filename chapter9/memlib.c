//private global variables
static char *mem_heap;      //points to fisrt byte of heap.
static char *mem_brk;       //points to last byte of heap plus 1
static char *mem_max_addr;  //max legal heap addr plus 1


/*
 * mem_init - initialize the memory system model.
 * */
void mem_init(void)
{
	mem_heap = (char *)Malloc(MAX_HEAP);
	mem_brk = (char *)mem_heap;
	mem_max_addr = (char *)(mem_heap + MAX_HEAP);
}

/*
 * mem_sbrk - simple model of the sbrk function. Extends the heap 
 *		by incr bytes and returns the start address of the new area. In 
 *		this model, the heap can not be shrunk.
 * */
void *mem_sbrk(int incr)
{
	char *old_brk = mem_brk;

	if((incr < 0) || ((mem_brk + incr) > mem_max_addr))
	{
		errno = ENOMEM;
		fprintf(stderr,"ERROR: mem_sbrk failed. Ran out of memory...\n");
		return (void *)-1;
	}
	mem_brk += incr;
	return (void *)old_brk;
}


