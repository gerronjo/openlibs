#include "ntifs.h"
#include <ntimage.h>

// Mark a HIGHADJ entry as needing an increment if reprocessing.
//
#define LDRP_RELOCATION_INCREMENT   0x1

//
// Mark a HIGHADJ entry as not suitable for reprocessing.
//
#define LDRP_RELOCATION_FINAL       0x2

PIMAGE_BASE_RELOCATION LdrProcessRelocationBlockLongLong(
								  IN ULONG_PTR VA,
								  IN ULONG SizeOfBlock,
								  IN PUSHORT NextOffset,
								  IN LONGLONG Diff
								  );

#define RTL_IMAGE_NT_HEADER_EX_FLAG_NO_RANGE_CHECK (0x00000001)

NTSTATUS
NTAPI
RtlImageNtHeaderEx(
    ULONG Flags,
    PVOID Base,
    ULONG64 Size,
    OUT PIMAGE_NT_HEADERS * OutHeaders
    );


PIMAGE_NT_HEADERS
NTAPI
RtlImageNtHeader(
				 PVOID Base
				 );

BOOL
FixBaseRelocTable (
    PVOID NewImageBase,
	DWORD ExistImageBase
    );