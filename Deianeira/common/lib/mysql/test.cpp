// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <Windows.h>
#include "mysql.h"

#pragma comment(linker,"/nodefaultlib:LIBCMT.lib")
#pragma comment(linker,"/nodefaultlib:MSVCRTD.lib")
#pragma comment(lib, "mysqlclient.lib")

#pragma comment(lib,"wsock32.lib")

//
int _tmain(int argc, _TCHAR* argv[])
{
	MYSQL mysql;
	MYSQL_RES *mysql_res;
	MYSQL_ROW mysql_row;

	//初始化mysql
	mysql_init (&mysql);

	//
	if(!mysql_real_connect(&mysql,
		"127.0.0.1", //服务器IP
		"root",      //username
		"root",      //password
		"emlog",      //database name
		3306,        //port
		NULL,0))
	{
		//
		printf("数据库连接失败,err:%s\n",mysql_error(&mysql));
		return 0;
	}

	//
	printf("数据库连接成功\n");

	//
	char szcmd[] = { "select * from emlog_link" };

	//
	if(mysql_real_query(&mysql,szcmd,sizeof(szcmd)) != ERROR_SUCCESS)
	{ 
		printf("执行失败,err:%s\n",mysql_error(&mysql));
		return 0;
	}

	//
	if(!(mysql_res = mysql_use_result(&mysql)))
	{ 
		printf("读取数据时失败,err:%s\n",mysql_error(&mysql));
		return 0;
	}

	//
	int fieldsCount = mysql_num_fields(mysql_res);

	//
	while(mysql_row = mysql_fetch_row(mysql_res))
	{
		for (int i = 0;i < fieldsCount;i++)
		{
			//
			printf("--->>> %s\n",mysql_row[i]);
		}
	}

	//
	mysql_free_result(mysql_res);

	return 0;
}

