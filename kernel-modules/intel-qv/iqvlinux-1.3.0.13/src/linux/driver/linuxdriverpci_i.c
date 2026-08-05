
#include <nalcodes.h>
#include <naltypes.h>
#include <os_i.h>
#include <hwbus_t.h>
#include <pci_i.h>
#include "linuxnaldriver.h"
#include <linux/module.h>
#include <linux/pci.h>
#include <asm/byteorder.h>
#include <nalbytes.h>

#ifndef CONFIG_PCI
#error "This driver requires PCI support to be available"
#endif

UINT16
_NalReadPciDeviceCount(
    VOID
    )
{

    return 0;
}

NAL_STATUS
_NalReadPciDevice(
    OUT NAL_DEVICE_LOCATION* PciLocations,
    IN UINT16* Count
    )
{

    return NAL_NOT_IMPLEMENTED;
}

NAL_STATUS
_NalFillPciConfigSpec(
    IN NAL_DEVICE_LOCATION* PciLocation,
    OUT PCI_DEVICE* Device,
    IN UINT32 DwordCount
    )
{

    return NAL_NOT_IMPLEMENTED;
}

NAL_STATUS
_NalOSReadPciConfig32(
    IN NAL_DEVICE_LOCATION* PciLocation,
    IN UINT32 DwordNumber,
    OUT UINT32* Value
    )
{
    struct pci_dev* LinuxPciDevice = NULL;
    struct pci_bus* LinuxPciBus = NULL;
    NAL_STATUS NalStatus = NAL_SUCCESS;
    unsigned int DeviceFunction = 0;
    int Result = 0;

    do
    {
        if(PciLocation == NULL || Value == NULL)
        {
            NalStatus = NAL_INVALID_PARAMETER;
            break;
        }

        LinuxPciBus = pci_find_bus(PciLocation->Pci.Segment, PciLocation->Pci.Bus);
        if(LinuxPciBus == NULL)
        {
            NalStatus = NAL_INVALID_PCI_SLOT_ID;
            break;
        }

        DeviceFunction = PCI_DEVFN(PciLocation->Pci.Device, PciLocation->Pci.Function);

        LinuxPciDevice = pci_get_slot(LinuxPciBus, DeviceFunction);
        if(LinuxPciDevice == NULL)
        {
            NalStatus = NAL_INVALID_PCI_SLOT_ID;
            break;
        }

        Result = pci_read_config_dword(LinuxPciDevice, DwordNumber*4, (u32*)Value);
        if(Result != 0)
        {
            NalStatus = NAL_PCI_CAPABILITY_NOT_FOUND;
            *Value = 0;
        }

        pci_dev_put(LinuxPciDevice);
    } while(0);

    return NalStatus;
}

NAL_STATUS
_NalOSWritePciConfig32(
    IN NAL_DEVICE_LOCATION* PciLocation,
    IN UINT32 DwordNumber,
    IN UINT32 Data
    )
{
    struct pci_dev* LinuxPciDevice = NULL;
    struct pci_bus* LinuxPciBus = NULL;
    NAL_STATUS NalStatus = NAL_INVALID_PCI_SLOT_ID;
    unsigned int DeviceFunction = 0;
    int Result = 0;

    do
    {
        if(PciLocation == NULL)
        {
            NalStatus = NAL_INVALID_PARAMETER;
            break;
        }

        DeviceFunction = PCI_DEVFN(PciLocation->Pci.Device, PciLocation->Pci.Function);

        LinuxPciBus = pci_find_bus(PciLocation->Pci.Segment, PciLocation->Pci.Bus);

        if(LinuxPciBus != NULL)
        {

            LinuxPciDevice = pci_get_slot(LinuxPciBus, DeviceFunction);

            if(LinuxPciDevice != NULL)
            {
                Result = pci_write_config_dword(LinuxPciDevice, DwordNumber*4, Data);
                if(Result == 0)
                {
                    NalStatus = NAL_SUCCESS;
                }
                else
                {
                    NalStatus = NAL_PCICONFIG_NOT_AVAILABLE;
                }
            }
            pci_dev_put(LinuxPciDevice);
        }
    } while(0);

    return NalStatus;
}

NAL_STATUS
_NalOSWritePciConfigVariable(
    IN NAL_DEVICE_LOCATION* PciLocation,
    IN UINT32 DwordNumber,
    IN UINT8 ByteMask,
    IN UINT32 Data
    )
{
    struct pci_dev* LinuxPciDevice = NULL;
    struct pci_bus* LinuxPcibus = NULL;
    NAL_STATUS NalStatus = NAL_SUCCESS;
    unsigned int DeviceFunction = 0;
    int Result = 0;
    UINT16 Data16 = 0;
    UINT8 Data8 = 0;

    do
    {
        if(PciLocation == NULL)
        {
            NalStatus = NAL_INVALID_PARAMETER;
            break;
        }

        DeviceFunction = PCI_DEVFN(PciLocation->Pci.Device, PciLocation->Pci.Function);

        LinuxPcibus = pci_find_bus(PciLocation->Pci.Segment, PciLocation->Pci.Bus);
        if(LinuxPcibus == NULL)
        {
            break;
        }

        LinuxPciDevice = pci_get_slot(LinuxPcibus, DeviceFunction);

        if(LinuxPciDevice == NULL)
        {
            break;
        }

        if(ByteMask == PCI_CONFIG_BM_ENTIRE_DWORD)
        {
            Result = pci_write_config_dword(LinuxPciDevice, DwordNumber*4, Data);
            break;
        }

        if(ByteMask == PCI_CONFIG_BM_LOWORD)
        {
            Data16 = LOWORD(Data);
            Result = pci_write_config_word(LinuxPciDevice, DwordNumber*4, Data16);
            break;
        }

        if(ByteMask == PCI_CONFIG_BM_HIWORD)
        {
            Data16 = HIWORD(Data);
            Result = pci_write_config_word(LinuxPciDevice, (DwordNumber*4 + 2), Data16);
            break;
        }

        if(ByteMask & PCI_CONFIG_BM_BYTE1)
        {
            Data8 = (UINT8)Data;
            Result = pci_write_config_byte(LinuxPciDevice, (DwordNumber*4), Data8);
            if(Result != 0)
            {
                break;
            }
        }

        if(ByteMask & PCI_CONFIG_BM_BYTE2)
        {
            Data8 = (UINT8)(Data >> 8);
            Result = pci_write_config_byte(LinuxPciDevice, (DwordNumber*4 + 1), Data8);
            if(Result != 0)
            {
                break;
            }
        }

        if(ByteMask & PCI_CONFIG_BM_BYTE3)
        {
            Data8 = (UINT8)(Data >> 16);
            Result = pci_write_config_byte(LinuxPciDevice, (DwordNumber*4 + 2), Data8);
            if(Result != 0)
            {
                break;
            }
       }

        if(ByteMask & PCI_CONFIG_BM_BYTE4)
        {
            Data8 = (UINT8)(Data >> 24);
            Result = pci_write_config_byte(LinuxPciDevice, (DwordNumber*4 + 3), Data8);
        }
    } while(0);

    if(NalStatus == NAL_SUCCESS && Result != 0)
    {
        NalStatus = NAL_PCICONFIG_NOT_AVAILABLE;
    }

    return NalStatus;
}

NAL_STATUS
_NalEnablePciDevice(
    IN NAL_DEVICE_LOCATION* DeviceLocation
    )
{
    struct pci_dev* LinuxPciDevice = NULL;
    struct pci_bus* LinuxPciBus = NULL;
    unsigned int DeviceFunction = 0;
    int Result = 0;
    NAL_STATUS Status = NAL_SUCCESS;

    do
    {
        if(DeviceLocation == NULL)
        {
            Status = NAL_INVALID_PARAMETER;
            break;
        }

        DeviceFunction = PCI_DEVFN(DeviceLocation->Pci.Device, DeviceLocation->Pci.Function);

        LinuxPciBus = pci_find_bus(DeviceLocation->Pci.Segment, DeviceLocation->Pci.Bus);

        if(LinuxPciBus != NULL)
        {

            LinuxPciDevice = pci_get_slot(LinuxPciBus, DeviceFunction);

            Result = pci_enable_device(LinuxPciDevice);
            if(Result != 0)
            {
                Status = NAL_PCICONFIG_NOT_AVAILABLE;
            }
        }
    } while(0);

    return Status;
}

NAL_STATUS
_NalOSWritePciExConfig32(
    IN NAL_DEVICE_LOCATION PciLocation,
    IN UINT32 DwordNumber,
    IN UINT32 Data
    )
{
    return _NalOSWritePciConfig32(&PciLocation, DwordNumber, Data);
}

NAL_STATUS
_NalOSReadPciExConfig32(
    IN NAL_DEVICE_LOCATION PciLocation,
    IN UINT32 DwordNumber,
    IN UINT32* Value
    )
{
   return _NalOSReadPciConfig32(&PciLocation, DwordNumber, Value);
}

NAL_STATUS
_NalOsReadPciExByte(
    IN NAL_DEVICE_LOCATION DeviceLocation,
    IN UINT32 ByteIndex,
    OUT UINT8* Value
    )
{
    struct pci_dev* LinuxPciDevice = NULL;
    struct pci_bus* LinuxPciBus = NULL;
    NAL_STATUS Status = NAL_INVALID_PCI_SLOT_ID;
    unsigned int DeviceFunction = 0;

    printk(KERN_DEBUG "in the driver Pci Ex config function byte index %d\n", ByteIndex);

    LinuxPciBus = pci_find_bus(DeviceLocation.Pci.Segment, DeviceLocation.Pci.Bus);

    if((LinuxPciBus != NULL) && (ByteIndex < 4096))
    {

        DeviceFunction = PCI_DEVFN(DeviceLocation.Pci.Device, DeviceLocation.Pci.Function);

        LinuxPciDevice = pci_get_slot(LinuxPciBus, DeviceFunction);

        if(LinuxPciDevice != NULL)
        {
            pci_read_config_byte(LinuxPciDevice, ByteIndex, Value);
            Status = NAL_SUCCESS;
        }
    }

    return Status;
}

NAL_STATUS
_NalOsWritePciExByte(
    IN NAL_DEVICE_LOCATION DeviceLocation,
    IN UINT32 ByteOffset,
    IN UINT8 Data
    )
{
    struct pci_dev* LinuxPciDevice = NULL;
    struct pci_bus* LinuxPciBus = NULL;
    NAL_STATUS Status = NAL_INVALID_PCI_SLOT_ID;
    int Result = 0;
    unsigned int DeviceFunction = 0;

    LinuxPciBus = pci_find_bus(DeviceLocation.Pci.Segment, DeviceLocation.Pci.Bus);

    if((LinuxPciBus != NULL) && (ByteOffset < 4096))
    {

        DeviceFunction = PCI_DEVFN(DeviceLocation.Pci.Device, DeviceLocation.Pci.Function);

        LinuxPciDevice = pci_get_slot(LinuxPciBus, DeviceFunction);

        if(LinuxPciDevice != NULL)
        {
            Result = pci_write_config_byte(LinuxPciDevice, ByteOffset, Data);
            if(Result == 0)
            {
                Status = NAL_SUCCESS;
            }
            else
            {
                Status = NAL_PCICONFIG_NOT_AVAILABLE;
            }
        }
    }

    return Status;
}
