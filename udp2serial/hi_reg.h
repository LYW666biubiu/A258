#ifndef _HIREG_H_
#define _HIREG_H_

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */
#define __IO volatile
#define unint32_t unsigned int
typedef struct
 {
    __IO unint32_t TIMERx_LOAD; 
    __IO unint32_t TIMERx_VALUE ; 
    __IO unint32_t TIMERx_CONTROL;
    __IO unint32_t TIMERx_INTCLR; 
    __IO unint32_t TIMERx_RIS; 
    __IO unint32_t TIMERx_MIS ; 
    __IO unint32_t TIMERx_BGLOAD; 
 

 } GPIO_TypeDef;

#define GPIO3 ((GPIO_TypeDef *) GPIO3_BASE)


typedef struct tag_MMAP_Node
{
    unsigned int Start_P;
    unsigned int Start_V;
    unsigned int length;
    unsigned int refcount;  /* map后的空间段的引用计数 */
    struct tag_MMAP_Node * next;
}TMMAP_Node_t;

void *memmap(unsigned int phy_addr, unsigned int size);

unsigned int set_reg(const unsigned int addr, const unsigned int value);
unsigned int get_reg(const unsigned int addr);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif //_HIREG_H_
