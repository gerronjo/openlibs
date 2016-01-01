// Deianeira.cpp: 主项目文件。

#include "stdafx.h"
#include "Form1.h"

using namespace Deianeira;

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	DWORD dwCheckSumHead;
	DWORD dwCheckSum;

	if(MapFileAndCheckSum(common_net::common_stringToHGlobalUni(Application::ExecutablePath),&dwCheckSumHead,&dwCheckSum) != CHECKSUM_SUCCESS)
	{
		MessageBox::Show("应用程序执行自校验时失败！","警告！",MessageBoxButtons::OK,MessageBoxIcon::Warning);
		return 0;
	}

	//
	if(dwCheckSumHead != dwCheckSum)
	{
		if(MessageBox::Show("应用程序可能被感染或被人为修改，你确定要执行吗？","警告！",MessageBoxButtons::OKCancel,MessageBoxIcon::Warning,
			MessageBoxDefaultButton::Button2) != System::Windows::Forms::DialogResult::OK)
		{
			return 0;
		}
	}

	// 在创建任何控件之前启用 Windows XP 可视化效果
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	// 创建主窗口并运行它
	Application::Run(gcnew Form1());
	return 0;
}
