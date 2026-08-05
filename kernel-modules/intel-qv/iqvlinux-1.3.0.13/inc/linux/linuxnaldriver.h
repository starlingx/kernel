
#ifndef SDK_NAL_INC_FREEBSD_FREEBSDNALDRIVER_H_
#define SDK_NAL_INC_FREEBSD_FREEBSDNALDRIVER_H_ 

#include <naltypes.h>
#include <linuxtypes.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/pci_ids.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/version.h>

NAL_STATUS
_NalInitializeInterrupts(
    IN NAL_OS_SPEC_ISR_DEVICE* NalIsrDevice
    );

NAL_STATUS
_NalUninitializeInterrupts(
    IN NAL_OS_SPEC_ISR_DEVICE* NalIsrDevice
    );

BOOLEAN
_NalHasInterruptOccurred(
    IN NAL_OS_SPEC_ISR_DEVICE* NalIsrDevice
    );

int
NalOpen(
    struct inode* Inode,
    struct file* File
    );

int
NalRelease(
    struct inode* Inode,
    struct file* File
    );

int
NalMmap(
    struct file* File,
    struct vm_area_struct* Vma
    );

long
NalDeviceControlUnlockedIoctl(
    struct file* File,
    unsigned int Cmd,
    unsigned long Arg
    );

int
NalDeviceControl(
    struct inode* Inode,
    struct file* File,
    unsigned int Cmd,
    unsigned long Arg
    );

VOID
_NalDriverDecrementReferenceCount(
    VOID
    );

VOID
_NalDriverIncrementReferenceCount(
    VOID
    );

UINT32
_NalDriverGetReferenceCount(
    VOID
    );

VOID
_NalDriverGetVersion(
    OUT CHAR* Version
    );

#endif
