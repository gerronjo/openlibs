// Plugin.h

#pragma once

#include "..\common\common.h"
#include "..\common\common_net.h"
#include "..\common\process.h"
#include "..\common\com_.h"
#include "..\common\sigverif.h"
#include "ListViewItemComparer.h"

using namespace System;
using namespace System::Windows::Forms;
using namespace Deianeira;

namespace Plugin {

	public ref class plugin abstract
	{
		// TODO: 在此处添加此类的方法。

	public:

		static TabControl^ tc_;

		//驱动句柄
		static HANDLE hDriver;

		//初始化，主要是告诉主程序要做啥处理
		virtual Plugin::InitData^ Init() abstract;

		//如果是独立的程序就运行
		virtual System::Void Run() abstract;

		//如果是独立的程序就运行
		//virtual System::Void InitializeComponent(TabControl^ tc_) abstract;
		virtual System::Void InitializeComponent(TabControl^ tc_){
			this->tc_ = tc_;
		}

		//
		virtual System::Void Load() abstract;

		//
		static TabControl^ GetTabControl(){
			//
			return tc_;
		}

		//获得驱动模块句柄
		static System::Void GetHDriver(HANDLE h){
			hDriver = h;
		}
		
		//
#pragma region sigverif
		//检测文件是否签名
		//lpFileName:目标文件路径
		//返回:true:有签名，false:没有签名
		static bool plugin_sigverif_check(LPCWSTR lpFileName){
			//
			return sigverif_check(lpFileName);
		}


		//获得数字签名厂商
		//lpFileName:目标文件路径
		//返回:数据签名厂商名称，否则为NULL
		static LPWSTR plugin_sigverif_getCompanyName(LPCWSTR lpFileName){
			//
			return sigverif_getCompanyName(lpFileName);
		}
#pragma endregion sigverif

		//
#pragma region common
		//关机重启
		//bReboot:标志是关机还是重启
		//返回:true:成功，false:失败
		static bool plugin_common_shutdown(bool bReboot){
			//
			return common_shutdown(bReboot);
		}


		//提升进程权限
		//hToken:进程令牌
		//lpszPrivilege:权限别名
		//bEnablePrivilege:使能
		//返回:true:成功，false:失败
		static bool plugin_common_setPrivilege(HANDLE hToken, LPCTSTR lpszPrivilege, BOOL bEnablePrivilege){
			//
			return common_setPrivilege(hToken,lpszPrivilege,bEnablePrivilege);
		}

		//提升进程权限
		//ulPID:进程ID
		//bEnablePrivilege:使能
		//返回:true:成功，false:失败
		static bool plugin_common_setPrivilege(ULONG ulPID, LPCTSTR lpszPrivilege, BOOL bEnablePrivilege){
			//
			return common_setPrivilege(ulPID,lpszPrivilege,bEnablePrivilege);
		}

		//加载驱动
		//lpFileName:驱动文件路径
		//lpLinkName:服务名称
		//返回:成功则返回驱动句柄,失败则返回NULL
		static HANDLE plugin_common_loadDriver(LPCTSTR lpFileName,LPTSTR lpLinkName){
			//
			return common_loadDriver(lpFileName,lpLinkName);
		}

		//卸载驱动
		//hDriver:驱动模块句柄
		//lpLinkName:服务名称
		//返回:true:成功，false:失败
		static bool plugin_common_unloadDriver(HANDLE hDriver,LPTSTR lpLinkName){
			//
			return common_unloadDriver(hDriver,lpLinkName);
		}


		//打开文件属性对话框
		//lpFilePath:目标文件路径
		//返回:无
		static void plugin_common_showFileProperty(LPTSTR lpFilePath){
			//
			return common_showFileProperty(lpFilePath);
		}

		//定位文件
		//lpFilePath:目标文件路径
		//返回:无
		static void plugin_common_explorerFile(LPSTR lpFilePath){
			//
			return common_explorerFile(lpFilePath);
		}

		//定位注册表
		//lpRegPath:注册表路径
		//返回:true:成功，false:失败
		static bool plugin_common_explorerReg(LPTSTR lpRegPath){
			//
			return common_explorerReg(lpRegPath);
		}

		//根据资源ID来生产文件 
		//resId:资源ID 
		//lpResType:资源类型
		//lpFileName:目标文件路径 
		//返回:true:成功，false:失败
		static bool plugin_common_createFileFromResource(int resId,LPTSTR lpResType,LPTSTR lpFileName){
			//
			return common_createFileFromResource(resId,lpResType,lpFileName);
		}

		//添加服务
		//lpName:服务名称
		//lpDisplayName:友好显示名称
		//ulStartMode:启动类型
		//lpPath:应用程序路径
		//返回:true:成功，false:失败
		static bool plugin_common_addService(LPTSTR lpName,LPTSTR lpDisplayName,ULONG ulStartMode,LPTSTR lpPath){
			//
			return common_addService(lpName,lpDisplayName,ulStartMode,lpPath);
		}

		//删除服务
		//lpName:服务名称
		//返回:true:成功，false:失败
		static bool plugin_common_delService(LPTSTR lpName){
			//
			return common_delService(lpName);
		}

		//设置代理
		//lpIP:IP地址
		//lpPort:端口
		//返回:true:成功，false:失败
		static bool plugin_common_setProxy(LPTSTR lpIP,LPTSTR lpPort){
			//
			return common_setProxy(lpIP,lpPort);
		}

		//取消代理
		//返回:true:成功，false:失败
		static bool plugin_common_delProxy(){
			//
			return common_delProxy();
		}
#pragma endregion common


		//Net公共方法体
#pragma region common_net
		//ListView排序
		//column:列
		//bAsc:表升序还是降序
		//type:0，数字，1，字符串，2，时间
		//type2:只对type为0时有效，当我10，表示十进制比较，为16表示16进制比较
		static ListViewItemComparer^ plugin_common_net_sortLV(int column,bool bAsc,int type,int type2){
			//
			return gcnew ListViewItemComparer(column, bAsc, type, type2);
		}

		//获得系统版本
		static ULONG plugin_common_net_getOSVersion(){
			//
			return Deianeira::common_net::common_getOSVersion();
		}

		//托管转换为非托管
		static LPSTR plugin_common_net_stringToHGlobalAnsi(String^ str_){
			//
			return Deianeira::common_net::common_stringToHGlobalAnsi(str_);
		}

		//托管转换为非托管
		static LPTSTR plugin_common_net_stringToHGlobalUni(String^ str_){
			//
			return Deianeira::common_net::common_stringToHGlobalUni(str_);
		}

		//读配置信息
		//str_secName:节名称
		//str_keyName:要读取的字段名称
		//str_filePath:配置文件路径
		//返回:目标字段值
		static System::String^ plugin_common_net_getPrivateProfileString(String^ str_secName,String^ str_keyName,String^ str_filePath){
			//
			return Deianeira::common_net::config_getPrivateProfileString(str_secName,str_keyName,str_filePath);
		}

		//写配置信息
		//str_secName:节名称
		//str_keyName:要设置的字段名称
		//str_keyValue:字段值
		//str_filePath:配置文件路径
		//返回:无
		static System::Void plugin_common_net_writePrivateProfileString(String^ str_secName,String^ str_keyName,String^ str_keyValue,String^ str_filePath){
			//
			return Deianeira::common_net::config_writePrivateProfileString(str_secName,str_keyName,str_keyValue,str_filePath);
		}

		//根据进程ID获得进程名称
		static System::String^ plugin_common_net_getProcessName(int PID) {
			//
			return Deianeira::common_net::common_getProcessName(PID);
		}

		//根据进程ID获得进程映像文件路径
		static System::String^ plugin_common_net_getProcessPath(int PID) {
			//
			return Deianeira::common_net::common_getProcessPath(PID);
		}

		//根据进程ID获得进程映像文件生产厂商
		static System::String^ plugin_common_net_getProcessCompanyName(int PID){
			//
			return Deianeira::common_net::common_getProcessCompanyName(PID);
		}

		//获得文件生产厂商
		static System::String^ plugin_common_net_getFileCompanyName(String^ str_) {
			//
			return Deianeira::common_net::common_getFileCompanyName(str_);
		}

		//检测文件的出厂厂商名称，是否是微软的名称
		//str_:目标文件的出厂厂商名称
		//返回:true:为微软厂商名称，false:不是
		static bool plugin_common_net_checkFileCompanyName(String^ str_){
			//
			return Deianeira::common_net::common_checkFileCompanyName(str_);
		}

		//获得文件MD5
		static System::String^ plugin_common_net_getFileMD5(String^ str_){
			return Deianeira::common_net::common_getFileMD5(str_);
		}

		//分析字符串
		//str_:目标字符串
		//返回:分析后的结果
		static System::String^ plugin_common_net_formatString(String^ str_){
			//
			return Deianeira::common_net::common_formatString(str_);
		}

		//导出ListView列中的数据到文件
		static System::Void plugin_common_net_getListViewData(ListView^ lv_){
			return Deianeira::common_net::common_getListViewData(lv_);
		}

		//获得IP物理地址
		static System::String^ plugin_common_net_getIPAddr(String^ str_){
			return Deianeira::common_net::common_getIPAddr(str_);
		}

		//判断指定rk_下是否存在str_子键
		static bool plugin_common_net_checkRegkey(RegistryKey^ rk_ ,String^ str_){
			return Deianeira::common_net::common_checkRegkey(rk_,str_);
		}

		//判断指定rk_下是否存在str_值
		static bool plugin_common_net_checkRegkeyValue(RegistryKey^ rk_ ,String^ str_){
			return Deianeira::common_net::common_checkRegkeyValue(rk_,str_);
		}
#pragma endregion common_net


		//进程方面
#pragma region process
		//获取进程的状态
		//返回0，表示发生异常
		//返回1，表示进程处于挂起状态
		//返回2，表示进程没有被挂起
		static ULONG plugin_process_getProcessState(ULONG ulPID){
			//
			return process_getProcessState(ulPID);
		}

		//获取进程的父进程ID
		//ulPID:目标进程ID
		//返回:-1:失败，否则成功
		static ULONG plugin_process_getProcessParentPID(ULONG ulPID){
			//
			return process_getProcessParentPID(ulPID);
		}

		//获取线程的状态
		//返回0，表示发生异常
		//返回1，表示线程处于挂起状态
		//返回2，表示线程没有被挂起
		static ULONG plugin_process_getThreadState(ULONG ulPID,ULONG ulTID){
			//
			return process_getThreadState(ulPID,ulTID);
		}

		//注入DLL
		//lpFilePath:DLL文件路径
		//ulPID:进程ID
		static bool plugin_process_injectDll(LPTSTR lpFilePath,ULONG ulPID){
			//
			return process_injectDll(lpFilePath,ulPID);
		}

		//卸载进程模块
		static bool plugin_process_unloadModule(ULONG ulPID,ULONG ulDllBase){
			//如果卸载模块成功
			if (process_unloadModule(hDriver,ulPID,ulDllBase))
			{
				return true;
			}

			//
			return false;
		}

		//挂起线程
		static bool plugin_process_suspendThread(ULONG ulTID){
			//如果挂起线程成功
			if (process_suspendThread(hDriver,ulTID))
			{
				return true;
			}

			//
			return false;
		}

		//恢复线程
		static bool plugin_process_resumeThread(ULONG ulTID){
			//如果恢复线程成功
			if (process_resumeThread(hDriver,ulTID))
			{
				return true;
			}

			//
			return false;
		}

		//结束线程
		static bool plugin_process_killThread(ULONG ulTID){
			//如果结束线程成功
			if (process_killThread(hDriver,ulTID))
			{
				return true;
			}

			//
			return false;
		}

		//挂起进程
		static bool plugin_process_suspendProcess(ULONG ulPID){
			//如果挂起进程成功
			if (process_suspendProcess(hDriver,ulPID))
			{
				return true;
			}

			//
			return false;
		}

		//恢复进程运行
		static bool plugin_process_resumeProcess(ULONG ulPID){
			//如果恢复进程运行成功
			if (process_resumeProcess(hDriver,ulPID))
			{
				return true;
			}

			//
			return false;
		}

		//结束进程
		static bool plugin_process_killPrcess(ULONG ulPID){
			//如果结束进程成功
			if (process_killProcess(hDriver,ulPID))
			{
				return true;
			}

			//
			return false;
		}

		//内存清零
		static bool plugin_process_forceKillProcess(ULONG ulPID){
			//如果内存清零成功
			if (process_killProcess_memclr(hDriver,ulPID))
			{
				return true;
			}

			//
			return false;
		}
#pragma endregion process


	};
}
