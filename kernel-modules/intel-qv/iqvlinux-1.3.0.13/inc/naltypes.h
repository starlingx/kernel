
#ifndef _NALTYPES_H_
#define _NALTYPES_H_ 

#if !defined(__KERNEL__) && !defined(NAL_DRIVER) && !defined(NAL_EFI2) && !defined(NAL_SOLARIS)
#include <stdbool.h>
#endif

#if defined(NAL_EFI2)

#include <Uefi.h>
#if defined(EFI264E)
#include <Include/X64/ProcessorBind.h>
#elif defined(EFI2AARCH64)
#include <Include/AARCH64/ProcessorBind.h>
#endif
#include <stdint.h>

typedef VOID KVOID;
typedef UINT16 PORT_ADDR;
typedef UINT64 NAL_PHYSICAL_ADDRESS;
typedef char CHAR;
typedef CHAR16 WCHAR;
typedef UINT32 NAL_STATUS;

typedef volatile UINT32 NAL_SPIN_LOCK;

#define NAL_MAX_INTERRUPT_VECTORS 256

typedef struct _NAL_VECTOR
{
    volatile BOOLEAN NalInterruptVector;
    UINT32 ReferenceCount;
} NAL_VECTOR;

#if defined(NAL_DEBUG)
#ifndef ASSERT
#define ASSERT(X) 
#endif
#ifndef ASSERTEX
#define ASSERTEX(X,f,l) 
#endif
#else
#ifndef ASSERT
#define ASSERT(X) 
#endif
#ifndef ASSERTEX
#define ASSERTEX(X,f,l) 
#endif
#endif

#elif defined(NAL_LINUX)

#ifdef NAL_DRIVER

#include <linux/time.h>

#undef VOID
#endif

#ifndef VOID
#define VOID void
#endif
#define KVOID void
typedef signed char INT8;
typedef unsigned char UINT8;
typedef signed short INT16;
typedef unsigned short UINT16;
typedef signed int INT32;
typedef unsigned int UINT32;
typedef signed long long INT64;
typedef unsigned long long UINT64;
typedef signed long INTN;
typedef unsigned long UINTN;

typedef _Bool BOOLEAN;
typedef UINT16 PORT_ADDR;
typedef UINT64 NAL_PHYSICAL_ADDRESS;
typedef char CHAR;
typedef UINT16 WCHAR;
typedef UINT32 NAL_STATUS;

#define INLINE static inline

#if defined(NAL_DRIVER)
typedef UINTN NAL_SPIN_LOCK;
#else
#include <sys/types.h>

typedef pthread_mutex_t NAL_SPIN_LOCK;

#include <stdint.h>
#endif

#if defined(NAL_DEBUG)
#define ASSERT(X) 
#define ASSERTEX(X,f,l) 
#else
#define ASSERT(X) 
#define ASSERTEX(X,f,l) 
#endif

#elif defined(NAL_ESXI)

#ifdef NAL_DRIVER

#include <linux/time.h>

#undef VOID
#endif

#ifndef VOID
#define VOID void
#endif
#define KVOID void
typedef signed char INT8;
typedef unsigned char UINT8;
typedef signed short INT16;
typedef unsigned short UINT16;
typedef signed int INT32;
typedef unsigned int UINT32;
typedef signed long long INT64;
typedef unsigned long long UINT64;
typedef signed long INTN;
typedef unsigned long UINTN;

typedef _Bool BOOLEAN;
typedef UINT16 PORT_ADDR;
typedef UINT64 NAL_PHYSICAL_ADDRESS;
typedef char CHAR;
typedef UINT16 WCHAR;
typedef UINT32 NAL_STATUS;

#define INLINE static inline

#if defined(NAL_DRIVER)
typedef UINTN NAL_SPIN_LOCK;
#else
#include <sys/types.h>

typedef pthread_mutex_t NAL_SPIN_LOCK;

#include <stdint.h>
#endif

#if defined(NAL_DEBUG)
#define ASSERT(X) 
#define ASSERTEX(X,f,l) 
#else
#define ASSERT(X) 
#define ASSERTEX(X,f,l) 
#endif

#elif defined(NAL_SOLARIS)

#ifndef VOID
typedef void VOID;
#endif
typedef void KVOID;
typedef signed char INT8;
typedef unsigned char UINT8;
typedef signed short INT16;
typedef unsigned short UINT16;
typedef signed int INT32;
typedef unsigned int UINT32;
typedef signed long long INT64;
typedef unsigned long long UINT64;
typedef signed long INTN;
typedef unsigned long UINTN;

typedef _Bool BOOLEAN;
typedef UINT16 PORT_ADDR;
typedef UINT64 NAL_PHYSICAL_ADDRESS;
typedef char CHAR;
typedef UINT16 WCHAR;
typedef UINT32 NAL_STATUS;

#define INLINE static inline

#ifndef NAL_BIG_ENDIAN
#if defined(SPARC32)
#define NAL_BIG_ENDIAN 1
#endif
#if defined(SPARC64)
#define NAL_BIG_ENDIAN 1
#endif
#endif

#include <pthread.h>
typedef pthread_mutex_t NAL_SPIN_LOCK;

#if defined(NAL_DEBUG)
#ifndef ASSERT
#define ASSERT(X) 
#endif
#ifndef ASSERTEX
#define ASSERTEX(X,f,l) 
#endif
#else
#ifndef ASSERT
#define ASSERT(X) 
#endif
#ifndef ASSERTEX
#define ASSERTEX(X,f,l) 
#endif
#endif

#elif defined(NAL_WINNT_WDM)

#if defined(_WIN64)
    typedef __int64 INTN;
    typedef unsigned __int64 UINTN;
#else
    typedef long INTN;
    typedef unsigned long UINTN;
#endif

#if defined(NAL_DRIVER)
#include <ntddk.h>
#else
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN 
#endif

#ifdef SWIG
#define INLINE static inline
    typedef unsigned int UINT32;
#endif

#include <windows.h>
#include <winioctl.h>
#endif

typedef signed char INT8;
typedef unsigned char UINT8;
typedef signed short INT16;
typedef unsigned short UINT16;
typedef void KVOID;
typedef UINTN PORT_ADDR;
typedef UINT64 NAL_PHYSICAL_ADDRESS;
typedef UINT32 NAL_STATUS;

typedef struct _NAL_SPIN_LOCK
{
    volatile UINT32 SpinLock;
    BOOLEAN Initialized;
    BOOLEAN Acquired;
} NAL_SPIN_LOCK;

#if defined(NAL_DEBUG)
#ifndef ASSERT
#define ASSERT(X) 
#endif
#ifndef ASSERTEX
#define ASSERTEX(X,f,l) 
#endif
#else
#ifndef ASSERT
#define ASSERT(X) 
#endif
#ifndef ASSERTEX
#define ASSERTEX(X,f,l) 
#endif
#endif

#elif defined(NAL_FREEBSD)

#ifndef NAL_DRIVER
#include <stdio.h>
#endif

#include <sys/types.h>

#ifndef VOID

typedef void VOID;
#endif

typedef void KVOID;

typedef signed char INT8;

typedef unsigned char UINT8;

typedef signed short INT16;

typedef unsigned short UINT16;

typedef signed int INT32;

typedef unsigned int UINT32;

typedef signed long long INT64;

typedef unsigned long long UINT64;

typedef intptr_t INTN;

typedef uintptr_t UINTN;

typedef _Bool BOOLEAN;

typedef UINT16 PORT_ADDR;

typedef UINT64 NAL_PHYSICAL_ADDRESS;

typedef char CHAR;

typedef UINT32 NAL_STATUS;

#ifdef NAL_DRIVER

    #include <sys/param.h>
    #include <sys/lock.h>
    #include <sys/mutex.h>

    typedef struct mtx NAL_SPIN_LOCK;

#else

    #include <pthread.h>

    typedef pthread_mutex_t NAL_SPIN_LOCK;

#endif

#if defined(NAL_DEBUG)
#define ASSERT(X) 
#define ASSERTEX(X,f,l) 
#else
#define ASSERT(X) 
#define ASSERTEX(X,f,l) 
#endif

#if defined(INLINE)
#undef INLINE
#endif
#define INLINE static inline
#elif defined(NAL_ESXIN)

#ifndef VOID
#define VOID void
#endif

#include <stdint.h>

typedef uint8_t UINT8;
typedef uint16_t UINT16;
typedef uint32_t UINT32;
typedef uint64_t UINT64;
typedef int8_t INT8;
typedef int16_t INT16;
typedef int32_t INT32;
typedef int64_t INT64;

typedef UINT32 NAL_STATUS;

typedef intptr_t INTN;
typedef uintptr_t UINTN;

typedef _Bool BOOLEAN;
typedef UINT16 PORT_ADDR;
typedef UINT64 NAL_PHYSICAL_ADDRESS;
typedef char CHAR;
typedef void KVOID;

#if defined(NAL_DRIVER)
typedef UINTN NAL_SPIN_LOCK;
#else
#include <sys/types.h>

typedef pthread_mutex_t NAL_SPIN_LOCK;

#include <stdint.h>
#endif
#define INLINE static inline

#if defined(NAL_DEBUG)
#define ASSERT(X) 
#define ASSERTEX(X,f,l) 
#else
#define ASSERT(X) 
#define ASSERTEX(X,f,l) 
#endif

#else

#error "Undefined NAL Operating System in naltypes.h."
#endif

#ifndef IN
#define IN 
#endif

#ifndef OUT
#define OUT 
#endif

#ifndef NULL
#define NULL (VOID *)0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef OPTIONAL
#define OPTIONAL 
#endif

#ifndef INLINE
#define INLINE __forceinline
#endif

#ifndef CONST
#define CONST const
#endif

#ifndef UINT32_MAX
#define UINT32_MAX 0xFFFFFFFF
#endif

#define PHYSICAL_NULL (NAL_PHYSICAL_ADDRESS)0

typedef enum _NAL_DEBUG_PRINT_CAPABILITIES
{
    NAL_DEBUG_PRINT_SUPPORTED = 0x00000001,
    NAL_DEBUG_PRINT_ASCII = 0x00000002,
    NAL_DEBUG_PRINT_RESERVED = 0x00000004,
    NAL_DEBUG_PRINT_FILEIO = 0x00000008,
    NAL_DEBUG_PRINT_SYSLOG = 0x00000010,
    NAL_DEBUG_PRINT_COUNT
} NAL_DEBUG_PRINT_CAPABILITIES;

#define NAL_SIZE_OF_8_BYTES 8

#define NAL_PIC1_FIRST_ACCESS_PORT 0x0020
#define NAL_PIC2_FIRST_ACCESS_PORT 0x00A0
#define NAL_PIC1_SECOND_ACCESS_PORT 0x0021
#define NAL_PIC2_SECOND_ACCESS_PORT 0x00A1
#define NAL_CLEAR_PIC_COMMAND 0x0020

#define NAL_TIMER_INTERVAL_LENGTH 100
#define NAL_MAX_TIMER_CALLBACKS 1

#define NAL_BYTES_PER_DWORD 4
#define NAL_BYTES_PER_WORD 2

typedef VOID (*NAL_TIMER_CALLBACK) (VOID*);

typedef VOID *(*NAL_THREAD_FUNC) (VOID*);

#ifndef NAL_DRIVER

typedef struct _NAL_TIMER_CALLBACK_ENTRY
{
    struct _NAL_TIMER_CALLBACK_ENTRY* Next;
    struct _NAL_TIMER_CALLBACK_ENTRY* Prev;

    volatile UINT64 NextExecution;

    NAL_TIMER_CALLBACK TimerCallback;
    VOID* Context;

    UINT32 CallbackId;

    UINT32 TimerInterval;

    UINT32 Status;

} NAL_TIMER_CALLBACK_ENTRY;

#define NAL_TIMER_CALLBACK_ID_INVALID 0
#define NAL_TIMER_CALLBACK_ID_NEW_ENTRY -1

#define NAL_TIMER_CALLBACK_STATUS_NONE 0
#define NAL_TIMER_CALLBACK_STATUS_WAITING 1
#define NAL_TIMER_CALLBACK_STATUS_RESCHEDULE 2
#define NAL_TIMER_CALLBACK_STATUS_EXECUTING 3
#define NAL_TIMER_CALLBACK_STATUS_REMOVE 4

#endif

typedef enum _NAL_ASF_SUPPORT
{
 NAL_ASF_NOT_SUPPORTED = 0,
 NAL_ASF1_SUPPORTED,
 NAL_ASF2_SUPPORTED,
 NAL_ASF1_ASF2_SUPPORTED
} NAL_ASF_SUPPORT;

typedef enum _NAL_OS_TYPE
{
 NAL_OS_UNKNOWN = 0,
    NAL_OS_DOS,
    NAL_OS_EFI32,
    NAL_OS_EFI64,
    NAL_OS_LINUX32,
    NAL_OS_WIN3XX,
    NAL_OS_WIN9X,
    NAL_OS_OS2,
    NAL_OS_WINNT4,
    NAL_OS_WIN2K,
    NAL_OS_WINXP32,
    NAL_OS_WINXP64,
    NAL_OS_WINXP64E,
    NAL_OS_LINUX64,
    NAL_OS_FREEBSD32,
    NAL_OS_FREEBSD64e,
    NAL_OS_LINUX64E,
    NAL_OS_ESXI32,
    NAL_OS_ESXI64E,
    NAL_OS_NWS,
    NAL_OS_EFI64E,
    NAL_OS_EFIAARCH64,
    NAL_OS_LINUXPPC,
    NAL_OS_LINUXPPC64,
    NAL_OS_LINUXAARCH64,
    NAL_OS_COUNT,
    NAL_OS_SOLARIS_X86,
    NAL_OS_SOLARIS_64E,
    NAL_OS_SOLARIS_SPARC32,
    NAL_OS_SOLARIS_SPARC64
} NAL_OS_TYPE;

typedef enum _NAL_OS_RUN_DOMAIN
{
    NAL_OS_DOMAIN_BAREMETAL = 0,
    NAL_OS_DOMAIN_0,
    NAL_OS_DOMAIN_U
} NAL_OS_RUN_DOMAIN;

#define NAL_OS_LINUX NAL_OS_LINUX32

#if !defined(NAL_DRIVER)
#define boolean_t BOOLEAN
#ifndef uint8_t
#define uint8_t UINT8
#endif
#define u8 UINT8
#ifndef int8_t
#define int8_t INT8
#endif
#define s8 INT8
#ifndef uint16_t
#define uint16_t UINT16
#endif
#define u16 UINT16
#ifndef int16_t
#define int16_t INT16
#endif
#define s16 INT16
#ifndef uint32_t
#define uint32_t UINT32
#endif
#define u32 UINT32
#ifndef int32_t
#define int32_t INT32
#endif
#define s32 INT32
#ifndef uint64_t
#define uint64_t UINT64
#endif
#define u64 UINT64
#ifndef int64_t
#define int64_t INT64
#endif
#define s64 INT64
#endif

typedef UINT32 NAL_REGISTER;

typedef UINTN NAL_MAC_TYPE;

#define MAKE_BOOLEAN(x) (!!(x))

typedef enum _NAL_QUEUE_TYPE
{
    NAL_RX_QUEUE = 0,
    NAL_TX_QUEUE
} NAL_QUEUE_TYPE;

typedef struct _NAL_NVM_FEATURE_CONFIG
{
    UINT16 Id;
    UINT16 Options;
    UINT16 Selection;
} NAL_NVM_FEATURE_CONFIG;

typedef enum _NAL_DRIVER_MODE
{
    NAL_DRIVER_MODE_QV_DRIVER ,
    NAL_DRIVER_MODE_BASE_DRIVER ,
    NAL_DRIVER_MODE_BASE_DRIVER_NATIVE = NAL_DRIVER_MODE_BASE_DRIVER,
    NAL_DRIVER_MODE_BASE_DRIVER_LEGACY ,
    NAL_DRIVER_MODE_NO_DRIVER ,
} NAL_DRIVER_MODE;

typedef struct _NAL_OS_INIT_CONFIG
{
    BOOLEAN ForceDriverless;
    BOOLEAN FailoverDriverless;
    BOOLEAN SkipQvDriverVerification;
    BOOLEAN QuietInitialization;
} NAL_OS_INIT_CONFIG;

typedef enum
{
    NAL_BASE_DRIVER_ID_IGBE,
    NAL_BASE_DRIVER_ID_IXGBE,
    NAL_BASE_DRIVER_ID_IXGBE_ENS,
    NAL_BASE_DRIVER_ID_I40E,
    NAL_BASE_DRIVER_ID_I40E_ENS,
    NAL_BASE_DRIVER_ID_ICE,
    NAL_BASE_DRIVER_ID_NUMBER_OF_BASE_DRIVERS,

    NAL_BASE_DRIVER_ID_IGBE_LEGACY,
    NAL_BASE_DRIVER_ID_IXGBE_LEGACY,
    NAL_BASE_DRIVER_ID_I40E_LEGACY,
    NAL_BASE_DRIVER_ID_ICE_LEGACY
} NAL_BASE_DRIVER_ID;

typedef enum
{
    NAL_IOMMU_STATE_DISABLED,
    NAL_IOMMU_STATE_ENABLED,
    NAL_IOMMU_STATE_NOT_IMPLEMENTED,
} NAL_IOMMU_STATE;

typedef enum _NAL_AQ_DESCRIPTOR_TYPE
{
    NAL_AQ_DESCRIPTOR_COMMAND,
    NAL_AQ_DESCRIPTOR_WRITEBACK,
    NAL_AQ_DESCRIPTOR_EVENT
} NAL_AQ_DESCRIPTOR_TYPE;

typedef struct _NAL_PAGE_REGISTER_VALUE
{
    UINT32 Page;
    NAL_REGISTER Register;
    UINT16 Value;
} NAL_PAGE_REGISTER_VALUE;

typedef struct _NAL_BASE_DRIVER_FEATURES
{
    UINT8 Major;
    UINT8 Minor;
    UINT16 Size;
    UINT8 Features[12];
} NAL_BASE_DRIVER_FEATURES;

#endif

#define NAL_FLASH_MODULE_SHADOW_RAM_MASK ((UINT64)1 << NAL_FLASH_MODULE_SHADOW_RAM)
#define NAL_FLASH_MODULE_PCIE_ANALOG_MASK ((UINT64)1 << NAL_FLASH_MODULE_PCIE_ANALOG)
#define NAL_FLASH_MODULE_PHY_ANALOG_MASK ((UINT64)1 << NAL_FLASH_MODULE_PHY_ANALOG)
#define NAL_FLASH_MODULE_EMP_GLOBAL_MASK ((UINT64)1 << NAL_FLASH_MODULE_EMP_GLOBAL)
#define NAL_FLASH_MODULE_EMP_CORE_MASK ((UINT64)1 << NAL_FLASH_MODULE_EMP_CORE)
#define NAL_FLASH_MODULE_MNG_MASK ((UINT64)1 << NAL_FLASH_MODULE_MNG)
#define NAL_FLASH_MODULE_EMP_SETTINGS_MASK ((UINT64)1 << NAL_FLASH_MODULE_EMP_SETTINGS)
#define NAL_FLASH_MODULE_OPTION_ROM_MASK ((UINT64)1 << NAL_FLASH_MODULE_OPTION_ROM)
#define NAL_FLASH_MODULE_PE_IMAGE_MASK ((UINT64)1 << NAL_FLASH_MODULE_PE_IMAGE)
#define NAL_FLASH_MODULE_PHY_FW_MASK ((UINT64)1 << NAL_FLASH_MODULE_PHY_FW)
#define NAL_FLASH_MODULE_PE_SETTINGS_MASK ((UINT64)1 << NAL_FLASH_MODULE_PE_SETTINGS)
#define NAL_FLASH_MODULE_PHY_ACTIVITY_LIST_MASK ((UINT64)1 << NAL_FLASH_MODULE_PHY_ACTIVITY_LIST)
#define NAL_FLASH_MODULE_CONFIGURATION_METADATA_MASK ((UINT64)1 << NAL_FLASH_MODULE_CONFIGURATION_METADATA)
#define NAL_FLASH_MODULE_NETLIST_MASK ((UINT64)1 << NAL_FLASH_MODULE_NETLIST)
#define NAL_FLASH_MODULE_NVM_MASK ((UINT64)1 << NAL_FLASH_MODULE_NVM)
#define NAL_FLASH_MODULE_UNIONVALE_GLOBAL_MASK ((UINT64)1 << NAL_FLASH_MODULE_UNIONVALE_GLOBAL)
#define NAL_FLASH_MODULE_PHY_PLL_0_CONFIGURATION_MASK ((UINT64)1 << NAL_FLASH_MODULE_PHY_PLL_0_CONFIGURATION)
#define NAL_FLASH_MODULE_PHY_0_CONFIGURATION_MASK ((UINT64)1 << NAL_FLASH_MODULE_PHY_0_CONFIGURATION)
#define NAL_FLASH_MODULE_PHY_PLL_1_CONFIGURATION_MASK ((UINT64)1 << NAL_FLASH_MODULE_PHY_PLL_1_CONFIGURATION)
#define NAL_FLASH_MODULE_PHY_1_CONFIGURATION_MASK ((UINT64)1 << NAL_FLASH_MODULE_PHY_1_CONFIGURATION)
#define NAL_FLASH_MODULE_EMP_IMAGE_MASK ((UINT64)1 << NAL_FLASH_MODULE_EMP_IMAGE)
#define NAL_FLASH_MODULE_PHY_CONFIG_MASK ((UINT64)1 << NAL_FLASH_MODULE_PHY_CONFIG)
#define NAL_FLASH_MODULE_BASE_CONFIGURATION_MASK ((UINT64)1 << NAL_FLASH_MODULE_BASE_CONFIGURATION)
#define NAL_FLASH_MODULE_SERIAL_NUMBERS_MASK ((UINT64)1 << NAL_FLASH_MODULE_SERIAL_NUMBERS)
#define NAL_FLASH_MODULE_LIBERTY_TRAIL_CONFIG_MASK ((UINT64)1 << NAL_FLASH_MODULE_LIBERTY_TRAIL_CONFIG)
#define NAL_FLASH_MODULE_BANK_B_MASK ((UINT64)1 << NAL_FLASH_MODULE_BANK_B)
#define NAL_FLASH_MODULE_BANK_A_MASK ((UINT64)1 << NAL_FLASH_MODULE_BANK_A)
#define NAL_FLASH_MODULE_PROGRAMMABLE_COUNT_MASK ((UINT64)1 << NAL_FLASH_MODULE_PROGRAMMABLE_COUNT)
#define NAL_FLASH_MODULE_BANK_AB_MASK ((UINT64)1 << NAL_FLASH_MODULE_BANK_AB)
#define NAL_FLASH_MODULE_1ST_FREE_AREA_MASK ((UINT64)1 << NAL_FLASH_MODULE_1ST_FREE_AREA)
#define NAL_FLASH_MODULE_2ND_FREE_AREA_MASK ((UINT64)1 << NAL_FLASH_MODULE_2ND_FREE_AREA)
#define NAL_FLASH_MODULE_3RD_FREE_AREA_MASK ((UINT64)1 << NAL_FLASH_MODULE_3RD_FREE_AREA)
#define NAL_FLASH_MODULE_4TH_FREE_AREA_MASK ((UINT64)1 << NAL_FLASH_MODULE_4TH_FREE_AREA)
#define NAL_FLASH_MODULE_5TH_FREE_AREA_MASK ((UINT64)1 << NAL_FLASH_MODULE_5TH_FREE_AREA)
#define NAL_FLASH_MODULE_PF_ALLOCATIONS_MASK ((UINT64)1 << NAL_FLASH_MODULE_PF_ALLOCATIONS)
#define NAL_FLASH_MODULE_COUNT_MASK ((UINT64)1 << NAL_FLASH_MODULE_COUNT)
#define NAL_FLASH_MODULE_RO_UPDATE_MASK ((UINT64)1 << NAL_FLASH_MODULE_RO_UPDATE)
#define NAL_GENERIC_FLASH_IMAGE_MASK ((UINT64)1 << NAL_GENERIC_FLASH_IMAGE)
