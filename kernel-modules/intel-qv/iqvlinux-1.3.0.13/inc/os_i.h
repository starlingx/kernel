
#ifndef _OSINTERFACE_H_
#define _OSINTERFACE_H_ 

#include <naltypes.h>
#include <nalcodes.h>
#ifndef NAL_DRIVER
#include <device_t.h>
#endif

#define NAL_DATE_STRING __DATE__
#define NAL_DEBUG_DISABLED 0x00000000
#define NAL_DEBUG_APP 0x00000001
#define NAL_DEBUG_MACREG_READ 0x00000002
#define NAL_DEBUG_MACREG_WRITE 0x00000004
#define NAL_DEBUG_MACREG_RW (NAL_DEBUG_MACREG_READ | NAL_DEBUG_MACREG_WRITE)
#define NAL_DEBUG_TRANSMIT 0x00000008
#define NAL_DEBUG_RECEIVE 0x00000010
#define NAL_DEBUG_TXRX (NAL_DEBUG_TRANSMIT | NAL_DEBUG_RECEIVE)
#define NAL_DEBUG_TXRX_LOOP_CODE 0x00000020
#define NAL_DEBUG_SHARED_CODE 0x00000040
#define NAL_DEBUG_PHYREG_READ 0x00000080
#define NAL_DEBUG_PHYREG_WRITE 0x00000100
#define NAL_DEBUG_PHYREG_RW (NAL_DEBUG_PHYREG_READ | NAL_DEBUG_PHYREG_WRITE)
#define NAL_DEBUG_INIT 0x00000200
#define NAL_DEBUG_RELEASE 0x00000400
#define NAL_DEBUG_RESET_ADAPTER 0x00000800
#define NAL_DEBUG_RESET_LINK 0x00001000
#define NAL_DEBUG_GET_LINK 0x00002000
#define NAL_DEBUG_OS_LEVEL 0x00004000
#define NAL_DEBUG_APP_TRACE 0x00008000
#define NAL_DEBUG_SDK_TRACE 0x00010000
#define NAL_DEBUG_KERNEL 0x00020000
#define NAL_DEBUG_EEPROM 0x00040000
#define NAL_DEBUG_FLASH 0x00080000
#define NAL_DEBUG_DIAGS 0x00100000
#define NAL_DEBUG_MEMORY 0x00200000
#define NAL_DEBUG_BUS_SCAN 0x00400000
#define NAL_DEBUG_FAILURES 0x00800000
#define NAL_DEBUG_TIMESYNC 0x01000000
#define NAL_DEBUG_PROTOCOL_ENGINE 0x02000000
#define NAL_DEBUG_HMC 0x04000000
#define NAL_DEBUG_USER 0x80000000
#define NAL_DEBUG_ALL 0xFFFFFFFF

#define NAL_DEBUG_MASK_STACK_SIZE 256
#define NAL_DEBUG_SYSTEM_VARIABLE "QV_DEBUG_LOG"
#define NAL_DEBUG_SYSTEM_SYSLOG_VARIABLE "QV_DEBUG_SYSLOG"
#define NAL_DEFAULT_SDK_LOG_FILE_NAME "QVSDK.LOG"
extern char Global_Year[5];
extern BOOLEAN Global_QuietInitialization;

#if defined(NAL_EFI2)
#include <efi2\efios_i.h>

#elif defined(NAL_LINUX)
#include <linuxos_i.h>
#include <linuxnalioctl.h>
#include <linuxmemory.h>
#ifndef NAL_DRIVER
#include <linuxbasedriver.h>
#endif
#elif defined(NAL_ESXI)
#include <esxios_i.h>

#elif defined(NAL_ESXIN)
#include <esxinos_i.h>
#include <esxinnalmgmt.h>
#include <esxinmemory.h>
#ifndef NAL_DRIVER
#include <esxinbasedriver.h>
#endif

#elif defined(NAL_SOLARIS)
#include <solarisos_i.h>
#include <solarisnalioctl.h>
#include <solarismemory.h>
#ifndef NAL_DRIVER
#include <solarisbasedriver.h>
#endif

#elif defined(NAL_WINNT_WDM)
#include <winnt_wdm\winos_i.h>

#elif defined(NAL_FREEBSD)
#include <freebsdos_i.h>
#include <freebsdnalioctl.h>
#include <freebsdmemory_i.h>
#ifndef NAL_DRIVER
#include <freebsdbasedriver_i.h>
#endif
#else

#error "Undefined NAL Operating System in os_i.h"
#endif

#define NAL_MIN(_x,_y) ((_x > _y)? (_y) : (_x))
#define NAL_MAX(_x,_y) ((_x > _y)? (_x) : (_y))

#define NAL_ABS(_x) (((_x) > 0)? (_x) : ((_x) * -1))

#if defined (NAL_DRIVER)
extern BOOLEAN Global_DebugPrintEnabled;
#endif

extern UINT32 Global_DebugPrintMask;
extern BOOLEAN Global_FileDebugPrintEnabled;
extern BOOLEAN Global_DebugPrintToSyslog;
extern UINT32 Global_SdkDebugPrintMask;

#if !defined(NAL_DRIVER)
extern FILE* Global_DebugFilePointer;
#endif

#define PTR_TO_PHYSICAL(Pointer) (NAL_PHYSICAL_ADDRESS)(UINTN)(Pointer)
#define PHYSICAL_TO_PTR(Address) (VOID*)(UINTN)(Address)

#if defined(_WIN64) || defined(LINUX64E)
#define NAL_FIELD_OFFSET32(type,field) ((UINT32)(UINT64)(UINT32*)&(((type *)0)->field))
#else
#define NAL_FIELD_OFFSET32(type,field) ((UINT32)(UINT32*)&(((type *)0)->field))
#endif

#ifdef NAL_WINNT_WDM
#define NAL_STRDUP(_x) _strdup(_x)
#else
#define NAL_STRDUP(_x) strdup(_x)
#endif

#if defined(NAL_BIG_ENDIAN)
#define NAL_LITTLE_ENDIAN_16(_x) (UINT16)( (((UINT16)(_x) & 0x00ff) << 8) | \
                                            (((UINT16)(_x) & 0xff00) >> 8) )
#define NAL_LITTLE_ENDIAN_32(dword) (UINT32)( (((UINT32)(dword) & 0x000000ff) << 24) | \
                                            (((UINT32)(dword) & 0x0000ff00) << 8) | \
                                            (((UINT32)(dword) & 0x00ff0000) >> 8) | \
                                            (((UINT32)(dword) & 0xff000000) >> 24) )
#define NAL_LITTLE_ENDIAN_64(_x) \
    (((UINT64)((_x) & 0xff00000000000000ull) >> 56) | \
    ((UINT64)((_x ) & 0xff000000000000ull) >> 40) | \
    ((UINT64)((_x ) & 0xff0000000000ull) >> 24) | \
    ((UINT64)((_x ) & 0xff00000000ull) >> 8) | \
    ((UINT64)((_x ) & 0xff000000ull) << 8) | \
    ((UINT64)((_x ) & 0xff0000ull) << 24) | \
    ((UINT64)((_x ) & 0xff00ull) << 40) | \
    ((UINT64)((_x ) & 0xffull) << 56))

#else
#define NAL_LITTLE_ENDIAN_16(_x) (_x)
#define NAL_LITTLE_ENDIAN_32(_x) (_x)
#define NAL_LITTLE_ENDIAN_64(_x) (_x)
#endif

#ifndef NAL_PAGE_SIZE_OS
#define NAL_PAGE_SIZE_OS 4096
#endif

UINT8
NalReadPort8(
    IN PORT_ADDR Port
    );

UINT16
NalReadPort16(
    IN PORT_ADDR Port
    );

UINT32
NalReadPort32(
    IN PORT_ADDR Port
    );

BOOLEAN
NalWritePort8(
    IN PORT_ADDR Port,
    IN UINT8 Value
    );

BOOLEAN
NalWritePort16(
    IN PORT_ADDR Port,
    IN UINT16 Value
    );

BOOLEAN
NalWritePort32(
    IN PORT_ADDR Port,
    IN UINT32 Value
    );

UINT8
NalReadRegister8(
    IN KVOID* Address
    );

UINT16
NalReadRegister16(
    IN KVOID* Address
    );

UINT32
NalReadRegister32(
    IN KVOID* Address
    );

BOOLEAN
NalWriteRegister8(
    IN KVOID* Address,
    IN UINT8 Value
    );

BOOLEAN
NalWriteRegister16(
    IN KVOID* Address,
    IN UINT16 Value
    );

BOOLEAN
NalWriteRegister32(
    IN KVOID* Address,
    IN UINT32 Value
    );

NAL_STATUS
NalMmapAddress(
    OUT KVOID** VirtualAddress,
    IN NAL_PHYSICAL_ADDRESS PhysicalAddress,
    IN OUT UINT32* Length
    );

NAL_STATUS
NalUnmapAddress(
    IN KVOID* VirtualAddress,
    IN NAL_PHYSICAL_ADDRESS PhysicalAddress,
    IN UINT32 Length
    );

UINT64
NalGetTimeStamp(VOID);

UINT64
NalGetTimeStampsPerMicrosecond(VOID);

UINT64
NalGetTimeStampsPerMillisecond(VOID);

NAL_STATUS
NalRegisterTimerCallback(
    IN NAL_TIMER_CALLBACK TimerCallback,
    IN OUT UINT32* TimerInterval,
    IN VOID* Context,
    OUT UINT32* CallbackId
    );

NAL_STATUS
NalUnregisterTimerCallback(
    IN UINT32 CallbackId
    );

VOID
NalDelayMilliseconds(
    IN UINT32 Milliseconds
    );

VOID
NalDelayMicroseconds(
    IN UINT32 Microseconds
    );

NAL_PHYSICAL_ADDRESS
NalGetPhysicalMemoryAddress(
    IN KVOID* VirtualAddress
    );

VOID*
_NalAllocateMemory(
    IN UINT32 ByteCount,
    IN CHAR* NamedLocator,
    IN UINT32 LineNumber
    );

VOID
_NalFreeMemory(
    IN VOID* Address,
    IN CHAR* NamedLocator,
    IN UINT32 LineNumber
    );

KVOID*
_NalAllocateMemoryNonPaged(
    IN UINT32 ByteCount,
    IN UINT32 Alignment,
    OUT NAL_PHYSICAL_ADDRESS* PhysicalAddress,
    IN CHAR* NamedLocator,
    IN UINT32 LineNumber
    );

VOID
_NalFreeMemoryNonPaged(
    IN KVOID* Address,
    IN CHAR* NamedLocator,
    IN UINT32 LineNumber
    );

#define NalAllocateMemory(ByteCount) \
    _NalAllocateMemory((ByteCount), __FILE__, __LINE__)

#define NalFreeMemory(VirtualAddress) \
    _NalFreeMemory((VirtualAddress), __FILE__, __LINE__)

#define NalAllocateMemoryNonPaged(ByteCount,Alignment,PhysicalAddress) \
    _NalAllocateMemoryNonPaged((ByteCount), (Alignment), (PhysicalAddress), \
                               __FILE__, __LINE__)

#define NalFreeMemoryNonPaged(VirtualAddress) \
    _NalFreeMemoryNonPaged((VirtualAddress), __FILE__, __LINE__)

UINT32
NalAtomicIncrement32(
    IN UINT32* Address
    );

UINT32
NalAtomicDecrement32(
    IN UINT32* Address
    );

UINT32
NalAtomicTestSet32(
    IN UINT32* Address,
    IN UINT32 Test,
    IN UINT32 Set
    );

NAL_STATUS
NalAcquireSpinLock(
    IN NAL_SPIN_LOCK* SpinLock
    );

VOID
NalReleaseSpinLock(
    IN NAL_SPIN_LOCK* SpinLock
    );

VOID
NalInitializeSpinLock(
    IN NAL_SPIN_LOCK* SpinLock
    );

VOID
NalFreeSpinLock(
    IN NAL_SPIN_LOCK* SpinLock
    );

NAL_STATUS
NalCheckSecurity(VOID);

NAL_STATUS
NalDebugPrint(
    IN CHAR* Format,
    ...
    );

NAL_STATUS
_NalDebugPrintToDebugger(
    IN CHAR* StringToPrint
    );

NAL_STATUS
NalDebugPrintSupport(
    OUT UINT32* DebugCapabilities
    );

NAL_STATUS
NalDebugPrintOpenFile(
    IN CHAR* DebugFileName
    );

NAL_STATUS
NalDebugPrintCloseFile(VOID);

#if defined(NAL_DRIVER)
VOID
NalEnableDebugPrint(
    IN BOOLEAN Enable
    );
#else
VOID
NalEnableDebugPrint(
    IN UINT32 Mask
    );
#endif

VOID
NalSendDebuggerMessagesToSyslog(
    IN BOOLEAN Enable
    );

BOOLEAN
_NalCheckSystemVariableForSysLog(VOID);

BOOLEAN
_NalCheckSystemVariableForDebugMask(
    OUT UINT32* Mask
    );

NAL_STATUS
_NalSdkDebugPrintOpenFile(VOID);

VOID
_NalEnableSdkDebugPrint(
    IN UINT32 Mask
    );

NAL_STATUS
_NalSdkDebugPrintCloseFile(VOID);

VOID
_NalClearScreen(VOID);

VOID
NalRebootSystem(VOID);

BOOLEAN
NalIsTargetOperatingSystem(
    OUT NAL_OS_TYPE* OsType
    );

#define _NalMaskedDebugPrintWithTrack(Mask,Format) _NalMaskedDebugPrintWithTrackFunction(Mask,__FUNCTION__,__LINE__,Format)
#define _NalMaskedDebugPrintWithTrack1(Mask,Format,A1) _NalMaskedDebugPrintWithTrackFunction(Mask,__FUNCTION__,__LINE__,Format,A1)
#define _NalMaskedDebugPrintWithTrack2(Mask,Format,A1,A2) _NalMaskedDebugPrintWithTrackFunction(Mask,__FUNCTION__,__LINE__,Format,A1,A2)
#define _NalMaskedDebugPrintWithTrack3(Mask,Format,A1,A2,A3) _NalMaskedDebugPrintWithTrackFunction(Mask,__FUNCTION__,__LINE__,Format,A1,A2,A3)
#define _NalMaskedDebugPrintWithTrack4(Mask,Format,A1,A2,A3,A4) _NalMaskedDebugPrintWithTrackFunction(Mask,__FUNCTION__,__LINE__,Format,A1,A2,A3,A4)
#define _NalMaskedDebugPrintWithTrack5(Mask,Format,A1,A2,A3,A4,A5) _NalMaskedDebugPrintWithTrackFunction(Mask,__FUNCTION__,__LINE__,Format,A1,A2,A3,A4,A5)
#define _NalMaskedDebugPrintWithTrack6(Mask,Format,A1,A2,A3,A4,A5,A6) _NalMaskedDebugPrintWithTrackFunction(Mask,__FUNCTION__,__LINE__,Format,A1,A2,A3,A4,A5,A6)
#define _NalMaskedDebugPrintWithTrack7(Mask,Format,A1,A2,A3,A4,A5,A6,A7) _NalMaskedDebugPrintWithTrackFunction(Mask,__FUNCTION__,__LINE__,Format,A1,A2,A3,A4,A5,A6,A7)

NAL_STATUS
_NalMaskedDebugPrintWithTrackFunction(
     IN UINT32 Mask,
     IN const CHAR* Function,
     IN UINT32 Line,
     IN CHAR* Format,
     ...
     );

NAL_STATUS
NalMaskedDebugPrint(
    IN UINT32 Mask,
    IN CHAR* Format,
    ...
    );

NAL_STATUS
NalDebugPrintPushMask(
    IN UINT32 Mask
    );

NAL_STATUS
NalDebugPrintCheckAndPushMask(
    IN UINT32 MaskToCheck,
    IN UINT32 MaskToPush,
    OUT UINT32* IndexToCheck,
    IN BOOLEAN AddToExisting
    );

UINT32
NalDebugPrintPopMask(VOID);

UINT32
NalDebugPrintCheckAndPopMask(
    IN UINT32 MaskToCheck,
    IN UINT32 IndexToCheck
    );

UINT32
NalGetCurrentDebugPrintMask(VOID);

VOID
_NalEnableDebugPrint(
    IN BOOLEAN Enable
    );

VOID
_NalDebugPrintEnvInfo(
    VOID
    );

BOOLEAN
NalIsDebugFileOpen(VOID);

BOOLEAN
NalIsSdkDebugFileOpen(VOID);

UINT32
NalGetMaximumContiguousAllocationSize(VOID);

BOOLEAN
NalCanMapMemoryToUserSpace(VOID);

#if defined(NAL_DRIVER)

NAL_STATUS
NalMmapAddressEx(
    IN OUT KVOID** VirtualAddress,
    IN NAL_PHYSICAL_ADDRESS PhysicalAddress,
    IN OUT UINT32* Length,
    IN UINTN ProcessId
    );

NAL_STATUS
NalUnmapAddressEx(
    IN KVOID* VirtualAddress,
    IN NAL_PHYSICAL_ADDRESS PhysicalAddress,
    IN UINT32 Length,
    IN UINTN ProcessId
    );

KVOID*
_NalAllocateMemoryNonPagedEx(
    IN UINT32 ByteCount,
    IN UINT32 Alignment,
    IN UINTN ProcessId,
    OUT NAL_PHYSICAL_ADDRESS* PhysicalAddress,
    IN CHAR* NamedLocator,
    IN UINT32 LineNumber
    );

VOID
_NalFreeMemoryNonPagedEx(
    IN KVOID* Address,
    IN UINTN ProcessId,
    IN CHAR* NamedLocator,
    IN UINT32 LineNumber
    );

NAL_PHYSICAL_ADDRESS
NalGetPhysicalMemoryAddressEx(
    IN KVOID* VirtualAddress,
    IN UINTN ProcessId
    );

#endif

VOID
NalDisableMapMemoryToUserSpace(VOID);

VOID
NalEnableMapMemoryToUserSpace(VOID);

BOOLEAN
NalOsSupportsIoMode(VOID);

NAL_STATUS
_NalOsInitIoPort(
        PORT_ADDR Port
        );

UINT32
_NalBitSwap(
    IN UINT32 Data,
    IN UINT32 Bits
    );

char*
NalGetCopyrightYear(VOID);

#if !defined(NAL_DRIVER)
UINT32
NalGetLogicalCpuCount(VOID);

NAL_STATUS
NalCreateThread(
    IN NAL_THREAD_FUNC ThreadFunction,
    IN VOID* Context,
    OUT NAL_THREAD_ID* ThreadId
    );

NAL_STATUS
NalTerminateThread(
    IN NAL_THREAD_ID* ThreadId
    );

BOOLEAN
NalIsThreadRunning(
    IN NAL_THREAD_ID* ThreadId
    );

#include <stdarg.h>

#ifndef NAL_STATIC_ASSERT
# if defined(__FILE__) && defined(__LINE__)
#define NAL_STATIC_ASSERT(CONDITION,MESSAGE) if(!(CONDITION)) NalMaskedDebugPrint( NAL_DEBUG_OS_LEVEL | NAL_DEBUG_FAILURES, "%s:%d: Static assertion failed " #CONDITION " - " #MESSAGE "\n", __FILE__, __LINE__)
# elif defined(__FUNCTION__) && defined(__LINE__)
#define NAL_STATIC_ASSERT(CONDITION,MESSAGE) if(!(CONDITION)) NalMaskedDebugPrint( NAL_DEBUG_OS_LEVEL | NAL_DEBUG_FAILURES, "%s:%d: Static assertion failed " #CONDITION " - " #MESSAGE "\n", __FUNCTION__, __LINE__)
# elif defined(__func__) && defined(__LINE__)
#define NAL_STATIC_ASSERT(CONDITION,MESSAGE) if(!(CONDITION)) NalMaskedDebugPrint( NAL_DEBUG_OS_LEVEL | NAL_DEBUG_FAILURES, "%s:%d: Static assertion failed " #CONDITION " - " #MESSAGE "\n", __func__, __LINE__)
# else
# warning static assertions nor __FILE__ and __func__ are not available in your compiler
#define NAL_STATIC_ASSERT(CONDITION,MESSAGE) 
# endif
#endif

#ifndef NAL_COMPILER_MESSAGE
#define NAL_COMPILER_MESSAGE(MESSAGE) 
#endif

#define NAL_BUFFER_SIZE_ASSERT(Buffer,Function,Replacement) NAL_STATIC_ASSERT(sizeof(Buffer) != sizeof(void*) , "The " #Function " cannot be used for pointers! - Use " #Replacement " instead of it")

#if defined(FORCE_USING_NAL_INSTEAD_OF_POSIX) && !defined(ALLOW_POSIX_USAGE)
#define NAL_DENY_POSIX_ASSERT(Function,Replacement) NAL_STATIC_ASSERT(FALSE,"Usage of the function " #Function " is forbidden! You should use " #Replacement " instead of it")
#else
#define NAL_DENY_POSIX_ASSERT(Function,Replacement) 
#endif

#ifndef clear_struct
#define clear_struct(S) memset(&S,0,sizeof(S))
#endif

#ifndef clear_array
#define clear_array(A) memset(A,0,sizeof(A))
#endif

#if defined(USE_NAL_INSTEAD_OF_POSIX) && !defined(USE_REAL_POSIX)

#undef strcpy
#undef strncpy
#undef sprintf
#undef vsprintf
#undef sscanf
#undef memcpy
#undef strcat
#undef strncat
#undef strtok
#undef fopen
#undef fscanf
#undef fclose
#undef bzero

# ifdef USE_AUTO_BUFFER_SIZE_DETECTION
#define strcpy(Dst,Src) NalStringCopySafe(Dst,sizeof(Dst),Src,strlen(Src)+1);\
                                                    NAL_BUFFER_SIZE_ASSERT(Dst,strcpy,NalStringCopyToEndSafe);\
                                                    NAL_DENY_POSIX_ASSERT(strcpy,NalStringCopy)
#define strncpy(Dst,Src,Size) NalStringCopySafe(Dst,sizeof(Dst),Src,Size);\
                                                    NAL_BUFFER_SIZE_ASSERT(Dst,strncpy,NalStringCopySafe);\
                                                    NAL_DENY_POSIX_ASSERT(strncpy,NalStringNCopy)
#define sprintf(Dst,...) NalPrintStringFormattedSafe(Dst,sizeof(Dst),__VA_ARGS__);\
                                                    NAL_BUFFER_SIZE_ASSERT(Dst,sprintf,NalPrintStringFormattedSafe);\
                                                    NAL_DENY_POSIX_ASSERT(sprintf,NalPrintStringFormatted)
#define vsprintf(Dst,Fmt,Arg) NalVPrintStringFormattedSafe(Dst,sizeof(Dst),Fmt,Arg);\
                                                    NAL_BUFFER_SIZE_ASSERT(Dst,vsprintf,NalVPrintStringFormattedSafe);\
                                                    NAL_DENY_POSIX_ASSERT(vsprintf,NalVPrintStringFormatted)
#define sscanf NalScanFormattedString
#define memcpy NalMemoryCopy
#define strcat(Dst,Src) NalStringConcatenateSafe(Dst,sizeof(Dst),Src,strlen(Src)+1);\
                                                    NAL_BUFFER_SIZE_ASSERT(Dst,strcat,NalStringConcatenateSafe);\
                                                    NAL_DENY_POSIX_ASSERT(strcat,NalStringConcatenate)
#define strncat(Dst,Src,N) NalStringConcatenateSafe(Dst,sizeof(Dst),Src,N);\
                                                    NAL_BUFFER_SIZE_ASSERT(Dst,strncat,NalStringConcatenateSafe);\
                                                    NAL_DENY_POSIX_ASSERT(strncat,NalStringNConcatenate)
#define strtok NalSplitStringIntoTokens
#define fopen NalOpenFile
#define fscanf NalScanFormattedFile
#define bzero(b,s) memset(b,0,s)
# else
#define strcpy NalStringCopy
#define strncpy NalStringNCopy
#define sprintf NalPrintStringFormatted
#define vsprintf NalVPrintStringFormatted
#define sscanf NalScanFormattedString
#define memcpy NalMemoryCopy
#define strcat NalStringConcatenate
#define strncat NalStringNConcatenate
#define strtok NalSplitStringIntoTokens
#define fopen NalOpenFile
#define fscanf NalScanFormattedFile
#define bzero(b,s) memset(b,0,s)
# endif

#endif

#ifdef USE_AUTO_BUFFER_SIZE_DETECTION
#define NalStringCopy(Dst,Src) NalStringCopySafe(Dst,sizeof(Dst),Src,strlen(Src)+1);\
                                                     NAL_BUFFER_SIZE_ASSERT(Dst,NalStringCopy,NalStringCopyToEndSafe)
#define NalStringNCopy(Dst,Src,Size) NalStringCopySafe(Dst,sizeof(Dst),Src,Size);\
                                                     NAL_BUFFER_SIZE_ASSERT(Dst,NalStringNCopy,NalStringCopySafe)
#define NalPrintStringFormatted(Dst,...) NalPrintStringFormattedSafe(Dst,sizeof(Dst),__VA_ARGS__);\
                                                     NAL_BUFFER_SIZE_ASSERT(Dst,NalPrintStringFormatted,NalPrintStringFormattedSafe)
#define NalVPrintStringFormatted(Dst,Fmt,Arg) NalVPrintStringFormattedSafe(Dst,sizeof(Dst),Fmt,Arg);\
                                                     NAL_BUFFER_SIZE_ASSERT(Dst,NalVPrintStringFormatted,NalVPrintStringFormattedSafe)
#define NalStringConcatenate(Dst,Src) NalStringConcatenateSafe(Dst,sizeof(Dst),Src,strlen(Src)+1);\
                                                     NAL_BUFFER_SIZE_ASSERT(Dst,NalStringConcatenate,NalStringConcatenateSafe)
#define NalStringNConcatenate(Dst,Src,N) NalStringConcatenateSafe(Dst,sizeof(Dst),Src,N);\
                                                     NAL_BUFFER_SIZE_ASSERT(Dst,NalStringNConcatenate,NalStringConcatenateSafe)
#ifndef bzero
#define bzero(b,s) memset(b,0,s)
#endif
#endif

#ifndef NalStringCopy
CHAR*
NalStringCopy(
    OUT CHAR * Destination,
    IN const CHAR * Source
  );
#endif

#ifndef NalStringNCopy
CHAR*
NalStringNCopy(
    OUT CHAR * Destination ,
 IN const CHAR * Source ,
 IN UINTN Size
   );
#endif

#ifndef NalStringCopyToEndSafe
#define NalStringCopyToEndSafe(Destination,DestinationSize,Source) NalStringCopySafe(Destination,DestinationSize,Source,strlen(Source)+1)
#endif

#ifndef NalStringCopySafe
CHAR*
NalStringCopySafe(
   OUT CHAR * Destination ,
   IN UINTN DestinationSize ,
   IN const CHAR * Source ,
   IN UINTN MaxCount
   );
#endif

#ifndef NalPrintStringFormatted
int
NalPrintStringFormatted(
   OUT CHAR * Destination,
   IN const CHAR * Format ,
   ...
   );
#endif

#ifndef NalVPrintStringFormatted
#define NalVPrintStringFormatted(Dst,Fmt,Arg) _NalVPrintStringFormatted(Dst,Fmt,&Arg)
#endif

#ifndef SWIG
#ifndef _NalVPrintStringFormatted
int
_NalVPrintStringFormatted(
   OUT CHAR* Destination,
   IN const CHAR* Format,
   IN va_list* Arguments
   );
#endif

#ifndef NalPrintStringFormattedSafe
int
NalPrintStringFormattedSafe(
   OUT CHAR * Destination,
   IN UINTN DestinationSize,
   IN const CHAR* Format,
   ...
   );
#endif
#endif

#ifndef NalVPrintStringFormattedSafe
#define NalVPrintStringFormattedSafe(Dst,Size,Fmt,Arg) _NalVPrintStringFormattedSafe(Dst,Size,Fmt,&Arg)
#endif

#ifndef SWIG
#ifndef _NalVPrintStringFormattedSafe
int
_NalVPrintStringFormattedSafe(
   OUT CHAR* Destination,
   IN UINTN DestinationSize,
   IN const CHAR* Format,
   IN va_list* Arguments
   );
#endif
#endif

#ifndef NalScanFormattedString
int
NalScanFormattedString(
   IN const CHAR* String,
   IN const CHAR* Format,
   ...
   );
#endif

#ifndef NalMemoryCopy
VOID*
NalMemoryCopy(
   OUT VOID* Destination,
   IN const VOID* Source,
   IN UINTN Size
   );
#endif

#ifndef NalMemoryCopySafe
VOID*
NalMemoryCopySafe(
   OUT VOID* Destination,
   IN UINTN DestinationSize,
   IN const VOID* Source,
   IN UINTN SourceSize
   );
#endif

#ifndef NalStringConcatenate
CHAR*
NalStringConcatenate(
   OUT CHAR* Destination,
   IN const CHAR* Source
   );
#endif

#ifndef NalStringNConcatenate
#define NalStringNConcatenate(Dest,Source,Size) NalStringConcatenateSafe(Dest,Size,Source,Size)
#endif

#ifndef NalStringConcatenateSafe
CHAR*
NalStringConcatenateSafe(
   OUT CHAR* Destination,
   IN UINTN DestinationSize,
   IN const CHAR* Source,
   IN UINTN SourceSize
   );
#endif

#ifndef NalStringConcatenateToEndSafe
#define NalStringConcatenateToEndSafe(Destination,DestinationSize,Source) NalStringConcatenateSafe(Destination,DestinationSize,Source,strlen(Source))
#endif

#ifndef NalSplitStringIntoTokens
CHAR*
NalSplitStringIntoTokens(
   IN CHAR * StringToSplit,
   IN const CHAR* Delimiters
   );
#endif

#ifndef NalOpenFile
VOID*
NalOpenFile(
   IN const CHAR* FileName,
   IN const CHAR* Mode
   );
#endif

#ifndef NalScanFormattedFile
int
NalScanFormattedFile(
   IN VOID* Stream,
   IN const CHAR* Format,
   ...
   );
#endif

#ifndef NalCloseFile
int
NalCloseFile(
   IN VOID* Stream
   );
#endif

#ifndef NalGetEnvironmentVariable
CHAR*
NalGetEnvironmentVariable(
   IN CHAR * Name ,
   OUT CHAR * Destination,
   IN UINTN Length
   );
#endif

#if !defined(NAL_WINNT_WDM)
#ifndef vfscanf
    extern int vfscanf(FILE *stream, const char *format, va_list ap);
#endif

#ifndef vsscanf
    extern int vsscanf(const char *str, const char *format, va_list ap);
#endif
#endif

#ifndef NalZeroFill
#define NalZeroFill(Buffer) memset(Buffer,0,sizeof(Buffer))
#endif

#ifndef UNREFERENCED_PARAMETER
#define UNREFERENCED_PARAMETER(X) 
#endif

#ifndef NAL_DRIVER

NAL_STATUS
NalGetBaseDriverVersionById(
      IN NAL_BASE_DRIVER_ID BaseDriverId,
      OUT CHAR* Version,
      IN UINT32 VersionLength
      );

NAL_STATUS
NalGetQvDriverVersionFromService(
      OUT CHAR* Version,
      IN UINT32 BufferSize
      );

NAL_STATUS
NalGetBaseDriverVersionByLocation(
      IN NAL_DEVICE_LOCATION* DeviceLocation,
      OUT CHAR* Version,
      IN UINT32 VersionLength
      );

NAL_STATUS
NalGetBaseDriverCapabilities(
    IN NAL_DEVICE_LOCATION DeviceLocation,
    OUT UINT8* Buffer,
    IN OUT UINT32 BufferSize
    );

BOOLEAN
_NalIsBaseDriverAvailable(
      IN NAL_DEVICE_LOCATION* PciDevice
      );

NAL_STATUS
_NalConnectToBaseDriver(
      IN NAL_ADAPTER_STRUCTURE* Adapter
      );

NAL_STATUS
_NalDisconnectFromBaseDriver(
      IN NAL_ADAPTER_STRUCTURE* Adapter
      );

NAL_STATUS
_NalStopBaseDriver(
      IN NAL_ADAPTER_STRUCTURE* Adapter
      );

NAL_STATUS
_NalStartBaseDriver(
      IN NAL_ADAPTER_STRUCTURE* Adapter
      );

BOOLEAN
_NalIsBaseDriverStopped(
      IN NAL_ADAPTER_STRUCTURE* Adapter
      );

NAL_STATUS
_NalLoadQvDriver( VOID );

NAL_STATUS
_NalUnloadQvDriver( VOID );

NAL_STATUS
_NalReloadQvDriver( VOID );

NAL_STATUS
_NalConnectToQvDriver( VOID );

NAL_STATUS
_NalDisconnectFromQvDriver( VOID );

BOOLEAN
NalIsQvDriverLoaded( VOID );

BOOLEAN
NalIsQvDriverInstalled( VOID );

BOOLEAN
NalIsQvDriverConnected( VOID );

BOOLEAN
NalIsDriverlessPossible( VOID );

NAL_STATUS
NalIsVirtualMachine(
    OUT BOOLEAN * VirtualMachine
    );

BOOLEAN
_NalIsQvDriverVersionCorrect( VOID );

NAL_STATUS
_NalEnableQvDriverLogs( VOID );

NAL_STATUS
_NalDisableQvDriverLogs( VOID );

NAL_STATUS
_NalPrintQvDriverLogs( VOID );

NAL_STATUS
NalInitializeQvDriverConnection(
    IN BOOLEAN SkipQvDriverVerification
    );

NAL_STATUS
NalInitializeQvDriverMode(
    IN BOOLEAN SkipQvDriverVerification
    );

NAL_STATUS
NalInitializeDriverlessMode( VOID );

INT32
_NalGetQvDriverReferenceCounter( VOID );

NAL_STATUS
_NalIncrementQvDriverReferenceCounter( VOID );

NAL_STATUS
_NalDecrementQvDriverReferenceCounter( VOID );

#if defined(NAL_EFI2)
NAL_STATUS
NalInitializeOs(
    IN EFI_HANDLE ImageHandle,
    IN EFI_SYSTEM_TABLE *SystemTable
);
#else
NAL_STATUS
NalInitializeOs( VOID );
#endif

NAL_STATUS
NalInitializeOsEx(
      IN NAL_OS_INIT_CONFIG InitConfig
      );

NAL_STATUS
NalExitOs(VOID);

#endif

#endif

#define NAL_PSEUDO_RANDOM_NUMBER_GENERATOR_INITIALIZER ((UINT64)0x005555555555)
#define NAL_PSEUDO_RANDOM_NUMBER_GENERATOR_MULTIPLIER ((UINT64)0x5DEECE66D)
#define NAL_PSEUDO_RANDOM_NUMBER_GENERATOR_INCREMENTER ((UINT64)0x0B)

#define NAL_PSEUDO_RANDOM_NUMBER_GENERATOR_STARTING_BIT (15)
#define NAL_PSEUDO_RANDOM_NUMBER_GENERATOR_48_BIT_MASK ((UINT64)0x0FFFFFFFFFFFFu)
#define NAL_PSEUDO_RANDOM_NUMBER_GENERATOR_31_BIT_MASK ((UINT64)0x07fffffffu)

VOID
_NalSeedRandomNumberGenerator(VOID);

INT32
_NalGetRandomNumber(VOID);

UINT32
_NalTimestamp32bit(VOID);

#endif
