
#include "nalcodes.h"
#include "naltypes.h"
#include "os_i.h"
#include "pci_i.h"
#include "linuxnaldriver.h"

#include <asm/io.h>
#include <asm/mman.h>
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

spinlock_t Global_AtomicTestSetSpinLock;

#if 0
BOOLEAN Global_DebugPrintEnabled = TRUE;
#else
BOOLEAN Global_DebugPrintEnabled = FALSE;
#endif

UINT8
NalReadPort8(
    IN PORT_ADDR Port
    )
{
    UINT8 Buffer = 0;
    Buffer = inb(Port);
    return Buffer;
}

UINT16
NalReadPort16(
    IN PORT_ADDR Port
    )
{
    UINT16 Buffer = 0;

    Buffer = inw(Port);
    return Buffer;
}

UINT32
NalReadPort32(
    IN PORT_ADDR Port
    )
{
    UINT32 Buffer = 0;
    Buffer = inl(Port);
    return Buffer;
}

BOOLEAN
NalWritePort8(
    IN PORT_ADDR Port,
    IN UINT8 Value
    )
{
    outb(Value, Port);
    return TRUE;
}

BOOLEAN
NalWritePort16(
    IN PORT_ADDR Port,
    IN UINT16 Value
    )
{
    outw(Value, Port);
    return TRUE;
}

BOOLEAN
NalWritePort32(
    IN PORT_ADDR Port,
    IN UINT32 Value
    )
{
    outl(Value, Port);
    return TRUE;
}

VOID
NalDelayMilliseconds(
    IN UINT32 Milliseconds
    )
{
    mdelay(Milliseconds);
}

VOID
NalDelayMicroseconds(
    IN UINT32 Microseconds
    )
{

    if(Microseconds>1000)
    {
        NalDelayMilliseconds((UINT32)(Microseconds/1000));
        Microseconds %= 1000;
    }
    udelay(Microseconds);
}

UINT64
NalGetTimeStamp( VOID )
{
    UINT32 Low = 0;
    UINT32 High = 0;

    return (UINT64)((((UINT64)High) << 32) | Low);
}

UINT64
NalGetTimeStampsPerMicrosecond( VOID )
{
    return (UINT64)0;
}

UINT32
NalAtomicIncrement32(
    IN UINT32* Address
    )
{
    UINT32 ReturnValue = 0;
    UINT32 AddressLocal = 0;
    UINTN BytesLeft = 0;

    if(Address != NULL)
    {

        spin_lock(&Global_AtomicTestSetSpinLock);
        BytesLeft = copy_from_user((void*)(&AddressLocal), (void*)Address, sizeof(UINT32));

        if(AddressLocal < NAL_LARGEST_32_BIT_NUMBER)
        {
            AddressLocal++;
        }

        ReturnValue = AddressLocal;

        BytesLeft = copy_to_user((void*)Address, (void*)(&AddressLocal), sizeof(UINT32));

        spin_unlock(&Global_AtomicTestSetSpinLock);
    }
    return ReturnValue;
}

UINT32
NalAtomicDecrement32(
    IN UINT32* Address
    )
{
    UINT32 ReturnValue = 0;
    UINT32 AddressLocal = 0;
    UINTN BytesLeft = 0;

    if(Address != NULL)
    {

        spin_lock(&Global_AtomicTestSetSpinLock);
        BytesLeft = copy_from_user((void*)(&AddressLocal), (void*)Address, sizeof(UINT32));

        if(AddressLocal > 0)
        {
            AddressLocal--;
        }

        ReturnValue = AddressLocal;

        BytesLeft = copy_to_user((void*)Address, (void*)(&AddressLocal), sizeof(UINT32));

        spin_unlock(&Global_AtomicTestSetSpinLock);
    }

    return ReturnValue;
}

UINT32
NalAtomicTestSet32(
    IN UINT32* Address,
    IN UINT32 Test,
    IN UINT32 Set
    )
{
    UINT32 ReturnValue = 0;
    UINT32 AddressLocal = 0;
    UINTN BytesLeft = 0;

    if(Address != NULL)
    {

        spin_lock(&Global_AtomicTestSetSpinLock);
        BytesLeft = copy_from_user((void*)(&AddressLocal), (void*)Address, sizeof(UINT32));

        ReturnValue = AddressLocal;

        if(AddressLocal == Test)
        {
            AddressLocal = Set;
        }

        BytesLeft = copy_to_user((void*)Address, (void*)(&AddressLocal), sizeof(UINT32));

        spin_unlock(&Global_AtomicTestSetSpinLock);
    }

    return ReturnValue;
}

NAL_STATUS
NalDebugPrintSupport(
    OUT UINT32* DebugCapabilities
    )
{
    if (DebugCapabilities != NULL)
    {
        *DebugCapabilities = NAL_DEBUG_PRINT_SUPPORTED |
                             NAL_DEBUG_PRINT_FILEIO;
    }

    return NAL_SUCCESS;
}

NAL_STATUS
NalDebugPrint(
    IN CHAR* Format,
    ...
    )
{
    CHAR FormattedStatus[500];
    va_list ArgPtr;
    NAL_STATUS DbgPrintStatus = NAL_DEBUGPRINT_FAILED;

    if(Global_DebugPrintEnabled == TRUE)
    {

        va_start(ArgPtr, Format);
        vsprintf(FormattedStatus, Format, ArgPtr);
        va_end(ArgPtr);

        printk(KERN_DEBUG "%s", FormattedStatus);
        DbgPrintStatus = NAL_SUCCESS;
    }
    return DbgPrintStatus;
}

VOID
NalEnableDebugPrint(
    IN BOOLEAN Enable
    )
{
    Global_DebugPrintEnabled = Enable;
}

NAL_OS_RUN_DOMAIN
_NalGetOsRunDomain(
    VOID
    )
{
    NAL_OS_RUN_DOMAIN Domain = NAL_OS_DOMAIN_BAREMETAL;

#ifdef is_running_on_xen
    if(is_running_on_xen())
    {
        if(is_initial_xendomain())
        {
            Domain = NAL_OS_DOMAIN_0;
        }
        else
        {
            Domain = NAL_OS_DOMAIN_U;
        }
    }
#endif

    return Domain;
}
