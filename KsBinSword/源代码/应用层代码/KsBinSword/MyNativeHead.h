#define OUT 
#define IN 
#define   ULONG_PTR   unsigned   long*   
typedef unsigned long           ULONG;
//记录进程信息的结构体ProcessInfo
struct PROC_ ;
typedef struct PROC_     
{
	ULONG addr;         //进程地址（对象（体）指针）           
	int pid;            //进程ID
	UCHAR name[16];     //进程名
	struct PROC *next;  //单向链表指针
}ProcessInfo;      
typedef struct _THREAD_INFO {   
	PVOID   pEThread ;  
	int   StartAddress; 
	ULONG   dwThreadId ;  

	struct _THREAD_INFO* Next;

} THREAD_INFO, *PTHREAD_INFO;  
typedef struct _CLIENT_ID
{
	PVOID UniqueProcess;
	PVOID UniqueThread;
} CLIENT_ID, *PCLIENT_ID;
typedef long NTSTATUS; 

// typedef struct _UNICODE_STRING
// {
// 	WORD Length;
// 	WORD MaximumLength;
// 	WORD * Buffer;
// } UNICODE_STRING, *PUNICODE_STRING;
typedef struct _UNICODE_STRING {
	USHORT  Length;
	USHORT  MaximumLength;
	PWSTR  Buffer;
} UNICODE_STRING, *PUNICODE_STRING;

typedef struct _OBJECT_ATTRIBUTES {
	ULONG  Length;   
	HANDLE  RootDirectory;  
	PUNICODE_STRING  ObjectName;   
	ULONG  Attributes;  
	PVOID  SecurityDescriptor;
	PVOID  SecurityQualityOfService;
} OBJECT_ATTRIBUTES, *POBJECT_ATTRIBUTES;
typedef struct
{
	DWORD ExitStatus; // 接收进程终止状态
	DWORD PebBaseAddress; // 接收进程环境块地址
	DWORD AffinityMask; // 接收进程关联掩码
	DWORD BasePriority; // 接收进程的优先级类
	ULONG UniqueProcessId; // 接收进程ID
	ULONG InheritedFromUniqueProcessId; //接收父进程ID
} PROCESS_BASIC_INFORMATION;
typedef struct _SYSTEM_HANDLE_TABLE_ENTRY_INFO{   
// 	int  UniqueProcessId;   
// 	int  CreatorBackTraceIndex;   
// 	byte  ObjectTypeIndex;   
// 	byte  HandleAttributes;   
// 	int   HandleValue ;   
// 	ULONG  pObject;   
// 	ULONG GrantedAccess;   
	USHORT UniqueProcessId;
	USHORT CreatorBackTraceIndex;
	UCHAR ObjectTypeIndex;
	UCHAR HandleAttributes;
	USHORT HandleValue;
	PVOID Object;
	ULONG GrantedAccess;
}SYSTEM_HANDLE_TABLE_ENTRY_INFO,*PSYSTEM_HANDLE_TABLE_ENTRY_INFO;   
//--------------------------------------------------------------------
typedef struct _SYSTEM_HANDLE_INFORMATION { // Information Class 16 
	ULONG ProcessId; 
	UCHAR ObjectTypeNumber; 
	UCHAR Flags;  // 0x01 = PROTECT_FROM_CLOSE, 0x02 = INHERIT 
	USHORT Handle; 
	PVOID Object; 
	ACCESS_MASK GrantedAccess; 
} SYSTEM_HANDLE_INFORMATION, *PSYSTEM_HANDLE_INFORMATION;
typedef struct _SYSTEM_HANDLE_INFORMATION_EX
{
	ULONG NumberOfHandles;
	SYSTEM_HANDLE_INFORMATION Information[1];
} SYSTEM_HANDLE_INFORMATION_EX,  *PSYSTEM_HANDLE_INFORMATION_EX;

//*********************************************************************


typedef struct
{
	UNICODE_STRING SectionFileName;
	WCHAR       NameBuffer[ANYSIZE_ARRAY];
} MEMORY_SECTION_NAME, *PMEMORY_SECTION_NAME;

//-------------------函数指针开始-------------------------
typedef  DWORD (WINAPI *ZwQueryVirtualMemory_)( 
	DWORD a, 
	DWORD b, 
	DWORD c, 
	DWORD d, 
	DWORD f, 
	DWORD e
	);
typedef NTSTATUS( WINAPI *pFnZwQueryInformationProcess)(
	HANDLE ProcessHandle,
	int ProcessInformationClass,
	PVOID ProcessInformation,
	ULONG ProcessInformationLength,
	PULONG ReturnLength
	);

typedef NTSTATUS(


				 NTAPI
				 *pFnNtOpenProcess)(

				 OUT PHANDLE             ProcessHandle,
				 IN ACCESS_MASK          AccessMask,
				 IN POBJECT_ATTRIBUTES   ObjectAttributes,
				 IN PCLIENT_ID           ClientId );

typedef NTSTATUS( 
				 NTAPI 
				 *pFnZwQuerySystemInformation)( 
				 ULONG SystemInformationClass, 
				 PVOID SystemInformation, 
				 ULONG SystemInformationLength, 
				 PULONG ReturnLength 
				 ); 
typedef NTSTATUS(

				 NTAPI
				 *pFnZwDuplicateObject) (
				 HANDLE SourceProcessHandle,
				 HANDLE SourceHandle,
				 HANDLE TargetProcessHandle,
				 PHANDLE TargetHandle,
				 ACCESS_MASK DesiredAccess,
				 ULONG HandleAttributes,
				 ULONG Options
				 );
typedef NTSTATUS( NTAPI
				 *pFnZwClose)(
				 IN HANDLE  Handle
				 );

typedef NTSTATUS(
				 NTAPI
				 *pFnZwAllocateVirtualMemory )(
				 HANDLE ProcessHandle,
				 PVOID *BaseAddress,
				 ULONG_PTR ZeroBits,
				 PSIZE_T RegionSize,
				 ULONG AllocationType,
				 ULONG Protect
				 );


typedef NTSTATUS(
				 NTAPI
				 *pFnZwFreeVirtualMemory )(
				 HANDLE ProcessHandle,
				 PVOID *BaseAddress,
				 PSIZE_T RegionSize,
				 ULONG FreeType
				 );
typedef NTSTATUS(

				 NTAPI
				 *pFnZwReadVirtualMemory) (
				 HANDLE ProcessHandle,
				 PVOID BaseAddress,
				 PVOID Buffer,
				 SIZE_T BufferSize,
				 PSIZE_T NumberOfBytesRead
				 );
typedef NTSTATUS(
				 NTAPI
				 *pFnZwWriteVirtualMemory) (
				 HANDLE ProcessHandle,
				 PVOID BaseAddress,
				 VOID *Buffer,
				 SIZE_T BufferSize,
				 PSIZE_T NumberOfBytesWritten
				 );
typedef NTSTATUS(

				 NTAPI
				 *pFnZwProtectVirtualMemory )(
				 HANDLE ProcessHandle,
				 PVOID *BaseAddress,
				 PSIZE_T RegionSize,
				 ULONG NewProtect,
				 PULONG OldProtect
				 );
typedef NTSTATUS(
NTAPI
*pFnNtOpenProcess)(
			  
			  OUT PHANDLE             ProcessHandle,
			  IN ACCESS_MASK          AccessMask,
			  IN POBJECT_ATTRIBUTES   ObjectAttributes,
			  IN PCLIENT_ID           ClientId );

#define InitializeObjectAttributes( p, n, a, r, s ) {(p)->Length = sizeof( OBJECT_ATTRIBUTES );(p)->RootDirectory = r;(p)->Attributes = a;(p)->ObjectName = n;(p)->SecurityDescriptor = s;(p)->SecurityQualityOfService = NULL;}


typedef VOID (CALLBACK* RTLINITUNICODESTRING)(PUNICODE_STRING,PCWSTR);


typedef NTSTATUS (WINAPI *ZWOPENDIRECTORYOBJECT)(
	OUT PHANDLE DirectoryHandle,
	IN ACCESS_MASK DesiredAccess,
	IN POBJECT_ATTRIBUTES ObjectAttributes
	);


typedef
NTSTATUS
(WINAPI *ZWQUERYDIRECTORYOBJECT)(
								 IN HANDLE DirectoryHandle,
								 OUT PVOID Buffer,
								 IN ULONG BufferLength,
								 IN BOOLEAN ReturnSingleEntry,
								 IN BOOLEAN RestartScan,
								 IN OUT PULONG Context,
								 OUT PULONG ReturnLength OPTIONAL
								 );


typedef
NTSTATUS
(WINAPI *ZWCLOSE)(
				  IN HANDLE Handle
				  );


typedef struct _DIRECTORY_BASIC_INFORMATION {
	UNICODE_STRING ObjectName;
	UNICODE_STRING ObjectTypeName;
} DIRECTORY_BASIC_INFORMATION, *PDIRECTORY_BASIC_INFORMATION;

typedef struct _SYSTEM_MODULE_INFORMATION { // Information Class 11
	ULONG Reserved[2];
	PVOID Base;
	ULONG Size;
	ULONG Flags;
	USHORT Index;
	USHORT Unknown;
	USHORT LoadCount;
	USHORT ModuleNameOffset;
	CHAR ImageName[256];
} SYSTEM_MODULE_INFORMATION, *PSYSTEM_MODULE_INFORMATION;
typedef struct _TIME_FIELDS {
    short  Year;        // range [1601...]
    short  Month;       // range [1..12]
    short  Day;         // range [1..31]
    short  Hour;        // range [0..23]
    short  Minute;      // range [0..59]
    short  Second;      // range [0..59]
    short  Milliseconds;// range [0..999]
    short  Weekday;     // range [0..6] == [Sunday..Saturday]
} TIME_FIELDS;
//typedef TIME_FIELDS *PTIME_FIELDS;
typedef struct _DIRECTORY_INFO{
       char FileName[50];      
	   LARGE_INTEGER  AllocationSize;
       TIME_FIELDS    CreationTime;
        TIME_FIELDS    LastAccessTime ; 
		TIME_FIELDS             LastWriteTime;  
		TIME_FIELDS			   ChangeTime; 	
        ULONG     FileAttributes ;    
}DIRECTORY_INFO ,*PDIRECTORY_INFO ;
typedef struct _DIRECTORY_INFO_EX{
       CString   path;
	   DIRECTORY_INFO  DirectoryInfo;
}DIRECTORY_INFO_EX ,*PDIRECTORY_INFO_EX ;
typedef struct _IO_STATUS_BLOCK
{

	NTSTATUS Status;

	ULONG Information;

} IO_STATUS_BLOCK,  *PIO_STATUS_BLOCK;
typedef struct
{

	ULONGLONG Vcn;

	ULONGLONG Lcn;

} MAPPING_PAIR,  *PMAPPING_PAIR;


typedef  struct tagLinkLcn
{

	ULONGLONG Lcn;
	ULONGLONG Len;
	tagLinkLcn*  Next;

} LinkLcn,  *PLinkLcn;//!!!!!!!!!!!!!!!!!!!
typedef struct
{

	ULONG NumberOfPairs;

	ULONGLONG StartVcn;

	MAPPING_PAIR Pair[1];

} GET_RETRIEVAL_DESCRIPTOR,  *PGET_RETRIEVAL_DESCRIPTOR;

typedef VOID(*PIO_APC_ROUTINE)(

							   PVOID ApcContext, 

							   PIO_STATUS_BLOCK IoStatusBlock, 

							   ULONG Reserved 

							   );
typedef  DWORD (WINAPI *PFNtFsControlFile)( 					
	HANDLE FileHandle,
	HANDLE Event,					// optional
	PIO_APC_ROUTINE ApcRoutine,		// optional
	PVOID ApcContext,				// optional
	PIO_STATUS_BLOCK IoStatusBlock,	
	ULONG FsControlCode,
	PVOID InputBuffer,				// optional
	ULONG InputBufferLength,
	PVOID OutputBuffer,				// optional
	ULONG OutputBufferLength);

//#define SectorsPerCluster  0x8
//#define SectorsPerTrack    0x3f
#define BytesPerSector_     0x200
#define STATUS_BUFFER_OVERFLOW           ((UINT)0x80000005L)
#define LLINVALID      ((ULONGLONG) -1)
#define FILEMAPSIZE         (512+2)
//typedef HANDLE      SC_HANDLE;
#define SC_MANAGER_CREATE_SERVICE      0x0002
#define OBJ_CASE_INSENSITIVE    0x00000040L
#define DIRECTORY_QUERY            (0x0001)
#define STATUS_SUCCESS            ((NTSTATUS)0x00000000L) // ntsubauth
#define STATUS_MORE_ENTRIES        ((NTSTATUS)0x00000105L)
#define STATUS_BUFFER_TOO_SMALL    ((NTSTATUS)0xC0000023L)
/////////////////////////////////////////////////////////////////////////
#define OBJ_CASE_INSENSITIVE    0x00000040L
#define DIRECTORY_QUERY            (0x0001)
#define STATUS_SUCCESS            ((NTSTATUS)0x00000000L) // ntsubauth
#define STATUS_MORE_ENTRIES        ((NTSTATUS)0x00000105L)
#define STATUS_BUFFER_TOO_SMALL    ((NTSTATUS)0xC0000023L)
#define STATUS_SUCCESS                          ((NTSTATUS)0x00000000L) // ntsubauth
#define STATUS_INFO_LENGTH_MISMATCH      ((NTSTATUS)0xC0000004)
#define ProcessBasicInformation 0
#define DUPLICATE_SAME_ATTRIBUTES 0x00000004
#define SystemHandleInformation 0x10
#define OBJ_INHERIT 0x00000002L
#define NT_SUCCESS(Status) ((NTSTATUS)(Status) >= 0) 


#define MemorySectionName 2
#define STR_LENGTH 512
#define	LC_ALL 0


typedef enum _SYSTEM_INFORMATION_CLASS     //    Q S
{
	SystemBasicInformation,                // 00 Y N
	SystemProcessorInformation,            // 01 Y N
	SystemPerformanceInformation,          // 02 Y N
	SystemTimeOfDayInformation,            // 03 Y N
	SystemNotImplemented1,                 // 04 Y N
	SystemProcessesAndThreadsInformation,  // 05 Y N
	SystemCallCounts,                      // 06 Y N
	SystemConfigurationInformation,        // 07 Y N
	SystemProcessorTimes,                  // 08 Y N
	SystemGlobalFlag,                      // 09 Y Y
	SystemNotImplemented2,                 // 10 Y N
	SystemModuleInformation,               // 11 Y N
	SystemLockInformation,                 // 12 Y N
	SystemNotImplemented3,                 // 13 Y N
    SystemNotImplemented4,                 // 14 Y N
	SystemNotImplemented5,                 // 15 Y N
	SystemHandleInformation_,               // 16 Y N
	SystemObjectInformation,               // 17 Y N
	SystemPagefileInformation,             // 18 Y N
	SystemInstructionEmulationCounts,      // 19 Y N
	SystemInvalidInfoClass1,               // 20
	SystemCacheInformation,                // 21 Y Y
	SystemPoolTagInformation,              // 22 Y N
	SystemProcessorStatistics,             // 23 Y N
	SystemDpcInformation,                  // 24 Y Y
	SystemNotImplemented6,                 // 25 Y N
	SystemLoadImage,                       // 26 N Y
	SystemUnloadImage,                     // 27 N Y
	SystemTimeAdjustment,                  // 28 Y Y
	SystemNotImplemented7,                 // 29 Y N
	SystemNotImplemented8,                 // 30 Y N
	SystemNotImplemented9,                 // 31 Y N
	SystemCrashDumpInformation,            // 32 Y N
	SystemExceptionInformation,            // 33 Y N
	SystemCrashDumpStateInformation,       // 34 Y Y/N
	SystemKernelDebuggerInformation,       // 35 Y N
	SystemContextSwitchInformation,        // 36 Y N
	SystemRegistryQuotaInformation,        // 37 Y Y
	SystemLoadAndCallImage,                // 38 N Y
	SystemPrioritySeparation,              // 39 N Y
	SystemNotImplemented10,                // 40 Y N
	SystemNotImplemented11,                // 41 Y N
	SystemInvalidInfoClass2,               // 42
	SystemInvalidInfoClass3,               // 43
	SystemTimeZoneInformation,             // 44 Y N
	SystemLookasideInformation,            // 45 Y N
	SystemSetTimeSlipEvent,                // 46 N Y
	SystemCreateSession,                   // 47 N Y
	SystemDeleteSession,                   // 48 N Y
	SystemInvalidInfoClass4,               // 49
	SystemRangeStartInformation,           // 50 Y N
	SystemVerifierInformation,             // 51 Y Y
	SystemAddVerifier,                     // 52 N Y
	SystemSessionProcessesInformation      // 53 Y N
} SYSTEM_INFORMATION_CLASS;


#define OB_TYPE_INDEX_TYPE              1 // [ObjT] "Type"
#define OB_TYPE_INDEX_DIRECTORY         2 // [Dire] "Directory"
#define OB_TYPE_INDEX_SYMBOLIC_LINK     3 // [Symb] "SymbolicLink"
#define OB_TYPE_INDEX_TOKEN             4 // [Toke] "Token"
#define OB_TYPE_INDEX_PROCESS           5 // [Proc] "Process"
#define OB_TYPE_INDEX_THREAD            6 // [Thre] "Thread"
#define OB_TYPE_INDEX_JOB               7 // [Job ] "Job"
#define OB_TYPE_INDEX_EVENT             8 // [Even] "Event"
#define OB_TYPE_INDEX_EVENT_PAIR        9 // [Even] "EventPair"
#define OB_TYPE_INDEX_MUTANT           10 // [Muta] "Mutant"
#define OB_TYPE_INDEX_CALLBACK         11 // [Call] "Callback"
#define OB_TYPE_INDEX_SEMAPHORE        12 // [Sema] "Semaphore"
#define OB_TYPE_INDEX_TIMER            13 // [Time] "Timer"
#define OB_TYPE_INDEX_PROFILE          14 // [Prof] "Profile"
#define OB_TYPE_INDEX_WINDOW_STATION   15 // [Wind] "WindowStation"
#define OB_TYPE_INDEX_DESKTOP          16 // [Desk] "Desktop"
#define OB_TYPE_INDEX_SECTION          17 // [Sect] "Section"
#define OB_TYPE_INDEX_KEY              18 // [Key ] "Key"
#define OB_TYPE_INDEX_PORT             19 // [Port] "Port"
#define OB_TYPE_INDEX_WAITABLE_PORT    20 // [Wait] "WaitablePort"
#define OB_TYPE_INDEX_ADAPTER          21 // [Adap] "Adapter"
#define OB_TYPE_INDEX_CONTROLLER       22 // [Cont] "Controller"
#define OB_TYPE_INDEX_DEVICE           23 // [Devi] "Device"
#define OB_TYPE_INDEX_DRIVER           24 // [Driv] "Driver"
#define OB_TYPE_INDEX_IO_COMPLETION    25 // [IoCo] "IoCompletion"
#define OB_TYPE_INDEX_FILE             26 // [File] "File"
#define OB_TYPE_INDEX_WMI_GUID         27 // [WmiG] "WmiGuid" 

// 打开设备