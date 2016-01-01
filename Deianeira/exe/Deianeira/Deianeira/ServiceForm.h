#pragma once
#include "..\common\common_net.h"

namespace Deianeira {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Diagnostics;
	using namespace System::ServiceProcess;

	/// <summary>
	/// ServiceForm 摘要
	/// </summary>
	public ref class ServiceForm : public System::Windows::Forms::Form
	{
	public:
		ServiceForm(ListViewItem^ item_)
		{
			InitializeComponent();
			//
			//TODO: 在此处添加构造函数代码
			//
			this->item_ = item_;
		}

	protected:
		/// <summary>
		/// 清理所有正在使用的资源。
		/// </summary>
		~ServiceForm()
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
	private: System::Windows::Forms::Button^  btnOK;
	private: System::Windows::Forms::Button^  btnQuit;
	private: System::Windows::Forms::Button^  btnApply;



	private: System::Windows::Forms::TextBox^  txt_path;

	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  lbl_name;
	private: System::Windows::Forms::TextBox^  txt_display;


	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::ComboBox^  cbo_startmode;

	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::Label^  lbl_status;

	private: System::Windows::Forms::Label^  label8;
	private: System::Windows::Forms::Button^  btnStart;
	private: System::Windows::Forms::RichTextBox^  rtb_desc;


	private: System::Windows::Forms::Button^  btnResume;

	private: System::Windows::Forms::Button^  btnPause;

	private: System::Windows::Forms::Button^  btnStop;




	private: System::Windows::Forms::Label^  label11;
	private: System::Windows::Forms::Label^  lbl_display;

	private: System::Windows::Forms::Label^  label9;

	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::Panel^  panel2;
	private: System::Windows::Forms::TreeView^  tv_depend2;

	private: System::Windows::Forms::Panel^  panel1;
	private: System::Windows::Forms::TreeView^  tv_depend1;


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
			this->btnResume = (gcnew System::Windows::Forms::Button());
			this->btnPause = (gcnew System::Windows::Forms::Button());
			this->btnStop = (gcnew System::Windows::Forms::Button());
			this->btnStart = (gcnew System::Windows::Forms::Button());
			this->rtb_desc = (gcnew System::Windows::Forms::RichTextBox());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->lbl_status = (gcnew System::Windows::Forms::Label());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->cbo_startmode = (gcnew System::Windows::Forms::ComboBox());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->lbl_name = (gcnew System::Windows::Forms::Label());
			this->txt_display = (gcnew System::Windows::Forms::TextBox());
			this->txt_path = (gcnew System::Windows::Forms::TextBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->tabPage2 = (gcnew System::Windows::Forms::TabPage());
			this->panel2 = (gcnew System::Windows::Forms::Panel());
			this->tv_depend2 = (gcnew System::Windows::Forms::TreeView());
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->tv_depend1 = (gcnew System::Windows::Forms::TreeView());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label11 = (gcnew System::Windows::Forms::Label());
			this->lbl_display = (gcnew System::Windows::Forms::Label());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->btnOK = (gcnew System::Windows::Forms::Button());
			this->btnQuit = (gcnew System::Windows::Forms::Button());
			this->btnApply = (gcnew System::Windows::Forms::Button());
			this->tabControl1->SuspendLayout();
			this->tabPage1->SuspendLayout();
			this->tabPage2->SuspendLayout();
			this->panel2->SuspendLayout();
			this->panel1->SuspendLayout();
			this->SuspendLayout();
			// 
			// tabControl1
			// 
			this->tabControl1->Controls->Add(this->tabPage1);
			this->tabControl1->Controls->Add(this->tabPage2);
			this->tabControl1->Location = System::Drawing::Point(12, 12);
			this->tabControl1->Name = L"tabControl1";
			this->tabControl1->SelectedIndex = 0;
			this->tabControl1->Size = System::Drawing::Size(381, 377);
			this->tabControl1->TabIndex = 0;
			// 
			// tabPage1
			// 
			this->tabPage1->Controls->Add(this->btnResume);
			this->tabPage1->Controls->Add(this->btnPause);
			this->tabPage1->Controls->Add(this->btnStop);
			this->tabPage1->Controls->Add(this->btnStart);
			this->tabPage1->Controls->Add(this->rtb_desc);
			this->tabPage1->Controls->Add(this->label8);
			this->tabPage1->Controls->Add(this->lbl_status);
			this->tabPage1->Controls->Add(this->label6);
			this->tabPage1->Controls->Add(this->cbo_startmode);
			this->tabPage1->Controls->Add(this->label5);
			this->tabPage1->Controls->Add(this->lbl_name);
			this->tabPage1->Controls->Add(this->txt_display);
			this->tabPage1->Controls->Add(this->txt_path);
			this->tabPage1->Controls->Add(this->label3);
			this->tabPage1->Controls->Add(this->label2);
			this->tabPage1->Controls->Add(this->label1);
			this->tabPage1->Location = System::Drawing::Point(4, 21);
			this->tabPage1->Name = L"tabPage1";
			this->tabPage1->Padding = System::Windows::Forms::Padding(3);
			this->tabPage1->Size = System::Drawing::Size(373, 352);
			this->tabPage1->TabIndex = 0;
			this->tabPage1->Text = L"服务详细信息";
			this->tabPage1->UseVisualStyleBackColor = true;
			// 
			// btnResume
			// 
			this->btnResume->Location = System::Drawing::Point(270, 165);
			this->btnResume->Name = L"btnResume";
			this->btnResume->Size = System::Drawing::Size(75, 23);
			this->btnResume->TabIndex = 10;
			this->btnResume->Text = L"恢复(R)";
			this->btnResume->UseVisualStyleBackColor = true;
			this->btnResume->Click += gcnew System::EventHandler(this, &ServiceForm::btnResume_Click);
			// 
			// btnPause
			// 
			this->btnPause->Location = System::Drawing::Point(190, 165);
			this->btnPause->Name = L"btnPause";
			this->btnPause->Size = System::Drawing::Size(75, 23);
			this->btnPause->TabIndex = 10;
			this->btnPause->Text = L"暂停(P)";
			this->btnPause->UseVisualStyleBackColor = true;
			this->btnPause->Click += gcnew System::EventHandler(this, &ServiceForm::btnPause_Click);
			// 
			// btnStop
			// 
			this->btnStop->Location = System::Drawing::Point(109, 165);
			this->btnStop->Name = L"btnStop";
			this->btnStop->Size = System::Drawing::Size(75, 23);
			this->btnStop->TabIndex = 10;
			this->btnStop->Text = L"停止(T)";
			this->btnStop->UseVisualStyleBackColor = true;
			this->btnStop->Click += gcnew System::EventHandler(this, &ServiceForm::btnStop_Click);
			// 
			// btnStart
			// 
			this->btnStart->Location = System::Drawing::Point(28, 165);
			this->btnStart->Name = L"btnStart";
			this->btnStart->Size = System::Drawing::Size(75, 23);
			this->btnStart->TabIndex = 10;
			this->btnStart->Text = L"启动(S)";
			this->btnStart->UseVisualStyleBackColor = true;
			this->btnStart->Click += gcnew System::EventHandler(this, &ServiceForm::btnStart_Click);
			// 
			// rtb_desc
			// 
			this->rtb_desc->Location = System::Drawing::Point(28, 216);
			this->rtb_desc->Name = L"rtb_desc";
			this->rtb_desc->Size = System::Drawing::Size(317, 119);
			this->rtb_desc->TabIndex = 9;
			this->rtb_desc->Text = L"";
			this->rtb_desc->TextChanged += gcnew System::EventHandler(this, &ServiceForm::rtb_desc_TextChanged);
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Location = System::Drawing::Point(29, 201);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(53, 12);
			this->label8->TabIndex = 8;
			this->label8->Text = L"描述(D):";
			// 
			// lbl_status
			// 
			this->lbl_status->AutoSize = true;
			this->lbl_status->Location = System::Drawing::Point(115, 145);
			this->lbl_status->Name = L"lbl_status";
			this->lbl_status->Size = System::Drawing::Size(0, 12);
			this->lbl_status->TabIndex = 7;
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(26, 145);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(59, 12);
			this->label6->TabIndex = 6;
			this->label6->Text = L"服务状态:";
			// 
			// cbo_startmode
			// 
			this->cbo_startmode->FormattingEnabled = true;
			this->cbo_startmode->Items->AddRange(gcnew cli::array< System::Object^  >(3) {L"Auto", L"Manual", L"Disabled"});
			this->cbo_startmode->Location = System::Drawing::Point(115, 111);
			this->cbo_startmode->Name = L"cbo_startmode";
			this->cbo_startmode->Size = System::Drawing::Size(121, 20);
			this->cbo_startmode->TabIndex = 5;
			this->cbo_startmode->SelectedIndexChanged += gcnew System::EventHandler(this, &ServiceForm::cbo_startmode_SelectedIndexChanged);
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(26, 115);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(77, 12);
			this->label5->TabIndex = 4;
			this->label5->Text = L"启动类型(E):";
			// 
			// lbl_name
			// 
			this->lbl_name->AutoSize = true;
			this->lbl_name->Location = System::Drawing::Point(115, 17);
			this->lbl_name->Name = L"lbl_name";
			this->lbl_name->Size = System::Drawing::Size(0, 12);
			this->lbl_name->TabIndex = 3;
			// 
			// txt_display
			// 
			this->txt_display->Location = System::Drawing::Point(115, 36);
			this->txt_display->Name = L"txt_display";
			this->txt_display->Size = System::Drawing::Size(230, 21);
			this->txt_display->TabIndex = 2;
			this->txt_display->TextChanged += gcnew System::EventHandler(this, &ServiceForm::txt_display_TextChanged);
			// 
			// txt_path
			// 
			this->txt_path->Location = System::Drawing::Point(28, 80);
			this->txt_path->Name = L"txt_path";
			this->txt_path->ReadOnly = true;
			this->txt_path->Size = System::Drawing::Size(317, 21);
			this->txt_path->TabIndex = 1;
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(26, 64);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(125, 12);
			this->label3->TabIndex = 0;
			this->label3->Text = L"可执行文件的路径(H):";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(26, 40);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(77, 12);
			this->label2->TabIndex = 0;
			this->label2->Text = L"显示名称(N):";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(26, 17);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(59, 12);
			this->label1->TabIndex = 0;
			this->label1->Text = L"服务名称:";
			// 
			// tabPage2
			// 
			this->tabPage2->Controls->Add(this->panel2);
			this->tabPage2->Controls->Add(this->panel1);
			this->tabPage2->Controls->Add(this->label4);
			this->tabPage2->Controls->Add(this->label11);
			this->tabPage2->Controls->Add(this->lbl_display);
			this->tabPage2->Controls->Add(this->label9);
			this->tabPage2->Location = System::Drawing::Point(4, 21);
			this->tabPage2->Name = L"tabPage2";
			this->tabPage2->Padding = System::Windows::Forms::Padding(3);
			this->tabPage2->Size = System::Drawing::Size(373, 352);
			this->tabPage2->TabIndex = 1;
			this->tabPage2->Text = L"依存关系";
			this->tabPage2->UseVisualStyleBackColor = true;
			// 
			// panel2
			// 
			this->panel2->Controls->Add(this->tv_depend2);
			this->panel2->Location = System::Drawing::Point(16, 240);
			this->panel2->Name = L"panel2";
			this->panel2->Size = System::Drawing::Size(341, 96);
			this->panel2->TabIndex = 13;
			// 
			// tv_depend2
			// 
			this->tv_depend2->Dock = System::Windows::Forms::DockStyle::Fill;
			this->tv_depend2->Location = System::Drawing::Point(0, 0);
			this->tv_depend2->Name = L"tv_depend2";
			this->tv_depend2->Size = System::Drawing::Size(341, 96);
			this->tv_depend2->TabIndex = 1;
			this->tv_depend2->BeforeExpand += gcnew System::Windows::Forms::TreeViewCancelEventHandler(this, &ServiceForm::tv_depend2_BeforeExpand);
			// 
			// panel1
			// 
			this->panel1->Controls->Add(this->tv_depend1);
			this->panel1->Location = System::Drawing::Point(16, 110);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(341, 102);
			this->panel1->TabIndex = 12;
			// 
			// tv_depend1
			// 
			this->tv_depend1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->tv_depend1->Location = System::Drawing::Point(0, 0);
			this->tv_depend1->Name = L"tv_depend1";
			this->tv_depend1->Size = System::Drawing::Size(341, 102);
			this->tv_depend1->TabIndex = 0;
			this->tv_depend1->BeforeExpand += gcnew System::Windows::Forms::TreeViewCancelEventHandler(this, &ServiceForm::tv_depend1_BeforeExpand);
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(14, 225);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(161, 12);
			this->label4->TabIndex = 11;
			this->label4->Text = L"以下系统组件依赖此服务(F):\r\n";
			// 
			// label11
			// 
			this->label11->AutoSize = true;
			this->label11->Location = System::Drawing::Point(14, 95);
			this->label11->Name = L"label11";
			this->label11->Size = System::Drawing::Size(161, 12);
			this->label11->TabIndex = 4;
			this->label11->Text = L"此服务依赖以下系统组件(T):";
			// 
			// lbl_display
			// 
			this->lbl_display->AutoSize = true;
			this->lbl_display->Location = System::Drawing::Point(14, 70);
			this->lbl_display->Name = L"lbl_display";
			this->lbl_display->Size = System::Drawing::Size(0, 12);
			this->lbl_display->TabIndex = 4;
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Location = System::Drawing::Point(14, 25);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(353, 24);
			this->label9->TabIndex = 1;
			this->label9->Text = L"一些服务依赖于其它服务、系统驱动服务程序和组的加载顺序。\r\n如果系统组件被停止或运行不正常，依赖于它的服务会受到影响。";
			// 
			// btnOK
			// 
			this->btnOK->Location = System::Drawing::Point(156, 396);
			this->btnOK->Name = L"btnOK";
			this->btnOK->Size = System::Drawing::Size(75, 23);
			this->btnOK->TabIndex = 1;
			this->btnOK->Text = L"确定";
			this->btnOK->UseVisualStyleBackColor = true;
			this->btnOK->Click += gcnew System::EventHandler(this, &ServiceForm::btnOK_Click);
			// 
			// btnQuit
			// 
			this->btnQuit->Location = System::Drawing::Point(237, 396);
			this->btnQuit->Name = L"btnQuit";
			this->btnQuit->Size = System::Drawing::Size(75, 23);
			this->btnQuit->TabIndex = 1;
			this->btnQuit->Text = L"取消";
			this->btnQuit->UseVisualStyleBackColor = true;
			this->btnQuit->Click += gcnew System::EventHandler(this, &ServiceForm::btnQuit_Click);
			// 
			// btnApply
			// 
			this->btnApply->Location = System::Drawing::Point(318, 395);
			this->btnApply->Name = L"btnApply";
			this->btnApply->Size = System::Drawing::Size(75, 23);
			this->btnApply->TabIndex = 1;
			this->btnApply->Text = L"应用";
			this->btnApply->UseVisualStyleBackColor = true;
			this->btnApply->Click += gcnew System::EventHandler(this, &ServiceForm::btnApply_Click);
			// 
			// ServiceForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(405, 429);
			this->Controls->Add(this->btnApply);
			this->Controls->Add(this->btnQuit);
			this->Controls->Add(this->btnOK);
			this->Controls->Add(this->tabControl1);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->MaximizeBox = false;
			this->Name = L"ServiceForm";
			this->ShowInTaskbar = false;
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = L"服务属性";
			this->Load += gcnew System::EventHandler(this, &ServiceForm::ServiceForm_Load);
			this->tabControl1->ResumeLayout(false);
			this->tabPage1->ResumeLayout(false);
			this->tabPage1->PerformLayout();
			this->tabPage2->ResumeLayout(false);
			this->tabPage2->PerformLayout();
			this->panel2->ResumeLayout(false);
			this->panel1->ResumeLayout(false);
			this->ResumeLayout(false);

		}
#pragma endregion

		 ListViewItem^ item_;
		 ServiceController^ sc_;

		 //窗体的加载事件
private: System::Void ServiceForm_Load(System::Object^  sender, System::EventArgs^  e) {
			 try
			 {
				 sc_ = (ServiceController^)item_->Tag;

				 //显示名称
				 lbl_name->Text = sc_->ServiceName;
				 //显示友好显示
				 txt_display->Text = sc_->DisplayName;
				 //显示执行文件路径
				 txt_path->Text = common_net::common_getServiceInformain(sc_->ServiceName,"PathName")->ToString();
				 //获得启动类型
				 cbo_startmode->Text = common_net::common_getServiceInformain(sc_->ServiceName,"StartMode")->ToString();
				 //显示状态
				 lbl_status->Text = sc_->Status.ToString();
				 //控制按钮的显示
				 if (sc_->Status == ServiceControllerStatus::Running)
				 {
					 btnStart->Enabled=false;
					 btnStop->Enabled=true;

					 if (sc_->CanPauseAndContinue)
					 {
						 btnPause->Enabled=true;
					 }
					 else
					 {
						 btnPause->Enabled=false;
					 }

					 btnResume->Enabled=false;
				 }
				 else if (sc_->Status == ServiceControllerStatus::Stopped)
				 {
					 btnStart->Enabled=true;
					 btnStop->Enabled=false;
					 btnPause->Enabled=false;
					 btnResume->Enabled=false;
				 }
				 else if(sc_->Status == ServiceControllerStatus::Paused)
				 {
					 btnStart->Enabled=false;
					 btnStop->Enabled=true;
					 btnPause->Enabled=false;
					 btnResume->Enabled=true;
				 }

				 //显示描述
				 rtb_desc->Text = common_net::common_getServiceInformain(sc_->ServiceName,"Description")->ToString();

				 lbl_display->Text = sc_->DisplayName;

				 //此系统依赖以下组件
				 for each(ServiceController^ sc in sc_->ServicesDependedOn)
				 {
					 TreeNode^ tn = gcnew TreeNode(sc->DisplayName);
					 tv_depend1->Nodes->Add(tn);
					 //设置tag标志
					 tn->Tag = sc;
					 //循环遍历
					 for each(ServiceController^ sc2 in sc->ServicesDependedOn)
					 {
						 TreeNode^ tn2 = gcnew TreeNode(sc2->DisplayName);
						 tn->Nodes->Add(tn2);
						 //设置tag标志
						 tn2->Tag = sc2;
					 }
				 }

				 //以下系统组件依赖此服务
				 for each(ServiceController^ sc in sc_->DependentServices)
				 {
					 TreeNode^ tn = gcnew TreeNode(sc->DisplayName);
					 tv_depend2->Nodes->Add(tn);
					 //设置tag标志
					 tn->Tag = sc;
					 //循环遍历
					 for each(ServiceController^ sc2 in sc->DependentServices)
					 {
						 TreeNode^ tn2 = gcnew TreeNode(sc2->DisplayName);
						 tn->Nodes->Add(tn2);
						 //设置tag标志
						 tn2->Tag = sc2;
					 }
				 }

				 btnApply->Enabled=false;
			 }
			 catch(Exception^ ex){ }
		 }

		 //启动
private: System::Void btnStart_Click(System::Object^  sender, System::EventArgs^  e) {
			 //
			 try
			 {
				 sc_->Start();
				 sc_->WaitForStatus(ServiceControllerStatus::Running);

				 //刷新主窗体信息
				 item_->SubItems[1]->Text = sc_->Status.ToString();
				 //修改信息
				 ServiceForm_Load(sender, e);
			 }
			 catch(Exception^ ex){ MessageBox::Show(ex->Message);}
		 }

		 //停止
private: System::Void btnStop_Click(System::Object^  sender, System::EventArgs^  e) {
			 try
			 {
				 sc_->Stop();
				 sc_->WaitForStatus(ServiceControllerStatus::Stopped);

				 //刷新主窗体信息
				 item_->SubItems[1]->Text = sc_->Status.ToString();
				 //修改信息
				 ServiceForm_Load(sender, e);
			 }
			 catch(Exception^ ex){ MessageBox::Show(ex->Message);}
		 }

		 //暂停
private: System::Void btnPause_Click(System::Object^  sender, System::EventArgs^  e) {
			 try
			 {
				 sc_->Pause();
				 sc_->WaitForStatus(ServiceControllerStatus::Paused);

				 //刷新主窗体信息
				 item_->SubItems[1]->Text = sc_->Status.ToString();
				 //修改信息
				 ServiceForm_Load(sender, e);
			 }
			 catch(Exception^ ex){ MessageBox::Show(ex->Message);}
		 }

		 //恢复
private: System::Void btnResume_Click(System::Object^  sender, System::EventArgs^  e) {
			 try
			 {
				 sc_->Continue();
				 sc_->WaitForStatus(ServiceControllerStatus::Running);

				 //刷新主窗体信息
				 item_->SubItems[1]->Text = sc_->Status.ToString();
				 //修改信息
				 ServiceForm_Load(sender, e);
			 }
			 catch(Exception^ ex){ MessageBox::Show(ex->Message);}
		 }

		 //确定
private: System::Void btnOK_Click(System::Object^  sender, System::EventArgs^  e) {
			 if (btnApply->Enabled)
			 {
				 if (!common_net::common_changeServiceInformain(lbl_name->Text,txt_display->Text,cbo_startmode->SelectedIndex+2,rtb_desc->Text))
				 {
					 MessageBox::Show("保存过程中发生错误！");
				 }
				 else
				 {
					 this->Close();
				 }
			 }
		 }

		 //取消
private: System::Void btnQuit_Click(System::Object^  sender, System::EventArgs^  e) {
			 this->Close();
		 }

		 //应用
private: System::Void btnApply_Click(System::Object^  sender, System::EventArgs^  e) {
			 if (!common_net::common_changeServiceInformain(lbl_name->Text,txt_display->Text,cbo_startmode->SelectedIndex+2,rtb_desc->Text))
			 {
				 MessageBox::Show("保存过程中发生错误！");
			 }
			 else
			 {
				 btnApply->Enabled=false;
			 }
		 }


private: System::Void tv_depend1_BeforeExpand(System::Object^  sender, System::Windows::Forms::TreeViewCancelEventArgs^  e) {
			 for each(TreeNode^ tn_ in e->Node->Nodes)
			 {
				 //清空原有的记录
				 tn_->Nodes->Clear();
				 //重新获得所有子节点
				 //循环遍历
				 for each(ServiceController^ sc in ((ServiceController^)tn_->Tag)->ServicesDependedOn)
				 {
					 TreeNode^ tn = gcnew TreeNode(sc->DisplayName);
					 tn_->Nodes->Add(tn);
					 //设置tag标志
					 tn->Tag = sc;
				 }
			 }
		 }


private: System::Void tv_depend2_BeforeExpand(System::Object^  sender, System::Windows::Forms::TreeViewCancelEventArgs^  e) {
			 for each(TreeNode^ tn_ in e->Node->Nodes)
			 {
				 //清空原有的记录
				 tn_->Nodes->Clear();
				 //重新获得所有子节点
				 //循环遍历
				 for each(ServiceController^ sc in ((ServiceController^)tn_->Tag)->ServicesDependedOn)
				 {
					 TreeNode^ tn = gcnew TreeNode(sc->DisplayName);
					 tn_->Nodes->Add(tn);
					 //设置tag标志
					 tn->Tag = sc;
				 }
			 }
		 }

		 //如果这里发生改变
private: System::Void txt_display_TextChanged(System::Object^  sender, System::EventArgs^  e) {
			 if (!btnApply->Enabled)
			 {
				 btnApply->Enabled = true;
			 }	 
		 }

		 //如果这里发生改变
private: System::Void cbo_startmode_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
			 if (!btnApply->Enabled)
			 {
				 btnApply->Enabled = true;
			 }
		 }

		 //如果这里发生改变
private: System::Void rtb_desc_TextChanged(System::Object^  sender, System::EventArgs^  e) {
			 if (!btnApply->Enabled)
			 {
				 btnApply->Enabled = true;
			 }
		 }


};
}
