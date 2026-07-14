
#include <naltypes.h>
#include <nalcodes.h>
#include <linuxtypes.h>
#include <linuxmemory.h>
#include <nalbytes.h>
#include <os_i.h>

#include <asm/io.h>
#include <linux/slab.h>
#include <asm/atomic.h>
#include <linux/sched.h>
#include <asm/page.h>

#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/pci_ids.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/mm.h>
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,12,0)
#include <linux/uaccess.h>
#else
#include <asm/uaccess.h>
#endif
#include <linux/pci.h>
#include <linux/delay.h>

#ifdef NAL_IOMMU_DMA_MEM_ALLOC
#include <linux/dma-map-ops.h>
#include <linux/iommu.h>
#ifdef IOMMU_SETUP_DMA_OPS_IN_DMAIOMMUH
#include <linux/dma-iommu.h>
#endif
#endif

NAL_OS_SPEC_DMA_PCI_MEMORY_TABLE Global_DmaPciMemoryTable[NAL_OS_SPEC_MAX_MEMORY_ALLOCATIONS] = {0};

VOID*
_NalAllocateMemory(
    IN UINT32 ByteCount,
    IN CHAR* NamedLocator,
    IN UINT32 LineNumber
    )
{
    VOID* Address = kmalloc(ByteCount, GFP_KERNEL);
    if(Address != NULL)
    {
        memset(Address, 0, ByteCount);
    }
    return Address;
}

KVOID*
_NalAllocateMemoryNonPaged(
    IN UINT32 ByteCount,
    IN UINT32 Alignment,
    OUT NAL_PHYSICAL_ADDRESS* PhysicalAddress,
    IN CHAR* NamedLocator,
    IN UINT32 LineNumber
    )
{
    KVOID* Allocation = NULL;
    phys_addr_t TempPhysicalAddress = PHYSICAL_NULL;

    do
    {
        if(Alignment == 0)
        {
            Alignment = 1;
        }

        if((Alignment & (Alignment - 1)) != 0)
        {
            break;
        }

        if(ByteCount < Alignment)
        {
            ByteCount = Alignment;
            NalDebugPrint("Aligned bytecount = %d\n", ByteCount);
        }

        if(ByteCount > NAL_OS_SPEC_MAX_CONTIGUOUS_MEMORY_ALLOCATION)
        {
            break;
        }

        Allocation = kmalloc(ByteCount, GFP_KERNEL | GFP_DMA);
        if(Allocation == NULL)
        {
            break;
        }

        TempPhysicalAddress = __pa(Allocation);
        if((UINT32)TempPhysicalAddress % Alignment != 0)
        {
            kfree(Allocation);
            Allocation = NULL;
            break;
        }

        memset(Allocation, 0, ByteCount);
        if(PhysicalAddress != NULL)
        {
            *PhysicalAddress = (NAL_PHYSICAL_ADDRESS)TempPhysicalAddress;
            NalDebugPrint("Phys Addr: %llx\n", (unsigned long long)*PhysicalAddress);
        }
    } while(0);

    if(Allocation == NULL)
    {
        NalDebugPrint("Allocation failed: File: %s, Line %d\n", NamedLocator, LineNumber);
    }

    return Allocation;
}

KVOID*
_NalAllocateMemoryNonPagedEx(
    IN UINT32 ByteCount,
    IN UINT32 Alignment,
    IN UINTN ProcessId,
    OUT NAL_PHYSICAL_ADDRESS* PhysicalAddress,
    IN CHAR* NamedLocator,
    IN UINT32 LineNumber
    )
{
    return _NalAllocateMemoryNonPaged(ByteCount,Alignment,PhysicalAddress,NamedLocator,LineNumber);
}

KVOID*
_NalAllocateMemoryNonPagedPci(
    IN KVOID* PDev,
    IN UINT32 ByteCount,
    IN UINT32 Alignment,
    OUT NAL_PHYSICAL_ADDRESS* PhysicalAddress,
    IN CHAR* NamedLocator,
    IN UINT32 LineNumber
    )
{
    KVOID* Allocation = NULL;
    struct pci_dev* LinuxPciDevice = (struct pci_dev*)PDev;
    dma_addr_t Dma;
    UINT32 i = 0;

    do
    {
        if(LinuxPciDevice == NULL)
        {
            break;
        }

        if(Alignment == 0)
        {
            Alignment = 1;
        }

        if((Alignment & (Alignment - 1)) != 0)
        {
            break;
        }

        if(ByteCount < Alignment)
        {
            ByteCount = Alignment;
            NalDebugPrint("Aligned bytecount = %d\n", ByteCount);
        }

        if(ByteCount > NAL_OS_SPEC_MAX_CONTIGUOUS_MEMORY_ALLOCATION)
        {
            break;
        }

        for(i = 0; i < NAL_OS_SPEC_MAX_MEMORY_ALLOCATIONS; i++)
        {
            if(Global_DmaPciMemoryTable[i].ReferenceCount == 0)
            {
                break;
            }
        }
        if(i >= NAL_OS_SPEC_MAX_MEMORY_ALLOCATIONS)
        {
            break;
        }

#ifdef NAL_IOMMU_DMA_MEM_ALLOC
        if(LinuxPciDevice->dev.dma_ops != NULL)
        {

            Allocation = LinuxPciDevice->dev.dma_ops->alloc(&LinuxPciDevice->dev,
                                                            ByteCount,
                                                            &Dma,
                                                            GFP_ATOMIC,
                                                            0);
        }
        else
        {
            Allocation = dma_alloc_coherent(&LinuxPciDevice->dev, ByteCount, &Dma, GFP_ATOMIC);
        }
#else
        Allocation = dma_alloc_coherent(&LinuxPciDevice->dev, ByteCount, &Dma, GFP_ATOMIC);
#endif

        if(Allocation == NULL)
        {
            break;
        }

        if(Dma % Alignment != 0)
        {
#ifdef NAL_IOMMU_DMA_MEM_ALLOC
            if(LinuxPciDevice->dev.dma_ops != NULL)
            {
                LinuxPciDevice->dev.dma_ops->free(&LinuxPciDevice->dev, ByteCount, Allocation, Dma, 0);
            }
            else
            {
                dma_free_coherent(&LinuxPciDevice->dev, ByteCount, Allocation, Dma);
            }
#else
            dma_free_coherent(&LinuxPciDevice->dev, ByteCount, Allocation, Dma);
#endif
            Allocation = NULL;
            break;
        }

        memset(Allocation, 0, ByteCount);

        Global_DmaPciMemoryTable[i].ReferenceCount = 1;
        Global_DmaPciMemoryTable[i].KernelAddress = Allocation;
        Global_DmaPciMemoryTable[i].PhysicalAddress = (NAL_PHYSICAL_ADDRESS)Dma;
        Global_DmaPciMemoryTable[i].Size = ByteCount;
        if(PhysicalAddress != NULL)
        {
            *PhysicalAddress = (NAL_PHYSICAL_ADDRESS)Dma;
            NalDebugPrint("Phys Addr: %llx\n", (unsigned long long)*PhysicalAddress);
        }
    } while(0);

    if(Allocation == NULL)
    {
        NalDebugPrint("Allocation failed: File: %s, Line %d\n", NamedLocator, LineNumber);
    }

    return Allocation;
}

VOID
_NalFreeMemory(
    IN VOID* Address,
    IN CHAR* NamedLocator,
    IN UINT32 LineNumber
    )
{
    if(Address != NULL)
    {
        kfree(Address);
    }
}

VOID
_NalFreeMemoryNonPaged(
    IN KVOID* Address,
    IN CHAR* NamedLocator,
    IN UINT32 LineNumber
    )
{
    if(Address != NULL)
    {

        kfree(Address);
    }
}

VOID
_NalFreeMemoryNonPagedEx(
    IN KVOID* Address,
    IN UINTN ProcessId,
    IN CHAR* NamedLocator,
    IN UINT32 LineNumber
    )
{
    _NalFreeMemoryNonPaged(Address,NamedLocator,LineNumber);
}

VOID
NalFreeMemoryNonPagedPci(
    IN KVOID* PDev,
    IN KVOID* Address,
    IN NAL_PHYSICAL_ADDRESS PhysicalAddress,
    IN UINT32 Size
    )
{
    struct pci_dev* LinuxPciDevice = (struct pci_dev*)PDev;
    UINT32 i = 0;

    if(PDev != NULL && Address != NULL)
    {
        for(i = 0; i < NAL_OS_SPEC_MAX_MEMORY_ALLOCATIONS; i++)
        {
            if((Global_DmaPciMemoryTable[i].ReferenceCount != 0) &&
               (Global_DmaPciMemoryTable[i].KernelAddress == Address))
            {
#ifdef NAL_IOMMU_DMA_MEM_ALLOC
                if(LinuxPciDevice->dev.dma_ops != NULL)
                {

                    LinuxPciDevice->dev.dma_ops->free(&LinuxPciDevice->dev,
                                                      Global_DmaPciMemoryTable[i].Size,
                                                      Global_DmaPciMemoryTable[i].KernelAddress,
                                                      Global_DmaPciMemoryTable[i].PhysicalAddress,
                                                      0);
                }
                else
                {
                    dma_free_coherent(&LinuxPciDevice->dev,
                                      Global_DmaPciMemoryTable[i].Size,
                                      Global_DmaPciMemoryTable[i].KernelAddress,
                                      Global_DmaPciMemoryTable[i].PhysicalAddress);
                }
#else
                dma_free_coherent(&LinuxPciDevice->dev,
                                  Global_DmaPciMemoryTable[i].Size,
                                  Global_DmaPciMemoryTable[i].KernelAddress,
                                  Global_DmaPciMemoryTable[i].PhysicalAddress);
#endif
                Global_DmaPciMemoryTable[i].ReferenceCount = 0;
                break;
            }
        }
    }
}

NAL_STATUS
NalMmapAddress(
    IN OUT KVOID** VirtualAddress,
    IN NAL_PHYSICAL_ADDRESS PhysicalAddress,
    IN OUT UINT32* Length
    )
{
    NAL_STATUS NalStatus = NAL_INVALID_PARAMETER;
    if(VirtualAddress != NULL && Length != NULL)
    {
        *VirtualAddress = ioremap(PhysicalAddress, (UINT64)(*Length));
        if(*VirtualAddress != NULL)
        {
            NalStatus = NAL_SUCCESS;
        }
        else
        {
            NalStatus = NAL_MMAP_FAILED;
        }
    }
    return NalStatus;
}

NAL_STATUS
NalUnmapAddress(
    IN KVOID* VirtualAddress,
    IN NAL_PHYSICAL_ADDRESS PhysicalAddress,
    IN UINT32 Length
    )
{
    NAL_STATUS NalStatus = NAL_INVALID_PARAMETER;
    if(VirtualAddress != NULL)
    {
        iounmap(VirtualAddress);
        NalStatus = NAL_SUCCESS;
    }
    return NalStatus;
}

NAL_STATUS
NalMmapAddressEx(
    IN OUT KVOID** VirtualAddress,
    IN NAL_PHYSICAL_ADDRESS PhysicalAddress,
    IN OUT UINT32* Length,
    IN UINTN ProcessId
    )
{
    return NalMmapAddress(VirtualAddress, PhysicalAddress, Length);
}

NAL_STATUS
NalUnmapAddressEx(
    IN KVOID* VirtualAddress,
    IN NAL_PHYSICAL_ADDRESS PhysicalAddress,
    IN UINT32 Length,
    IN UINTN ProcessId
    )
{
    return NalUnmapAddress(VirtualAddress, PhysicalAddress, Length);
}

UINT8
NalReadRegister8(
    IN KVOID* Address
    )
{
    UINT8 Value = 0;

    if(Address != NULL)
    {
        Value = readb((CHAR*)Address);
    }
    return Value;
}

UINT16
NalReadRegister16(
    IN KVOID* Address
    )
{
    UINT16 Value = 0;

    if(Address != NULL)
    {
        Value = readw((CHAR*)Address);
    }
    return Value;
}

UINT32
NalReadRegister32(
    IN KVOID* Address
    )
{
    UINT32 Value = 0;

    if(Address != NULL)
    {
        Value = readl((CHAR*)Address);
    }
    return Value;
}

BOOLEAN
NalWriteRegister8(
    IN KVOID* Address,
    IN UINT8 Value
    )
{
    BOOLEAN Success = FALSE;

    if(Address != NULL)
    {
        writeb(Value, (CHAR*)Address);
        Success = TRUE;
    }
    return Success;
}

BOOLEAN
NalWriteRegister16(
    IN KVOID* Address,
    IN UINT16 Value
    )
{
    BOOLEAN Success = FALSE;

    if(Address != NULL)
    {
        writew(Value, (CHAR*)Address);
        Success = TRUE;
    }
    return Success;
}

BOOLEAN
NalWriteRegister32(
    IN KVOID* Address,
    IN UINT32 Value
    )
{
    BOOLEAN Success = FALSE;

    if(Address != NULL)
    {
        writel(Value, (CHAR*)Address);
        Success = TRUE;
    }
    return Success;
}

NAL_PHYSICAL_ADDRESS
NalGetPhysicalMemoryAddress(
    IN KVOID* VirtualAddress
    )
{
    NAL_PHYSICAL_ADDRESS NalPhysical = 0;

    if(VirtualAddress != NULL)
    {
        NalPhysical = __pa(VirtualAddress);
    }

    return NalPhysical;
}

NAL_PHYSICAL_ADDRESS
NalGetPhysicalMemoryAddressEx(
    IN KVOID* VirtualAddress,
    IN UINTN ProcessId
    )
{
    return NalGetPhysicalMemoryAddress(VirtualAddress);
}

KVOID*
NalKMemset(
    IN KVOID* Dest,
    IN int Value,
    IN UINTN Size
    )
{
    return memset(Dest, Value, Size);
}

VOID*
NalKtoUMemcpy(
    IN VOID* Dest,
    IN const KVOID* Source,
    IN UINTN Size
    )
{
    UINTN BytesLeft = 0;
    BytesLeft = copy_to_user(Dest, Source, Size);
    return Dest;
}

KVOID*
NalKtoKMemcpy(
    IN KVOID* Dest,
    IN const KVOID* Source,
    IN UINTN Size
    )
{
    return memcpy(Dest, Source, Size);
}

KVOID*
NalUtoKMemcpy(
    IN KVOID* Dest,
    IN const VOID* Source,
    IN UINTN Size
    )
{
    UINTN BytesLeft = 0;
    BytesLeft = copy_from_user(Dest, Source, Size);
    return Dest;
}
