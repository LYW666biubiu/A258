#ifndef HI_MPP_H
#define HI_MPP_H

#include "hi_type.h"
#include "hi_common.h"
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#ifndef __31DV200__
#include "hi_comm_sys.h"
#include "mpi_sys.h"

HI_S32 Hi_SetReg(HI_U32 u32Addr, HI_U32 u32Value);
HI_S32 Hi_GetReg(HI_U32 u32Addr, HI_U32 *pu32Value);
#else
#define MAP_FAILED	((void *) -1)

#define PROT_READ	0x1		/* Page can be read.  */
#define PROT_WRITE	0x2		/* Page can be written.  */
#define PROT_EXEC	0x4		/* Page can be executed.  */
#define PROT_NONE	0x0		/* Page can not be accessed.  */
#define PROT_GROWSDOWN	0x01000000	/* Extend change to start of
                       growsdown vma (mprotect only).  */
#define PROT_GROWSUP	0x02000000	/* Extend change to start of
                       growsup vma (mprotect only).  */
#define MAP_SHARED	0x01		/* Share changes.  */
#define MAP_PRIVATE	0x02		/* Changes are private.  */
#define Hi_SetReg   hi_sys_set_reg
#define Hi_GetReg   hi_sys_get_reg
unsigned int hi_sys_get_reg(hi_u64 addr, unsigned int *value);
hi_s32 hi_sys_set_reg(hi_u64 addr, hi_u32 value);
#endif

#endif // HI_MPP_H
