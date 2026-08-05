
#ifndef SDK_NAL_INC_FREEBSD_FREEBSDPCI_I_H_
#define SDK_NAL_INC_FREEBSD_FREEBSDPCI_I_H_ 

#include <hwbus_t.h>

#ifndef NAL_DRIVER

#include <net/if.h>

#endif
UINT16
_NalReadPciDeviceCount(
    VOID
    );

NAL_STATUS
_NalEnablePciDevice(
    IN NAL_DEVICE_LOCATION* DeviceLocation
    );

NAL_STATUS
_NalReadPciDevice(
    OUT NAL_DEVICE_LOCATION* PciLocations,
    IN OUT UINT16* Count
    );

NAL_STATUS
_NalFillPciConfigSpec(
    IN NAL_DEVICE_LOCATION* PciLocation,
    OUT PCI_DEVICE* Device,
    IN UINT32 DwordCount
    );

NAL_STATUS
_NalOSReadPciConfig32(
    IN NAL_DEVICE_LOCATION* PciLocation,
    IN UINT32 DwordNumber,
    OUT UINT32* Value
    );

NAL_STATUS
_NalOSWritePciConfig32(
    IN NAL_DEVICE_LOCATION* PciLocation,
    IN UINT32 DwordNumber,
    IN UINT32 Data
    );

NAL_STATUS
_NalOSWritePciConfigVariable(
    IN NAL_DEVICE_LOCATION* PciLocation,
    IN UINT32 DwordNumber,
    IN UINT8 ByteMask,
    IN UINT32 Data
    );

NAL_STATUS
_NalOsWritePciExConfigSpace(
    IN NAL_DEVICE_LOCATION DeviceLocation,
    IN UINT32 ByteOffset,
    IN UINT32 NoOfBytes,
    IN UINT8* Data
    );

NAL_STATUS
_NalOsReadPciExConfigSpace(
    IN NAL_DEVICE_LOCATION DeviceLocation,
    IN UINT32 ByteOffset,
    IN UINT32 NoOfBytes,
    OUT UINT8* Data
    );

NAL_STATUS
_NalOSReadPciExConfig32(
    IN NAL_DEVICE_LOCATION PciLocation,
    IN UINT32 DwordNumber,
    OUT UINT32* Data
    );

NAL_STATUS
_NalOsReadPciExByte(
    IN NAL_DEVICE_LOCATION DeviceLocation,
    IN UINT32 ByteIndex,
    OUT UINT8* Value
    );

NAL_STATUS
_NalOsWritePciExByte(
    IN NAL_DEVICE_LOCATION DeviceLocation,
    IN UINT32 ByteOffset,
    IN UINT8 Data
    );

NAL_STATUS
_NalOSWritePciExConfig32(
    IN NAL_DEVICE_LOCATION PciLocation,
    IN UINT32 DwordNumber,
    IN UINT32 Data
    );

NAL_STATUS
_NalOsPciDeviceEnableSysfs(
        IN NAL_DEVICE_LOCATION PciLocation
        );

#endif
