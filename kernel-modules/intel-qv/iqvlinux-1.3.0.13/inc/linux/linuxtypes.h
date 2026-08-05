
#ifndef INC_FREEBSD_FREEBSDTYPES_H_
#define INC_FREEBSD_FREEBSDTYPES_H_ 

#include <naltypes.h>
#include <linuxdefs.h>
#include <hwbus_t.h>

#ifdef NAL_DRIVER
#else
#include <net/if.h>
#include <pthread.h>
#include <sys/time.h>
#endif

typedef UINTN NAL_OS_SPECIFIC_DEVICE;

typedef struct _NAL_OS_SPEC_DEVICE_CONTEXT
{
    BOOLEAN MarkedInUse;
    KVOID* KernelModeContext;
    KVOID* PciDevPtr;
    CHAR InterfaceName[IFNAMSIZ];
    CHAR BaseDriverName[NAL_OS_SPEC_BASE_DRIVER_NAME_MAX_LENGTH];
    BOOLEAN MemoryAllocatedByQvDriver;
    BOOLEAN IsQvPciDriverBound;
} NAL_OS_SPEC_DEVICE_CONTEXT;

typedef struct _NAL_OS_SPEC_ISR_DEVICE
{
    UINT32 Signature;
    BOOLEAN DeviceInterrupted;
    KVOID* HardwareVirtualAddress;
    UINT32 Irq;
    UINT32 MacType;
} NAL_OS_SPEC_ISR_DEVICE;

#ifdef NAL_DRIVER
typedef struct _NAL_OS_SPEC_ADAPTER_IN_USE_TABLE
{
    NAL_DEVICE_LOCATION DeviceLocation;
    BOOLEAN InUse;
} NAL_OS_SPEC_ADAPTER_IN_USE_TABLE;

#else

typedef struct _NAL_OS_SPEC_TIMER_OBJECT
{
    NAL_TIMER_CALLBACK Callback;
    UINT32 ThreadId;
    struct itimerval TimerVal;
    VOID* Context;
} NAL_OS_SPEC_TIMER_OBJECT;

typedef struct _NAL_OS_SPEC_MEMORY_MAP_TABLE
{
    INT32 ReferenceCount;
    VOID* VirtualAddress;
    VOID* AlignedVirtualAddress;
    NAL_PHYSICAL_ADDRESS AlignedPhysicalAddress;
    UINT32 Alignment;
    UINT32 BytesAllocated;
} NAL_OS_SPEC_MEMORY_MAP_TABLE;

typedef struct NAL_OS_SPEC_NONPAGED_MEMORY_TABLE
{
    UINT32 ReferenceCount;
    KVOID* VirtualAddress;
    VOID* MappedVirtualAddress;
} NAL_OS_SPEC_NONPAGED_MEMORY_TABLE;

#endif

typedef struct _NAL_OS_SPEC_DMA_PCI_MEMORY_TABLE
{
    UINT32 ReferenceCount;
    KVOID* VirtualAddress;
    KVOID* KernelAddress;
    NAL_PHYSICAL_ADDRESS PhysicalAddress;
    UINT32 Size;
} NAL_OS_SPEC_DMA_PCI_MEMORY_TABLE;

#ifndef NAL_DRIVER

typedef struct _NAL_OS_SPEC_THREAD_CONTEXT
{
        pthread_t Thread;
        VOID* Context;
        NAL_THREAD_FUNC ThreadFunction;
        BOOLEAN ThreadRunning;
} NAL_OS_SPEC_THREAD_CONTEXT;

typedef struct _NAL_OS_SPEC_DRIVER_INFO {
    UINT32 Command;
    char Driver[32];
    char Version[32];
    char FirmwareVersion[32];
    char BusInfo[32];
    char Reserved1[48];
    char Reserved2[16];
    UINT32 NetStats;
    UINT32 TestInfoLength;
    UINT32 EepromDumpLength;
    UINT32 RegisterDumpLength;
} NAL_OS_SPEC_DRIVER_INFO;

typedef NAL_OS_SPEC_THREAD_CONTEXT NAL_THREAD_ID;

typedef struct _NAL_OS_SPEC_GLOBAL_VARIABLES
{
    BOOLEAN MemMappingWithQvDriverPossible;
    BOOLEAN QvDriverMode;
    BOOLEAN InitialQvDriverMode;
    BOOLEAN MemMappingToUserSpaceDisabled;
    int NalFileDescriptor;
    int IoFileDescriptor;
    int MemFileDescriptor;
    int PciFileDescriptor;
    NAL_OS_SPEC_MEMORY_MAP_TABLE MemoryMapTable[NAL_OS_SPEC_MAX_MEMORY_ALLOCATIONS];
    NAL_OS_SPEC_TIMER_OBJECT TimerObjects[NAL_OS_SPEC_MAX_TIMERS];
    NAL_OS_SPEC_NONPAGED_MEMORY_TABLE NonPagedMapTable[NAL_OS_SPEC_MAX_MEMORY_ALLOCATIONS];
    NAL_OS_SPEC_DMA_PCI_MEMORY_TABLE DmaPciMemoryTable[NAL_OS_SPEC_MAX_MEMORY_ALLOCATIONS];
} NAL_OS_SPEC_GLOBAL_VARIABLES;

#endif

#endif
