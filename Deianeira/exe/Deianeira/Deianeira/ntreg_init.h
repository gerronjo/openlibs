#include "../common/common.h"
#include "ntreg.h"

//
#define HIVE_MACHINE_SAM		"\\REGISTRY\\MACHINE\\SAM"
#define HIVE_MACHINE_SECURITY	"\\REGISTRY\\MACHINE\\SECURITY"
#define HIVE_MACHINE_SOFTWARE	"\\REGISTRY\\MACHINE\\SOFTWARE"
#define HIVE_MACHINE_SYSTEM		"\\REGISTRY\\MACHINE\\SYSTEM"
#define HIVE_USER_DEFAULT	    "\\REGISTRY\\USER\\.DEFAULT"

//
typedef ULONG (WINAPI *pNtQuerySystemInformation)(ULONG, PVOID, ULONG, PULONG);
//
typedef ULONG (WINAPI *pZwQueryInformationFile)(HANDLE, PVOID, PVOID, ULONG, ULONG);
//
typedef ULONG (WINAPI *pZwQueryObject)(HANDLE, ULONG, PVOID, ULONG, PULONG);
//
typedef ULONG (WINAPI *pZwTerminateThread)(HANDLE, LONG);


//全局
extern map<wstring, wstring> map_ntreg;
extern map<wstring, HANDLE> map2_ntreg;


//提示权限
bool EnablePrivilege(LPTSTR lpName);

//根据镜像名称获得进程ID
int ntreg_init_getProcessIDByImageName(LPTSTR lpName);

//
bool ntreg_init_func(map<wstring, wstring> &map_,map<wstring, HANDLE> &map2_);

//
void ntreg_init_closeHive(struct hive *hdesc);

//
bool ntreg_init_writeHive(struct hive *hdesc);

//
struct hive* ntreg_init_openHive (char *filename,int mode);