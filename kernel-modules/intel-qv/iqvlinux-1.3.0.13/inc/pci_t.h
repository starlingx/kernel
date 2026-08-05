
#ifndef _PCITYPES_H_
#define _PCITYPES_H_ 

#include "naltypes.h"

typedef enum _NAL_PCI_CAPABILITY
{
    NAL_VPD_CAPABILITY_ID = 0x03,
    NAL_SLOT_ID_CAPABILITY_ID = 0x04,
    NAL_MSI_CAPABILITY_ID = 0x05,
    NAL_HOTSWAP_CAPABILITY_ID = 0x06,
    NAL_PCIX_CAPABILITY_ID = 0x07,
    NAL_PCIEXPRESS_CAPABILITY_ID = 0x10,
    NAL_MSI_X_CAPABILITY_ID = 0x11
} NAL_PCI_CAPABILITY;

#define PCI_CONFIG_BM_VALID_BITS 0x0F
#define PCI_CONFIG_BM_BYTE1 0x01
#define PCI_CONFIG_BM_BYTE2 0x02
#define PCI_CONFIG_BM_BYTE3 0x04
#define PCI_CONFIG_BM_BYTE4 0x08
#define PCI_CONFIG_BM_ENTIRE_DWORD 0xF
#define PCI_CONFIG_BM_DWORD 0xF
#define PCI_CONFIG_BM_HIWORD 0xC
#define PCI_CONFIG_BM_LOWORD 0x3

#define PCI_DEVICE_ALL_DWORDS 64
#define PCI_DEVICE_CONFIG_DWORDS 16

#define PCI_EXPRESS_DEVICE_ALL_DWORDS 1024
#define PCI_EXPRESS_DEVICE_ALL_BYTES 4096
#define MAX_PCIEXP_SLOTS 32

#define PCI_MULTI_FUNCTION 0x00800000

#define PCI_BAR_IO_MASK 0x00000001
#define PCI_BAR_IO_MODE 0x00000001

#define PCI_BAR_MEM_MASK 0x0000000F
#define PCI_BAR_MEM_MODE 0x00000000
#define PCI_BAR_MEM_64BIT_MASK 0x00000006
#define PCI_BAR_MEM_64BIT 0x00000004
#define PCI_BAR_MEM_PREFETCHABLE 0x00000008

#define PCI_EXP_BAR_DWORD 12
#define PCI_EXP_BAR_ADDR_DECODE_ENA 0x00000001

#define PCI_CMD_IO_SPACE 0x00000001
#define PCI_CMD_MEM_SPACE 0x00000002
#define PCI_CMD_BUS_PRIMARY 0x00000004

#define PCI_CAP 0xCF8
#define PCI_CDP 0xCFC
#define PCI_MAX_BUSES 256
#define PCI_MAX_DEVICES 32
#define PCI_MAX_FUNCTIONS 8

#define PCI_CAPABILITIES_LIST 0x0010

#define PCI_MSIX_MSGCTL_SHIFT 16
#define PCI_MSIX_NXTPTR_SHIFT 8
#define PCI_MSIX_ENABLE (0x8000 << PCI_MSIX_MSGCTL_SHIFT)
#define PCI_MSIX_BLOCK (0x4000 << PCI_MSIX_MSGCTL_SHIFT)
#define PCI_MSIX_SIZE_MASK (0x3FFF << PCI_MSIX_MSGCTL_SHIFT)

#pragma pack(1)

typedef struct _PCI_CONFIG_SPACE
{
    union {
        UINT32 Dwords[64];
        UINT8 Bytes[256];
    };

} PCI_CONFIG_SPACE;

typedef struct _PCI_EXPRESS_CONFIG_SPACE
{
    union {
        UINT32 Dwords[PCI_EXPRESS_DEVICE_ALL_DWORDS];
        UINT8 Bytes[PCI_EXPRESS_DEVICE_ALL_BYTES];
    };

} PCI_EXPRESS_CONFIG_SPACE;

typedef struct _PCI_DEVICE
{
#if defined (NAL_BIG_ENDIAN)
    UINT16 DeviceId;
    UINT16 VendorId;
    UINT16 StatusRegister;
    UINT16 CommandRegister;
    UINT8 ClassCode;
    UINT8 SubclassCode;
    UINT8 ProgIf;
    UINT8 RevisionId;
    UINT8 Bist;
    UINT8 HeaderType;
    UINT8 LatencyTimer;
    UINT8 CacheLineSize;
    UINT32 Bar0;
    UINT32 Bar1;
    UINT32 Bar2;
    UINT32 Bar3;
    UINT32 Bar4;
    UINT32 Bar5;
    UINT32 CardBusCisPointer;
    UINT16 SubsystemId;
    UINT16 SubsystemVendorId;
    UINT32 ExpansionRomBaseAddress;
    UINT8 Reserved[3];
    UINT8 CapabilitiesPointer;
    UINT32 Reserved2;
    UINT8 MaxLatency;
    UINT8 MinGrant;
    UINT8 InterruptPin;
    UINT8 InterruptLine;
#else
    UINT16 VendorId;
    UINT16 DeviceId;
    UINT16 CommandRegister;
    UINT16 StatusRegister;
    UINT8 RevisionId;
    UINT8 ProgIf;
    UINT8 SubclassCode;
    UINT8 ClassCode;
    UINT8 CacheLineSize;
    UINT8 LatencyTimer;
    UINT8 HeaderType;
    UINT8 Bist;
    UINT32 Bar0;
    UINT32 Bar1;
    UINT32 Bar2;
    UINT32 Bar3;
    UINT32 Bar4;
    UINT32 Bar5;
    UINT32 CardBusCisPointer;
    UINT16 SubsystemVendorId;
    UINT16 SubsystemId;
    UINT32 ExpansionRomBaseAddress;
    UINT8 CapabilitiesPointer;
    UINT8 Reserved[3];
    UINT32 Reserved2;
    UINT8 InterruptLine;
    UINT8 InterruptPin;
    UINT8 MinGrant;
    UINT8 MaxLatency;
#endif

    UINT32 ConfigSpace[48];
} PCI_DEVICE;

typedef union _PCI_SLOT_ID
{
    UINT32 SlotId;

    struct
    {
#if defined (NAL_BIG_ENDIAN)
        UINT32 Segment : 8;
        UINT32 Dword : 6;
        UINT32 _Byte : 2;
        UINT32 Device : 5;
        UINT32 Function : 3;
        UINT32 Bus : 8;
#else
        UINT32 Bus : 8;
        UINT32 Device : 5;
        UINT32 Function : 3;
        UINT32 Dword : 6;
        UINT32 _Byte : 2;
        UINT32 Segment : 8;
#endif
    };
} PCI_SLOT_ID;

typedef union _PCI_EXPRESS_SLOT_ID
{
    UINT32 SlotId;

    struct
    {
#if defined(NAL_BIG_ENDIAN)
        UINT32 Dword2 : 2;
        UINT32 _Byte : 2;
        UINT32 Reserved : 4;
        UINT32 Dword1 : 8;
        UINT32 Device : 5;
        UINT32 Function : 3;
        UINT32 Bus : 8;
#else
        UINT32 Bus : 8;
        UINT32 Device : 5;
        UINT32 Function : 3;
        UINT32 Dword : 10;
        UINT32 _Byte : 2;
        UINT32 Reserved : 4;
#endif
    };

} PCI_EXPRESS_SLOT_ID;

typedef struct _PCI_COMMAND_REGISTER_STRUCT
{
#if defined(NAL_BIG_ENDIAN)
    UINT16 SerrEnable : 1;
    UINT16 FastBackToBackEnable : 1;
    UINT16 InterruptDisable : 1;
    UINT16 Reserved : 5;
    UINT16 IoSpace : 1;
    UINT16 MemorySpace : 1;
    UINT16 BusPrimary : 1;
    UINT16 SpecialCycles : 1;
    UINT16 WmiEnable : 1;
    UINT16 VgaPaletteSnoopEnable : 1;
    UINT16 ParityErrorResponce : 1;
    UINT16 SteppingControl : 1;
#else
    UINT16 IoSpace : 1;
    UINT16 MemorySpace : 1;
    UINT16 BusPrimary : 1;
    UINT16 SpecialCycles : 1;
    UINT16 WmiEnable : 1;
    UINT16 VgaPaletteSnoopEnable : 1;
    UINT16 ParityErrorResponce : 1;
    UINT16 SteppingControl : 1;
    UINT16 SerrEnable : 1;
    UINT16 FastBackToBackEnable : 1;
    UINT16 InterruptDisable : 1;
    UINT16 Reserved : 5;
#endif
} PCI_COMMAND_REGISTER_STRUCT;

typedef struct _PCI_STATUS_REGISTER_STRUCT
{
#if defined(NAL_BIG_ENDIAN)
    UINT16 PrimaryDataParityError : 1;
    UINT16 DevselTiming : 2;
    UINT16 SignalledTargetAbort : 1;
    UINT16 ReceivedTargetAbort : 1;
    UINT16 ReceivedPrimaryAbort : 1;
    UINT16 SignalledSystemError : 1;
    UINT16 DetectedParityError : 1;
    UINT16 Reserved : 4;
    UINT16 CapabilitiesList : 1;
    UINT16 Capable66Mhz : 1;
    UINT16 ReservedUdfSupport : 1;
    UINT16 FastBackToBack : 1;
#else
    UINT16 Reserved : 4;
    UINT16 CapabilitiesList : 1;
    UINT16 Capable66Mhz : 1;
    UINT16 ReservedUdfSupport : 1;
    UINT16 FastBackToBack : 1;
    UINT16 PrimaryDataParityError : 1;
    UINT16 DevselTiming : 2;
    UINT16 SignalledTargetAbort : 1;
    UINT16 ReceivedTargetAbort : 1;
    UINT16 ReceivedPrimaryAbort : 1;
    UINT16 SignalledSystemError : 1;
    UINT16 DetectedParityError : 1;
#endif
} PCI_STATUS_REGISTER_STRUCT;

typedef struct _PCIX_DEVICE
{
#if defined (NAL_BIG_ENDIAN)
    UINT16 DeviceId;
    UINT16 VendorId;
    UINT16 StatusRegister;
    UINT16 CommandRegister;
    UINT8 ClassCode;
    UINT8 SubclassCode;
    UINT8 ProgIf;
    UINT8 RevisionId;
    UINT8 Bist;
    UINT8 HeaderType;
    UINT8 LatencyTimer;
    UINT8 CacheLineSize;
    UINT64 Bar0;
    UINT64 Bar1;
    UINT64 Bar2;
    UINT32 CardBusCisPointer;
    UINT16 SubsystemId;
    UINT16 SubsystemVendorId;
    UINT32 ExpansionRomBaseAddress;
    UINT8 Reserved[3];
    UINT8 CapabilitiesPointer;
    UINT32 Reserved2;
    UINT8 MaxLatency;
    UINT8 MinGrant;
    UINT8 InterruptPin;
    UINT8 InterruptLine;

    UINT32 ConfigSpace[48];
#else
    UINT16 VendorId;
    UINT16 DeviceId;
    UINT16 CommandRegister;
    UINT16 StatusRegister;
    UINT8 RevisionId;
    UINT8 ProgIf;
    UINT8 SubclassCode;
    UINT8 ClassCode;
    UINT8 CacheLineSize;
    UINT8 LatencyTimer;
    UINT8 HeaderType;
    UINT8 Bist;
    UINT64 Bar0;
    UINT64 Bar1;
    UINT64 Bar2;
    UINT32 CardBusCisPointer;
    UINT16 SubsystemVendorId;
    UINT16 SubsystemId;
    UINT32 ExpansionRomBaseAddress;
    UINT8 CapabilitiesPointer;
    UINT8 Reserved[3];
    UINT32 Reserved2;
    UINT8 InterruptLine;
    UINT8 InterruptPin;
    UINT8 MinGrant;
    UINT8 MaxLatency;

    UINT32 ConfigSpace[48];
#endif
} PCIX_DEVICE;

typedef struct _PCIX_COMMAND_REGISTER_STRUCT
{
#if defined(NAL_BIG_ENDIAN)
    UINT16 ReservedHi : 8;
    UINT16 DataParityErrorRecoverEnable : 1;
    UINT16 EnableRelaxedOrdering : 1;
    UINT16 MaxMemoryReadByteCount : 2;
    UINT16 MaxOutstandingSplitTransactions : 3;
    UINT16 ReservedLo : 1;
#else
    UINT16 DataParityErrorRecoverEnable : 1;
    UINT16 EnableRelaxedOrdering : 1;
    UINT16 MaxMemoryReadByteCount : 2;
    UINT16 MaxOutstandingSplitTransactions : 3;
    UINT16 Reserved : 9;
#endif
} PCIX_COMMAND_REGISTER_STRUCT;

typedef struct _PCIX_STATUS_REGISTER_STRUCT
{
#if defined(NAL_BIG_ENDIAN)
    UINT32 MaxOutstandingSplitTransactionsHi : 2;
    UINT32 MaxCumulativeReadSize : 3;
    UINT32 ReceivedSplitCompletionError : 1;
    UINT32 Reserved : 2;
    UINT32 Device64bit : 1;
    UINT32 Capable133Mhz : 1;
    UINT32 SplitCompletionDiscarded : 1;
    UINT32 UnexpectedSplitCompletion : 1;
    UINT32 DeviceComplexity : 1;
    UINT32 MaxMemoryReadByteCount : 2;
    UINT32 MaxOutstandingSplitTransactionsLo : 1;
    UINT32 BusNumber : 8;
    UINT32 FunctionNumber : 3;
    UINT32 DeviceNumber : 5;
#else
    UINT32 FunctionNumber : 3;
    UINT32 DeviceNumber : 5;
    UINT32 BusNumber : 8;
    UINT32 Device64bit : 1;
    UINT32 Capable133Mhz : 1;
    UINT32 SplitCompletionDiscarded : 1;
    UINT32 UnexpectedSplitCompletion : 1;
    UINT32 DeviceComplexity : 1;
    UINT32 MaxMemoryReadByteCount : 2;
    UINT32 MaxOutstandingSplitTransactions : 3;
    UINT32 MaxCumulativeReadSize : 3;
    UINT32 ReceivedSplitCompletionError : 1;
    UINT32 Reserved : 2;
#endif
} PCIX_STATUS_REGISTER_STRUCT;

typedef struct _CAPABILITY_REGISTER_SET_STRUCT
{
    UINT8 CapabilityId;
    UINT8 NextCapability;
    PCIX_COMMAND_REGISTER_STRUCT CommandRegister;
    PCIX_STATUS_REGISTER_STRUCT StatusRegister;
} CAPABILITY_REGISTER_SET_STRUCT;

#pragma pack()
#endif
