#include "hi_mpp.h"

#ifndef __31DV200__
HI_S32 Hi_SetReg(HI_U32 u32Addr, HI_U32 u32Value)
{
    HI_U32 *pu32Addr = NULL;
    HI_U32 u32MapLen = sizeof(u32Value);

    pu32Addr = (HI_U32 *)HI_MPI_SYS_Mmap(u32Addr, u32MapLen);
    if(NULL == pu32Addr)
    {
        return HI_FAILURE;
    }

    *pu32Addr = u32Value;

    return HI_MPI_SYS_Munmap(pu32Addr, u32MapLen);
}

HI_S32 Hi_GetReg(HI_U32 u32Addr, HI_U32 *pu32Value)
{
    HI_U32 *pu32Addr = NULL;
    HI_U32 u32MapLen;

    if (NULL == pu32Value)
    {
        return HI_ERR_SYS_NULL_PTR;
    }

    u32MapLen = sizeof(*pu32Value);
    pu32Addr = (HI_U32 *)HI_MPI_SYS_Mmap(u32Addr, u32MapLen);
    if(NULL == pu32Addr)
    {
        return HI_FAILURE;
    }

    *pu32Value = *pu32Addr;

    return HI_MPI_SYS_Munmap(pu32Addr, u32MapLen);
}

#else

static hi_s32 s_sample_mem_dev = -1;
#define SAMPLE_MEM_DEV_OPEN if (s_sample_mem_dev <= 0)\
{\
    s_sample_mem_dev = open("/dev/mem", O_RDWR|O_SYNC);\
    if (s_sample_mem_dev < 0)\
    {\
        perror("open dev/mem error");\
        return NULL;\
    }\
}\


void * hi_sys_io_mmap(hi_u64 phy_addr, hi_u32 size)
{
    hi_u32 diff;
    hi_u64 page_phy;
    hi_u8 * page_addr;
    hi_ulong    ul_page_size;

    SAMPLE_MEM_DEV_OPEN;

    /**********************************************************
    page_size will be 0 when u32size is 0 and diff is 0,
    and then mmap will be error (error: invalid argument)
    ***********************************************************/
    if (!size)
    {
        printf("func: %s size can't be 0.\n", __FUNCTION__);
        return NULL;
    }

    /* the mmap address should align with page */
    page_phy = phy_addr & 0xfffffffffffff000ULL;
    diff    = phy_addr - page_phy;

    /* the mmap size shuld be mutliples of 1024 */
    ul_page_size = ((size + diff - 1) & 0xfffff000UL) + 0x1000;

    page_addr    = (hi_u8 *)mmap ((void *)0, ul_page_size, PROT_READ|PROT_WRITE,
                                    MAP_SHARED, s_sample_mem_dev, page_phy);
    if (MAP_FAILED == page_addr )
    {
        perror("mmap error");
        return NULL;
    }
    return (hi_void *) (page_addr + diff);
}


hi_s32 sample_sys_munmap(hi_void* vir_addr, hi_u32 size)
{
    hi_u64 page_addr;
    hi_u32 page_size;
    hi_u32 diff;

    page_addr = (((hi_ulong)vir_addr) & 0xfffffffffffff000ULL);
    diff     = (hi_ulong)vir_addr - page_addr;
    page_size = ((size + diff - 1) & 0xfffff000UL) + 0x1000;

    return munmap((hi_void*)(hi_ulong)page_addr, page_size);
}



hi_s32 hi_sys_set_reg(hi_u64 addr, hi_u32 value)
{
    hi_u32 *reg_addr = NULL;
    hi_u32 map_len = sizeof(value);

    reg_addr = (hi_u32 *)hi_sys_io_mmap(addr, map_len);
    if(NULL == reg_addr)
    {
        return HI_FAILURE;
    }

    *reg_addr = value;

    return sample_sys_munmap(reg_addr, map_len);
}

unsigned int hi_sys_get_reg(hi_u64 addr, unsigned int *value)
{
    unsigned int  *reg_addr = NULL;
    unsigned int map_len;

    if (NULL == value)
    {
        return HI_FAILURE;
    }

    map_len = sizeof(*value);
    reg_addr = (unsigned int *)hi_sys_io_mmap(addr, map_len);
    if(NULL == reg_addr)
    {
        return HI_FAILURE;
    }

    *value = *reg_addr;

    return sample_sys_munmap(reg_addr, map_len);
}

#endif
