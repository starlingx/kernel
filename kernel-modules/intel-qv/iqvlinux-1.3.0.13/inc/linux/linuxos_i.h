
#ifndef SDK_NAL_INC_LINUX_LINUXOS_I_H_
#define SDK_NAL_INC_LINUX_LINUXOS_I_H_ 

#ifndef NAL_DRIVER
#include <sys/time.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <nalbytes.h>
#include <sys/errno.h>
#include <sys/stat.h>
#endif

#include <linuxdefs.h>
#include <linuxtypes.h>
#include <naltypes.h>
#include <hwbus_t.h>

#ifndef NAL_DRIVER

#include <pthread.h>
#include <linuxnallibrary.h>

NAL_STATUS
_NalOsSpecIoctlResultToNalErrorCode(
    IN INT32 IoctlResultCode,
    IN NAL_STATUS OperationFailSatus
    );

NAL_STATUS
_NalConnectToIoDriver(
    VOID
    );

NAL_STATUS
_NalConnectToMemDriver(
    VOID
    );

NAL_STATUS
_NalConnectToPciDriver(
    VOID
    );

NAL_STATUS
_NalDisconnectFromIoDriver(
    VOID
    );

NAL_STATUS
_NalDisconnectFromMemDriver(
    VOID
    );

NAL_STATUS
_NalDisconnectFromPciDriver(
    VOID
    );

CHAR *
_NalGetCurrentShell(
    VOID
    );

NAL_PHYSICAL_ADDRESS
_NalFindEfiRsdPtrStructureTable(
    VOID
    );

NAL_STATUS
_NalSilentCommand(
    IN CHAR * Command ,
    IN NAL_STATUS CommandFailStatus
    );

NAL_STATUS
_NalScanResponseOfCommand(
      IN CHAR* Command ,
      IN NAL_STATUS FailedStatus ,
      IN CHAR* Format ,
      ...
      );

extern NAL_OS_SPEC_GLOBAL_VARIABLES Global_OsVariables;
#else

NAL_OS_RUN_DOMAIN
_NalGetOsRunDomain(
    VOID
    );
#endif

#endif
