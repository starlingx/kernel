
#ifndef _NAL_HWBUS_TYPES_H_
#define _NAL_HWBUS_TYPES_H_ 

#include "pci_t.h"
#include "cardbus_t.h"

#define NAL_MAX_BARS 6
#define NAL_DEVICE_SIGNATURE 0xA55A5AA5

#if defined(NAL_EFI2)
typedef struct _NAL_OS_SLOT_ID
{
    PCI_SLOT_ID Pci;
    UINT32 Signature;
    VOID* Handle;
} NAL_OS_SLOT_ID;

#elif defined(NAL_LINUX) || defined(NAL_ESXI)
typedef struct _NAL_OS_SLOT_ID
{
    PCI_SLOT_ID Pci;
    UINT32 Signature;
} NAL_OS_SLOT_ID;
#elif defined(NAL_ESXIN)
typedef struct _NAL_OS_SLOT_ID
{
    PCI_SLOT_ID Pci;
    UINT32 Signature;
    NAL_BASE_DRIVER_ID BaseDriverId;
    BOOLEAN IsNativeDevice;
} NAL_OS_SLOT_ID;
#elif defined(NAL_SOLARIS)
typedef struct _NAL_OS_SLOT_ID
{
    PCI_SLOT_ID Pci;
    UINT32 Signature;
    INT32 DevInfo;
} NAL_OS_SLOT_ID;
#elif defined(NAL_WINNT_WDM)

typedef struct _NAL_OS_SLOT_ID
{
    PCI_SLOT_ID Pci;
    CHAR RegistryKey[256];
    UINT32 Signature;
} NAL_OS_SLOT_ID;

#elif defined(NAL_FREEBSD)
typedef struct _NAL_OS_SLOT_ID
{
    PCI_SLOT_ID Pci;
    UINT32 Signature;
} NAL_OS_SLOT_ID;
#else

#error "Undefined NAL Operating System in hwbus_t.h."
#endif

typedef union _VF_SLOT_ID
{
    struct
    {
        UINT32 SlotId;
        UINT32 ReservedForSignature;
        UINT32 ParentSlotId;
        UINT16 SupportedModule;
        BOOLEAN IsVirtualDevice;
        UINT8 Pad;
    } slot_id;
    struct
    {
        struct
        {
#if defined (NAL_BIG_ENDIAN)
            UINT32 DwordLo : 2;
            UINT32 _Byte : 2;
            UINT32 Reserved : 4;
            UINT32 DwordHi : 8;
            UINT32 Device : 7;
            UINT32 Function : 1;
            UINT32 Bus : 8;
#else
            UINT32 Bus : 8;
            UINT32 Device : 7;
            UINT32 Function : 1;
            UINT32 Dword : 10;
            UINT32 _Byte : 2;
            UINT32 Reserved : 4;
#endif
        } vf_slot_id;
        UINT32 ReservedForSignature;
        PCI_EXPRESS_SLOT_ID ParentDevice;
    };
} VF_SLOT_ID;

typedef union _NAL_DEVICE_LOCATION
{
    PCI_SLOT_ID Pci;
    PCI_EXPRESS_SLOT_ID PciExpress;
    VF_SLOT_ID Vf;
    CARDBUS_SLOT_ID Cardbus;
    NAL_OS_SLOT_ID OsDeviceLocation;
    UINT64 Reserved;
} NAL_DEVICE_LOCATION;

#pragma pack(1)

typedef struct _NAL_SUBST_DEVICE_TABLE
{
    UINT16 DeviceId;
    NAL_DEVICE_LOCATION DeviceLocation;

} NAL_SUBST_DEVICE_TABLE;

typedef struct _PCIEXP_CONFIG_STORE
{
    NAL_DEVICE_LOCATION DeviceLocation;

    PCI_EXPRESS_CONFIG_SPACE ConfigSpace;
}PCIEXP_CONFIG_STORE;

typedef enum _NAL_IO_RESOURCE_TYPE
{
    NAL_IO_TYPE_UNUSED = 0,
    NAL_IO_TYPE_IO,
    NAL_IO_TYPE_MEM
} NAL_IO_RESOURCE_TYPE;

typedef struct _NAL_IO_RESOURCE
{
    NAL_IO_RESOURCE_TYPE Type;
    NAL_PHYSICAL_ADDRESS MemoryAddress;
} NAL_IO_RESOURCE;

typedef struct _NAL_INTERRUPT_INFO
{
    BOOLEAN Valid;
    UINT32 Irq;
    UINT32 Vector;
    UINT32 Trigger;
    UINT32 Affinity;
    UINT32 Mode;
    UINT64 Reserved;
} NAL_INTERRUPT_INFO;

typedef enum _NAL_DEVICE_CLASS
{
    NAL_CLASS_LEGACY = 0,
    NAL_CLASS_STORAGE,
    NAL_CLASS_NETWORK,
    NAL_CLASS_DISPLAY,
    NAL_CLASS_MULTIMEDIA,
    NAL_CLASS_MEMORY_CONTROLLER,
    NAL_CLASS_BRIDGE_DEVICES,
    NAL_CLASS_SIMPLE_COMMUNICATION,
    NAL_CLASS_SYSTEM,
    NAL_CLASS_USB,
    NAL_CLASS_UNKNOWN = 0xFF
} NAL_DEVICE_CLASS;

typedef UINT8 NAL_DEVICE_SUBCLASS;

typedef struct _NAL_DEVICE
{
    UINT32 Signature;

    UINT16 VendorId;

    UINT16 DeviceId;

    UINT16 SubSystemId;
    UINT16 SubVendorId;
    UINT8 RevisionId;
    NAL_DEVICE_CLASS Class;

    NAL_DEVICE_SUBCLASS SubClass;
    UINT32 ResourceCount;
    NAL_IO_RESOURCE IoResource[NAL_MAX_BARS];

    NAL_INTERRUPT_INFO InterruptInfo;

    NAL_DEVICE_LOCATION DeviceLocation;
    BOOLEAN DeviceIsOsConfigured;
} NAL_DEVICE;
#pragma pack()

typedef struct _NAL_ADAPTER_VENDOR_INFO
{
    UINT16 Vendor;
    UINT16 Device;
    UINT16 SubVendor;
    UINT16 SubDevice;
    UINT8 Revision;
} NAL_ADAPTER_VENDOR_INFO;

#endif
