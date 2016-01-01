#pragma once

/*
this header file defines all thing about LPC
including prototype of functions , structure and size of field of some structure
*/

#ifdef _cplusplus
extern "c"{
#endif

//data size info
#define MAX_DATA_SIZE (ULONG)0x104
#define MAX_MESSAGE_SIZE (ULONG)0x148
#define  MAX_SMALL_DATA_SIZE MAX_DATA_SIZE-sizeof(MESSAGE_DATA_INFO)

#ifndef _KERNEL_MODE
	#include <Windows.h>
	#include "ntstatus.h"
	#include "UserModeDefs.h"
	
#else
	#include <ntddk.h>
	#include "KernelModeDefs.h"
#endif

#include "CommonDefs.h"



typedef struct _PORT_MESSAGE { 
    USHORT DataSize; 
    USHORT MessageSize; 
    USHORT MessageType; 
    USHORT VirtualRangesOffset; 
    CLIENT_ID ClientId; 
    ULONG MessageId; 
    ULONG SectionSize; 
    BYTE Data[MAX_DATA_SIZE]; 
} PORT_MESSAGE, *PPORT_MESSAGE; 
	

typedef enum _LPC_TYPE { 
    LPC_NEW_MESSAGE,           // A new message 
    LPC_REQUEST,               // A request message 
    LPC_REPLY,                 // A reply to a request message 
    LPC_DATAGRAM,              // 
    LPC_LOST_REPLY,            // 
    LPC_PORT_CLOSED,           // Sent when port is deleted 
    LPC_CLIENT_DIED,           // Messages to thread termination ports 
    LPC_EXCEPTION,             // Messages to thread exception port 
    LPC_DEBUG_EVENT,           // Messages to thread debug port 
    LPC_ERROR_EVENT,           // Used by ZwRaiseHardError 
    LPC_CONNECTION_REQUEST     // Used by ZwConnectPort 
} LPC_TYPE;


typedef struct _PORT_SECTION_WRITE { 
    ULONG Length; 
    HANDLE SectionHandle; 
    ULONG SectionOffset; 
    ULONG ViewSize; 
    PVOID ViewBase; 
    PVOID TargetViewBase; 
} PORT_SECTION_WRITE, *PPORT_SECTION_WRITE;
	

 typedef struct _PORT_SECTION_READ {
	ULONG Length;
	ULONG ViewSize;
	PVOID ViewBase; 
} PORT_SECTION_READ, *PPORT_SECTION_READ; 


//unexported functions declare

typedef NTSTATUS (NTAPI* _CreatePort)(
	OUT PHANDLE PortHandle,
	IN POBJECT_ATTRIBUTES ObjectAttributes,
	IN ULONG MaxConnectionInfoLength,
	IN ULONG MaxMessageLength,
	IN ULONG MaxPoolUsage
	);
	
	
typedef NTSTATUS (NTAPI* _ConnectPort)(
	OUT PHANDLE PortHandle,
	IN PUNICODE_STRING PortName,
	IN PSECURITY_QUALITY_OF_SERVICE SecurityQos,
	IN OUT PPORT_SECTION_WRITE ClientView OPTIONAL,
	OUT PPORT_SECTION_READ ServerView OPTIONAL,
	OUT PULONG MaxMessageLength OPTIONAL,
	IN OUT PVOID ConnectInformation OPTIONAL,
	IN OUT PULONG ConnectInformationLength OPTIONAL
	);


typedef NTSTATUS (NTAPI* _ListenPort)(
	IN HANDLE PortHandle,
	OUT PPORT_MESSAGE Message
	);

	
typedef NTSTATUS (NTAPI* _AcceptConnectPort)(
	OUT PHANDLE PortHandle,
	IN PVOID PortIdentifier,
	IN PPORT_MESSAGE Message,
	IN BOOLEAN Accept,
	IN OUT PPORT_SECTION_WRITE ServerView OPTIONAL,
	OUT PPORT_SECTION_READ ClientView OPTIONAL
	);
	

typedef NTSTATUS (NTAPI* _CompleteConnectPort)(
	IN HANDLE PortHandle
	);
	
typedef NTSTATUS (NTAPI* _ReplyPort)(
	IN HANDLE PortHandle,
	IN PPORT_MESSAGE ReplyMessage
	);
	

typedef NTSTATUS (NTAPI* _ReplyWaitReceivePort)(
	IN HANDLE PortHandle,
	OUT PULONG PortIdentifier OPTIONAL,
	IN PPORT_MESSAGE ReplyMessage OPTIONAL,
	OUT PPORT_MESSAGE Message
	);
	

typedef NTSTATUS (NTAPI* _ReplyWaitReceivePortEx)(
	IN HANDLE PortHandle,
	OUT PVOID* PortIdentifier OPTIONAL,
	IN PPORT_MESSAGE ReplyMessage OPTIONAL,
	OUT PPORT_MESSAGE Message,
	IN PLARGE_INTEGER Timeout
	);



typedef NTSTATUS (NTAPI* _RequestPort)(
	IN HANDLE PortHandle,
	IN PPORT_MESSAGE RequestMessage
	);
	

typedef NTSTATUS (NTAPI* _RequestWaitReplyPort)(
	IN HANDLE PortHandle,
	IN PPORT_MESSAGE RequestMessage,
	OUT PPORT_MESSAGE ReplyMessage
	);
#ifdef _cplusplus
}
#endif