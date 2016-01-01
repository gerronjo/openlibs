#include "stdafx.h"
#include "shadow_ssdt.h"


//定义为static只能被一个cpp访问
//SSDT用于保存相关信息
PSSDTSaveTable pSSDTSaveTable;
//SSDT函数数量
ULONG ulSSDTTotal;
//
PSYSTEM_MODULE_LIST pSystem_Module_List;


//获得SSDT函数索引
ULONG GetSSDTFuncIndex(LPSTR lpName)
{
 //获得函数地址
 FARPROC pAddress1 = GetProcAddress(GetModuleHandle(L"ntdll.dll"),lpName);

 //转换成PULONG
 PULONG pAddress2 = (PULONG)pAddress1;

 if (*(PBYTE)pAddress1 == 0xB8)
 {
	  //kd> u ntdll!NtCreateFile
      //ntdll!NtCreateFile:
      //772555c8 b842000000      mov     eax,42h
      //772555cd ba0003fe7f      mov     edx,offset SharedUserData!SystemCallStub (7ffe0300)
      //772555d2 ff12            call    dword ptr [edx]
      //772555d4 c22c00          ret     2Ch
      //772555d7 90              nop

      //(ULONG)pAddress2+1，b8汇编指令为一个字节所以要加1，然后重新转换地址指针
      pAddress2 = (PULONG)((ULONG)pAddress2+1);

      //*pAddress2表示获得此地址的内容，内容长度为4个字节
      return *pAddress2;
 }

 return -1;
}

//通过地址得到模块名
BOOL GetModuleNameByAddr(ULONG ulAddr,PSYSTEM_MODULE_LIST pSystem_module_list,LPSTR lpBuffer)
{
	ULONG i;

	for (i=0;i<pSystem_module_list->ulCount; i++)
	{
		//得到驱动模块基址
		ULONG ulBase = (ULONG)pSystem_module_list->smi[i].Base;
		//得到驱动模块大小
		ULONG ulMax  = ulBase + pSystem_module_list->smi[i].Size;

		//如果ulAddr处于模块地址之间的话，说明就是此模块里面的函数
		if (ulBase<=ulAddr && ulAddr<=ulMax)
		{
			//复制到缓冲区
			lstrcpynA(lpBuffer,pSystem_module_list->smi[i].ImageName,sizeof(pSystem_module_list->smi[i].ImageName));
			//返回真
			return TRUE;
		}
	}

	//没有找到的话返回假
	return FALSE;
}

//根据服务号将SSDT缓冲表排序
VOID SSDTSTOrderByServiceNum()
{
	ULONG i, j;

	//冒泡排序法
	for(i = ulSSDTTotal - 1; i > 0; i --)
	{
		for(j = 0; j < i; j ++)
		{
			if(((PSSDTSaveTable)((ULONG)pSSDTSaveTable + j * sizeof(SSDTSaveTable)))->ulServiceNumber > ((PSSDTSaveTable)((ULONG)pSSDTSaveTable + (j+1) * sizeof(SSDTSaveTable)))->ulServiceNumber)
			{
				//交换
				SSDTSaveTable SSDTSTTemp;
				memcpy((LPVOID)&SSDTSTTemp,(LPVOID)((ULONG)pSSDTSaveTable + j * sizeof(SSDTSaveTable)),sizeof(SSDTSaveTable));
				memcpy((LPVOID)((ULONG)pSSDTSaveTable + j * sizeof(SSDTSaveTable)),(LPVOID)((ULONG)pSSDTSaveTable + (j+1) * sizeof(SSDTSaveTable)),sizeof(SSDTSaveTable));
				memcpy((LPVOID)((ULONG)pSSDTSaveTable + (j+1) * sizeof(SSDTSaveTable)),(LPVOID)&SSDTSTTemp,sizeof(SSDTSaveTable));
			}
		}
	}
}

//枚举SSDT
BOOL GetCurrentSSDTFunc(HANDLE hDriver)
{
	ULONG i;
	ULONG ulReal=0;
	DWORD dwRet=0;
	//内核文件基址
	ULONG ulNTBase;
	//ntdll句柄
	HMODULE hNtdll;

	//创建模块信息链表
	pSystem_Module_List = GetSystemModuleList();

	//如果创建失败
	if(pSystem_Module_List == NULL)
	{
		return FALSE;
	}
	//得到NT内核文件基址
	ulNTBase = (ULONG)(pSystem_Module_List->smi->Base);

	//如果ntdll句柄获取失败
	if(!(hNtdll=LoadLibrary(L"ntdll.dll")))
	{
		return FALSE;
	}

	//得到SSDT个数
	BOOL bRet = DeviceIoControl(hDriver,IOCTL_SSDT_GET, NULL, 0, &ulSSDTTotal, sizeof(ULONG), &dwRet, NULL);
	//如果获得成功
	if(!bRet)
	{
		return NULL;
	}

	//填充
	pSSDTSaveTable  = (PSSDTSaveTable)malloc(ulSSDTTotal*sizeof(SSDTSaveTable));
	//如果分配内存失败
	if(pSSDTSaveTable == NULL)
	{
		return FALSE;
	}

	//初始化它
	for(i = 0; i < ulSSDTTotal; i ++ )
	{
		((PSSDTSaveTable)((ULONG)pSSDTSaveTable + i * sizeof(SSDTSaveTable)))->ulServiceNumber = -1;
		((PSSDTSaveTable)((ULONG)pSSDTSaveTable + i * sizeof(SSDTSaveTable)))->ulCurrentFuncAddress = 0L;
		((PSSDTSaveTable)((ULONG)pSSDTSaveTable + i * sizeof(SSDTSaveTable)))->ulOriginalFuncAddress = 0L;
		//清零ServiceFunctionName
		memset(((PSSDTSaveTable)((ULONG)pSSDTSaveTable + i * sizeof(SSDTSaveTable)))->ServiceFunctionName, 0, MAX_PATH);
		//清零ModuleName
		memset(((PSSDTSaveTable)((ULONG)pSSDTSaveTable + i * sizeof(SSDTSaveTable)))->ModuleName, 0, MAX_PATH);
	}

	//枚举
	ULONG ulNtDllOffset = (ULONG)hNtdll;
	//PE头部
	ulNtDllOffset = ulNtDllOffset + ((PIMAGE_DOS_HEADER)hNtdll)->e_lfanew + 4;
	//数据目录
	ulNtDllOffset = ulNtDllOffset + sizeof(IMAGE_FILE_HEADER) + sizeof(IMAGE_OPTIONAL_HEADER) - IMAGE_NUMBEROF_DIRECTORY_ENTRIES*sizeof(IMAGE_DATA_DIRECTORY);
	//导出表
	ulNtDllOffset = (DWORD)hNtdll + ((PIMAGE_DATA_DIRECTORY)ulNtDllOffset)->VirtualAddress;

	//导出目录表
	PIMAGE_EXPORT_DIRECTORY pImage_export_directory = (PIMAGE_EXPORT_DIRECTORY)ulNtDllOffset;
	//得到函数个数
	ULONG ulFuncNameCount = pImage_export_directory->NumberOfNames;

	//函数名数组指针
	PULONG pFuncName = (PULONG)((ULONG)hNtdll + pImage_export_directory->AddressOfNames);

	//循环找函数名
	for( i = 0; i < ulFuncNameCount; i ++ )
	{
		//函数名
		LPSTR pszName = (LPSTR)((ULONG)hNtdll + pFuncName[i]);

		//Nt 开头的函数
		if(pszName[0] == 'N' && pszName[1] == 't')	
		{
			//获得函数的服务号
			ULONG ulIndex = GetSSDTFuncIndex(pszName);

			if (ulIndex==-1)
			{
				continue;
			}

			ULONG ulAddr = 0;
			DeviceIoControl(hDriver, IOCTL_SSDT_GETFUNCADDR, &ulIndex, sizeof(ULONG), &ulAddr, sizeof(ULONG), &dwRet, NULL);

			//通过地址得到模块名
			char buffer[MAX_PATH];
			memset(buffer, 0, sizeof(buffer));

			//获得模块名
			if(GetModuleNameByAddr(ulAddr, pSystem_Module_List, buffer))
			{
				memcpy(((PSSDTSaveTable)((ULONG)pSSDTSaveTable + ulReal*sizeof(SSDTSaveTable)))->ModuleName, buffer, sizeof(buffer));
			}

			//保存SSDT信息到缓冲表中
			((PSSDTSaveTable)((ULONG)pSSDTSaveTable + ulReal*sizeof(SSDTSaveTable)))->ulServiceNumber = ulIndex;
			((PSSDTSaveTable)((ULONG)pSSDTSaveTable + ulReal*sizeof(SSDTSaveTable)))->ulCurrentFuncAddress = ulAddr;
			memcpy(((PSSDTSaveTable)((ULONG)pSSDTSaveTable + ulReal*sizeof(SSDTSaveTable)))->ServiceFunctionName, pszName,sizeof(((PSSDTSaveTable)((ULONG)pSSDTSaveTable + ulReal*sizeof(SSDTSaveTable)))->ServiceFunctionName));

			ulReal++;
		}
	}

	return TRUE;
}




DWORD FindKiServiceTable(HMODULE hModule,DWORD ulKeServiceDescriptorTable)
{ 
	PIMAGE_FILE_HEADER    pImage_file_header;
	PIMAGE_OPTIONAL_HEADER    pImage_optional_header;
	PIMAGE_SECTION_HEADER    pImage_section_header;
	PIMAGE_BASE_RELOCATION    pbr;
	PIMAGE_FIXUP_ENTRY    pfe;

	DWORD    dwFixups=0,i,dwPointerRva,dwPointsToRva,dwKiServiceTable;
	BOOL    bFirstChunk;

	//获得DOS头部
	PIMAGE_DOS_HEADER pImage_dos_header=(PIMAGE_DOS_HEADER)hModule;

	//不是有效PE标志
	if ((pImage_dos_header->e_magic!=IMAGE_DOS_SIGNATURE))
	{
		return FALSE;
	}

	//NT头部
	PIMAGE_NT_HEADERS pImage_nt_header = (PIMAGE_NT_HEADERS)((ULONG)pImage_dos_header + pImage_dos_header->e_lfanew);

	//不是有效PE标志
	if (pImage_nt_header->Signature!=IMAGE_NT_SIGNATURE)
	{
		return FALSE;
	}

	//文件头部
	pImage_file_header = (PIMAGE_FILE_HEADER)((ULONG)pImage_nt_header + sizeof(IMAGE_NT_SIGNATURE));

	//可选头部
	pImage_optional_header = (PIMAGE_OPTIONAL_HEADER)((ULONG)pImage_file_header + sizeof(IMAGE_FILE_HEADER));

	//块头部
	pImage_section_header = (PIMAGE_SECTION_HEADER)((ULONG)pImage_optional_header + sizeof(IMAGE_OPTIONAL_HEADER));

	if ((pImage_optional_header->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress) &&
		(!((pImage_file_header->Characteristics)&IMAGE_FILE_RELOCS_STRIPPED))) {

			pbr=(PIMAGE_BASE_RELOCATION)RVATOVA(pImage_optional_header->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress,hModule);

			bFirstChunk=TRUE;

			while (bFirstChunk || pbr->VirtualAddress) { 
				bFirstChunk=FALSE;

				pfe=(PIMAGE_FIXUP_ENTRY)((DWORD)pbr+sizeof(IMAGE_BASE_RELOCATION));

				for (i=0;i<(pbr->SizeOfBlock-sizeof(IMAGE_BASE_RELOCATION))>>1;i++,pfe++) { 
					if (pfe->type==IMAGE_REL_BASED_HIGHLOW) { 
						dwFixups++;
						dwPointerRva=pbr->VirtualAddress+pfe->offset;

						dwPointsToRva=*(PDWORD)((DWORD)hModule+dwPointerRva)-(DWORD)pImage_optional_header->ImageBase;

						if (dwPointsToRva==ulKeServiceDescriptorTable) { 

							if (*(PWORD)((DWORD)hModule+dwPointerRva-2)==0x05c7) {

								dwKiServiceTable=*(PDWORD)((DWORD)hModule+dwPointerRva+4)-pImage_optional_header->ImageBase;
								return dwKiServiceTable;
							}
						}

					} 

				}
				*(PDWORD)&pbr+=pbr->SizeOfBlock;
			}
	}    

	return 0;
}

VOID GetOldSSDTAddress()
{   
	PIMAGE_FILE_HEADER        pImage_file_header;
	PIMAGE_OPTIONAL_HEADER    pImage_optional_header;
	PIMAGE_SECTION_HEADER     pImage_section_header;

	//获得基址
	ULONG ulKernelBase = (ULONG)(pSystem_Module_List->smi->Base);;

	//得到kernel文件镜像名称
	PCHAR pKernelName=pSystem_Module_List->smi->ModuleNameOffset + pSystem_Module_List->smi->ImageName;

	//加载kernel
	HMODULE hKernel=LoadLibraryExA(pKernelName,0,DONT_RESOLVE_DLL_REFERENCES);

	//如果失败
	if (hKernel==NULL) 
	{
		return;
	}

	//获得SSDT表基址
	ULONG ulKeServiceDescriptorTable=(DWORD)GetProcAddress(hKernel,"KeServiceDescriptorTable");

	//如果获得失败
	if (ulKeServiceDescriptorTable==NULL) 
	{
		return;
	}

	//获得相对偏移量
	ulKeServiceDescriptorTable = ulKeServiceDescriptorTable - (ULONG)hKernel;

	//查找KiServiceTable地址
	ULONG ulKiServiceTable = FindKiServiceTable(hKernel,ulKeServiceDescriptorTable);

	//如果失败
	if (ulKiServiceTable==NULL)
	{ 
		return;
	}

	//填充相关PE结构体
	//获得DOS头部
	PIMAGE_DOS_HEADER pImage_dos_header=(PIMAGE_DOS_HEADER)hKernel;

	//不是有效PE标志
	if ((pImage_dos_header->e_magic!=IMAGE_DOS_SIGNATURE))
	{
		return;
	}

	//NT头部
	PIMAGE_NT_HEADERS pImage_nt_header = (PIMAGE_NT_HEADERS)((ULONG)pImage_dos_header + pImage_dos_header->e_lfanew);

	//不是有效PE标志
	if (pImage_nt_header->Signature!=IMAGE_NT_SIGNATURE)
	{
		return;
	}

	//文件头部
	pImage_file_header = (PIMAGE_FILE_HEADER)((ULONG)pImage_nt_header + sizeof(IMAGE_NT_SIGNATURE));

	//可选头部
	pImage_optional_header = (PIMAGE_OPTIONAL_HEADER)((ULONG)pImage_file_header + sizeof(IMAGE_FILE_HEADER));

	//块头部
	pImage_section_header = (PIMAGE_SECTION_HEADER)((ULONG)pImage_optional_header + sizeof(IMAGE_OPTIONAL_HEADER));

	PULONG    pService;
	ULONG ulServices=0;

	for (pService=(PULONG)((ULONG)hKernel + ulKiServiceTable); *pService - pImage_optional_header->ImageBase < pImage_optional_header->SizeOfImage; pService++,ulServices++)
	{
		((PSSDTSaveTable)((ULONG)pSSDTSaveTable + ulServices * sizeof(SSDTSaveTable)))->ulOriginalFuncAddress = *pService - pImage_optional_header->ImageBase + ulKernelBase;
	}

	FreeLibrary(hKernel);
}