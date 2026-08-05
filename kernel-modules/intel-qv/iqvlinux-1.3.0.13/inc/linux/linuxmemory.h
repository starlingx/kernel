
#ifndef INC_FREEBSD_FREEBSDMEMORY_H_
#define INC_FREEBSD_FREEBSDMEMORY_H_ 

#include <naltypes.h>
#include <linuxtypes.h>

#define _NalReadPortOs8(a,p) NalReadPort8(p)
#define _NalReadPortOs16(a,p) NalReadPort16(p)
#define _NalReadPortOs32(a,p) NalReadPort32(p)
#define _NalWritePortOs8(a,p,v) NalWritePort8(p,v)
#define _NalWritePortOs16(a,p,v) NalWritePort16(p,v)
#define _NalWritePortOs32(a,p,v) NalWritePort32(p,v)
#define _NalReadRegisterOs8(a,d) NalReadRegister8(d)
#define _NalReadRegisterOs16(a,d) NalReadRegister16(d)
#define _NalReadRegisterOs32(a,d) NalReadRegister32(d)
#define _NalWriteRegisterOs8(a,d,v) NalWriteRegister8(d,v)
#define _NalWriteRegisterOs16(a,d,v) NalWriteRegister16(d,v)
#define _NalWriteRegisterOs32(a,d,v) NalWriteRegister32(d,v)

#define NalAllocateMemoryNonPagedPci(PDev,ByteCount,Alignment,PhysicalAddress) \
    _NalAllocateMemoryNonPagedPci((PDev), (ByteCount), (Alignment), (PhysicalAddress),__FILE__, __LINE__)

KVOID*
_NalAllocateMemoryNonPagedPci(
    IN KVOID* PDev,
    IN UINT32 ByteCount,
    IN UINT32 Alignment,
    OUT NAL_PHYSICAL_ADDRESS* PhysicalAddress,
    IN CHAR* NamedLocator,
    IN UINT32 LineNumber
    );

VOID
NalFreeMemoryNonPagedPci(
    IN KVOID* PDev,
    IN KVOID* Address,
    IN NAL_PHYSICAL_ADDRESS PhysicalAddress,
    IN UINT32 Size
    );

BOOLEAN
_NalOsSpecIsMappingByQvDriverPossible(
    VOID
    );

KVOID*
NalKMemset(
    IN KVOID* Dest,
    IN int Value,
    IN UINTN Size
    );

VOID*
NalKtoUMemcpy(
    IN VOID* Dest,
    IN const KVOID* Source,
    IN UINTN Size
    );

KVOID*
NalKtoKMemcpy(
    IN KVOID* Dest,
    IN const KVOID* Source,
    IN UINTN Size
    );

KVOID*
NalUtoKMemcpy(
    IN KVOID* Dest,
    IN const VOID* Source,
    IN UINTN Size
    );

BOOLEAN
_NalIsResourceExclusive(
    IN NAL_PHYSICAL_ADDRESS PhysicalAddress
    );

#endif
