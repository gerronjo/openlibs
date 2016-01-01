#include "DDK_C.h"

unsigned g_ddk_val = 0x20100617;;

bool DDKAPI DDKHello(void)
{
	bool bRet = false;
	DbgPrint("[%s]msg : Hello DDK!\n",__func__);

	__try
	{
		int *pi = (int*)0x1000;
		DbgPrint("[%s]msg : can you see %d\n",__func__,*pi);
		bRet = true;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		#if defined(GOTO_DEBUG)
		DbgPrint("%s's try block rise a Except!\n",__func__);
		#endif
	}
	return bRet;
}
