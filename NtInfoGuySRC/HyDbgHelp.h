#if !defined(HYDBGHELP_H)
	#define HYDBGHELP_H

	#include <stdbool.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include <windows.h>
	#include "gccDbgHelp.h"
	#include "dll.h"
	#include "dllext.h"
	#include "comm_ext.h"

	#define IN /* IN */
	#define OUT /* OUT */
	#define OPTIONAL /*OPTIONAL*/
	typedef long NTSTATUS;
	#define STATUS_INFO_LENGTH_MISMATCH ((NTSTATUS)0xC0000004L)
	#define NT_SUCCESS(Status) ((NTSTATUS)(Status) >= 0)
	
	#define SYMOPT_DEBUG 0x80000000
	
	typedef struct _IMAGEHLP_MODULE64 {
		DWORD    SizeOfStruct;           // set to sizeof(IMAGEHLP_MODULE64)
		DWORD64  BaseOfImage;            // base load address of module
		DWORD    ImageSize;              // virtual size of the loaded module
		DWORD    TimeDateStamp;          // date/time stamp from pe header
		DWORD    CheckSum;               // checksum from the pe header
		DWORD    NumSyms;                // number of symbols in the symbol table
		SYM_TYPE SymType;                // type of symbols loaded
		CHAR     ModuleName[32];         // module name
		CHAR     ImageName[256];         // image name
		CHAR     LoadedImageName[256];   // symbol file name
		// new elements: 07-Jun-2002
		CHAR     LoadedPdbName[256];     // pdb file name
		DWORD    CVSig;                  // Signature of the CV record in the debug directories
		CHAR     CVData[MAX_PATH * 3];   // Contents of the CV record
		DWORD    PdbSig;                 // Signature of PDB
		GUID     PdbSig70;               // Signature of PDB (VC 7 and up)
		DWORD    PdbAge;                 // DBI age of pdb
		bool     PdbUnmatched;           // loaded an unmatched pdb
		bool     DbgUnmatched;           // loaded an unmatched dbg
		bool     LineNumbers;            // we have line number information
		bool     GlobalSymbols;          // we have internal symbol information
		bool     TypeInfo;               // we have type information
		// new elements: 17-Dec-2003
		bool     SourceIndexed;          // pdb supports source server
		bool     Publics;                // contains public symbols
	}IMAGEHLP_MODULE64, *PIMAGEHLP_MODULE64;
	
	typedef struct _SYMBOL_INFO {
		ULONG       SizeOfStruct;
		ULONG       TypeIndex;        // Type Index of symbol
		ULONG64     Reserved[2];
		ULONG       Index;
		ULONG       Size;
		ULONG64     ModBase;          // Base Address of module comtaining this symbol
		ULONG       Flags;
		ULONG64     Value;            // Value of symbol, ValuePresent should be 1
		ULONG64     Address;          // Address of symbol including base address of module
		ULONG       Register;         // register holding value or pointer to value
		ULONG       Scope;            // scope of the symbol
		ULONG       Tag;              // pdb classification
		ULONG       NameLen;          // Actual length of name
		ULONG       MaxNameLen;
		CHAR        Name[1];          // Name of symbol
	}SYMBOL_INFO, *PSYMBOL_INFO;
	
	enum {sfImage = 0,sfDbg,sfPdb,sfMpd,sfMax};
	
	typedef bool (CALLBACK *PSYM_ENUMERATESYMBOLS_CALLBACK)(\
		PSYMBOL_INFO pSymInfo,ULONG SymbolSize,\
		PVOID UserContext);
	
	typedef enum 
	{
	  TI_GET_SYMTAG,
	  TI_GET_SYMNAME,
	  TI_GET_LENGTH,
	  TI_GET_TYPE,
	  TI_GET_TYPEID,
	  TI_GET_BASETYPE,
	  TI_GET_ARRAYINDEXTYPEID,
	  TI_FINDCHILDREN,
	  TI_GET_DATAKIND,
	  TI_GET_ADDRESSOFFSET,
	  TI_GET_OFFSET,
	  TI_GET_VALUE,
	  TI_GET_COUNT,
	  TI_GET_CHILDRENCOUNT,
	  TI_GET_BITPOSITION,
	  TI_GET_VIRTUALBASECLASS,
	  TI_GET_VIRTUALTABLESHAPEID,
	  TI_GET_VIRTUALBASEPOINTEROFFSET,
	  TI_GET_CLASSPARENTID,
	  TI_GET_NESTED,
	  TI_GET_SYMINDEX,
	  TI_GET_LEXICALPARENT,
	  TI_GET_ADDRESS,
	  TI_GET_THISADJUST,
	  TI_GET_UDTKIND,
	  TI_IS_EQUIV_TO,
	  TI_GET_CALLING_CONVENTION,
	  TI_IS_CLOSE_EQUIV_TO,
	  TI_GTIEX_REQS_VALID,
	  TI_GET_VIRTUALBASEOFFSET,
	  TI_GET_VIRTUALBASEDISPINDEX,
	  TI_GET_IS_REFERENCE,
	  TI_GET_INDIRECTVIRTUALBASECLASS 
	} IMAGEHLP_SYMBOL_TYPE_INFO;

	typedef struct _TI_FINDCHILDREN_PARAMS {
	  ULONG Count;
	  ULONG Start;
	  ULONG ChildId[1];
	}TI_FINDCHILDREN_PARAMS;

	
	#define SYMSTOREOPT_OVERWRITE 0x02
	#define MAX_SYM_NAME 2000

	bool WINAPI SymGetSymbolFile(HANDLE hProcess,char *SymPath,\
		const char *ImageFile,DWORD Type,char *SymbolFile,
		size_t cSymbolFile,char *DbgFile,size_t cDbgFile);
		
	bool WINAPI SymGetModuleInfo64(HANDLE hProcess,DWORD64 qwAddr,\
		PIMAGEHLP_MODULE64 ModuleInfo);
		
	DWORD64 WINAPI SymLoadModule64(HANDLE hProcess,HANDLE hFile,\
		const char *ImageName,const char *ModuleName,\
		DWORD64 BaseOfDll,DWORD SizeOfDll);
		
	bool WINAPI SymUnloadModule64(HANDLE hProcess,DWORD64 BaseOfDll);
	
	const char WINAPI *SymSrvStoreFile(HANDLE hProcess,\
		const char *SvrPath,const char *File,DWORD Flags);
		
	bool WINAPI SymEnumSymbols(HANDLE hProcess,ULONG64 BaseOfDll,\
		const char *Mask,\
		PSYM_ENUMERATESYMBOLS_CALLBACK EnumSymbolsCallback,\
		PVOID UserContext);

	bool WINAPI SymFromAddr(HANDLE hProcess,DWORD64 Address,\
		PDWORD64 Displacement,PSYMBOL_INFO Symbol);
		
	bool WINAPI SymFromName(HANDLE hProcess,const char *Name,\
		PSYMBOL_INFO Symbol);
		
	bool WINAPI SymEnumTypes(HANDLE hProcess,ULONG64 BaseOfDll,\
		PSYM_ENUMERATESYMBOLS_CALLBACK EnumSymbolsCallback,\
		PVOID UserContext);
		
	bool WINAPI SymGetTypeInfo(HANDLE hProcess,DWORD64 ModBase,\
		ULONG TypeId,IMAGEHLP_SYMBOL_TYPE_INFO GetType,\
		PVOID pInfo);

	typedef enum _SYSTEM_INFORMATION_CLASS
	{
		SystemModuleInformation = 11
	}SYSTEM_INFORMATION_CLASS;

	#define MAXIMUM_FILENAME_LENGTH 256
	typedef struct _SYSTEM_MODULE_INFORMATION
	{
		ULONG Reserved[2];
		PVOID Base;
		ULONG Size;
		ULONG Flags;
		USHORT Index;
		USHORT Rank;
		USHORT LoadCount;
		USHORT ModuleNameOffset;
		char ImageName[MAXIMUM_FILENAME_LENGTH];
	}SYSTEM_MODULE_INFORMATION,*PSYSTEM_MODULE_INFORMATION;

	NTSTATUS WINAPI NtQuerySystemInformation(\
		IN SYSTEM_INFORMATION_CLASS SystemInformationClass,\
		OUT PVOID SystemInformation,\
		IN ULONG SystemInformationLength,\
		OUT PULONG ReturnLength OPTIONAL);

	bool HyInitDbgHelp(const char *pSymPath);
	bool HyClearDbgHelp(void);
	bool HyLoadModulePdb(const char *pModName);
	DWORD64 HyLoadModule(const char *pModName);
	bool HyUnloadModule(DWORD64 DllBase);
	void HyPrintModuleInfo(DWORD64 DllBase);
	PVOID HyGetSymAddrRaw(DWORD64 DllBase,const char *pSymName);
	bool HyStoreModFile(const char *pModName);

	size_t HyFlushSysModsInfo(void);
	void HyClearSysModsInfo(void);
	bool HyGetRTModInfo(const char *pModName,OUT PSYSTEM_MODULE_INFORMATION pModInfo);
	PVOID HyGetRTModBase(const char *pModName);
	bool HyIsOpenPAE(void);
	bool HySymFromAddrRaw(DWORD64 Addr,OUT PSYMBOL_INFO pSI);
	bool HySymPrintStructElements(DWORD64 ModBase,const char *StructName);
#endif
