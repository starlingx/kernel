
#include <nalioctl.h>
#include <pci_i.h>

#if defined(NAL_WINNT_WDM)
#include <windriverdevice_i.h>
#endif

#if defined(NAL_LINUX)
#include <linuxnaldriver.h>
#endif

#if defined(NAL_ESXI)
#include <esxinaldriver.h>
#endif

#if defined(NAL_ESXIN)
#include <esxinnaldriver.h>
#include <esxinmemory.h>
#include <esxindriverdevice_i.h>
#endif

#if defined(NAL_SOLARIS)
#include <osdevice_i.h>
#include <solarisnaldriver.h>
#endif

#if defined(NAL_FREEBSD)
#include <freebsdnaldriver.h>
#endif

NAL_STATUS
NalResolveNdiIoctl(
    IN NAL_IOCTL_INPUT_DATA* NalIoctl
    )
{
    NAL_STATUS NalStatus = NAL_INVALID_PARAMETER;

    if(NalIoctl != NULL)
    {
        switch(NalIoctl->FunctionId)
        {
#ifdef NAL_DRIVER_UNSECURE
            case NAL_INITIALIZEINTERRUPTS_FUNCID:
                if(NalIoctl->Size == sizeof(NAL_INTERRUPT_FUNCS))
                {
                    NAL_INTERRUPT_FUNCS* FunctionData =
                        (NAL_INTERRUPT_FUNCS*)(&(NalIoctl->InputBuffer));
                    if(FunctionData != NULL)
                    {
                        FunctionData->ReturnValue =
                            _NalInitializeInterrupts(FunctionData->NalOsDevice);
                    }
                    NalStatus = NAL_SUCCESS;
                }
                break;
#endif

#ifdef NAL_DRIVER_UNSECURE
            case NAL_UNINITIALIZEINTERRUPTS_FUNCID:
                if(NalIoctl->Size == sizeof(NAL_INTERRUPT_FUNCS))
                {
                    NAL_INTERRUPT_FUNCS* FunctionData =
                        (NAL_INTERRUPT_FUNCS*)(&(NalIoctl->InputBuffer));
                    if(FunctionData != NULL)
                    {
                        FunctionData->ReturnValue =
                            _NalUninitializeInterrupts(FunctionData->NalOsDevice);
                    }
                    NalStatus = NAL_SUCCESS;
                }
                break;
#endif

#ifdef NAL_DRIVER_UNSECURE
            case NAL_HASINTERRUPTOCCURRED_FUNCID:
                if(NalIoctl->Size == sizeof(NAL_INTERRUPT_FUNCS))
                {
                    NAL_INTERRUPT_FUNCS* FunctionData =
                        (NAL_INTERRUPT_FUNCS*)(&(NalIoctl->InputBuffer));
                    if(FunctionData != NULL)
                    {
                        FunctionData->Triggered =
                            _NalHasInterruptOccurred(FunctionData->NalOsDevice);
                    }
                    NalStatus = NAL_SUCCESS;
                }
                break;
#endif

           default:
                NalStatus = NAL_IOCTL_INVALID_FUNCTION_ID;
                break;
        }
    }
    return NalStatus;
}

NAL_STATUS
NalResolveOsiIoctl(
    IN NAL_IOCTL_INPUT_DATA* NalIoctl
    )
{
    NAL_STATUS NalStatus = NAL_INVALID_PARAMETER;

    if(NalIoctl != NULL)
    {
        switch(NalIoctl->FunctionId)
        {
#if defined(NAL_WINNT_WDM)
        case NAL_GETDRIVERVERSION_FUNCID:
            if(NalIoctl->Size == sizeof(NAL_GETDRIVERVERSION_FUNC))
            {
                NAL_GETDRIVERVERSION_FUNC* FunctionData =
                    (NAL_GETDRIVERVERSION_FUNC*)(&(NalIoctl->InputBuffer));
                FunctionData->ReturnValue =
                    _NalGetDriverVersion(&FunctionData->QvDriverVersion);

                NalStatus = NAL_SUCCESS;
            }
            break;
#endif

#ifdef NAL_DRIVER_UNSECURE
        case NAL_READPORT8_FUNCID:
            if(NalIoctl->Size == sizeof(NAL_READPORT_FUNCS))
            {
                NAL_READPORT_FUNCS* FunctionData =
                    (NAL_READPORT_FUNCS*)(&(NalIoctl->InputBuffer));
                FunctionData->ReturnValue.Uint8 =
                    NalReadPort8(FunctionData->Port);

                NalStatus = NAL_SUCCESS;
            }
            break;

        case NAL_READPORT16_FUNCID:
            if(NalIoctl->Size == sizeof(NAL_READPORT_FUNCS))
            {
                NAL_READPORT_FUNCS* FunctionData =
                    (NAL_READPORT_FUNCS*)(&(NalIoctl->InputBuffer));
                FunctionData->ReturnValue.Uint16 =
                    NalReadPort16(FunctionData->Port);

                NalStatus = NAL_SUCCESS;
            }
            break;

        case NAL_READPORT32_FUNCID:
            if(NalIoctl->Size == sizeof(NAL_READPORT_FUNCS))
            {
                NAL_READPORT_FUNCS* FunctionData =
                    (NAL_READPORT_FUNCS*)(&(NalIoctl->InputBuffer));
                FunctionData->ReturnValue.Uint32 =
                    NalReadPort32(FunctionData->Port);

                NalStatus = NAL_SUCCESS;
            }
            break;

        case NAL_WRITEPORT8_FUNCID:
            if(NalIoctl->Size == sizeof(NAL_WRITEPORT_FUNCS))
            {
                NAL_WRITEPORT_FUNCS* FunctionData =
                    (NAL_WRITEPORT_FUNCS*)(&(NalIoctl->InputBuffer));
                FunctionData->ReturnValue =
                    NalWritePort8(FunctionData->Port,
                                  FunctionData->Value.Uint8);

                NalStatus = NAL_SUCCESS;
            }
            break;

        case NAL_WRITEPORT16_FUNCID:
            if(NalIoctl->Size == sizeof(NAL_WRITEPORT_FUNCS))
            {
                NAL_WRITEPORT_FUNCS* FunctionData =
                    (NAL_WRITEPORT_FUNCS*)(&(NalIoctl->InputBuffer));
                FunctionData->ReturnValue =
                    NalWritePort16(FunctionData->Port,
                                   FunctionData->Value.Uint16);

                NalStatus = NAL_SUCCESS;
            }
            break;

        case NAL_WRITEPORT32_FUNCID:
            if(NalIoctl->Size == sizeof(NAL_WRITEPORT_FUNCS))
            {
                NAL_WRITEPORT_FUNCS* FunctionData =
                    (NAL_WRITEPORT_FUNCS*)(&(NalIoctl->InputBuffer));
                FunctionData->ReturnValue =
                    NalWritePort32(FunctionData->Port,
                                   FunctionData->Value.Uint32);

                NalStatus = NAL_SUCCESS;
            }
            break;
#endif
       case NAL_READREGISTER8_FUNCID:
            if(NalIoctl->Size == sizeof(NAL_READREGISTER_FUNCS))
            {
                NAL_READREGISTER_FUNCS* FunctionData =
                    (NAL_READREGISTER_FUNCS*)(&(NalIoctl->InputBuffer));
                FunctionData->ReturnValue.Uint8 =
                    NalReadRegister8(FunctionData->Address);

                NalStatus = NAL_SUCCESS;
            }
            break;

        case NAL_READREGISTER16_FUNCID:
            if(NalIoctl->Size == sizeof(NAL_READREGISTER_FUNCS))
            {
                NAL_READREGISTER_FUNCS* FunctionData =
                    (NAL_READREGISTER_FUNCS*)(&(NalIoctl->InputBuffer));
                FunctionData->ReturnValue.Uint16 =
                    NalReadRegister16(FunctionData->Address);

                NalStatus = NAL_SUCCESS;
            }
            break;

        case NAL_READREGISTER32_FUNCID:
            if(NalIoctl->Size == sizeof(NAL_READREGISTER_FUNCS))
            {
                NAL_READREGISTER_FUNCS* FunctionData =
                    (NAL_READREGISTER_FUNCS*)(&(NalIoctl->InputBuffer));
                FunctionData->ReturnValue.Uint32 =
                    NalReadRegister32(FunctionData->Address);
                NalStatus = NAL_SUCCESS;
            }
            break;

        case NAL_WRITEREGISTER8_FUNCID:
            if(NalIoctl->Size == sizeof(NAL_WRITEREGISTER_FUNCS))
            {
                NAL_WRITEREGISTER_FUNCS* FunctionData =
                    (NAL_WRITEREGISTER_FUNCS*)(&(NalIoctl->InputBuffer));
                FunctionData->ReturnValue =
                    NalWriteRegister8(FunctionData->Address,
                                      FunctionData->Value.Uint8);

                NalStatus = NAL_SUCCESS;
            }
            break;

        case NAL_WRITEREGISTER16_FUNCID:
            if(NalIoctl->Size == sizeof(NAL_WRITEREGISTER_FUNCS))
            {
                NAL_WRITEREGISTER_FUNCS* FunctionData =
                    (NAL_WRITEREGISTER_FUNCS*)(&(NalIoctl->InputBuffer));
                FunctionData->ReturnValue =
                    NalWriteRegister16(FunctionData->Address,
                                       FunctionData->Value.Uint16);

                NalStatus = NAL_SUCCESS;
            }
            break;

        case NAL_WRITEREGISTER32_FUNCID:
            if(NalIoctl->Size == sizeof(NAL_WRITEREGISTER_FUNCS))
            {
                NAL_WRITEREGISTER_FUNCS* FunctionData =
                    (NAL_WRITEREGISTER_FUNCS*)(&(NalIoctl->InputBuffer));
                FunctionData->ReturnValue =
                    NalWriteRegister32(FunctionData->Address,
                                       FunctionData->Value.Uint32);

                NalStatus = NAL_SUCCESS;
            }
            break;

        case NAL_MMAPADDRESS_FUNCID:
            if(NalIoctl->Size == sizeof(NAL_MEMMAP_FUNCS))
            {
                NAL_MEMMAP_FUNCS* FunctionData =
                    (NAL_MEMMAP_FUNCS*)(&(NalIoctl->InputBuffer));
                FunctionData->ReturnValue =
                    NalMmapAddress((KVOID**)&FunctionData->VirtualAddress,
                                   FunctionData->PhysicalAddress,
                                   &FunctionData->Length);

                NalStatus = NAL_SUCCESS;
            }
            break;

        case NAL_UNMAPADDRESS_FUNCID:
            if(NalIoctl->Size == sizeof(NAL_MEMMAP_FUNCS))
            {
                NAL_MEMMAP_FUNCS* FunctionData =
                    (NAL_MEMMAP_FUNCS*)(&(NalIoctl->InputBuffer));
                FunctionData->ReturnValue =
                    NalUnmapAddress(FunctionData->VirtualAddress,
                                    FunctionData->PhysicalAddress,
                                    FunctionData->Length);

                NalStatus = NAL_SUCCESS;
            }
            break;

        case NAL_GETTIMESTAMP_FUNCID:
            if(NalIoctl->Size == sizeof(NAL_TIMESTAMP_FUNCS))
            {
                NAL_TIMESTAMP_FUNCS* FunctionData =
                    (NAL_TIMESTAMP_FUNCS*)(&(NalIoctl->InputBuffer));
                FunctionData->ReturnValue =
                    NalGetTimeStamp();

                NalStatus = NAL_SUCCESS;
            }
            break;

        case NAL_GETTIMESTAMPSPERMICROSECOND_FUNCID:
            if(NalIoctl->Size == sizeof(NAL_TIMESTAMP_FUNCS))
            {
                NAL_TIMESTAMP_FUNCS* FunctionData =
                    (NAL_TIMESTAMP_FUNCS*)(&(NalIoctl->InputBuffer));
                FunctionData->ReturnValue =
                    NalGetTimeStampsPerMicrosecond();

                NalStatus = NAL_SUCCESS;
            }
            break;

        case NAL_DELAYMICROSECONDS_FUNCID:
            if(NalIoctl->Size == sizeof(NAL_DELAY_FUNCS))
            {
                NAL_DELAY_FUNCS* FunctionData =
                    (NAL_DELAY_FUNCS*)(&(NalIoctl->InputBuffer));

                    NalDelayMicroseconds(FunctionData->Delay);

                NalStatus = NAL_SUCCESS;
            }
            break;

        case NAL_DELAYMILLISECONDS_FUNCID:
            if(NalIoctl->Size == sizeof(NAL_DELAY_FUNCS))
            {
                NAL_DELAY_FUNCS* FunctionData =
                    (NAL_DELAY_FUNCS*)(&(NalIoctl->InputBuffer));

                    NalDelayMilliseconds(FunctionData->Delay);

                NalStatus = NAL_SUCCESS;
            }
            break;

#ifdef NAL_DRIVER_UNSECURE
        case NAL_GETPHYSICALMEMORYADDRESS_FUNCID:
            if(NalIoctl->Size == sizeof(NAL_GETPHYSICALMEMORYADDRESS_FUNC))
            {
                NAL_GETPHYSICALMEMORYADDRESS_FUNC* FunctionData =
                    (NAL_GETPHYSICALMEMORYADDRESS_FUNC*)(&(NalIoctl->InputBuffer));

                    FunctionData->ReturnValue =
                        NalGetPhysicalMemoryAddress(FunctionData->VirtualAddress);

                NalStatus = NAL_SUCCESS;
            }
            break;
#endif

#ifdef NAL_DRIVER_UNSECURE
         case NAL_ALLOCATEMEMORYNONPAGED_FUNCID:
            if(NalIoctl->Size == sizeof(NAL_ALLOCATEMEMORYNONPAGED_FUNC))
            {
                NAL_ALLOCATEMEMORYNONPAGED_FUNC* FunctionData =
                    (NAL_ALLOCATEMEMORYNONPAGED_FUNC*)(&(NalIoctl->InputBuffer));

                    FunctionData->ReturnValue =
                        _NalAllocateMemoryNonPaged(FunctionData->ByteCount,
                                                   FunctionData->Alignment,
                                                   &FunctionData->PhysicalAddress,
                                                   NULL,
                                                   0);

                NalStatus = NAL_SUCCESS;
            }
            break;
#endif

#ifdef NAL_DRIVER_UNSECURE
         case NAL_FREEMEMORYNONPAGED_FUNCID:
            if(NalIoctl->Size == sizeof(NAL_FREEMEMORYNONPAGED_FUNC))
            {
                NAL_FREEMEMORYNONPAGED_FUNC* FunctionData =
                    (NAL_FREEMEMORYNONPAGED_FUNC*)(&(NalIoctl->InputBuffer));

                    _NalFreeMemoryNonPaged(FunctionData->Address,
                                           NULL,
                                           0);

                NalStatus = NAL_SUCCESS;
            }
            break;
#endif

#ifdef NAL_DRIVER_UNSECURE
         case NAL_KMEMSET_FUNCID:
            if(NalIoctl->Size == sizeof(NAL_KMEMSET_FUNC))
            {
                NAL_KMEMSET_FUNC* FunctionData =
                    (NAL_KMEMSET_FUNC*)(&(NalIoctl->InputBuffer));

                if(FunctionData->Destination != NULL)
                {
                    NalKMemset(FunctionData->Destination,
                               FunctionData->Source,
                               FunctionData->Size);
                    NalStatus = NAL_SUCCESS;
                }
                else
                {
                    NalDebugPrint("NAL_KMEMSET_FUNCID: One of the buffers was NULL\n");
                    NalStatus = NAL_INVALID_PARAMETER;
                }
            }
            break;
#endif

#ifdef NAL_DRIVER_UNSECURE
         case NAL_KUMEMCPY_FUNCID:
            if(NalIoctl->Size == sizeof(NAL_KMEM_FUNCS))
            {
                NAL_KMEM_FUNCS* FunctionData =
                    (NAL_KMEM_FUNCS*)(&(NalIoctl->InputBuffer));

                if(FunctionData->Destination != NULL &&
                   FunctionData->Source != NULL)
                {
                    NalKtoUMemcpy(FunctionData->Destination,
                                  FunctionData->Source,
                                  FunctionData->Size);
                    NalStatus = NAL_SUCCESS;
                }
                else
                {
                    NalDebugPrint("NAL_KUMEMCPY_FUNCID: One of the buffers was NULL\n");
                    NalStatus = NAL_INVALID_PARAMETER;
                }
            }
            break;
#endif

#ifdef NAL_DRIVER_UNSECURE
         case NAL_KKMEMCPY_FUNCID:
            if(NalIoctl->Size == sizeof(NAL_KMEM_FUNCS))
            {
                NAL_KMEM_FUNCS* FunctionData =
                    (NAL_KMEM_FUNCS*)(&(NalIoctl->InputBuffer));

                if(FunctionData->Destination != NULL &&
                   FunctionData->Source != NULL)
                {
                    NalKtoKMemcpy(FunctionData->Destination,
                                  FunctionData->Source,
                                  FunctionData->Size);
                    NalStatus = NAL_SUCCESS;
                }
                else
                {
                    NalDebugPrint("NAL_KKMEMCPY_FUNCID: One of the buffers was NULL\n");
                    NalStatus = NAL_INVALID_PARAMETER;
                }
            }
            break;
#endif

#ifdef NAL_DRIVER_UNSECURE
         case NAL_UKMEMCPY_FUNCID:
            if(NalIoctl->Size == sizeof(NAL_KMEM_FUNCS))
            {
                NAL_KMEM_FUNCS* FunctionData =
                    (NAL_KMEM_FUNCS*)(&(NalIoctl->InputBuffer));

                NalUtoKMemcpy(FunctionData->Destination,
                              FunctionData->Source,
                              FunctionData->Size);

                NalStatus = NAL_SUCCESS;
            }
            break;
#endif

#ifdef NAL_DRIVER_UNSECURE
         case NAL_ATOMICINCREMENT32_FUNCID:
            if(NalIoctl->Size == sizeof(NAL_ATOMIC_FUNCS))
            {
                NAL_ATOMIC_FUNCS* FunctionData =
                    (NAL_ATOMIC_FUNCS*)(&(NalIoctl->InputBuffer));

                FunctionData->ReturnValue =
                    NalAtomicIncrement32(FunctionData->Address);
                NalStatus = NAL_SUCCESS;
            }
            break;
#endif

#ifdef NAL_DRIVER_UNSECURE
         case NAL_ATOMICDECREMENT32_FUNCID:
            if(NalIoctl->Size == sizeof(NAL_ATOMIC_FUNCS))
            {
                NAL_ATOMIC_FUNCS* FunctionData =
                    (NAL_ATOMIC_FUNCS*)(&(NalIoctl->InputBuffer));

                FunctionData->ReturnValue =
                    NalAtomicDecrement32(FunctionData->Address);
                NalStatus = NAL_SUCCESS;
            }
            break;
#endif

#ifdef NAL_DRIVER_UNSECURE
         case NAL_ATOMICTESTSET32_FUNCID:
            if(NalIoctl->Size == sizeof(NAL_ATOMICTESTSET32_FUNC))
            {
                NAL_ATOMICTESTSET32_FUNC* FunctionData =
                    (NAL_ATOMICTESTSET32_FUNC*)(&(NalIoctl->InputBuffer));

                FunctionData->ReturnValue =
                    NalAtomicTestSet32(FunctionData->Address,
                                       FunctionData->Test,
                                       FunctionData->Set);
                NalStatus = NAL_SUCCESS;
            }
            break;
#endif

        case NAL_ENABLE_DEBUG_PRINT_FUNCID:
            if(NalIoctl->Size == sizeof(NAL_ENABLE_DEBUG_PRINT_FUNC))
            {
                NAL_ENABLE_DEBUG_PRINT_FUNC* FunctionData =
                    (NAL_ENABLE_DEBUG_PRINT_FUNC*)(&(NalIoctl->InputBuffer));

                if(FunctionData != NULL)
                {
                    NalEnableDebugPrint(FunctionData->Enable);
                    NalStatus = NAL_SUCCESS;
                }
                else
                {
                    NalDebugPrint("NAL_ENABLE_DEBUG_PRINT_FUNCID: FunctionData is NULL\n");
                    NalStatus = NAL_INVALID_PARAMETER;
                }
            }
            break;

#ifdef NAL_DRIVER_UNSECURE
        case NAL_DEBUGPRINT_FUNCID:
            if(NalIoctl->Size == sizeof(NAL_DEBUGPRINT_FUNC))
            {
                NAL_DEBUGPRINT_FUNC* FunctionData =
                    (NAL_DEBUGPRINT_FUNC*)(&(NalIoctl->InputBuffer));

                if(FunctionData != NULL)
                {
                    FunctionData->ReturnValue = NalDebugPrint(FunctionData->Message);
                    NalStatus = NAL_SUCCESS;
                }
                else
                {
                    NalDebugPrint("NAL_ENABLE_DEBUG_PRINT_FUNCID: FunctionData is NULL\n");
                    NalStatus = NAL_INVALID_PARAMETER;
                }
            }
            break;
#endif

         case NAL_ALLOCATEMEMORYNONPAGEDEX_FUNCID:
            if(NalIoctl->Size == sizeof(NAL_ALLOCATEMEMORYNONPAGED_FUNC))
            {
                NAL_ALLOCATEMEMORYNONPAGED_FUNC* FunctionData =
                    (NAL_ALLOCATEMEMORYNONPAGED_FUNC*)(&(NalIoctl->InputBuffer));

                    FunctionData->ReturnValue =
                        _NalAllocateMemoryNonPagedEx(FunctionData->ByteCount,
                                                     FunctionData->Alignment,
                                                     FunctionData->ProcessId,
                                                     &FunctionData->PhysicalAddress,
                                                     NULL,
                                                     0);

                NalStatus = NAL_SUCCESS;
            }
            break;

         case NAL_FREEMEMORYNONPAGEDEX_FUNCID:
            if(NalIoctl->Size == sizeof(NAL_FREEMEMORYNONPAGED_FUNC))
            {
                NAL_FREEMEMORYNONPAGED_FUNC* FunctionData =
                    (NAL_FREEMEMORYNONPAGED_FUNC*)(&(NalIoctl->InputBuffer));

                    _NalFreeMemoryNonPagedEx(FunctionData->Address,
                                             FunctionData->ProcessId,
                                             NULL,
                                             0);

                NalStatus = NAL_SUCCESS;
            }
            break;

#ifdef NAL_DRIVER_UNSECURE
        case NAL_MMAPADDRESSEX_FUNCID:
            if(NalIoctl->Size == sizeof(NAL_MEMMAP_FUNCS))
            {
                NAL_MEMMAP_FUNCS* FunctionData =
                    (NAL_MEMMAP_FUNCS*)(&(NalIoctl->InputBuffer));
                FunctionData->ReturnValue =
                    NalMmapAddressEx((KVOID**)&FunctionData->VirtualAddress,
                                     FunctionData->PhysicalAddress,
                                     &FunctionData->Length,
                                     FunctionData->ProcessId);

                NalStatus = NAL_SUCCESS;
            }
            break;
#endif

#ifdef NAL_DRIVER_UNSECURE
        case NAL_UNMAPADDRESSEX_FUNCID:
            if(NalIoctl->Size == sizeof(NAL_MEMMAP_FUNCS))
            {
                NAL_MEMMAP_FUNCS* FunctionData =
                    (NAL_MEMMAP_FUNCS*)(&(NalIoctl->InputBuffer));
                FunctionData->ReturnValue =
                    NalUnmapAddressEx(FunctionData->VirtualAddress,
                                      FunctionData->PhysicalAddress,
                                      FunctionData->Length,
                                      FunctionData->ProcessId);

                NalStatus = NAL_SUCCESS;
            }
            break;
#endif

#ifdef NAL_DRIVER_UNSECURE
        case NAL_GETPHYSICALMEMORYADDRESSEX_FUNCID:
            if(NalIoctl->Size == sizeof(NAL_GETPHYSICALMEMORYADDRESS_FUNC))
            {
                NAL_GETPHYSICALMEMORYADDRESS_FUNC* FunctionData =
                    (NAL_GETPHYSICALMEMORYADDRESS_FUNC*)(&(NalIoctl->InputBuffer));

                    FunctionData->ReturnValue =
                        NalGetPhysicalMemoryAddressEx(FunctionData->VirtualAddress,
                                                      FunctionData->ProcessId);

                NalStatus = NAL_SUCCESS;
            }
            break;
#endif

        default:
            NalDebugPrint("NalResolveOsiIoctl: FuctionId = %llu\n",
                          (unsigned long long)NalIoctl->FunctionId);
            NalStatus = NAL_IOCTL_INVALID_FUNCTION_ID;
            break;
        }
    }
    return NalStatus;
}

NAL_STATUS
NalResolveHwBusIoctl(
    IN NAL_IOCTL_INPUT_DATA* NalIoctl
    )
{
    NAL_STATUS NalStatus = NAL_INVALID_PARAMETER;

    if(NalIoctl != NULL)
    {
        NalDebugPrint("NalResolveHwBusIoctl: FunctionId = %llu\n",
                      (unsigned long long)NalIoctl->FunctionId);
        switch(NalIoctl->FunctionId)
        {
#ifdef NAL_DRIVER_UNSECURE
            case NAL_READPCIDEVICECOUNT_FUNCID:
                if(NalIoctl->Size == sizeof(NAL_READPCIDEVICECOUNT_FUNC))
                {
                    NAL_READPCIDEVICECOUNT_FUNC* FunctionData =
                        (NAL_READPCIDEVICECOUNT_FUNC*)(&(NalIoctl->InputBuffer));
                    FunctionData->ReturnValue =
                        _NalReadPciDeviceCount();

                    NalStatus = NAL_SUCCESS;
                }
                break;
#endif

#ifdef NAL_DRIVER_UNSECURE
            case NAL_READPCIDEVICE_FUNCID:
                if(NalIoctl->Size == sizeof(NAL_READPCIDEVICE_FUNC))
                {
                    NAL_READPCIDEVICE_FUNC* FunctionData =
                        (NAL_READPCIDEVICE_FUNC*)(&(NalIoctl->InputBuffer));
                    FunctionData->ReturnValue =
                        _NalReadPciDevice(FunctionData->PciLocations,
                                          &FunctionData->Count);

                    NalStatus = NAL_SUCCESS;
                }
                break;
#endif

#ifdef NAL_DRIVER_UNSECURE
            case NAL_FILLPCICONFIGSPEC_FUNCID:
                if(NalIoctl->Size == sizeof(NAL_FILLPCICONFIGSPEC_FUNC))
                {
                    NAL_FILLPCICONFIGSPEC_FUNC* FunctionData =
                        (NAL_FILLPCICONFIGSPEC_FUNC*)(&(NalIoctl->InputBuffer));
                    FunctionData->ReturnValue =
                        _NalFillPciConfigSpec(&FunctionData->PciLocation,
                                              &FunctionData->PciDevice,
                                              FunctionData->DwordCount);

                    NalStatus = NAL_SUCCESS;
                }
                break;
#endif

            case NAL_OSREADPCICONFIG32_FUNCID:
                if(NalIoctl->Size == sizeof(NAL_OSREADPCICONFIG32_FUNC))
                {
                    NAL_OSREADPCICONFIG32_FUNC* FunctionData =
                        (NAL_OSREADPCICONFIG32_FUNC*)(&(NalIoctl->InputBuffer));
                    FunctionData->ReturnValue =
                        _NalOSReadPciConfig32(&FunctionData->PciLocation,
                                              FunctionData->DwordNumber,
                                              &FunctionData->Value);

                    NalStatus = NAL_SUCCESS;
                }
                break;

            case NAL_OSWRITEPCICONFIG32_FUNCID:
                if(NalIoctl->Size == sizeof(NAL_OSWRITEPCICONFIG32_FUNC))
                {
                    NAL_OSWRITEPCICONFIG32_FUNC* FunctionData =
                        (NAL_OSWRITEPCICONFIG32_FUNC*)(&(NalIoctl->InputBuffer));
                    FunctionData->ReturnValue =
                        _NalOSWritePciConfig32(&FunctionData->PciLocation,
                                               FunctionData->DwordNumber,
                                               FunctionData->Data);

                    NalStatus = NAL_SUCCESS;
                }
                break;

#ifdef NAL_DRIVER_UNSECURE
            case NAL_OSWRITEPCICONFIGVARIABLE_FUNCID:
                if(NalIoctl->Size == sizeof(NAL_OSWRITEPCICONFIGVARIABLE_FUNC))
                {
                    NAL_OSWRITEPCICONFIGVARIABLE_FUNC* FunctionData =
                        (NAL_OSWRITEPCICONFIGVARIABLE_FUNC*)(&(NalIoctl->InputBuffer));
                    FunctionData->ReturnValue =
                        _NalOSWritePciConfigVariable(&FunctionData->PciLocation,
                                                     FunctionData->DwordNumber,
                                                     FunctionData->ByteMask,
                                                     FunctionData->Data);

                    NalStatus = NAL_SUCCESS;
                }
                break;
#endif

            case NAL_ENABLEPCIDEVICE_FUNCID:
                if(NalIoctl->Size == sizeof(NAL_ENABLEPCIDEVICE_FUNC))
                {
                    NAL_ENABLEPCIDEVICE_FUNC* FunctionData =
                        (NAL_ENABLEPCIDEVICE_FUNC*)(&(NalIoctl->InputBuffer));
                    FunctionData->ReturnValue =
                        _NalEnablePciDevice(&FunctionData->DeviceLocation);
                    NalStatus = NAL_SUCCESS;
                }
                break;

            default:
                NalDebugPrint("NalResolveHwBusIoctl: FuctionId = %llu\n",
                              (unsigned long long)NalIoctl->FunctionId);
                NalStatus = NAL_IOCTL_INVALID_FUNCTION_ID;
                break;
        }
    }
    return NalStatus;
}
