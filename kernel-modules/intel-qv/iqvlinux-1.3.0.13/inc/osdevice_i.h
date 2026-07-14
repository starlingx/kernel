
#ifndef _OSDEVICE_I_H_
#define _OSDEVICE_I_H_ 

#ifndef NAL_DRIVER
#include <device_t.h>

NAL_STATUS
_NalInitializeAdapterOs(
    IN NAL_DEVICE_LOCATION DeviceLocation,
    IN VOID* DeviceInfo,
    IN NAL_ADAPTER_STRUCTURE* NalAdapter,
    IN UINT32 InitFlags
    );

NAL_STATUS
_NalPostInitializeAdapterOs(
    IN NAL_ADAPTER_STRUCTURE* NalAdapter
    );

NAL_STATUS
_NalReleaseAdapterOs(
    IN NAL_ADAPTER_STRUCTURE* NalAdapter
    );

NAL_STATUS
_NalInitializeInterruptsOs(
    IN NAL_ADAPTER_STRUCTURE* NalAdapter
    );

NAL_STATUS
_NalUninitializeInterruptsOs(
    IN NAL_ADAPTER_STRUCTURE* NalAdapter
    );

BOOLEAN
_NalHasInterruptOccurredOs(
    IN NAL_ADAPTER_STRUCTURE* NalAdapter
    );

NAL_STATUS
_NalClearInterruptsOs(
    IN NAL_ADAPTER_STRUCTURE* NalAdapter
    );

BOOLEAN
_NalIsDeviceDriverLoadedOs(
    IN NAL_ADAPTER_STRUCTURE* NalAdapter
    );

NAL_STATUS
_NalUnloadDeviceDriverOs(
    IN NAL_ADAPTER_STRUCTURE* NalAdapter
    );

BOOLEAN
_NalReloadDeviceDriverOs(
    IN NAL_ADAPTER_STRUCTURE* NalAdapter
    );

UINT8
_NalReadPortOs8(
    IN NAL_ADAPTER_STRUCTURE* NalAdapter,
    IN PORT_ADDR Port
    );

UINT16
_NalReadPortOs16(
    IN NAL_ADAPTER_STRUCTURE* NalAdapter,
    IN PORT_ADDR Port
    );

UINT32
_NalReadPortOs32(
    IN NAL_ADAPTER_STRUCTURE* NalAdapter,
    IN PORT_ADDR Port
    );

BOOLEAN
_NalWritePortOs8(
    IN NAL_ADAPTER_STRUCTURE* NalAdapter,
    IN PORT_ADDR Port,
    IN UINT8 Value
    );

BOOLEAN
_NalWritePortOs16(
    IN NAL_ADAPTER_STRUCTURE* NalAdapter,
    IN PORT_ADDR Port,
    IN UINT16 Value
    );

BOOLEAN
_NalWritePortOs32(
    IN NAL_ADAPTER_STRUCTURE* NalAdapter,
    IN PORT_ADDR Port,
    IN UINT32 Value
    );

UINT8
_NalReadRegisterOs8(
    IN NAL_ADAPTER_STRUCTURE* NalAdapter,
    IN KVOID* Address
    );

UINT16
_NalReadRegisterOs16(
    IN NAL_ADAPTER_STRUCTURE* NalAdapter,
    IN KVOID* Address
    );

UINT32
_NalReadRegisterOs32(
    IN NAL_ADAPTER_STRUCTURE* NalAdapter,
    IN KVOID* Address
    );

BOOLEAN
_NalWriteRegisterOs8(
    IN NAL_ADAPTER_STRUCTURE* NalAdapter,
    IN KVOID* Address,
    IN UINT8 Value
    );

BOOLEAN
_NalWriteRegisterOs16(
    IN NAL_ADAPTER_STRUCTURE* NalAdapter,
    IN KVOID* Address,
    IN UINT16 Value
    );

BOOLEAN
_NalWriteRegisterOs32(
    IN NAL_ADAPTER_STRUCTURE* NalAdapter,
    IN KVOID* Address,
    IN UINT32 Value
    );

KVOID*
_NalAllocateMemoryNonPagedOs(
    IN NAL_ADAPTER_STRUCTURE* NalAdapter,
    IN UINT32 AllocationAmount,
    IN UINT32 Alignment,
    OUT NAL_PHYSICAL_ADDRESS* PhysicalAddress
    );

VOID
_NalFreeMemoryNonPagedOs(
    IN NAL_ADAPTER_STRUCTURE* NalAdapter,
    IN KVOID* Address
    );

BOOLEAN
_NalMarkAdapterInUse(
    IN NAL_ADAPTER_STRUCTURE* NalAdapter,
    IN NAL_DEVICE_LOCATION* NalDevice,
    IN BOOLEAN Lock
    );

BOOLEAN
_NalIsDriverlessModeOs(
    VOID
    );

VOID
_NalSetQvDriverModeOs(
    VOID
    );

VOID
_NalForceDriverlessModeOs(
    VOID
    );

VOID
_NalRestoreInitialDriverModeOs(
    VOID
    );

NAL_STATUS
_NalIsMemoryBarExclusive(
    IN NAL_DEVICE* NalDevice,
    OUT BOOLEAN* IsExclusive
    );

NAL_STATUS
_NalIsRuntimeD3Enabled(
    IN NAL_ADAPTER_STRUCTURE* NalAdapter,
    OUT BOOLEAN* IsEnabled
    );

NAL_STATUS
_NalEnableRuntimeD3(
    IN NAL_ADAPTER_STRUCTURE* NalAdapter,
    IN BOOLEAN Enable
    );

#endif

#if defined(NAL_EFI2)
#include <efi2/efidevice_i.h>

#elif defined(NAL_LINUX)
#if !defined(NAL_DRIVER)
#include <linux/linuxdevice_i.h>
#endif

#elif defined(NAL_ESXI)
#if defined(NAL_DRIVER)
#include <esxidriverdevice_i.h>
#else
#include <esxi/esxidevice_i.h>
#endif

#elif defined(NAL_ESXIN)
#if defined(NAL_DRIVER)
#include <esxindriverdevice_i.h>
#else
#include <esxindevice_i.h>
#endif

#elif defined(NAL_SOLARIS)
#if defined(NAL_DRIVER)
#include <solaris/solarisdriverdevice_i.h>
#else
#include <solaris/solarisdevice_i.h>
#endif

#elif defined(NAL_WINNT_WDM)
#if defined(NAL_DRIVER)
#include <winnt_wdm\windriverdevice_i.h>
#else
#include <winnt_wdm\windevice_i.h>
#endif

#elif defined(NAL_FREEBSD)
#include <freebsddevice_i.h>
#include <freebsdtypes.h>
#else

#error "Undefined NAL Operating System in osdevice_i.h."
#endif

typedef struct _NAL_OS_DEVICE
{
    NAL_MAC_TYPE MacType;
    NAL_DEVICE_LOCATION DeviceLocation;
    NAL_PHYSICAL_ADDRESS PhysicalAddress;
    KVOID* VirtualAddress;
    PORT_ADDR PortAddress;
    BOOLEAN IsDriverLoaded;
    BOOLEAN HasInterruptOccurred;

    NAL_OS_SPECIFIC_DEVICE OsSpecificDevice;

} NAL_OS_DEVICE;

#endif
