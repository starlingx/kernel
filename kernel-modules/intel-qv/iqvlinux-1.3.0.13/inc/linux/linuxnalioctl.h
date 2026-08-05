
#ifndef SDK_NAL_INC_LINUX_LINUXNALIOCTL_H_
#define SDK_NAL_INC_LINUX_LINUXNALIOCTL_H_ 

#include <linuxdefs.h>
#include <naltypes.h>
#include <linuxtypes.h>

#ifndef NAL_DRIVER
#include <sys/ioctl.h>
#include <device_t.h>
#endif

#define NAL_MAKE_IOCTL(IoCtlNumber) (NAL_OS_SPEC_IOCTL_BASE + IoCtlNumber)

#ifndef NAL_DRIVER

#define NAL_SEND_IOCTL(NalIoctlNumber,InputBuffer,InputBufferSize,OutputBuffer,OutputBufferSize) \
    { \
        if(Global_OsVariables.NalFileDescriptor != -1) \
        { \
            ioctl(Global_OsVariables.NalFileDescriptor, NalIoctlNumber, InputBuffer); \
            UNREFERENCED_PARAMETER(InputBufferSize); \
            UNREFERENCED_PARAMETER(OutputBuffer); \
            UNREFERENCED_PARAMETER(OutputBufferSize); \
        } \
    }

KVOID*
_NalAllocateMemoryNonPagedPciIoctl(
    IN KVOID* PDev,
    IN UINT32 ByteCount,
    IN UINT32 Alignment,
    OUT NAL_PHYSICAL_ADDRESS* PhysicalAddress,
    IN CHAR* NamedLocator,
    IN UINT32 LineNumber
    );

VOID
_NalFreeMemoryNonPagedPciIoctl(
    IN KVOID* PDev,
    IN KVOID* Address,
    IN NAL_PHYSICAL_ADDRESS PhyAddr,
    IN UINT32 MemSize
    );

NAL_STATUS
_NalFillDeviceResourceIoctl(
    IN NAL_OS_SPEC_DEVICE_CONTEXT* DeviceContext,
    IN NAL_DEVICE_LOCATION PciLocation,
    IN OUT NAL_DEVICE* Device
    );

NAL_STATUS
_NalDeviceResourceIoctl(
    IN UINT64 FunctionId,
    IN NAL_OS_SPEC_DEVICE_CONTEXT* DeviceContext,
    IN NAL_DEVICE_LOCATION PciLocation,
    IN OUT NAL_DEVICE* Device
    );

NAL_STATUS
_NalRequestDeviceResourceIoctl(
    IN NAL_OS_SPEC_DEVICE_CONTEXT* DeviceContext,
    IN NAL_DEVICE_LOCATION PciLocation,
    IN OUT NAL_DEVICE* Device
    );

NAL_STATUS
_NalReleaseDeviceResourceIoctl(
    IN NAL_OS_SPEC_DEVICE_CONTEXT* DeviceContext,
    IN NAL_DEVICE_LOCATION PciLocation
    );

NAL_STATUS
_NalOsSpecReadPciExByteIoctl(
    IN NAL_DEVICE_LOCATION DeviceLocation,
    IN UINT32 ByteIndex,
    OUT UINT8* Value
    );

NAL_STATUS
_NalOsWritePciExByteIoctl(
    IN NAL_DEVICE_LOCATION DeviceLocation,
    IN UINT32 ByteIndex,
    IN UINT8 Value
    );

#else

#endif

#endif
