#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>

#include "hi_reg.h"

#define PAGE_SIZE       0x1000
#define PAGE_SIZE_MASK  0xfffff000
#define DEFAULT_MD_LEN  128

TMMAP_Node_t *pTMMAPNode = NULL;

unsigned int set_reg(const unsigned int addr, const unsigned int value)
{
    void *pMem = NULL;

    pMem = memmap(addr, DEFAULT_MD_LEN);
    *(unsigned int*)pMem = value;

    return *(unsigned int*)pMem;
}

unsigned int get_reg(const unsigned int addr)
{
    return *(unsigned int*)memmap(addr, DEFAULT_MD_LEN);
}

void *memmap(unsigned int phy_addr, unsigned int size)
{
    static int fd = -1;
    static const char dev[]="/dev/mem";

    unsigned int phy_addr_in_page;
    unsigned int page_diff;
    unsigned int size_in_page;

    TMMAP_Node_t * pTmp;
    TMMAP_Node_t * pNew;

    void *addr=NULL;

    if(size == 0)
    {
        printf("memmap():size can't be zero!\n");
        return NULL;
    }

    /* check if the physical memory space have been mmaped */
    pTmp = pTMMAPNode;
    while(pTmp != NULL)
    {
        if( (phy_addr >= pTmp->Start_P) &&
            ( (phy_addr + size) <= (pTmp->Start_P + pTmp->length) ) )
        {
            pTmp->refcount++;   /* referrence count increase by 1  */
            return (void *)(pTmp->Start_V + phy_addr - pTmp->Start_P);
        }

        pTmp = pTmp->next;
    }

    /* not mmaped yet */
    if(fd < 0)
    {
        /* dev not opened yet, so open it */
        fd = open (dev, O_RDWR | O_SYNC);
        if (fd < 0)
        {
            printf("memmap():open %s error!\n", dev);
            return NULL;
        }
    }

    /* addr align in page_size(4K) */
    phy_addr_in_page = phy_addr & PAGE_SIZE_MASK;
    page_diff = phy_addr - phy_addr_in_page;

    /* size in page_size */
    size_in_page =((size + page_diff - 1) & PAGE_SIZE_MASK) + PAGE_SIZE;

    addr = mmap ((void *)0, size_in_page, PROT_READ|PROT_WRITE, MAP_SHARED, fd, phy_addr_in_page);
    if (addr == MAP_FAILED)
    {
        printf("memmap():mmap @ 0x%x error!\n", phy_addr_in_page);
        return NULL;
    }

    /* add this mmap to MMAP Node */
    pNew = (TMMAP_Node_t *)malloc(sizeof(TMMAP_Node_t));
    if(NULL == pNew)
    {
        printf("memmap():malloc new node failed!\n");
        return NULL;
    }
    pNew->Start_P = phy_addr_in_page;
    pNew->Start_V = (unsigned int)addr;
    pNew->length = size_in_page;
    pNew->refcount = 1;
    pNew->next = NULL;

    if(pTMMAPNode == NULL)
    {
        pTMMAPNode = pNew;
    }
    else
    {
        pTmp = pTMMAPNode;
        while(pTmp->next != NULL)
        {
            pTmp = pTmp->next;
        }

        pTmp->next = pNew;
    }

    return (void *)((unsigned int)addr+page_diff);
}
