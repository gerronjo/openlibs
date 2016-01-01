

#ifndef _KSBINSWORD_H
#define _KSBINSWORD_H 1



#define FILE_DEVICE_KSBINSWORD	0x8000



#define KSBINSWORD_IOCTL_BASE	0x800

//
// The device driver IOCTLs
//

#define CTL_CODE_KSBINSWORD(i) CTL_CODE(FILE_DEVICE_KSBINSWORD, KSBINSWORD_IOCTL_BASE+i, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_KSBINSWORD_HELLO	CTL_CODE_KSBINSWORD(0)
#define IOCTL_KSBINSWORD_TEST	CTL_CODE_KSBINSWORD(1)
#define IOCTL_KSBINSWORD_GETPRONUM	CTL_CODE_KSBINSWORD(2)//得到进程的数量，随便分配内存
#define IOCTL_KSBINSWORD_GETPRO	CTL_CODE_KSBINSWORD(3)//得到进程的各项内容
#define IOCTL_KSBINSWORD_GETTHRNUM	CTL_CODE_KSBINSWORD(4)//得到线程
#define IOCTL_KSBINSWORD_GETTHR	CTL_CODE_KSBINSWORD(5)//得到线程
#define IOCTL_KSBINSWORD_PMON CTL_CODE_KSBINSWORD(6)//进程监视开
#define IOCTL_KSBINSWORD_PMOFF CTL_CODE_KSBINSWORD(7)//进程监视关
#define IOCTL_KSBINSWORD_RMON CTL_CODE_KSBINSWORD(8)//注册表监视开
#define IOCTL_KSBINSWORD_RMOFF CTL_CODE_KSBINSWORD(9)//注册表监视关
#define IOCTL_KSBINSWORD_MMON CTL_CODE_KSBINSWORD(10)//模块监视开
#define IOCTL_KSBINSWORD_MMOFF CTL_CODE_KSBINSWORD(11)//模块监视关

#define IOCTL_KSBINSWORD_GETSSDT CTL_CODE_KSBINSWORD(12)//得到SSDT
#define IOCTL_KSBINSWORD_SETSSDT CTL_CODE_KSBINSWORD(13)//设置SSDT
#define IOCTL_KSBINSWORD_GETHOOK CTL_CODE_KSBINSWORD(14)//查询SSDT HOOK函数地址
#define IOCTL_KSBINSWORD_SETHOOK CTL_CODE_KSBINSWORD(15)//设置SSDT HOOK函数地址

#define IOCTL_KSBINSWORD_KILLTHREAD CTL_CODE_KSBINSWORD(16)//强制kill 线程
#define IOCTL_KSBINSWORD_KILLPROCESS CTL_CODE_KSBINSWORD(17)//强制kill 进程
#define IOCTL_KSBINSWORD_CLEARPROCESS CTL_CODE_KSBINSWORD(18)//清除查询进程痕迹
#define IOCTL_KSBINSWORD_CLEARTHREAD CTL_CODE_KSBINSWORD(19)//清除查询线程痕迹

#define IOCTL_KSBINSWORD_SCSI_READ  CTL_CODE_KSBINSWORD(20)//SRB读取扇区
#define IOCTL_KSBINSWORD_SCSI_WRITE CTL_CODE_KSBINSWORD(21)//SRB写入扇区

#define IOCTL_KSBINSWORD_GETMONINFO CTL_CODE_KSBINSWORD(22)//得到xx启动的信息

#define EPROCESS_SIZE 0
#define PEB_OFFSET 1
#define FILE_NAME_OFFSET 2

#define PROCESS_LINK_OFFSET 3
#define PROCESS_ID_OFFSET 4
#define EXIT_TIME_OFFSET 5

#define IN 
#define OUT
//_OBJECT_HEADER结构以及基于该结构实现从对象（体）指针获得对象（头）指针的宏
typedef struct _OBJECT_HEADER {
    union {
        struct {
            LONG PointerCount;
            LONG HandleCount;
        };
        LIST_ENTRY Entry;
    };
    POBJECT_TYPE Type;
    UCHAR NameInfoOffset;
    UCHAR HandleInfoOffset;
    UCHAR QuotaInfoOffset;
    UCHAR Flags;
	
    union {
        //POBJECT_CREATE_INFORMATION ObjectCreateInfo;
        PVOID QuotaBlockCharged;
    };
	
    PSECURITY_DESCRIPTOR SecurityDescriptor;
	
    QUAD Body;
} OBJECT_HEADER, *POBJECT_HEADER;


#define OBJECT_TO_OBJECT_HEADER(obj)         CONTAINING_RECORD( (obj), OBJECT_HEADER, Body )

//系统偏移量（因系统而异，可使用Windbg查询）
#define ProcessNameOffset  0x1fc
#define TYPE 0X08               //_OBJECT_HEADER中的偏移
#define NEXTFREETABLEENTRY 0X04 //_HANDLE_TABLE_ENTRY中的偏移
#define UNIQUEPROCESSID 0X84    //_EPROCESS中的偏移
#define IMAGEFILENAME 0X174     //_EPROCESS中的偏移
#define FLAGS 0x248             //_EPROCESS中的偏移
#define BOOL ULONG
typedef unsigned char BYTE;
//记录进程信息的结构体ProcessInfo
typedef struct PROC     
{
	ULONG addr;         //进程地址（对象（体）指针）           
	int pid;            //进程ID
	UCHAR name[16];     //进程名
	struct PROC *next;  //单向链表指针
}ProcessInfo;   
       
typedef struct tagKillThreadStruct
{
	ULONG dwProcessId;
	ULONG dwCid;
}KillThreadStruct,*PKillThreadStruct;

typedef struct _THREAD_INFO {   
    PVOID   pEThread ;  
	int   StartAddress;
    ULONG   dwThreadId ;   

    struct _THREAD_INFO* Next;
	
} THREAD_INFO, *PTHREAD_INFO;   
//////////////////////////////////////////////////////////////////////////
//SSDT 结构体
typedef struct _tagSSDT {
    PVOID pvSSDTBase;
    PVOID pvServiceCounterTable;
    ULONG ulNumberOfServices;
    PVOID pvParamTableBase;
} SSDT, *PSSDT;
/////////////////////////////////////////////////////////////////////////
// ModuleInfo 结构体
typedef struct ModuleInfo_t {
	ULONG Unused;
	ULONG Always0;
	ULONG ModuleBaseAddress;
	ULONG ModuleSize;
	ULONG Unknown;
	ULONG ModuleEntryIndex;
	/* Length of module name not including the path, this field contains valid value only for NTOSKRNL module*/
	USHORT ModuleNameLength; 
	USHORT ModulePathLength; /*Length of 'directory path' part of modulename*/
	char ModuleName[256];
} DRIVERMODULEINFO, *PDRIVERMODULEINFO;
typedef unsigned short WORD;

typedef struct ServiceDescriptorEntry {
    unsigned int *ServiceTableBase;
    unsigned int *ServiceCounterTableBase; //Used only in checked build
    unsigned int NumberOfServices;
    unsigned char *ParamTableBase;
} ServiceDescriptorTableEntry, *PServiceDescriptorTableEntry;




typedef struct _SECTION_IMAGE_INFORMATION {
	PVOID EntryPoint;
	ULONG StackZeroBits;
	ULONG StackReserved;
	ULONG StackCommit;
	ULONG ImageSubsystem;
	WORD SubsystemVersionLow;
	WORD SubsystemVersionHigh;
	ULONG Unknown1;
	ULONG ImageCharacteristics;
	ULONG ImageMachineType;
	ULONG Unknown2[3];
} SECTION_IMAGE_INFORMATION, *PSECTION_IMAGE_INFORMATION;


typedef ULONG	DWORD,	*PDWORD;
typedef USHORT	WORD;
typedef UCHAR	BYTE,	*PBYTE;
typedef LONG	INT;
typedef unsigned long DWORD;
typedef struct _SENSE_DATA
{
  unsigned char Valid;
  unsigned char SegmentNumber;
  unsigned char FileMark;
  unsigned char Information[4];
  unsigned char AdditionalSenseLength;
  unsigned char CommandSpecificInformation[4];
  unsigned char AdditionalSenseCode;
  unsigned char AdditionalSenseCodeQualifier;
  unsigned char FieldReplaceableUnitCode;
  unsigned char SenseKeySpecific[3];
} SENSE_DATA,  *PSENSE_DATA;

#pragma pack(1)
typedef struct _PARTITION_ENTRY
{
  UCHAR active;
  UCHAR StartHead;
  UCHAR StartSector;
  UCHAR StartCylinder;
  UCHAR PartitionType;
  UCHAR EndHead;
  UCHAR EndSector;
  UCHAR EndCylinder;
  ULONG StartLBA;
  ULONG TotalSector;
} PARTITION_ENTRY,  *PPARTITION_ENTRY;

typedef struct _MBR_SECTOR
{
  UCHAR BootCode[446];
  PARTITION_ENTRY Partition[4];
  USHORT Signature;
} MBR_SECTOR,  *PMBR_SECTOR;

typedef struct _BBR_SECTOR
{
  USHORT JmpCode;
  UCHAR NopCode;
  UCHAR OEMName[8];
  USHORT BytesPerSector;
  UCHAR SectorsPerCluster;
  USHORT ReservedSectors;
  UCHAR NumberOfFATs;
  USHORT RootEntries;
  USHORT NumberOfSectors16;
  UCHAR MediaDescriptor;
  USHORT SectorsPerFAT16;
  USHORT SectorsPerTrack;
  USHORT HeadsPerCylinder;
  ULONG HiddenSectors;
  ULONG NumberOfSectors32;
  ULONG SectorsPerFAT32;
} BBR_SECTOR,  *PBBR_SECTOR;
#pragma pack()

typedef struct _SYSTEM_MODULE_INFORMATION
{
  ULONG Reserved[2];
  PVOID Base;
  ULONG Size;
  ULONG Flags;
  USHORT Index;
  USHORT Unknown;
  USHORT LoadCount;
  USHORT ModuleNameOffset;
  CHAR ImageName[255];
} SYSTEM_MODULE_INFORMATION,  *PSYSTEM_MODULE_INFORMATION;
typedef struct _DEVICE_EXTENSION
{
    ULONG  StateVariable;
	
} DEVICE_EXTENSION, *PDEVICE_EXTENSION;
// 
// SCSI I/O Request Block 
// 
// 
// typedef struct _SCSI_REQUEST_BLOCK { 
//     USHORT Length;                  // offset 0 
//     UCHAR Function;                 // offset 2 
//     UCHAR SrbStatus;                // offset 3 
//     UCHAR ScsiStatus;               // offset 4 
//     UCHAR PathId;                   // offset 5 
//     UCHAR TargetId;                 // offset 6 
//     UCHAR Lun;                      // offset 7 
//     UCHAR QueueTag;                 // offset 8 
//     UCHAR QueueAction;              // offset 9 
//     UCHAR CdbLength;                // offset a 
//     UCHAR SenseInfoBufferLength;    // offset b 
//     ULONG SrbFlags;                 // offset c 
//     ULONG DataTransferLength;       // offset 10 
//     ULONG TimeOutValue;             // offset 14 
//     PVOID DataBuffer;               // offset 18 
//     PVOID SenseInfoBuffer;          // offset 1c 
//     struct _SCSI_REQUEST_BLOCK *NextSrb; // offset 20 
//     PVOID OriginalRequest;          // offset 24 
//     PVOID SrbExtension;             // offset 28 
//     ULONG QueueSortKey;             // offset 2c 
//     UCHAR Cdb[16];                  // offset 30 
// } SCSI_REQUEST_BLOCK, *PSCSI_REQUEST_BLOCK; 


typedef struct _FileSystemControl{
	ULONG OutputBufferLength;
	ULONG POINTER_ALIGNMENT InputBufferLength;
	ULONG POINTER_ALIGNMENT FsControlCode;
	PVOID Type3InputBuffer;
} FileSystemControl,*PFileSystemControl;



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
		SystemHandleInformation,               // 16 Y N
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
#define KSBINSWORD_WIN32_DEVICE_NAME_A	"\\\\.\\KsBinSword"
#define KSBINSWORD_WIN32_DEVICE_NAME_W	L"\\\\.\\KsBinSword"
#define KSBINSWORD_DEVICE_NAME_A			"\\Device\\KsBinSword"
#define KSBINSWORD_DEVICE_NAME_W			L"\\Device\\KsBinSword"
#define KSBINSWORD_DOS_DEVICE_NAME_A		"\\DosDevices\\KsBinSword"
#define KSBINSWORD_DOS_DEVICE_NAME_W		L"\\DosDevices\\KsBinSword"

#ifdef _UNICODE
#define KSBINSWORD_WIN32_DEVICE_NAME KSBINSWORD_WIN32_DEVICE_NAME_W
#define KSBINSWORD_DEVICE_NAME		KSBINSWORD_DEVICE_NAME_W
#define KSBINSWORD_DOS_DEVICE_NAME	KSBINSWORD_DOS_DEVICE_NAME_W
#else
#define KSBINSWORD_WIN32_DEVICE_NAME KSBINSWORD_WIN32_DEVICE_NAME_A
#define KSBINSWORD_DEVICE_NAME		KSBINSWORD_DEVICE_NAME_A
#define KSBINSWORD_DOS_DEVICE_NAME	KSBINSWORD_DOS_DEVICE_NAME_A
#endif

#endif