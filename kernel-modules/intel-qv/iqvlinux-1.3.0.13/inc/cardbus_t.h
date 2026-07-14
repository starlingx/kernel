
#include <naltypes.h>

#ifndef _CARDBUS_H_
#define _CARDBUS_H_ 

#define CARDBUS_0 (1 << 31)

#pragma pack(1)

typedef union _CARDBUS_SLOT_ID
{
    UINT32 SlotId;
#if defined(NAL_BIG_ENDIAN)
    struct _CARDBUS_PARTS
    {
        UINT32 Reserved : 7;
        UINT32 Enable : 1;
        UINT32 Bus : 8;
        UINT32 Function : 3;
        UINT32 Device : 5;
        UINT32 _Byte : 2;
        UINT32 Dword : 6;
    } CARDBUS_PARTS;
#else
    struct _CARDBUS_PARTS
    {
        UINT32 _Byte : 2;
        UINT32 Dword : 6;
        UINT32 Function : 3;
        UINT32 Device : 5;
        UINT32 Bus : 8;
        UINT32 Reserved : 7;
        UINT32 Enable : 1;
    } CARDBUS_PARTS;
#endif
} CARDBUS_SLOT_ID;

typedef union _CARDBUS_CIS_POINTER
{
    UINT32 CISPointer;
    struct _CARDBUS_CIS_POINTER_REGISTER
    {

        UINT32 AddressSpaceIndicator : 3;

        UINT32 AddressSpaceOffset : 25;

        UINT32 ROMImage : 4;

    } CARDBUS_CIS_POINTER_REGISTER;
} CARDBUS_CIS_POINTER;

typedef struct _CARDBUS_DEVICE
{
    UINT32 Reserved;
    UINT16 CommandRegister;
    UINT16 StatusRegister;
    UINT32 Reserved1;
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
    UINT32 Reserved2;
    UINT32 ExpansionRomBaseAddress;
    UINT32 Reserved3;
    UINT32 Reserved4;
    UINT8 Reserved5;
    UINT8 InterruptPin;
    UINT16 Reserved6;
} CARDBUS_DEVICE;

typedef struct _CARDBUS_BRIDGE
{
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
    UINT8 PciBusNumber;
    UINT8 CardbusBusNumber;
    UINT8 SubordinateBusNumber;
    UINT8 CardbusLatencyTimer;
    UINT32 Bar0;
    UINT32 Limit0;
    UINT32 Bar1;
    UINT32 Limit1;
    UINT16 IoBaseRegister0Lower;
    UINT16 IoBaseRegister0OptionalUpper;
    UINT16 IoLimitRegister0Lower;
    UINT16 IoLimitRegiseter0OptionalUpper;
    UINT16 IoBaseRegister1Lower;
    UINT16 IoBaseRegister1OptionalUpper;
    UINT16 IoLimitRegister1Lower;
    UINT16 IoLimitRegiseter1OptionalUpper;
    UINT8 InterruptLine;
    UINT8 InterruptPin;
    UINT16 BridgeControl;
    UINT16 SubsystemVendorId;
    UINT16 SubsystemId;
    UINT32 Optional16BitPcCardBaseAddress;
} CARDBUS_BRIDGE;

#pragma pack()
#endif
