#pragma once

/*
this header file defines all thing that could be used by ring0 and ring3
for example, simple convention of connection , user-defined struct ,  macros , and so on
*/

#define  DELAY_ONE_MICROSECOND  (-10)
#define  DELAY_ONE_MILLISECOND (DELAY_ONE_MICROSECOND*1000)
#define  DELAY_ONE_SECOND (DELAY_ONE_MILLISECOND*1000)

#define SERVERNAME_W L"\\MY_SERVER"
#define SERVERNAME_A  "\\MY_SERVER"

//data type
#define DATA_NOTIFY_SHUTDOWN 1
#define DATA_TRANSFER_CHATTING 2
#define DATA_TRANSFER_PROCESSS 3

//some macro 
#define  MESSAGE_TO_REALDATA_BUF(msg) (PVOID)((ULONG)(msg.Data) + sizeof(MESSAGE_DATA_INFO))
#define  MESSAGE_TO_DATAINFO_BUF(msg) (PMESSAGE_DATA_INFO)(msg.Data)

#define SERVERNAME "ring3"
#define CLIENTNAME "ring0"
#define ASK_METTING "what hell about u ?"
#define REPLY_METTING "fine"
#define ASK_TIME  "what's time is it now ?"
#define REPLY_TIME "what a fuck question , shut up"


typedef struct _PROCESS_INFO{
	ULONG ulPid;
	char wchName[0x10];
} PROCESS_INFO , *PPROCESS_INFO;

typedef struct _PROCESS_COLLECTION
{
	ULONG dwCount;
	PROCESS_INFO piInfo[];
}PROCESS_COLLECTION , *PPROCESS_COLLECTION;

//the information of data that transfered with message
typedef struct _MESSAGE_DATA_INFO{
	union
	{
		struct
		{
			BYTE     DataType;
			BOOLEAN  bInSection;
			DWORD    DataLength;
			USHORT   Reserved;
		} TransInfo;
		BOOLEAN bSectionUsed;
	};
} MESSAGE_DATA_INFO, *PMESSAGE_DATA_INFO;

//
typedef struct _SHARED_MEMORY_INFO
{
	PVOID Base;
	DWORD Size;
}SHARED_MEMORY_INFO;

//the information of a connection , the most important thing is to indicate whether section is used 
//if used , point to the info of section for reading and section for writing
typedef struct _CONNECTION_INFO{
	HANDLE hDataPort;
	struct  
	{
		SHARED_MEMORY_INFO memr;
		SHARED_MEMORY_INFO memw;
	}MemInfo;
} CONNECTION_INFO,*PCONNECTION_INFO;