/*
≤‚ ‘øÿ÷∆Ã®¥˙¬Î
*/
#include "dll.h"
#include "dllext.h"

static SvrHnds g_shs = {0};

/*
static void mb(const char *msg)
{
	MessageBox(NULL,msg,STRA(PRJNAME),MB_OK);
}
*/

int main(int argc,char *argv[])
{	
	IsFileExist(SYSNAME);
	
	if(!NewSvr(&g_shs))
	{
		PRINT("[%s]err : NewSvr Failed!\n",__func__);
		goto QUIT;
	}
	
	if(!StartSvr(&g_shs))
	{
		PRINT("[%s]err : StartSvr Failed!\n",__func__);
		goto QUIT;
	}
	
	//CallDrv(&g_shs,IOCTL_Ctl_GET_SDT,NULL,0,NULL,0);
	//CallDrv(&g_shs,IOCTL_Ctl_GET_SDT_SDW,NULL,0,NULL,0);
	
QUIT:
	if(!DelSvr(&g_shs))
	{
		PRINT("[%s]err : DelSvr Failed!\n",__func__);
		PRINT("[%s]Try To Force Del The Drv!\n",__func__);
		DelSvrForce();
	}

	(void)getchar();
	return 0;
}
