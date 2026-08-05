
#ifndef _PCIINTERFACE_H_
#define _PCIINTERFACE_H_ 

#include <naltypes.h>
#include <nalcodes.h>
#include <hwbus_t.h>

#if defined(NAL_EFI2)
#include <efi2/efipci_i.h>

#elif defined(NAL_LINUX)
#include <linux/linuxpci_i.h>

#elif defined(NAL_ESXI)
#include <esxipci_i.h>

#elif defined(NAL_ESXIN)
#include <esxinpci_i.h>

#elif defined(NAL_SOLARIS)
#include <solaris/solarispci_i.h>

#elif defined(NAL_WINNT_WDM)
#include <winnt_wdm\winpci_i.h>

#elif defined(NAL_FREEBSD)
#include <freebsdpci_i.h>
#else

#error "Undefined NAL Operating System in pci_i.h."
#endif

#define PCI_CONFIG_CAPABILITIES_POINTER 0x34

NAL_STATUS
NalScanPciBus(
    OUT NAL_DEVICE_LOCATION* PciLocations,
    IN OUT UINT32* Count
    );

NAL_STATUS
NalGetPciDeviceInformation(
    IN NAL_DEVICE_LOCATION PciLocation,
    OUT PCI_DEVICE* Device,
    IN UINT32 DwordCount
    );

NAL_STATUS
NalReadPciConfig32(
    IN NAL_DEVICE_LOCATION PciLocation,
    IN UINT32 DwordIndex,
    OUT UINT32* Value
    );

NAL_STATUS
NalReadPciConfigVariable(
    IN NAL_DEVICE_LOCATION PciLocation,
    IN UINT32 DwordIndex,
    IN UINT8 ByteMask,
    OUT UINT32* Value
    );

NAL_STATUS
NalWritePciConfig32(
    IN NAL_DEVICE_LOCATION PciLocation,
    IN UINT32 DwordIndex,
    IN UINT32 Data
    );

NAL_STATUS
NalWritePciConfigVariable(
    IN NAL_DEVICE_LOCATION PciLocation,
    IN UINT32 DwordIndex,
    IN UINT8 ByteMask,
    IN UINT32 Data
    );

NAL_STATUS
_NalWritePciConfig16(
    IN NAL_DEVICE_LOCATION PciLocation,
    IN UINT32 ByteOffset,
    IN UINT16 Value
    );

NAL_STATUS
_NalReadPciConfig16(
    IN NAL_DEVICE_LOCATION PciLocation,
    IN UINT32 ByteOffset,
    IN UINT16* Value
    );

BOOLEAN
NalIsPcixAdapter(
    IN NAL_DEVICE_LOCATION* PciLocation,
    IN PCI_DEVICE* Device
    );

NAL_STATUS
NalIsPciExAdapter(
    IN NAL_DEVICE_LOCATION* PciLocation,
    IN PCI_DEVICE* Device
    );

VOID*
NalFindPciCapability(
    IN PCI_DEVICE* Device,
    IN UINT8 CapabilityId,
    OUT UINT32* ByteIndex
    );

BOOLEAN
NalHasPciCapability(
    IN NAL_DEVICE_LOCATION* DeviceLocation,
    IN NAL_PCI_CAPABILITY Capability,
    OUT UINT32* ByteIndex
    );

NAL_STATUS
NalGetPciMaxFunction(
    IN NAL_DEVICE_LOCATION* DeviceLocation,
    OUT UINT32* MaxFunction
    );

INT32
NalGetPhysicalSlotNumber(
    IN NAL_DEVICE_LOCATION* DeviceLocation
    );

#endif
