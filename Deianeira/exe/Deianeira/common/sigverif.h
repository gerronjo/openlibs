//sigverif.h
#ifndef _SIGVERIF_
#define _SIGVERIF_

#include "common.h"


#define ENCODING (X509_ASN_ENCODING | PKCS_7_ASN_ENCODING)

//相关结构体
typedef struct _SPROG_PUBLISHERINFO{
	LPCWSTR lpszProgramName;
	LPCWSTR lpszPublisherLink;
	LPCWSTR lpszMoreInfoLink;
} SPROG_PUBLISHERINFO, *PSPROG_PUBLISHERINFO;


//检测文件是否签名
//lpFileName:目标文件路径
//返回:true:有签名，false:没有签名
bool sigverif_check(LPCWSTR lpFileName);


//获得数字签名厂商
//lpFileName:目标文件路径
//返回:数据签名厂商名称，否则为NULL
LPWSTR sigverif_getCompanyName(LPCWSTR lpFileName);

#endif //_SIGVERIF_