#pragma once

#include "..\common\common_net.h"

namespace Deianeira {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// ProcessInjectDllByHookMsg1Form 摘要
	/// </summary>
	public ref class ProcessInjectDllByHookMsg1Form : public System::Windows::Forms::Form
	{
	public:
		ProcessInjectDllByHookMsg1Form(void)
		{
			InitializeComponent();
			//
			//TODO: 在此处添加构造函数代码
			//
		}

	protected:
		/// <summary>
		/// 清理所有正在使用的资源。
		/// </summary>
		~ProcessInjectDllByHookMsg1Form()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  btnOK;
	private: System::Windows::Forms::Button^  btnQuit;
	protected: 

	protected: 

	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::TextBox^  txt_path;

	private: System::Windows::Forms::Button^  btnBrowse;
	private: System::Windows::Forms::TextBox^  txt_func;


	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::OpenFileDialog^  OFD;

	private:
		/// <summary>
		/// 必需的设计器变量。
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// 设计器支持所需的方法 - 不要
		/// 使用代码编辑器修改此方法的内容。
		/// </summary>
		void InitializeComponent(void)
		{
			this->btnOK = (gcnew System::Windows::Forms::Button());
			this->btnQuit = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->txt_path = (gcnew System::Windows::Forms::TextBox());
			this->btnBrowse = (gcnew System::Windows::Forms::Button());
			this->txt_func = (gcnew System::Windows::Forms::TextBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->OFD = (gcnew System::Windows::Forms::OpenFileDialog());
			this->SuspendLayout();
			// 
			// btnOK
			// 
			this->btnOK->Location = System::Drawing::Point(181, 116);
			this->btnOK->Name = L"btnOK";
			this->btnOK->Size = System::Drawing::Size(75, 23);
			this->btnOK->TabIndex = 3;
			this->btnOK->Text = L"确定";
			this->btnOK->UseVisualStyleBackColor = true;
			this->btnOK->Click += gcnew System::EventHandler(this, &ProcessInjectDllByHookMsg1Form::btnOK_Click);
			// 
			// btnQuit
			// 
			this->btnQuit->Location = System::Drawing::Point(271, 116);
			this->btnQuit->Name = L"btnQuit";
			this->btnQuit->Size = System::Drawing::Size(75, 23);
			this->btnQuit->TabIndex = 4;
			this->btnQuit->Text = L"取消";
			this->btnQuit->UseVisualStyleBackColor = true;
			this->btnQuit->Click += gcnew System::EventHandler(this, &ProcessInjectDllByHookMsg1Form::btnQuit_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(12, 19);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(53, 12);
			this->label1->TabIndex = 2;
			this->label1->Text = L"Dll路径:";
			// 
			// txt_path
			// 
			this->txt_path->Location = System::Drawing::Point(14, 34);
			this->txt_path->Name = L"txt_path";
			this->txt_path->Size = System::Drawing::Size(296, 21);
			this->txt_path->TabIndex = 0;
			// 
			// btnBrowse
			// 
			this->btnBrowse->Location = System::Drawing::Point(316, 34);
			this->btnBrowse->Name = L"btnBrowse";
			this->btnBrowse->Size = System::Drawing::Size(30, 23);
			this->btnBrowse->TabIndex = 1;
			this->btnBrowse->Text = L"...";
			this->btnBrowse->UseVisualStyleBackColor = true;
			this->btnBrowse->Click += gcnew System::EventHandler(this, &ProcessInjectDllByHookMsg1Form::btnBrowse_Click);
			// 
			// txt_func
			// 
			this->txt_func->Location = System::Drawing::Point(14, 83);
			this->txt_func->Name = L"txt_func";
			this->txt_func->Size = System::Drawing::Size(167, 21);
			this->txt_func->TabIndex = 2;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(12, 68);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(143, 12);
			this->label2->TabIndex = 5;
			this->label2->Text = L"Dll导出函数(StartHook):";
			// 
			// ProcessInjectDllByHookMsg1Form
			// 
			this->AcceptButton = this->btnOK;
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(361, 150);
			this->Controls->Add(this->txt_func);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->btnBrowse);
			this->Controls->Add(this->txt_path);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->btnQuit);
			this->Controls->Add(this->btnOK);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->MaximizeBox = false;
			this->Name = L"ProcessInjectDllByHookMsg1Form";
			this->ShowInTaskbar = false;
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = L"Dll消息钩子注入1";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

		 //浏览
private: System::Void btnBrowse_Click(System::Object^  sender, System::EventArgs^  e) {
			 OFD->Filter="dll files(*.dll)|*.dll|all files(*.*)|*.*";
			 OFD->Multiselect=false;

			 //打开对话框
			 if (OFD->ShowDialog()==System::Windows::Forms::DialogResult::OK)
			 {
				 txt_path->Text = OFD->FileName;
			 }
		 }

		 //确定
private: System::Void btnOK_Click(System::Object^  sender, System::EventArgs^  e) {
			 if (String::IsNullOrEmpty(txt_path->Text))
			 {
				 MessageBox::Show("DLL路径不能为空！");
				 return;
			 }
			 if (String::IsNullOrEmpty(txt_func->Text))
			 {
				 MessageBox::Show("DLL导出函数不能为空！");
				 return;
			 }

			 //加载目标DLL
			 HMODULE hModule = LoadLibrary(common_net::common_stringToHGlobalUni(OFD->FileName));
			 //如果加载失败
			 if (hModule==NULL)
			 {
				 MessageBox::Show("安装消息钩子失败！");
				 return;
			 }

			 //获得函数地址
			 FARPROC farproc_func = GetProcAddress(hModule,common_net::common_stringToHGlobalAnsi(txt_func->Text));
			 //运行函数
			 (*farproc_func)();
		 }

		 //退出
private: System::Void btnQuit_Click(System::Object^  sender, System::EventArgs^  e) {
			 this->Close();
		 }
};
}
