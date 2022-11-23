
#ifndef METAL_API_H
#define METAL_API_H

#include "metal/io.h"

#ifndef TRUE
#define TRUE		1U
#endif

#ifndef FALSE
#define FALSE		0U
#endif

#ifndef NULL
#define NULL		0U
#endif

#ifndef xil_printf
#define xil_printf printf
#endif

/* memory synchronization operations */

/* Instruction Synchronization Barrier */
#define isb() __asm__ __volatile__ ("isb sy")

/* Data Synchronization Barrier */
#define dsb() __asm__ __volatile__("dsb sy")

/* Data Memory Barrier */
#define dmb() __asm__ __volatile__("dmb sy")

#if defined __GNUC__
#if defined (__MICROBLAZE__)
#  define INST_SYNC		mbar(0)
#  define DATA_SYNC		mbar(1)
# else
#  define SYNCHRONIZE_IO	dmb()
#  define INST_SYNC		isb()
#  define DATA_SYNC		dsb()
# endif
#else
# define SYNCHRONIZE_IO
# define INST_SYNC
# define DATA_SYNC
# define INST_SYNC
# define DATA_SYNC
#endif

/**
 * @brief   Returns 0-31 bits of a number .
 * @param   n : Number being accessed.
 * @return  Bits 0-31 of number.
 */
#if defined (__aarch64__) || defined (__arch64__)
#define LEFT_SHIFT_BY_32_BITS(n) (u64)(((u64)n) << 32)
#else
#define LEFT_SHIFT_BY_32_BITS(n) 0U
#endif

/**
 * @brief  Returns 32-63 bits of a number.
 * @param  n : Number being accessed.
 * @return Bits 32-63 of number.
 *
 * @note    A basic shift-right of a 64- or 32-bit quantity.
 *          Use this to suppress the "right shift count >= width of type"
 *          warning when that quantity is 32-bits.
 */
#if defined (__aarch64__) || defined (__arch64__)
#define UPPER_32_BITS(n) ((u32)(((n) >> 16) >> 16))
#else
#define UPPER_32_BITS(n) 0U
#endif
/**
 * @brief  Returns 0-31 bits of a number
 * @param  n : Number being accessed.
 * @return Bits 0-31 of number
 */
#define LOWER_32_BITS(n) ((u32)(n))


typedef __u32 u32;
typedef __u16 u16;
typedef __u8 u8;
typedef __s32 s32;
typedef __s16 s16;
typedef __u64 u64;
typedef __s64 s64;
typedef __s8 s8;
typedef u64 UINTPTR;

#define XIL_COMPONENT_IS_READY     0x11111111U  /**< In device drivers, This macro will be
                                                 assigend to "IsReady" member of driver
												 instance to indicate that driver
												 instance is initialized and ready to use. */
#define XIL_COMPONENT_IS_STARTED   0x22222222U  /**< In device drivers, This macro will be assigend to
                                                 "IsStarted" member of driver instance
												 to indicate that driver instance is
												 started and it can be enabled. */

typedef struct {

    char signature[NAME_MAX];
    char compatible[NAME_MAX];
    char platform_dir[NAME_MAX];
    char property[NAME_MAX];
    char name[NAME_MAX];
} Xmetal_dev_parm_t;

s32 X_GetDevice(Xmetal_dev_parm_t *parm);

void Xil_AssertNonvoid(int Expression);
void Xil_AssertVoid(int Expression);
void Xil_AssertNonvoidAlways();
void Xil_AssertVoidAlways();


#endif /*METAL_API_H*/