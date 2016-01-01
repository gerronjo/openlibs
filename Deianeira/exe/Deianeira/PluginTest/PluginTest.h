// PluginTest.h

#pragma once
#include <Windows.h>
#using <..\..\..\release\Plugin.dll>

#include "Form1.h"

using namespace System;
using namespace System::Drawing;
using namespace System::Collections;
using namespace System::Windows::Forms;

namespace MyPlugin {

	public ref class myPlugin: Plugin::plugin
	{
		// TODO: 在此处添加此类的方法。
	public:

		//初始化
		virtual Plugin::InitData^ Init() override {

			Plugin::InitData^ initData = gcnew Plugin::InitData;
			initData->Name = "Plugin Test";
			initData->IsWindowApp = true;
			initData->GetHDriver = true;

		    return initData;
		}

		//初始化
		virtual System::Void Run() override {
			//可以置顶
			(gcnew PluginTest::Form1())->ShowDialog();
			//也可以不置顶
			//(gcnew PluginTest::Form1())->Show();
		}

		virtual System::Void Load() override {

		}

		//初始化
		virtual System::Void InitializeComponent(TabControl^ tc_) override {
			//
		}
	};
}
