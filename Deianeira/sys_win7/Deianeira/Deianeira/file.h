#ifndef _FILE_
#define _FILE_

#include "common.h"
#include <string.h>

//线程信息单向列表
typedef struct _FileHandleInfo
{   
	//
	ULONG ulPID;
	//
	ULONG ulHandle;
	//
	WCHAR wstrHandleName[256];
	//单向列表指针
	struct FILE_HANDLE_INFO* next;
}FILE_HANDLE_INFO, *PFILE_HANDLE_INFO; 

//
PFILE_HANDLE_INFO pFileHandleInfo,pFileHandleInfoHead;


NTSTATUS NewIoCreateFile(
						 __out PHANDLE FileHandle,
						 __in  ACCESS_MASK DesiredAccess,
						 __in  POBJECT_ATTRIBUTES ObjectAttributes,
						 __out PIO_STATUS_BLOCK IoStatusBlock,
						 __in_opt PLARGE_INTEGER AllocationSize,
						 __in  ULONG FileAttributes,
						 __in  ULONG ShareAccess,
						 __in  ULONG Disposition,
						 __in  ULONG CreateOptions,
						 __in_opt PVOID EaBuffer,
						 __in  ULONG EaLength,
						 __in  CREATE_FILE_TYPE CreateFileType,
						 __in_opt PVOID InternalParameters,
						 __in  ULONG Options
						 );


NTSTATUS JmpNtSetInformationFile (
								  __in HANDLE FileHandle,
								  __out PIO_STATUS_BLOCK IoStatusBlock,
								  __in_bcount(Length) PVOID FileInformation,
								  __in ULONG Length,
								  __in FILE_INFORMATION_CLASS FileInformationClass
								  );

NTSTATUS NewNtSetInformationFile (
								  __in HANDLE FileHandle,
								  __out PIO_STATUS_BLOCK IoStatusBlock,
								  __in_bcount(Length) PVOID FileInformation,
								  __in ULONG Length,
								  __in FILE_INFORMATION_CLASS FileInformationClass
								  );

typedef NTSTATUS (*PNtDeleteFile)(
								  IN POBJECT_ATTRIBUTES ObjectAttributes
								  );

NTSTATUS NewZwDeleteFile(
						 IN POBJECT_ATTRIBUTES ObjectAttributes
						 );

//打开文件
HANDLE file_openFile(
					 IN LPTSTR lpFileName,
					 IN ACCESS_MASK DesiredAccess,
					 IN ULONG ShareAccess
					 );

//设置文件属性
BOOLEAN file_setFileAttributes(
							   IN HANDLE hFile,
							   IN ULONG FileAttributes
							   );

//删除文件
BOOLEAN file_deleteFile(
						IN HANDLE hFile
						);

//
VOID file_inlineHookDeleteFileFunc();

//
VOID file_unInlineHookDeleteFileFunc();

//挂钩IoCreateFile
VOID file_inlineHookIoCreateFile();

//恢复IoCreateFile
VOID file_unInlineHookIoCreateFile();


//从DisableCreateFileInfo中查找目标进程ID
//返回
//0表示没有找到，1表示找到了
ULONG file_findPIDInDisableCreateFileInfo(ULONG ulPID);

//从DisableCreateFileInfo中删除目标进程ID
VOID file_deletePIDInDisableCreateFileInfo(ULONG ulPID);


//从DisableDeleteFileInfo中查找目标进程ID
//返回
//0表示没有找到，1表示找到了
ULONG file_findPIDInDisableDeleteFileInfo(ULONG ulPID);

//从DisableDeleteFileInfo中删除目标进程ID
VOID file_deletePIDInDisableDeleteFileInfo(ULONG ulPID);


//从DisableOpenFileInfo中查找目标进程ID
//返回
//0表示没有找到，1表示找到了
ULONG file_findPIDInDisableOpenFileInfo(ULONG ulPID);

//从DisableOpenFileInfo中删除目标进程ID
VOID file_deletePIDInDisableOpenFileInfo(ULONG ulPID);

//获得文件句柄信息
NTSTATUS file_getFileHandleInfo(LPTSTR lpFilePath);

//获得大小
ULONG file_getFileHandleInfoCount();

//发送数据到ring3
NTSTATUS file_getFileHandleInfo_send(PFILE_HANDLE_INFO pBuffer);

#endif //_FILE_