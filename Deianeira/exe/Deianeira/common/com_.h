//com_.h
#ifndef _com__
#define _com__

#pragma once

namespace Plugin {

	using namespace System;

	//枚举类型
    public enum class TabControls{
		//主面板
		tc_main,
		//内核相关面板
		tc_kernel,
		//应用层相关面板
		tc_app,
		//网络连接相关面板
		tc_net,
		//其他辅助相关面板
		tc_other
	};

	//
	public ref class InitData 
	{
	public:
		InitData(void)
		{
			//
			//TODO: 在此处添加构造函数代码
			//
		}

		//插件名称,显示在“插件”下拉菜单中
		String^ Name;

		//标志是否是独立窗体程序,还是附加到主程序窗体上
		bool IsWindowApp;

		//tabControl
		TabControls tc_;

		//是否有Load事件（面板显现之前是否要执行相关函数，如Form_Load，在窗体显现前要执行的代码）
		bool IsDoAnythingBeforeLoad;

		//要获得主程序的相关变量信息
		//是否获得驱动程序句柄
		bool GetHDriver;

	};
}

#endif //com_