#pragma once
#include "..\common\common.h"
#include "..\common\common_net.h"

namespace Deianeira {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// ServiceCreateForm 摘要
	/// </summary>
	public ref class ServiceCreateForm : public System::Windows::Forms::Form
	{
	public:
		ServiceCreateForm(void)
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
		~ServiceCreateForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  btnQuit;
	private: System::Windows::Forms::ComboBox^  cbo_startmode;
	protected: 

	protected: 

	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::TextBox^  txt_name;

	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::TextBox^  txt_display;

	private: System::Windows::Forms::TextBox^  txt_path;



	private: System::Windows::Forms::Button^  btnBrowse;

	private: System::Windows::Forms::Button^  btnOK;
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
			this->btnQuit = (gcnew System::Windows::Forms::Button());
			this->cbo_startmode = (gcnew System::Windows::Forms::ComboBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->txt_name = (gcnew System::Windows::Forms::TextBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->txt_display = (gcnew System::Windows::Forms::TextBox());
			this->txt_path = (gcnew System::Windows::Forms::TextBox());
			this->btnBrowse = (gcnew System::Windows::Forms::Button());
			this->btnOK = (gcnew System::Windows::Forms::Button());
			this->OFD = (gcnew System::Windows::Forms::OpenFileDialog());
			this->SuspendLayout();
			// 
			// btnQuit
			// 
			this->btnQuit->Location = System::Drawing::Point(289, 168);
			this->btnQuit->Name = L"btnQuit";
			this->btnQuit->Size = System::Drawing::Size(71, 23);
			this->btnQuit->TabIndex = 6;
			this->btnQuit->Text = L"取消";
			this->btnQuit->UseVisualStyleBackColor = true;
			this->btnQuit->Click += gcnew System::EventHandler(this, &ServiceCreateForm::btnQuit_Click);
			// 
			// cbo_startmode
			// 
			this->cbo_startmode->FormattingEnabled = true;
			this->cbo_startmode->Items->AddRange(gcnew cli::array< System::Object^  >(3) {L"Auto", L"Manual", L"Disabled"});
			this->cbo_startmode->Location = System::Drawing::Point(31, 103);
			this->cbo_startmode->Name = L"cbo_startmode";
			this->cbo_startmode->Size = System::Drawing::Size(121, 20);
			this->cbo_startmode->TabIndex = 2;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(29, 9);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(53, 12);
			this->label1->TabIndex = 2;
			this->label1->Text = L"名称(*):";
			// 
			// txt_name
			// 
			this->txt_name->Location = System::Drawing::Point(31, 25);
			this->txt_name->Name = L"txt_name";
			this->txt_name->Size = System::Drawing::Size(152, 21);
			this->txt_name->TabIndex = 0;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(29, 88);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(77, 12);
			this->label2->TabIndex = 4;
			this->label2->Text = L"启动类型(*):";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(29, 126);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(113, 12);
			this->label3->TabIndex = 5;
			this->label3->Text = L"可执行文件路径(*):";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(29, 49);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(83, 12);
			this->label4->TabIndex = 7;
			this->label4->Text = L"友好显示名称:";
			// 
			// txt_display
			// 
			this->txt_display->Location = System::Drawing::Point(31, 64);
			this->txt_display->Name = L"txt_display";
			this->txt_display->Size = System::Drawing::Size(250, 21);
			this->txt_display->TabIndex = 1;
			// 
			// txt_path
			// 
			this->txt_path->Location = System::Drawing::Point(31, 141);
			this->txt_path->Name = L"txt_path";
			this->txt_path->Size = System::Drawing::Size(297, 21);
			this->txt_path->TabIndex = 3;
			// 
			// btnBrowse
			// 
			this->btnBrowse->Location = System::Drawing::Point(330, 139);
			this->btnBrowse->Name = L"btnBrowse";
			this->btnBrowse->Size = System::Drawing::Size(30, 23);
			this->btnBrowse->TabIndex = 4;
			this->btnBrowse->Text = L"...";
			this->btnBrowse->UseVisualStyleBackColor = true;
			this->btnBrowse->Click += gcnew System::EventHandler(this, &ServiceCreateForm::btnBrowse_Click);
			// 
			// btnOK
			// 
			this->btnOK->Location = System::Drawing::Point(206, 168);
			this->btnOK->Name = L"btnOK";
			this->btnOK->Size = System::Drawing::Size(71, 23);
			this->btnOK->TabIndex = 5;
			this->btnOK->Text = L"确定";
			this->btnOK->UseVisualStyleBackColor = true;
			this->btnOK->Click += gcnew System::EventHandler(this, &ServiceCreateForm::btnOK_Click);
			// 
			// ServiceCreateForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(392, 201);
			this->Controls->Add(this->btnOK);
			this->Controls->Add(this->btnBrowse);
			this->Controls->Add(this->txt_path);
			this->Controls->Add(this->txt_display);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->txt_name);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->cbo_startmode);
			this->Controls->Add(this->btnQuit);
			this->MaximizeBox = false;
			this->Name = L"ServiceCreateForm";
			this->ShowInTaskbar = false;
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = L"服务创建配置";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

		 //浏览
private: System::Void btnBrowse_Click(System::Object^  sender, System::EventArgs^  e) {
			 OFD->Filter="exe files(*.exe)|*.exe|all files(*.*)|*.*";
			 OFD->Multiselect = false;
			 if (OFD->ShowDialog()==System::Windows::Forms::DialogResult::OK)
			 {
				 txt_path->Text = OFD->FileName;
			 }
		 }

		 //确定
private: System::Void btnOK_Click(System::Object^  sender, System::EventArgs^  e) {
			 if (String::IsNullOrEmpty(txt_name->Text))
			 {
				 MessageBox::Show("名称不能为空！");
				 txt_name->Focus();
				 return;
			 }
			 if (String::IsNullOrEmpty(cbo_startmode->Text))
			 {
				 MessageBox::Show("启动类型不能为空！");
				 cbo_startmode->Focus();
				 return;
			 }
			 if (String::IsNullOrEmpty(txt_path->Text))
			 {
				 MessageBox::Show("可执行文件路径不能为空！");
				 txt_path->Focus();
				 return;
			 }

			 if (common_addService(common_net::common_stringToHGlobalUni(txt_name->Text),
				 common_net::common_stringToHGlobalUni(txt_display->Text),
				 cbo_startmode->SelectedIndex+2,
				 common_net::common_stringToHGlobalUni(txt_path->Text)))
			 {
				 MessageBox::Show("服务创建成功！");
			 }
			 else
			 {
				 MessageBox::Show("服务创建失败！");
			 }
		 }

		 //取消
private: System::Void btnQuit_Click(System::Object^  sender, System::EventArgs^  e) {
			 this->Close();
		 }

};
}
