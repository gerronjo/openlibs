#include "common.h"


//获得win32k基址
ULONG GetWin32kBase();

//根据服务号获得原始函数地址
ULONG GetOldShadowSSDTAddress(ULONG ulIndex);

//方式2
ULONG GetSSDTShadowAddress2();