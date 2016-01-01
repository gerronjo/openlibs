/*********************************************************************
* QQwry.h
*
* 说明:纯真IP数据库 QQWry.dat的操作类
* 
* 部分代码来源于网络,你可以随意使用,传播或修改.但是不可用于商业用途
*********************************************************************/
#ifndef _QQWRY_
#define _QQWRY_

#include "..\..\..\exe\Deianeira\common\common.h"

class QQwry
{
	//
public:
	QQwry();
	QQwry(LPCTSTR lpszFilePathName);
	~QQwry();

	//
public:
	//
	BOOL OpenQQwry(LPCTSTR lpszFilePathName);
	BOOL GetIpInfo(LPSTR lpszDotIp,LPSTR lpszInfoBuf,int nMaxBufSize); //由点分IP地址得到信息
	BOOL GetIpInfo(ULONG uIP ,LPSTR lpszInfoBuf,int nMaxBufSize);        //由网络字节顺序IP地址得到信息
	DWORD GetRecordCount() const { return m_dwRecordCount; };
	void CloseQQwry();

	//
protected:
	LPBYTE m_pDataBuffer;
	DWORD m_dwRecordCount;
	BOOL m_bInit;
};


#endif //_QQWRY_