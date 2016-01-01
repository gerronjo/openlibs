#ifndef _FUNCTION_H_
#define _FUNCTION_H_

#include "ntifs.h"
#include "ntos.h"

extern PSERVICE_DESCRIPTOR_TABLE OriginalServiceDescriptorTable;

#define SystemModuleInformation 11

#define SEC_IMAGE    0x01000000

extern POBJECT_TYPE *PsProcessType;

NTSTATUS ZwQueryInformationProcess(
	__in       HANDLE ProcessHandle,
	__in       PROCESSINFOCLASS ProcessInformationClass,
	__out      PVOID ProcessInformation,
	__in       ULONG ProcessInformationLength,
	__out_opt  PULONG ReturnLength
	);

NTSTATUS ZwOpenProcess(
	__out     PHANDLE ProcessHandle,
	__in      ACCESS_MASK DesiredAccess,
	__in      POBJECT_ATTRIBUTES ObjectAttributes,
	__in_opt  PCLIENT_ID ClientId
	);

NTSTATUS KillProcess(
	ULONG ulEprocess
	);

NTSTATUS SafeCopyMemory(
	PVOID SrcAddr, 
	PVOID DstAddr,
	ULONG Size
	);

BOOLEAN ValidateUnicodeString(
	PUNICODE_STRING usStr
	);

//ULONG KeGetPreviousMode();

#endif