#pragma once
#include "..\common\common.h"
#include "..\common\common_net.h"
#include "..\common\process.h"

namespace Deianeira {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// WinPropertyForm 摘要
	/// </summary>
	public ref class WinPropertyForm : public System::Windows::Forms::Form
	{
	public:
		WinPropertyForm(HWND hWnd,HANDLE hDriver)
		{
			InitializeComponent();
			//
			//TODO: 在此处添加构造函数代码
			//
			this->hWnd = hWnd;
			this->hDriver = hDriver;
		}

	protected:
		/// <summary>
		/// 清理所有正在使用的资源。
		/// </summary>
		~WinPropertyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TabControl^  tabControl1;
	protected: 
	private: System::Windows::Forms::TabPage^  tabPage1;
	private: System::Windows::Forms::TabPage^  tabPage2;




	private: System::Windows::Forms::Label^  label7;
	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::Label^  label10;
	private: System::Windows::Forms::Label^  label11;
	private: System::Windows::Forms::Label^  label9;
	private: System::Windows::Forms::Label^  label8;
	private: System::Windows::Forms::Label^  label12;
	private: System::Windows::Forms::Label^  label13;






	private: System::Windows::Forms::Label^  label16;
	private: System::Windows::Forms::Label^  label17;
	private: System::Windows::Forms::Label^  label18;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Label^  label15;
	private: System::Windows::Forms::ListBox^  lb_exStyle;

	private: System::Windows::Forms::ListBox^  lb_style;

	private: System::Windows::Forms::Label^  label14;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::ComboBox^  cbo_classStyle;

	private: System::Windows::Forms::Label^  lbl_handle;
	private: System::Windows::Forms::Label^  lbl_pClassName;

	private: System::Windows::Forms::Label^  lbl_pName;

	private: System::Windows::Forms::Label^  lbl_pHandle;

	private: System::Windows::Forms::Label^  lbl_classStyle;

	private: System::Windows::Forms::Label^  lbl_exStyle;

	private: System::Windows::Forms::Label^  lbl_style;

	private: System::Windows::Forms::Label^  lbl_className;

	private: System::Windows::Forms::Label^  lbl_name;
	private: System::Windows::Forms::Label^  lbl_pmPath;

	private: System::Windows::Forms::Label^  lbl_pmName;


	private: System::Windows::Forms::Label^  lbl_ppPath;

	private: System::Windows::Forms::Label^  lbl_ppName;

	private: System::Windows::Forms::Label^  lbl_pPath;

	private: System::Windows::Forms::Label^  lbl_pName2;

	private: System::Windows::Forms::Label^  lbl_ppid;

	private: System::Windows::Forms::Label^  lbl_tid;

	private: System::Windows::Forms::Label^  lbl_pid;






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
			this->tabControl1 = (gcnew System::Windows::Forms::TabControl());
			this->tabPage1 = (gcnew System::Windows::Forms::TabPage());
			this->lbl_pClassName = (gcnew System::Windows::Forms::Label());
			this->lbl_pName = (gcnew System::Windows::Forms::Label());
			this->lbl_pHandle = (gcnew System::Windows::Forms::Label());
			this->lbl_classStyle = (gcnew System::Windows::Forms::Label());
			this->lbl_exStyle = (gcnew System::Windows::Forms::Label());
			this->lbl_style = (gcnew System::Windows::Forms::Label());
			this->lbl_className = (gcnew System::Windows::Forms::Label());
			this->lbl_name = (gcnew System::Windows::Forms::Label());
			this->lbl_handle = (gcnew System::Windows::Forms::Label());
			this->cbo_classStyle = (gcnew System::Windows::Forms::ComboBox());
			this->label15 = (gcnew System::Windows::Forms::Label());
			this->lb_exStyle = (gcnew System::Windows::Forms::ListBox());
			this->lb_style = (gcnew System::Windows::Forms::ListBox());
			this->label14 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label16 = (gcnew System::Windows::Forms::Label());
			this->label17 = (gcnew System::Windows::Forms::Label());
			this->label18 = (gcnew System::Windows::Forms::Label());
			this->tabPage2 = (gcnew System::Windows::Forms::TabPage());
			this->label12 = (gcnew System::Windows::Forms::Label());
			this->label13 = (gcnew System::Windows::Forms::Label());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->label11 = (gcnew System::Windows::Forms::Label());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->lbl_pmPath = (gcnew System::Windows::Forms::Label());
			this->lbl_pmName = (gcnew System::Windows::Forms::Label());
			this->lbl_ppPath = (gcnew System::Windows::Forms::Label());
			this->lbl_ppName = (gcnew System::Windows::Forms::Label());
			this->lbl_pPath = (gcnew System::Windows::Forms::Label());
			this->lbl_pName2 = (gcnew System::Windows::Forms::Label());
			this->lbl_ppid = (gcnew System::Windows::Forms::Label());
			this->lbl_tid = (gcnew System::Windows::Forms::Label());
			this->lbl_pid = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->tabControl1->SuspendLayout();
			this->tabPage1->SuspendLayout();
			this->tabPage2->SuspendLayout();
			this->SuspendLayout();
			// 
			// tabControl1
			// 
			this->tabControl1->Controls->Add(this->tabPage1);
			this->tabControl1->Controls->Add(this->tabPage2);
			this->tabControl1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->tabControl1->Location = System::Drawing::Point(0, 0);
			this->tabControl1->Name = L"tabControl1";
			this->tabControl1->SelectedIndex = 0;
			this->tabControl1->Size = System::Drawing::Size(370, 386);
			this->tabControl1->TabIndex = 0;
			// 
			// tabPage1
			// 
			this->tabPage1->BackColor = System::Drawing::SystemColors::Window;
			this->tabPage1->Controls->Add(this->lbl_pClassName);
			this->tabPage1->Controls->Add(this->lbl_pName);
			this->tabPage1->Controls->Add(this->lbl_pHandle);
			this->tabPage1->Controls->Add(this->lbl_classStyle);
			this->tabPage1->Controls->Add(this->lbl_exStyle);
			this->tabPage1->Controls->Add(this->lbl_style);
			this->tabPage1->Controls->Add(this->lbl_className);
			this->tabPage1->Controls->Add(this->lbl_name);
			this->tabPage1->Controls->Add(this->lbl_handle);
			this->tabPage1->Controls->Add(this->cbo_classStyle);
			this->tabPage1->Controls->Add(this->label15);
			this->tabPage1->Controls->Add(this->lb_exStyle);
			this->tabPage1->Controls->Add(this->lb_style);
			this->tabPage1->Controls->Add(this->label14);
			this->tabPage1->Controls->Add(this->label4);
			this->tabPage1->Controls->Add(this->label1);
			this->tabPage1->Controls->Add(this->label2);
			this->tabPage1->Controls->Add(this->label3);
			this->tabPage1->Controls->Add(this->label16);
			this->tabPage1->Controls->Add(this->label17);
			this->tabPage1->Controls->Add(this->label18);
			this->tabPage1->Location = System::Drawing::Point(4, 21);
			this->tabPage1->Name = L"tabPage1";
			this->tabPage1->Padding = System::Windows::Forms::Padding(3);
			this->tabPage1->Size = System::Drawing::Size(362, 361);
			this->tabPage1->TabIndex = 0;
			this->tabPage1->Text = L"窗口信息";
			// 
			// lbl_pClassName
			// 
			this->lbl_pClassName->AutoSize = true;
			this->lbl_pClassName->Location = System::Drawing::Point(100, 337);
			this->lbl_pClassName->Name = L"lbl_pClassName";
			this->lbl_pClassName->Size = System::Drawing::Size(0, 12);
			this->lbl_pClassName->TabIndex = 20;
			// 
			// lbl_pName
			// 
			this->lbl_pName->AutoSize = true;
			this->lbl_pName->Location = System::Drawing::Point(100, 315);
			this->lbl_pName->Name = L"lbl_pName";
			this->lbl_pName->Size = System::Drawing::Size(0, 12);
			this->lbl_pName->TabIndex = 20;
			// 
			// lbl_pHandle
			// 
			this->lbl_pHandle->AutoSize = true;
			this->lbl_pHandle->Location = System::Drawing::Point(100, 292);
			this->lbl_pHandle->Name = L"lbl_pHandle";
			this->lbl_pHandle->Size = System::Drawing::Size(0, 12);
			this->lbl_pHandle->TabIndex = 20;
			// 
			// lbl_classStyle
			// 
			this->lbl_classStyle->AutoSize = true;
			this->lbl_classStyle->Location = System::Drawing::Point(100, 264);
			this->lbl_classStyle->Name = L"lbl_classStyle";
			this->lbl_classStyle->Size = System::Drawing::Size(0, 12);
			this->lbl_classStyle->TabIndex = 20;
			// 
			// lbl_exStyle
			// 
			this->lbl_exStyle->AutoSize = true;
			this->lbl_exStyle->Location = System::Drawing::Point(100, 170);
			this->lbl_exStyle->Name = L"lbl_exStyle";
			this->lbl_exStyle->Size = System::Drawing::Size(0, 12);
			this->lbl_exStyle->TabIndex = 20;
			// 
			// lbl_style
			// 
			this->lbl_style->AutoSize = true;
			this->lbl_style->Location = System::Drawing::Point(100, 87);
			this->lbl_style->Name = L"lbl_style";
			this->lbl_style->Size = System::Drawing::Size(0, 12);
			this->lbl_style->TabIndex = 20;
			// 
			// lbl_className
			// 
			this->lbl_className->AutoSize = true;
			this->lbl_className->Location = System::Drawing::Point(100, 61);
			this->lbl_className->Name = L"lbl_className";
			this->lbl_className->Size = System::Drawing::Size(0, 12);
			this->lbl_className->TabIndex = 20;
			// 
			// lbl_name
			// 
			this->lbl_name->AutoSize = true;
			this->lbl_name->Location = System::Drawing::Point(100, 39);
			this->lbl_name->Name = L"lbl_name";
			this->lbl_name->Size = System::Drawing::Size(0, 12);
			this->lbl_name->TabIndex = 20;
			// 
			// lbl_handle
			// 
			this->lbl_handle->AutoSize = true;
			this->lbl_handle->Location = System::Drawing::Point(100, 18);
			this->lbl_handle->Name = L"lbl_handle";
			this->lbl_handle->Size = System::Drawing::Size(0, 12);
			this->lbl_handle->TabIndex = 20;
			// 
			// cbo_classStyle
			// 
			this->cbo_classStyle->FormattingEnabled = true;
			this->cbo_classStyle->Location = System::Drawing::Point(185, 260);
			this->cbo_classStyle->Name = L"cbo_classStyle";
			this->cbo_classStyle->Size = System::Drawing::Size(146, 20);
			this->cbo_classStyle->TabIndex = 19;
			// 
			// label15
			// 
			this->label15->AutoSize = true;
			this->label15->Location = System::Drawing::Point(24, 264);
			this->label15->Name = L"label15";
			this->label15->Size = System::Drawing::Size(47, 12);
			this->label15->TabIndex = 18;
			this->label15->Text = L"类样式:";
			// 
			// lb_exStyle
			// 
			this->lb_exStyle->FormattingEnabled = true;
			this->lb_exStyle->ItemHeight = 12;
			this->lb_exStyle->Location = System::Drawing::Point(26, 186);
			this->lb_exStyle->Name = L"lb_exStyle";
			this->lb_exStyle->Size = System::Drawing::Size(305, 64);
			this->lb_exStyle->TabIndex = 17;
			// 
			// lb_style
			// 
			this->lb_style->FormattingEnabled = true;
			this->lb_style->ItemHeight = 12;
			this->lb_style->Location = System::Drawing::Point(26, 103);
			this->lb_style->Name = L"lb_style";
			this->lb_style->Size = System::Drawing::Size(305, 64);
			this->lb_style->TabIndex = 17;
			// 
			// label14
			// 
			this->label14->AutoSize = true;
			this->label14->Location = System::Drawing::Point(24, 170);
			this->label14->Name = L"label14";
			this->label14->Size = System::Drawing::Size(59, 12);
			this->label14->TabIndex = 16;
			this->label14->Text = L"扩展样式:";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(24, 87);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(59, 12);
			this->label4->TabIndex = 16;
			this->label4->Text = L"窗口样式:";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(24, 337);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(71, 12);
			this->label1->TabIndex = 15;
			this->label1->Text = L"父窗口类名:";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(24, 315);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(71, 12);
			this->label2->TabIndex = 14;
			this->label2->Text = L"父窗口标题:";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(24, 292);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(71, 12);
			this->label3->TabIndex = 13;
			this->label3->Text = L"父窗口句柄:";
			// 
			// label16
			// 
			this->label16->AutoSize = true;
			this->label16->Location = System::Drawing::Point(24, 61);
			this->label16->Name = L"label16";
			this->label16->Size = System::Drawing::Size(59, 12);
			this->label16->TabIndex = 12;
			this->label16->Text = L"窗口类名:";
			// 
			// label17
			// 
			this->label17->AutoSize = true;
			this->label17->Location = System::Drawing::Point(24, 39);
			this->label17->Name = L"label17";
			this->label17->Size = System::Drawing::Size(59, 12);
			this->label17->TabIndex = 11;
			this->label17->Text = L"窗口标题:";
			// 
			// label18
			// 
			this->label18->AutoSize = true;
			this->label18->Location = System::Drawing::Point(24, 18);
			this->label18->Name = L"label18";
			this->label18->Size = System::Drawing::Size(59, 12);
			this->label18->TabIndex = 10;
			this->label18->Text = L"窗口句柄:";
			// 
			// tabPage2
			// 
			this->tabPage2->BackColor = System::Drawing::SystemColors::Window;
			this->tabPage2->Controls->Add(this->label12);
			this->tabPage2->Controls->Add(this->label13);
			this->tabPage2->Controls->Add(this->label10);
			this->tabPage2->Controls->Add(this->label11);
			this->tabPage2->Controls->Add(this->label9);
			this->tabPage2->Controls->Add(this->label8);
			this->tabPage2->Controls->Add(this->label7);
			this->tabPage2->Controls->Add(this->label6);
			this->tabPage2->Controls->Add(this->lbl_pmPath);
			this->tabPage2->Controls->Add(this->lbl_pmName);
			this->tabPage2->Controls->Add(this->lbl_ppPath);
			this->tabPage2->Controls->Add(this->lbl_ppName);
			this->tabPage2->Controls->Add(this->lbl_pPath);
			this->tabPage2->Controls->Add(this->lbl_pName2);
			this->tabPage2->Controls->Add(this->lbl_ppid);
			this->tabPage2->Controls->Add(this->lbl_tid);
			this->tabPage2->Controls->Add(this->lbl_pid);
			this->tabPage2->Controls->Add(this->label5);
			this->tabPage2->Location = System::Drawing::Point(4, 21);
			this->tabPage2->Name = L"tabPage2";
			this->tabPage2->Padding = System::Windows::Forms::Padding(3);
			this->tabPage2->Size = System::Drawing::Size(362, 361);
			this->tabPage2->TabIndex = 1;
			this->tabPage2->Text = L"进程信息";
			// 
			// label12
			// 
			this->label12->AutoSize = true;
			this->label12->Location = System::Drawing::Point(24, 298);
			this->label12->Name = L"label12";
			this->label12->Size = System::Drawing::Size(59, 12);
			this->label12->TabIndex = 9;
			this->label12->Text = L"模块路径:";
			// 
			// label13
			// 
			this->label13->AutoSize = true;
			this->label13->Location = System::Drawing::Point(24, 269);
			this->label13->Name = L"label13";
			this->label13->Size = System::Drawing::Size(59, 12);
			this->label13->TabIndex = 8;
			this->label13->Text = L"模块名称:";
			// 
			// label10
			// 
			this->label10->AutoSize = true;
			this->label10->Location = System::Drawing::Point(24, 226);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(71, 12);
			this->label10->TabIndex = 7;
			this->label10->Text = L"父进程路径:";
			// 
			// label11
			// 
			this->label11->AutoSize = true;
			this->label11->Location = System::Drawing::Point(24, 197);
			this->label11->Name = L"label11";
			this->label11->Size = System::Drawing::Size(71, 12);
			this->label11->TabIndex = 6;
			this->label11->Text = L"父进程名称:";
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Location = System::Drawing::Point(24, 153);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(59, 12);
			this->label9->TabIndex = 5;
			this->label9->Text = L"进程路径:";
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Location = System::Drawing::Point(24, 124);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(59, 12);
			this->label8->TabIndex = 4;
			this->label8->Text = L"进程名称:";
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(24, 83);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(59, 12);
			this->label7->TabIndex = 3;
			this->label7->Text = L"父进程ID:";
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(24, 54);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(47, 12);
			this->label6->TabIndex = 2;
			this->label6->Text = L"线程ID:";
			// 
			// lbl_pmPath
			// 
			this->lbl_pmPath->AutoSize = true;
			this->lbl_pmPath->Location = System::Drawing::Point(101, 298);
			this->lbl_pmPath->Name = L"lbl_pmPath";
			this->lbl_pmPath->Size = System::Drawing::Size(0, 12);
			this->lbl_pmPath->TabIndex = 1;
			// 
			// lbl_pmName
			// 
			this->lbl_pmName->AutoSize = true;
			this->lbl_pmName->Location = System::Drawing::Point(101, 269);
			this->lbl_pmName->Name = L"lbl_pmName";
			this->lbl_pmName->Size = System::Drawing::Size(0, 12);
			this->lbl_pmName->TabIndex = 1;
			// 
			// lbl_ppPath
			// 
			this->lbl_ppPath->AutoSize = true;
			this->lbl_ppPath->Location = System::Drawing::Point(101, 226);
			this->lbl_ppPath->Name = L"lbl_ppPath";
			this->lbl_ppPath->Size = System::Drawing::Size(0, 12);
			this->lbl_ppPath->TabIndex = 1;
			// 
			// lbl_ppName
			// 
			this->lbl_ppName->AutoSize = true;
			this->lbl_ppName->Location = System::Drawing::Point(101, 197);
			this->lbl_ppName->Name = L"lbl_ppName";
			this->lbl_ppName->Size = System::Drawing::Size(0, 12);
			this->lbl_ppName->TabIndex = 1;
			// 
			// lbl_pPath
			// 
			this->lbl_pPath->AutoSize = true;
			this->lbl_pPath->Location = System::Drawing::Point(101, 153);
			this->lbl_pPath->Name = L"lbl_pPath";
			this->lbl_pPath->Size = System::Drawing::Size(0, 12);
			this->lbl_pPath->TabIndex = 1;
			// 
			// lbl_pName2
			// 
			this->lbl_pName2->AutoSize = true;
			this->lbl_pName2->Location = System::Drawing::Point(101, 124);
			this->lbl_pName2->Name = L"lbl_pName2";
			this->lbl_pName2->Size = System::Drawing::Size(0, 12);
			this->lbl_pName2->TabIndex = 1;
			// 
			// lbl_ppid
			// 
			this->lbl_ppid->AutoSize = true;
			this->lbl_ppid->Location = System::Drawing::Point(101, 83);
			this->lbl_ppid->Name = L"lbl_ppid";
			this->lbl_ppid->Size = System::Drawing::Size(0, 12);
			this->lbl_ppid->TabIndex = 1;
			// 
			// lbl_tid
			// 
			this->lbl_tid->AutoSize = true;
			this->lbl_tid->Location = System::Drawing::Point(101, 54);
			this->lbl_tid->Name = L"lbl_tid";
			this->lbl_tid->Size = System::Drawing::Size(0, 12);
			this->lbl_tid->TabIndex = 1;
			// 
			// lbl_pid
			// 
			this->lbl_pid->AutoSize = true;
			this->lbl_pid->Location = System::Drawing::Point(101, 25);
			this->lbl_pid->Name = L"lbl_pid";
			this->lbl_pid->Size = System::Drawing::Size(0, 12);
			this->lbl_pid->TabIndex = 1;
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(24, 25);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(47, 12);
			this->label5->TabIndex = 1;
			this->label5->Text = L"进程ID:";
			// 
			// WinPropertyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(370, 386);
			this->Controls->Add(this->tabControl1);
			this->MaximizeBox = false;
			this->Name = L"WinPropertyForm";
			this->ShowInTaskbar = false;
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = L"窗口属性";
			this->Load += gcnew System::EventHandler(this, &WinPropertyForm::WinPropertyForm_Load);
			this->tabControl1->ResumeLayout(false);
			this->tabPage1->ResumeLayout(false);
			this->tabPage1->PerformLayout();
			this->tabPage2->ResumeLayout(false);
			this->tabPage2->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion

		 //
		 static HWND hWnd = NULL;
		 static HANDLE hDriver = NULL;

		 static array<LONG>^ style = gcnew array<LONG>{
			 WS_BORDER,WS_CAPTION,WS_CHILD,WS_CLIPCHILDREN,WS_CLIPSIBLINGS,WS_DLGFRAME,WS_DISABLED,
				 WS_GROUP,WS_HSCROLL,WS_MAXIMIZE,WS_MAXIMIZEBOX,WS_MINIMIZE,WS_MINIMIZEBOX,WS_OVERLAPPED,
				 WS_POPUP,WS_SYSMENU,WS_TABSTOP,WS_THICKFRAME,WS_VISIBLE,WS_VSCROLL
		 };

		 static array<String^>^ strs_style = gcnew array<String^>{
			 "WS_BORDER","WS_CAPTION","WS_CHILD","WS_CLIPCHILDREN","WS_CLIPSIBLINGS","WS_DLGFRAME","WS_DISABLED",
				 "WS_GROUP","WS_HSCROLL","WS_MAXIMIZE","WS_MAXIMIZEBOX","WS_MINIMIZE","WS_MINIMIZEBOX","WS_OVERLAPPED",
				 "WS_POPUP","WS_SYSMENU","WS_TABSTOP","WS_THICKFRAME","WS_VISIBLE","WS_VSCROLL"
		 };

		 static array<LONG>^ exstyle = gcnew array<LONG>{
			 WS_EX_ACCEPTFILES,WS_EX_APPWINDOW,WS_EX_CLIENTEDGE,WS_EX_COMPOSITED,WS_EX_CONTEXTHELP,WS_EX_CONTROLPARENT,WS_EX_DLGMODALFRAME,
				 WS_EX_LAYERED,WS_EX_LAYOUTRTL,WS_EX_LEFT,WS_EX_LEFTSCROLLBAR,WS_EX_LTRREADING,WS_EX_MDICHILD,WS_EX_NOACTIVATE,
				 WS_EX_NOINHERITLAYOUT,WS_EX_NOPARENTNOTIFY,WS_EX_RIGHT,WS_EX_RIGHTSCROLLBAR,WS_EX_RTLREADING,WS_EX_STATICEDGE,
				 WS_EX_TOOLWINDOW,WS_EX_TOPMOST,WS_EX_TRANSPARENT,WS_EX_WINDOWEDGE
		 };

		 static array<String^>^ strs_exstyle = gcnew array<String^>{
			 "WS_EX_ACCEPTFILES","WS_EX_APPWINDOW","WS_EX_CLIENTEDGE","WS_EX_COMPOSITED","WS_EX_CONTEXTHELP","WS_EX_CONTROLPARENT","WS_EX_DLGMODALFRAME",
				 "WS_EX_LAYERED","WS_EX_LAYOUTRTL","WS_EX_LEFT","WS_EX_LEFTSCROLLBAR","WS_EX_LTRREADING","WS_EX_MDICHILD","WS_EX_NOACTIVATE",
				 "WS_EX_NOINHERITLAYOUT","WS_EX_NOPARENTNOTIFY","WS_EX_RIGHT","WS_EX_RIGHTSCROLLBAR","WS_EX_RTLREADING","WS_EX_STATICEDGE",
				 "WS_EX_TOOLWINDOW","WS_EX_TOPMOST","WS_EX_TRANSPARENT","WS_EX_WINDOWEDGE"
		 };

		 static array<LONG>^ clsstyle = gcnew array<LONG>{
			 CS_BYTEALIGNCLIENT,CS_BYTEALIGNWINDOW,CS_CLASSDC,CS_DBLCLKS,CS_DROPSHADOW,CS_GLOBALCLASS,
				 CS_HREDRAW,CS_IME,CS_NOCLOSE,CS_OWNDC,CS_PARENTDC,CS_SAVEBITS,CS_VREDRAW
		 };

		 static array<String^>^ strs_clsstyle = gcnew array<String^>{
			 "CS_BYTEALIGNCLIENT","CS_BYTEALIGNWINDOW","CS_CLASSDC","CS_DBLCLKS","CS_DROPSHADOW","CS_GLOBALCLASS",
				 "CS_HREDRAW","CS_IME","CS_NOCLOSE","CS_OWNDC","CS_PARENTDC","CS_SAVEBITS","CS_VREDRAW"
		 };

		 //
private: System::Void WinPropertyForm_Load(System::Object^  sender, System::EventArgs^  e) {

			 //窗体句柄
			 lbl_handle->Text = "0x" + ((ULONG)hWnd).ToString("X8");

			 //
			 TCHAR tcBuffer[MAX_PATH] = { 0 };

			 //获得窗体标题栏
			 GetWindowText(hWnd,tcBuffer,MAX_PATH);

			 //窗体标题栏
			 lbl_name->Text = gcnew String(tcBuffer);

			 ZeroMemory(tcBuffer,0);
			 //窗体类名
			 GetClassName(hWnd,tcBuffer,MAX_PATH);

			 lbl_className->Text = gcnew String(tcBuffer);

			 //窗口样式
			 LONG ulStyle = GetWindowLong(hWnd, GWL_STYLE);

			 lbl_style->Text = "0x" + ulStyle.ToString("X8");

			 for (int i = 0; i < style->Length; i++)
			 {
				 if (ulStyle & style[i])
				 {
					 lb_style->Items->Add(strs_style[i]);
				 }
			 }

			 //扩展样式
			 LONG ulStyleEx = GetWindowLong(hWnd, GWL_EXSTYLE);

			 lbl_exStyle->Text = "0x" + ulStyleEx.ToString("X8");

			 for (int i = 0; i < exstyle->Length; i++)
			 {
				 if (ulStyleEx & exstyle[i])
				 {
					 lb_exStyle->Items->Add(strs_exstyle[i]);
				 }
			 }

			 //类样式
			 LONG ulClassStyle = GetClassLong(hWnd,GCL_STYLE);

			 lbl_classStyle->Text = "0x" + ulClassStyle.ToString("X8");

			 for (int i = 0; i < clsstyle->Length; i++)
			 {
				 if (ulClassStyle & clsstyle[i])
				 {
					 cbo_classStyle->Items->Add(strs_clsstyle[i]);
				 }
			 }

			 //
			 if (cbo_classStyle->Items->Count > 0)
			 {
				 cbo_classStyle->SelectedIndex = 0;
			 }

			 /*
			 GetParent 的返回值比较复杂，对于overlapped类型的窗口，它返回0，对于WS_CHILD类型，
			 它返回其父窗口，对于WS_POPUP类型，它返回其所有 者窗口，如果想得到创建它时所传递进
			 去的那个hwndParent参数，应该用GetWindowWord(GWW_HWNDPARENT)函数。
			 */

			 //获得父窗体句柄
			 HWND hParent = (HWND)GetWindowLong(hWnd,GWL_HWNDPARENT);

			 //
			 if (hParent != NULL)
			 {
				 lbl_pHandle->Text = "0x" + ((ULONG)hParent).ToString("X8");

				 //父窗体标题栏
				 ZeroMemory(tcBuffer,0);
				 //获得窗体标题栏
				 GetWindowText(hParent,tcBuffer,MAX_PATH);

				 lbl_pName->Text = gcnew String(tcBuffer);

				 //父窗体类名
				 ZeroMemory(tcBuffer,0);
				 //窗体类名
				 GetClassName(hParent,tcBuffer,MAX_PATH);

				 lbl_pClassName->Text = gcnew String(tcBuffer);
			 }
			 
			 //进程信息
			 DWORD dwThreadID,dwProcessID,dwPProcessID,dwRet = 0;
			 ULONG ulBaseAddress = 0;
			 //
			 dwThreadID = GetWindowThreadProcessId(hWnd,&dwProcessID);
			 //
			 lbl_pid->Text = dwProcessID.ToString();
			 //
			 lbl_tid->Text = dwThreadID.ToString();
			 //
			 dwPProcessID = process_getProcessParentPID(dwProcessID);
			 //
			 lbl_ppid->Text = dwPProcessID.ToString();
			 //
			 lbl_pName2->Text = common_net::common_getProcessName(dwProcessID);
			 //
			 lbl_pPath->Text = gcnew String(common_net::common_getShortPath(common_net::common_getProcessPath(dwProcessID)));
			 //
			 lbl_ppName->Text = common_net::common_getProcessName(dwPProcessID);
			 //
			 lbl_ppPath->Text = gcnew String(common_net::common_getShortPath(common_net::common_getProcessPath(dwPProcessID)));

			 //首先确保里面的信息为最新
			 if (pProcessModuleInfo)
			 {
				 //
				 common_net::common_freeProcessModuleInfo();
			 }
			 
			 //刷新最新信息
			 common_net::common_getProcessModuleInfo(dwProcessID);

			 //获得线程所在模块
			 CLIENT_ID clientID = { 0 };
			 //
			 clientID.ulPID = dwProcessID;
			 clientID.ulTID = dwThreadID;

			 //获得基址
			 if(DeviceIoControl(hDriver, IOCTL_PROCESS_THREAD_GETBASEADDR, &clientID, sizeof(CLIENT_ID), &ulBaseAddress, sizeof(ULONG), &dwRet, NULL))
			 {
				 PPROCESS_MODULE_INFO p = common_net::common_getProcessModuleName(ulBaseAddress);
				 //
				 if (p)
				 {
					 //
					 lbl_pmName->Text = gcnew String(p->FileName);
					 //
					 lbl_pmPath->Text = gcnew String(common_net::common_getShortPath(gcnew String(p->FilePath)));
				 }		 
			 }
		 }
};
}
