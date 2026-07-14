
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/pci_ids.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/version.h>
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,12,0)
#include <linux/uaccess.h>
#else
#include <asm/uaccess.h>
#endif
#include <linux/pci.h>
#include "naltypes.h"
#include "nalioctl.h"
#include "linuxnaldriver.h"

#ifdef NAL_IOMMU_DMA_MEM_ALLOC
#include <linux/dma-map-ops.h>
#include <linux/iommu.h>
#ifdef IOMMU_SETUP_DMA_OPS_IN_DMAIOMMUH
#include <linux/dma-iommu.h>
#endif
#endif

INT32
_NalPciDriverProbe(
    struct pci_dev *PciDevice,
    const struct pci_device_id *Id
    );

VOID
_NalPciDriverRemove(
    struct pci_dev *PciDevice
    );

extern spinlock_t Global_AtomicTestSetSpinLock;
static char Global_NalDeviceName[] = NAL_OS_SPEC_QV_DRIVER_NAME;
static int Global_NalMajor = 0;
NAL_OS_SPEC_ADAPTER_IN_USE_TABLE Global_AdapterInUse[NAL_OS_SPEC_MAX_PCI_DEVICES];
UINT32 Global_DriverReferenceCount = 0;

static struct file_operations Global_NalFops =
{
    owner: THIS_MODULE,

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36)
    ioctl: NalDeviceControl,
#endif
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,10)

    unlocked_ioctl: NalDeviceControlUnlockedIoctl,
#endif
    open: NalOpen,
    release: NalRelease,
    mmap: NalMmap
};

static struct pci_driver Global_QvPciDriver =
{
    .name = NAL_OS_SPEC_QV_DRIVER_MODULE_NAME,
    .id_table = NULL,
    .probe = _NalPciDriverProbe,
    .remove = _NalPciDriverRemove,
};

MODULE_AUTHOR(NAL_OS_SPEC_DRIVER_COMPANYNAME);
MODULE_DESCRIPTION(NAL_OS_SPEC_DRIVER_DESCRIPTION);
MODULE_LICENSE("GPL");
MODULE_VERSION(NAL_OS_SPEC_DRIVER_VERSION);

static int __init
nal_init_module(void)
{
    int Result = 0;
    UINT32 i = 0;

    printk(KERN_DEBUG "Intel Pro Diagnostic Driver loading (v. %s)\n", NAL_OS_SPEC_DRIVER_VERSION);
    Result = register_chrdev(0, Global_NalDeviceName, &Global_NalFops);
    if(Result < 0)
    {
        Result = -ENODEV;
    }
    else
    {
        Global_NalMajor = Result;
        Result = 0;
    }

    for(i=0; i<NAL_OS_SPEC_MAX_PCI_DEVICES; i++)
    {
        memset(&Global_AdapterInUse[i], 0, sizeof(NAL_OS_SPEC_ADAPTER_IN_USE_TABLE));
    }

    Result = pci_register_driver(&Global_QvPciDriver);

    spin_lock_init(&Global_AtomicTestSetSpinLock);

    return Result;
}

module_init(nal_init_module);

static void __exit
nal_cleanup_module(void)
{
    printk(KERN_DEBUG "Intel Pro Diagnostic Driver exiting\n");
    pci_unregister_driver(&Global_QvPciDriver);
    unregister_chrdev(Global_NalMajor, Global_NalDeviceName);
}

module_exit(nal_cleanup_module);

INT32
_NalPciDriverProbe(
    struct pci_dev *PciDevice,
    const struct pci_device_id *Id
    )
{
    UINT32 BarIndex = 0;
    INT32 Result = 0;

    do
    {
        if(PciDevice == NULL || Id == NULL)
        {
            Result = EINVAL;
            break;
        }

        Result = pci_enable_device(PciDevice);
        if(Result != 0)
        {
            break;
        }

        Result = dma_set_mask_and_coherent(&PciDevice->dev, DMA_BIT_MASK(64));
        if(Result != 0)
        {
            Result = dma_set_mask_and_coherent(&PciDevice->dev, DMA_BIT_MASK(32));
            if(Result != 0)
            {
                break;
            }
        }

        for(BarIndex = 0; BarIndex < NAL_MAX_BARS; BarIndex++)
        {

            if(pci_resource_len(PciDevice, BarIndex) == 0)
            {
                continue;
            }

            if(pci_resource_flags(PciDevice, BarIndex) & IORESOURCE_IO)
            {
                request_region(pci_resource_start(PciDevice, BarIndex),
                               pci_resource_len(PciDevice, BarIndex),
                               NAL_OS_SPEC_QV_DRIVER_NAME);
            }
            else if(pci_resource_flags(PciDevice, BarIndex) & IORESOURCE_MEM)
            {
                request_mem_region(pci_resource_start(PciDevice, BarIndex),
                                   pci_resource_len(PciDevice, BarIndex),
                                   NAL_OS_SPEC_QV_DRIVER_NAME);
            }
        }

        pci_set_master(PciDevice);

#ifdef NAL_IOMMU_DMA_MEM_ALLOC

        iommu_setup_dma_ops(&PciDevice->dev, 0ULL, ~0ULL);
#endif
    } while(0);

    return Result;
}

VOID
_NalPciDriverRemove(
    struct pci_dev *PciDevice
    )
{
    UINT32 BarIndex = 0;

    do
    {
        if(PciDevice == NULL)
        {
            break;
        }

        for(BarIndex = 0; BarIndex < NAL_MAX_BARS; BarIndex++)
        {

            if(pci_resource_len(PciDevice, BarIndex) == 0)
            {
                continue;
            }

            if(pci_resource_flags(PciDevice, BarIndex) & IORESOURCE_IO)
            {
                release_region(pci_resource_start(PciDevice, BarIndex),
                               pci_resource_len(PciDevice, BarIndex));
            }
            else if(pci_resource_flags(PciDevice, BarIndex) & IORESOURCE_MEM)
            {
                release_mem_region(pci_resource_start(PciDevice, BarIndex),
                                   pci_resource_len(PciDevice, BarIndex));
            }
        }
        pci_dev_put(PciDevice);
        pci_disable_device(PciDevice);
    } while(0);

    return;
}

long
NalDeviceControlUnlockedIoctl(
    struct file* File,
    unsigned int Cmd,
    unsigned long Arg
    )

{
    return NalDeviceControl(NULL, File, Cmd, Arg);
}

int
NalDeviceControl(
    struct inode* Inode,
    struct file* File,
    unsigned int Cmd,
    unsigned long Arg
    )
{
    NAL_IOCTL_INPUT_DATA* NalIoctlInputData =
        (NAL_IOCTL_INPUT_DATA*)kmalloc(sizeof(NAL_IOCTL_INPUT_DATA), GFP_KERNEL);
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,0)
    mm_segment_t old_fs;
#endif
    UINT32 IoctlNumber = 0;
    int Result = 0;

    do
    {
        if(NalIoctlInputData == NULL)
        {
            Result = ENOMEM;
            break;
        }

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,0)
        old_fs = get_fs();
        set_fs(get_ds());
        Result = copy_from_user(NalIoctlInputData, (void*)Arg, sizeof(NAL_IOCTL_INPUT_DATA));
        set_fs(old_fs);
#else
        Result = copy_from_user(NalIoctlInputData, (void*)Arg, sizeof(NAL_IOCTL_INPUT_DATA));
#endif
        if(Result != 0)
        {
            break;
        }

        IoctlNumber = Cmd;

        switch (IoctlNumber)
        {
        case IOCTL_NAL_OSI:
            NalResolveOsiIoctl(NalIoctlInputData);
            break;

        case IOCTL_NAL_HW_BUS:
            NalResolveHwBusIoctl(NalIoctlInputData);
            break;

        case IOCTL_NAL_NDI:
            NalResolveNdiIoctl(NalIoctlInputData);
            break;

        case IOCTL_NAL_OS_SPECIFIC:
            NalResolveOsSpecificIoctl(NalIoctlInputData);
            break;

        default:

            Result = EINVAL;
            break;
        }

        if(Result != 0)
        {
            break;
        }

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,0)
        old_fs = get_fs();
        set_fs(get_ds());
        Result = copy_to_user((void*)Arg, NalIoctlInputData, sizeof(NAL_IOCTL_INPUT_DATA));
        set_fs(old_fs);
#else
        Result = copy_to_user((void*)Arg, NalIoctlInputData, sizeof(NAL_IOCTL_INPUT_DATA));
#endif
    } while(0);

    if(NalIoctlInputData != NULL)
    {
        kfree(NalIoctlInputData);
    }

    return Result;
}

int
NalOpen(
    struct inode* Inode,
    struct file* File
    )
{
    return 0;
}

int
NalRelease(
    struct inode* Inode,
    struct file* File
    )
{
    return 0;
}

int
NalMmap(
    struct file* File,
    struct vm_area_struct* Vma
    )
{
    if(io_remap_pfn_range(Vma,
                          Vma->vm_start,
                          Vma->vm_pgoff,
                          Vma->vm_end - Vma->vm_start,
                          Vma->vm_page_prot))
    {
        return -EAGAIN;
    }

    return 0;
}

UINT32
_NalDriverGetReferenceCount(
 VOID
 )
{
    UINT32 Local_DriverReferenceCount = 0;

    spin_lock(&Global_AtomicTestSetSpinLock);
    Local_DriverReferenceCount = Global_DriverReferenceCount;
    spin_unlock(&Global_AtomicTestSetSpinLock);

    return Local_DriverReferenceCount;
}

VOID
_NalDriverIncrementReferenceCount(
    VOID
    )
{

    spin_lock(&Global_AtomicTestSetSpinLock);
    Global_DriverReferenceCount++;
    spin_unlock(&Global_AtomicTestSetSpinLock);
}

VOID
_NalDriverDecrementReferenceCount(
    VOID
    )
{

    spin_lock(&Global_AtomicTestSetSpinLock);
    if(Global_DriverReferenceCount > 0)
    {
        Global_DriverReferenceCount--;
    }
    spin_unlock(&Global_AtomicTestSetSpinLock);
}

VOID
_NalDriverGetVersion(
 OUT CHAR* Version
 )
{
    sprintf(Version, "%d.%d.%d.%d",
                                   NAL_OS_SPEC_DRIVER_MAJOR_VERSION ,
                                   NAL_OS_SPEC_DRIVER_MINOR_VERSION ,
                                   NAL_OS_SPEC_DRIVER_BUILD_VERSION ,
                                   NAL_OS_SPEC_DRIVER_FIX_VERSION );
}
