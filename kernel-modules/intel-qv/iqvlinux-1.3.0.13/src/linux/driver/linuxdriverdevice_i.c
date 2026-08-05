
#include <nalcodes.h>
#include <naltypes.h>
#include "nalioctl.h"
#include <osdevice_i.h>
#include <os_i.h>
#include "linuxnaldriver.h"
#include "linuxpci_i.h"
#include <linux/sched.h>
#include <asm/io.h>
#include <linux/pci.h>

#if defined(NAL_IOMMU_API_PRESENT) || defined(NAL_IOMMU_DMA_MEM_ALLOC)
#include <linux/iommu.h>
#endif

#ifdef NAL_IOMMU_DMA_MEM_ALLOC
#include <linux/dma-map-ops.h>
#ifdef IOMMU_SETUP_DMA_OPS_IN_DMAIOMMUH
#include <linux/dma-iommu.h>
#endif
#endif

#ifndef DMA_BIT_MASK
#define DMA_BIT_MASK(n) (((n) == 64) ? ~0ULL : ((1ULL<<(n))-1))
#endif

extern NAL_OS_SPEC_ADAPTER_IN_USE_TABLE Global_AdapterInUse[NAL_OS_SPEC_MAX_PCI_DEVICES];

BOOLEAN
_NalCheckAndClearAdapterInterrupt(
    IN NAL_MAC_TYPE MacType,
    IN KVOID* BaseAddress
    );

void
_NalServiceRoutine(
    IN int Irq,
    IN void* Context,
    IN struct pt_regs* Regs
    );

NAL_STATUS
_NalEnableDevice(
    IN NAL_DEVICE_LOCATION DeviceLocation,
    OUT NAL_IO_RESOURCE* IoResource,
    OUT KVOID** Pdev
    );

NAL_STATUS
_NalFillDeviceResource(
    IN NAL_DEVICE_LOCATION DeviceLocation,
    OUT NAL_IO_RESOURCE* IoResource,
    OUT KVOID** Pdev
    );

NAL_STATUS
_NalReleaseRegions(
    IN KVOID* PDev
    );

BOOLEAN
_NalMarkAdapterInUse(
    IN NAL_DEVICE_LOCATION* NalDevice,
    IN BOOLEAN Lock
    );

BOOLEAN
_NalIsAdapterInUse(
    IN NAL_DEVICE_LOCATION* NalDevice
    );

BOOLEAN
_NalMarkAdapterInUse(
    IN NAL_DEVICE_LOCATION* NalDevice,
    IN BOOLEAN Lock
    )
{
    UINTN i = 0;
    BOOLEAN CanBeUsed = FALSE;

    do
    {
        if(NalDevice == NULL)
        {
            break;
        }

        for(i = 0; i < NAL_OS_SPEC_MAX_PCI_DEVICES; i++)
        {

            if(Global_AdapterInUse[i].DeviceLocation.Reserved == NalDevice->Reserved)
            {

                if(Global_AdapterInUse[i].InUse == TRUE)
                {

                    if(Lock == FALSE)
                    {
                        Global_AdapterInUse[i].InUse = FALSE;
                        Global_AdapterInUse[i].DeviceLocation.Reserved = 0;
                        CanBeUsed = TRUE;
                        break;
                    }
                    else
                    {
                        CanBeUsed = FALSE;
                        break;
                    }
                }
                else if(Lock == TRUE)
                {

                    Global_AdapterInUse[i].InUse = TRUE;
                    CanBeUsed = TRUE;
                    break;
                }
            }
        }

        if(i == NAL_OS_SPEC_MAX_PCI_DEVICES && Lock == TRUE)
        {

            for(i=0; i<NAL_OS_SPEC_MAX_PCI_DEVICES; i++)
            {
                if(Global_AdapterInUse[i].DeviceLocation.Reserved == 0)
                {
                    break;
                }
            }

            if(i<NAL_OS_SPEC_MAX_PCI_DEVICES)
            {
                Global_AdapterInUse[i].DeviceLocation.Reserved = NalDevice->Reserved;
                Global_AdapterInUse[i].InUse = Lock;
                CanBeUsed = TRUE;
            }
        }
        else if(i == NAL_OS_SPEC_MAX_PCI_DEVICES && Lock == FALSE)
        {

            CanBeUsed = TRUE;
        }
    } while(0);

    return CanBeUsed;
}

BOOLEAN
_NalIsAdapterInUse(
    IN NAL_DEVICE_LOCATION* NalDevice
    )
{
    UINTN i = 0;
    BOOLEAN IsInUse = FALSE;

    do
    {
        if(NalDevice == NULL)
        {
            break;
        }

        for(i = 0; i < NAL_OS_SPEC_MAX_PCI_DEVICES; i++)
        {

            if(Global_AdapterInUse[i].DeviceLocation.Reserved == NalDevice->Reserved)
            {

                if(Global_AdapterInUse[i].InUse == TRUE)
                {
                    IsInUse = TRUE;
                    break;
                }
            }
        }
    } while(0);

    return IsInUse;
}

void
_NalServiceRoutine(
    IN int Irq,
    IN void* Context,
    IN struct pt_regs* Regs
    )
{
    NAL_OS_SPEC_ISR_DEVICE* NalIsrDevice = (NAL_OS_SPEC_ISR_DEVICE*)Context;

    if(NalIsrDevice != NULL)
    {

        NalIsrDevice->DeviceInterrupted =
            _NalCheckAndClearAdapterInterrupt((NAL_MAC_TYPE)NalIsrDevice->MacType,
                                              NalIsrDevice->HardwareVirtualAddress);
    }
}

NAL_STATUS
_NalInitializeInterrupts(
    IN NAL_OS_SPEC_ISR_DEVICE* NalIsrDevice
    )
{
    return NAL_NOT_IMPLEMENTED;
}

NAL_STATUS
_NalUninitializeInterrupts(
    IN NAL_OS_SPEC_ISR_DEVICE* NalIsrDevice
    )
{
    return NAL_NOT_IMPLEMENTED;
}

BOOLEAN
_NalHasInterruptOccurred(
    IN NAL_OS_SPEC_ISR_DEVICE* NalIsrDevice
    )
{
    BOOLEAN InterruptOccurred = FALSE;

    if(NalIsrDevice != NULL && NalIsrDevice->Signature == NAL_OS_SPEC_INTERRUPT_SIGNATURE)
    {
        InterruptOccurred = NalIsrDevice->DeviceInterrupted;
    }

    return InterruptOccurred;
}

BOOLEAN
_NalCheckAndClearAdapterInterrupt(
    IN NAL_MAC_TYPE MacType,
    IN KVOID* BaseAddress
    )
{
    return FALSE;
}

NAL_STATUS
NalResolveOsSpecificIoctl(
    IN NAL_IOCTL_INPUT_DATA* NalIoctl
    )
{
    NAL_STATUS NalStatus = NAL_INVALID_PARAMETER;

    if(NalIoctl != NULL)
    {
        switch(NalIoctl->FunctionId)
        {
           case NAL_LINUX_ADAPTER_IN_USE_FUNCID:
                {
                    NAL_LINUX_ADAPTER_IN_USE_FUNC* FunctionData =
                        (NAL_LINUX_ADAPTER_IN_USE_FUNC*)(&(NalIoctl->InputBuffer));

                    printk(KERN_DEBUG "calling ioctl In mark adapter lock \n");
                    if(FunctionData != NULL)
                    {
                        FunctionData->CanBeUsed = _NalMarkAdapterInUse(&FunctionData->NalDevice,
                                                                       FunctionData->Locked);
                        NalStatus = NAL_SUCCESS;
                    }
                    else
                    {
                        NalStatus = NAL_INVALID_PARAMETER;
                        NalDebugPrint("NalResolveOsSpecificIoctl: NAL_LINUX_ADAPTER_IN_USE_FUNCID FunctionData is NULL\n");
                    }
                }
                break;

           case NAL_LINUX_IS_ADAPTER_IN_USE_FUNCID:
                {
                    NAL_LINUX_IS_ADAPTER_IN_USE_FUNC* FunctionData =
                        (NAL_LINUX_IS_ADAPTER_IN_USE_FUNC*)(&(NalIoctl->InputBuffer));

                    if(FunctionData != NULL)
                    {
                        FunctionData->IsInUse = _NalIsAdapterInUse(&FunctionData->NalDevice);
                        NalStatus = NAL_SUCCESS;
                    }
                    else
                    {
                        NalStatus = NAL_INVALID_PARAMETER;
                        NalDebugPrint("NalResolveOsSpecificIoctl: NAL_LINUX_IS_ADAPTER_IN_USE_FUNCID FunctionData is NULL\n");
                    }
                }
                break;

            case NAL_LINUX_REQUEST_REGIONS_FUNCID:
                {
                    NAL_LINUX_DEVICERESOURCE_FUNC* FunctionData =
                        (NAL_LINUX_DEVICERESOURCE_FUNC*)(&(NalIoctl->InputBuffer));

                    if(FunctionData != NULL)
                    {
                        FunctionData->ReturnValue = _NalEnableDevice(FunctionData->DeviceLocation,
                                                                     &(FunctionData->NalIoResource[0]),
                                                                     &FunctionData->Pdev);
                        NalStatus = NAL_SUCCESS;
                    }
                    else
                    {
                        NalStatus = NAL_INVALID_PARAMETER;
                        NalDebugPrint("NalResolveOsSpecificIoctl: NAL_LINUX_REQUEST_REGIONS_FUNCID FunctionData is NULL\n");
                    }
                }
                break;

            case NAL_LINUX_RELEASE_REGIONS_FUNCID:
                {
                    NAL_LINUX_DEVICERESOURCE_FUNC* FunctionData =
                        (NAL_LINUX_DEVICERESOURCE_FUNC*)(&(NalIoctl->InputBuffer));
                    NalDebugPrint("NalResolveOsSpecificIoctl: NAL_LINUX_RELEASE_REGIONS_FUNCID FunctionData is NULL\n");

                    if(FunctionData != NULL)
                    {
                        FunctionData->ReturnValue = _NalReleaseRegions(FunctionData->Pdev);
                        NalStatus = NAL_SUCCESS;
                    }
                    else
                    {
                        NalStatus = NAL_INVALID_PARAMETER;
                        NalDebugPrint("NalResolveOsSpecificIoctl: NAL_LINUX_RELEASE_REGIONS_FUNCID FunctionData is NULL\n");
                    }
                }
                break;

            case NAL_LINUX_INC_DRIVER_REFCOUNT_FUNCID:
                {
                    _NalDriverIncrementReferenceCount();
                    NalStatus = NAL_SUCCESS;
                }
                break;

            case NAL_LINUX_DEC_DRIVER_REFCOUNT_FUNCID:
                {
                    _NalDriverDecrementReferenceCount();
                    NalStatus = NAL_SUCCESS;
                }
                break;

            case NAL_LINUX_GET_DRIVER_REFCOUNT_FUNCID:
                {
                    NAL_LINUX_REFCOUNT_FUNCS* FunctionData =
                        (NAL_LINUX_REFCOUNT_FUNCS*)(&(NalIoctl->InputBuffer));

                    if(FunctionData != NULL)
                    {
                        FunctionData->ReturnValue = _NalDriverGetReferenceCount();
                        NalStatus = NAL_SUCCESS;
                    }
                    else
                    {
                        NalStatus = NAL_INVALID_PARAMETER;
                    }
                }
                break;

            case NAL_LINUX_DRIVER_GET_VERSION:
                {
                    NAL_LINUX_DRIVER_GET_VERSION_FUNCS* FunctionData =
                        (NAL_LINUX_DRIVER_GET_VERSION_FUNCS*)(&(NalIoctl->InputBuffer));

                   NalDebugPrint("NalResolveOsSpecificIoctl: Nal get version Ioctl\n");

                    if(FunctionData != NULL)
                    {
                        _NalDriverGetVersion(FunctionData->Version);
                        NalStatus = NAL_SUCCESS;
                    }
                    else
                    {
                        NalStatus = NAL_INVALID_PARAMETER;
                    }
                }
                break;

            case NAL_LINUX_FILL_DEVICE_RESOURCE_FUNCID:
                {
                    NAL_LINUX_DEVICERESOURCE_FUNC* FunctionData =
                        (NAL_LINUX_DEVICERESOURCE_FUNC*)(&(NalIoctl->InputBuffer));

                    NalDebugPrint("NalResolveOsSpecificIoctl: Nal Fill device resource Ioctl\n");

                    if(FunctionData != NULL)
                    {
                        FunctionData->ReturnValue = _NalFillDeviceResource(FunctionData->DeviceLocation,
                                                                           &(FunctionData->NalIoResource[0]),
                                                                           &FunctionData->Pdev);
                        NalStatus = NAL_SUCCESS;
                    }
                    else
                    {
                        NalStatus = NAL_INVALID_PARAMETER;
                        NalDebugPrint("NalResolveOsSpecificIoctl: NAL_LINUX_FILL_DEVICE_RESOURCE_FUNCID FunctionData is NULL\n");
                    }
                }
                break;

            case NAL_LINUX_FREEMEMORYNONPAGEDPCI_FUNCID:
                {
                    NAL_LINUX_FREEMEMORYNONPAGEDPCI_FUNC* FunctionData =
                        (NAL_LINUX_FREEMEMORYNONPAGEDPCI_FUNC*)(&(NalIoctl->InputBuffer));

                        NalFreeMemoryNonPagedPci(FunctionData->PDev,
                                               FunctionData->Address,
                                               FunctionData->PhysicalAddress,
                                               FunctionData->Size);

                    NalStatus = NAL_SUCCESS;
                }
                break;

            case NAL_LINUX_ALLOCATEMEMORYNONPAGEDPCI_FUNCID:
                {
                    NAL_LINUX_ALLOCATEMEMORYNONPAGEDPCI_FUNC* FunctionData =
                        (NAL_LINUX_ALLOCATEMEMORYNONPAGEDPCI_FUNC*)(&(NalIoctl->InputBuffer));

                        FunctionData->ReturnValue =
                            _NalAllocateMemoryNonPagedPci(FunctionData->PDev,
                                                       FunctionData->ByteCount,
                                                       FunctionData->Alignment,
                                                       &FunctionData->PhysicalAddress,
                                                       NULL,
                                                       0);

                    NalStatus = NAL_SUCCESS;
                }
                break;

            case NAL_LINUX_READPCIEXBYTE_FUNCID:
                {
                    NAL_LINUX_READPCIEXBYTE_FUNC* FunctionData =
                        (NAL_LINUX_READPCIEXBYTE_FUNC*)(&(NalIoctl->InputBuffer));

                    FunctionData->ReturnValue =
                        _NalOsReadPciExByte(FunctionData->PciLocation,
                                                   FunctionData->ByteIndex,
                                                   &FunctionData->Value);

                    NalStatus = NAL_SUCCESS;
                }
                break;

            case NAL_LINUX_WRITEPCIEXBYTE_FUNCID:
                {
                    NAL_LINUX_WRITEPCIEXBYTE_FUNC* FunctionData =
                        (NAL_LINUX_WRITEPCIEXBYTE_FUNC*)(&(NalIoctl->InputBuffer));
                    FunctionData->ReturnValue =
                        _NalOsWritePciExByte(FunctionData->PciLocation,
                                                    FunctionData->ByteIndex,
                                                    FunctionData->Value);

                    NalStatus = NAL_SUCCESS;
                }
                break;

            case NAL_LINUX_GET_RUN_DOMAIN_FUNCID:
                {
                    NAL_LINUX_RUN_DOMAIN_FUNC* FunctionData =
                        (NAL_LINUX_RUN_DOMAIN_FUNC*)(&(NalIoctl->InputBuffer));

                    printk(KERN_DEBUG "calling ioctl get run domain\n");
                    if(FunctionData != NULL)
                    {
                        FunctionData->RunDomain = _NalGetOsRunDomain();
                        FunctionData->ReturnValue = NAL_SUCCESS;
                        NalStatus = NAL_SUCCESS;
                    }
                    else
                    {
                        NalStatus = NAL_INVALID_PARAMETER;
                        NalDebugPrint("NalResolveOsSpecificIoctl: NAL_LINUX_GET_RUN_DOMAIN_FUNCID FunctionData is NULL\n");
                    }
                }
                break;

            case NAL_LINUX_IS_IOMMU_ENABLED_FUNCID:
                {
                    NAL_LINUX_IS_IOMMU_ENABLED_FUNC* FunctionData =
                        (NAL_LINUX_IS_IOMMU_ENABLED_FUNC*)(&(NalIoctl->InputBuffer));
#if defined(NAL_IOMMU_API_PRESENT) && !defined(NAL_IOMMU_PRESENT_AVAILABLE)

                    FunctionData->ReturnValue = NAL_IOMMU_STATE_ENABLED;
#elif defined(NAL_IOMMU_API_PRESENT) && defined(NAL_IOMMU_PRESENT_AVAILABLE)
                    if(iommu_present(&pci_bus_type) == TRUE)
                    {
                        FunctionData->ReturnValue = NAL_IOMMU_STATE_ENABLED;
                    }
                    else
                    {
                        FunctionData->ReturnValue = NAL_IOMMU_STATE_DISABLED;
                    }
#else
                    FunctionData->ReturnValue = NAL_IOMMU_STATE_NOT_IMPLEMENTED;
#endif
                }
                break;

           default:
                NalStatus = NAL_IOCTL_INVALID_FUNCTION_ID;
                break;
        }
    }

    return NalStatus;
}

NAL_STATUS
_NalEnableDevice(
    IN NAL_DEVICE_LOCATION DeviceLocation,
    OUT NAL_IO_RESOURCE* IoResource,
    OUT KVOID** PDev
    )
{
    struct pci_dev* PciDevice = NULL;
    struct pci_bus* LinuxPciBus = NULL;
    NAL_STATUS NalStatus = NAL_SUCCESS;
    UINT32 i = 0;
    unsigned int DeviceFunction = 0;
    int Result = 0;

    DeviceFunction = PCI_DEVFN(DeviceLocation.Pci.Device, DeviceLocation.Pci.Function);

    do
    {

        LinuxPciBus = pci_find_bus(DeviceLocation.Pci.Segment, DeviceLocation.Pci.Bus);
        if(LinuxPciBus == NULL)
        {
            NalStatus = NAL_PCICONFIG_NOT_AVAILABLE;
            break;
        }

        PciDevice = pci_get_slot(LinuxPciBus, DeviceFunction);
        if(PciDevice == NULL)
        {
            NalStatus = NAL_PCICONFIG_NOT_AVAILABLE;
            break;
        }

        Result = pci_enable_device(PciDevice);
        if(Result != 0)
        {
            NalStatus = NAL_PCICONFIG_NOT_AVAILABLE;
            break;
        }

        if (dma_set_mask_and_coherent(&PciDevice->dev, DMA_BIT_MASK(64)))
        {
            if (dma_set_mask_and_coherent(&PciDevice->dev, DMA_BIT_MASK(32)))
            {
                    printk(KERN_DEBUG "No usable DMA configuration, aborting\n");
                    NalStatus = NAL_DMA_NOT_SUPPORTED;
                    break;
            }
        }

        for(i=0; i<6; i++)
        {

            if(pci_resource_len(PciDevice, i) == 0)
            {
                continue;
            }

            if(pci_resource_flags(PciDevice, i) & IORESOURCE_IO)
            {
                request_region(pci_resource_start(PciDevice, i),
                               pci_resource_len(PciDevice, i),
                               NAL_OS_SPEC_QV_DRIVER_NAME);
                IoResource[i].MemoryAddress = pci_resource_start(PciDevice, i);
                IoResource[i].Type = NAL_IO_TYPE_IO;
            }
            else if(pci_resource_flags(PciDevice, i) & IORESOURCE_MEM)
            {
                if(request_mem_region(pci_resource_start(PciDevice, i),
                                       pci_resource_len(PciDevice, i),
                                       NAL_OS_SPEC_QV_DRIVER_NAME))
                {
                    IoResource[i].MemoryAddress = pci_resource_start(PciDevice, i);
                }
                else
                {
                    IoResource[i].MemoryAddress = 0;
                }
                IoResource[i].Type = NAL_IO_TYPE_MEM;
            }
        }

        pci_set_master(PciDevice);

#ifdef NAL_IOMMU_DMA_MEM_ALLOC

        iommu_setup_dma_ops(&PciDevice->dev, 0ULL, ~0ULL);
#endif

        *PDev = PciDevice;
    } while(0);

    return NalStatus;
}

NAL_STATUS
_NalFillDeviceResource(
    IN NAL_DEVICE_LOCATION DeviceLocation,
    OUT NAL_IO_RESOURCE* IoResource,
    OUT KVOID** PDev
    )
{
    struct pci_dev* PciDevice = NULL;
    struct pci_bus* LinuxPciBus = NULL;
    NAL_STATUS NalStatus = NAL_SUCCESS;
    UINT32 i = 0;
    unsigned int DeviceFunction = 0;

    DeviceFunction = PCI_DEVFN(DeviceLocation.Pci.Device, DeviceLocation.Pci.Function);

    do
    {

        LinuxPciBus = pci_find_bus(DeviceLocation.Pci.Segment, DeviceLocation.Pci.Bus);
        if(LinuxPciBus == NULL)
        {
            NalStatus = NAL_PCICONFIG_NOT_AVAILABLE;
            break;
        }

        PciDevice = pci_get_slot(LinuxPciBus, DeviceFunction);
        if(PciDevice == NULL)
        {
            NalStatus = NAL_PCICONFIG_NOT_AVAILABLE;
            break;
        }

        for(i = 0; i < NAL_MAX_BARS; i++)
        {

            if(pci_resource_len(PciDevice, i) == 0)
            {
                continue;
            }

            if(pci_resource_flags(PciDevice, i) & IORESOURCE_IO)
            {
                IoResource[i].MemoryAddress = pci_resource_start(PciDevice, i);
                IoResource[i].Type = NAL_IO_TYPE_IO;
            }
            else if(pci_resource_flags(PciDevice, i) & IORESOURCE_MEM)
            {
                IoResource[i].MemoryAddress = pci_resource_start(PciDevice, i);
                IoResource[i].Type = NAL_IO_TYPE_MEM;
            }
        }

        *PDev = PciDevice;
    } while(0);

    return NalStatus;
}

NAL_STATUS
_NalReleaseRegions(
    IN KVOID* PDev
    )
{
    struct pci_dev* PciDevice = (struct pci_dev*)PDev;
    NAL_STATUS NalStatus = NAL_SUCCESS;
    UINT32 i = 0;

    for(i=0; i<6; i++)
    {

        if(pci_resource_len(PciDevice, i) == 0)
        {
            continue;
        }

        if(pci_resource_flags(PciDevice, i) & IORESOURCE_IO)
        {
            release_region(pci_resource_start(PciDevice, i),
                           pci_resource_len(PciDevice, i));
        }
        else if(pci_resource_flags(PciDevice, i) & IORESOURCE_MEM)
        {
            release_mem_region(pci_resource_start(PciDevice, i),
                               pci_resource_len(PciDevice, i));
        }
    }
    pci_dev_put(PciDevice);
    pci_disable_device(PciDevice);

    return NalStatus;
}
