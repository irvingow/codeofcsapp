//basic constants and macros
#define WSIZE 4              //word and header/footer size (bytes)
#define DSIZE 8				 //double word size (bytes)
#define CHUNKSIZE (1<<12)	 //extend heap by this amount (bytes)

#define MAX(x,y) ((x) > (y) ? (x) : (y))

//pack a size and allocated bit into a word.
#define PACK(size, alloc) ((size) | (alloc))

//read and write a word at address p
//注意强制类型转换,需要先把p转换位指向unsigned int的指针,再解引用
#define GET(p)        (*(unsigned int *)(p))
#define PUT(p,val)    (*(unsigned int *)(p) = (val))

//read the size and allocated fields from address p
//~0x7的意思是忽略低三位,其他位都是1,&之后还是本身,获得了当前快的大小
#define GET_SIZE(p) (GET(p) & ~0x7)
//只保留了最低位,最低位代表是否被分配
#define GET_ALLOC(p) (GET(p) & 0x1)

//given block ptr bp, compute address of its header and footer 
#define HDRP(bp) ((char *)(bp) - WSIZE)
//获得块的大小之后注意要减去8字节,由于整个块的大小包括头部和脚部,共8个字节
#define FTRP(bp) ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)

//given block ptr bp, compute address of next and previous blocks
#define NEXT_BLKP(bp) ((char *)(bp) + GET_SIZE(((char *)(bp) - WSIZE)))
//利用上个块的脚部来确定上个块的大小,直接用当前块的指针减去这个大小就OK了,头部脚部都已经被考虑进去了
#define PREV_BLKP(bp) ((char *)(bp) - GET_SIZE(((char *)(bp) - DSIZE)))

//参数asize标示需要的字节数(已经包括了开销字节)
static void *find_fit(size_t asize)
{
	void *temp = heap_listp;
	temp += DSIZE;
	while(GET_SIZE(HDRP(temp)) >= asize && GET_ALLOC(HDRP(temp)) == 0)
	{
		return (void *)temp;
	}
	return NULL;
}


static void place(void *bp, size_t asize)
{
	if(GET_SIZE(HDRP(bp)) - asize >= 16)
	{
		//多余的字节满足最小块的要求,需要进行分割
		PUT(HDRP(bp),PACK(asize,1));
		PUT(FTRP(bp),PACK(asize,1));
		PUT(HDRP(NEXT_BLKP(bp)),PACK(GET_SIZE(HDRP(bp)) - asize, 0));
		PUT(FTRP(NEXT_BLKP(bp)),PACK(GET_SIZE(HDRP(bp)) - asize, 0));
	}
	else{
		PUT(HDRP(bp),PACK(GET_SIZE(HDRP(bp)) - asize, 1));
		PUT(FTRP(bp),PACK(GET_SIZE(HDRP(bp)) - asize, 1));
	}
}

static void *coalesce(void *bp)
{
	size_t prev_alloc = GET_ALLOC(FTRP(PREV_BLKP(bp)));
	size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
	size_t size = GET_SIZE(HDRP(bp));

	if(prev_alloc && next_alloc){						//第一种情况,前后块都是已分配的,直接返回
		return bp;
	}
	else if(prev_alloc && !next_alloc){					//第二种情况,只有后面的块没被分配,合并后面的,主要是改变下块的size,然后修改下
														//头部和脚部的位置及内容
		size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
		PUT(HDRP(bp),PACK(size,0));
		PUT(FTRP(bp),PACK(size,0));
	}
	else if!(prev_alloc && next_alloc){					//第三种情况,只有前面的块没被分配,合并前面的
		size + GET_SIZE(FTRP(PREV_BLKP(bp)));
		PUT(FTRP(bp),PACK(size,0));
		PUT(HDRP(PREV_BLKP(bp)),PACK(size,0));
		bp = PREV_BLKP(bp);
	}
	else{												//第四种情况,前后都是未分配的,全部合并
		size += (GET_SIZE(HDRP(NEXT_BLKP(bp))) + GET_SIZE(HDRP(PREV_BLKP(bp))));
		PUT(HDRP(PREV_BLKP(bp)),PACK(size,0));
		PUT(HDRP(NEXT_BLKP(bp)),PACK(size,0));
		bp = PREV_BLKP(bp);
	}
}

static void *extend_heap(size_t words)
{
	char *bp;
	size_t size;

	//allocate an even number of words to maintain alignment 
	//使分配字节满足双字对齐
	size = (words % 2) ? (words + 1) * WSIZE : words * WSIZE;
	if((long) (bp = mem_sbrk(size)) == -1)
		return NULL;

	//initialize free block header/footer and epilogue header 
	//把原先的结尾块变为新分配块的头部
	PUT(HDRP(bp), PACK(size,0));						//free block header 
	PUT(FTRP(bp), PACK(size,0));						//free block footer 
	//创建结尾块,为一个大小为零的已分配的块,只由一个头部组成
	PUT(HDRP(NEXT_BLKP(bp)),PACK(0,1));					//new epilogue header 

	//coalesce if the previous block was free 
	return coalesce(bp);
}



int mm_init(void)
{
	//create the initial empty heap 
	if((heap_listp = mem_sbrk(4*WSIZE)) == (void *)-1)
		return -1;
	PUT(heap_listp,0);									//alignment padding 双字边界对齐的不使用的填充字
	PUT(heap_listp + (1*WSIZE), PACK(DSIZE,1));			//prologue header 序言块头部
	PUT(heap_listp + (2*WSIZE), PACK(DSIZE,1));			//prologue footer 序言块脚部 序言块在初始化时创建,并且永不释放
	PUT(heap_listp + (3*WSIZE), PACK(0,1));				//epilogue header 结尾块,大小为零的已分配块
	heap_listp += (2*WSIZE);							//作为一个小的优化,让这个全局变量head_listp指向序言块的脚部

	//extend the empty heap with a free block of CHUNKSIZE bytes 
	if(extend_heap(CHUNKSIZE / WSIZE) == NULL)
	   return -1;
	return 0;	
}

void mm_free(void *bp)
{
	size_t size = GET_SIZE(HDRP(bp));

	PUT(HDRP(bp),PACK(size,0));
	PUT(FTRP(bp),PACK(size,0));
	coalesce(bp);
}

void *mm_malloc(size_t size)
{
	size_t asize;			//adjust block size 
	size_t extendsize;		//amount to extend heap if not fit
	char *bp;

	//ignore spurious requests
	if(size == 0)
		return NULL;

	//adjust block size to include overhead and alignment reqs.
	if(size <= DSIZE)
		asize = 2*DSIZE;
	else 
		asize = DSIZE * ((size + DSIZE + DSIZE -1) / DSIZE);

	//search the free list for a fit 
	if((bp = find_fit(asize)) != NULL)
	{
		place(bp,asize);
		return bp;
	}

	//no fit found. get more memory and place the block 
	extendsize = MAX(asize,CHUNKSIZE);
	if((bp = extend_heap(extendsize / WSIZE)) == NULL)
		return NULL;
	place(bp,asize);
	return bp;
}
