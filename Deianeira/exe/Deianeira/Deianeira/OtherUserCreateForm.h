#pragma once
#include "..\common\common_net.h"

namespace Deianeira {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Management;

	/// <summary>
	/// OtherUserCreateForm 摘要
	/// </summary>
	public ref class OtherUserCreateForm : public System::Windows::Forms::Form
	{
	public:
		OtherUserCreateForm(void)
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
		~OtherUserCreateForm()
		{
			if (components)
			{
				delete components;
			}
		}

	protected: 
	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::Button^  btnFolderBrowse;
	private: System::Windows::Forms::Button^  btnOK;

	private: System::Windows::Forms::TextBox^  txt_desc;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::TextBox^  txt_name;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::FolderBrowserDialog^  FBD;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::TextBox^  txt_path;
	private: System::Windows::Forms::Button^  btnQuit;
	private: System::Windows::Forms::ComboBox^  cbo_group;

	private: System::Windows::Forms::TextBox^  txt_pass;

	private: System::Windows::Forms::Label^  label3;

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
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->btnFolderBrowse = (gcnew System::Windows::Forms::Button());
			this->btnOK = (gcnew System::Windows::Forms::Button());
			this->txt_desc = (gcnew System::Windows::Forms::TextBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->txt_name = (gcnew System::Windows::Forms::TextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->FBD = (gcnew System::Windows::Forms::FolderBrowserDialog());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->txt_path = (gcnew System::Windows::Forms::TextBox());
			this->btnQuit = (gcnew System::Windows::Forms::Button());
			this->cbo_group = (gcnew System::Windows::Forms::ComboBox());
			this->txt_pass = (gcnew System::Windows::Forms::TextBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(37, 133);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(41, 12);
			this->label6->TabIndex = 26;
			this->label6->Text = L"描述：";
			// 
			// btnFolderBrowse
			// 
			this->btnFolderBrowse->Location = System::Drawing::Point(385, 54);
			this->btnFolderBrowse->Name = L"btnFolderBrowse";
			this->btnFolderBrowse->Size = System::Drawing::Size(31, 23);
			this->btnFolderBrowse->TabIndex = 3;
			this->btnFolderBrowse->Text = L"...";
			this->btnFolderBrowse->UseVisualStyleBackColor = true;
			this->btnFolderBrowse->Click += gcnew System::EventHandler(this, &OtherUserCreateForm::btnFolderBrowse_Click);
			// 
			// btnOK
			// 
			this->btnOK->Location = System::Drawing::Point(247, 232);
			this->btnOK->Name = L"btnOK";
			this->btnOK->Size = System::Drawing::Size(75, 23);
			this->btnOK->TabIndex = 6;
			this->btnOK->Text = L"确定";
			this->btnOK->UseVisualStyleBackColor = true;
			this->btnOK->Click += gcnew System::EventHandler(this, &OtherUserCreateForm::btnOK_Click);
			// 
			// txt_desc
			// 
			this->txt_desc->Location = System::Drawing::Point(84, 133);
			this->txt_desc->Multiline = true;
			this->txt_desc->Name = L"txt_desc";
			this->txt_desc->Size = System::Drawing::Size(295, 90);
			this->txt_desc->TabIndex = 5;
			this->txt_desc->Text = L"(\"空\"表示没有描述)";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(31, 59);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(53, 12);
			this->label2->TabIndex = 20;
			this->label2->Text = L"主目录：";
			// 
			// txt_name
			// 
			this->txt_name->Location = System::Drawing::Point(84, 20);
			this->txt_name->Name = L"txt_name";
			this->txt_name->Size = System::Drawing::Size(100, 21);
			this->txt_name->TabIndex = 0;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(31, 24);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(53, 12);
			this->label1->TabIndex = 17;
			this->label1->Text = L"用户名：";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(31, 97);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(53, 12);
			this->label4->TabIndex = 24;
			this->label4->Text = L"用户组：";
			// 
			// txt_path
			// 
			this->txt_path->Location = System::Drawing::Point(84, 55);
			this->txt_path->Name = L"txt_path";
			this->txt_path->Size = System::Drawing::Size(295, 21);
			this->txt_path->TabIndex = 2;
			// 
			// btnQuit
			// 
			this->btnQuit->Location = System::Drawing::Point(341, 232);
			this->btnQuit->Name = L"btnQuit";
			this->btnQuit->Size = System::Drawing::Size(75, 23);
			this->btnQuit->TabIndex = 7;
			this->btnQuit->Text = L"取消";
			this->btnQuit->UseVisualStyleBackColor = true;
			this->btnQuit->Click += gcnew System::EventHandler(this, &OtherUserCreateForm::btnQuit_Click);
			// 
			// cbo_group
			// 
			this->cbo_group->FormattingEnabled = true;
			this->cbo_group->Location = System::Drawing::Point(84, 93);
			this->cbo_group->Name = L"cbo_group";
			this->cbo_group->Size = System::Drawing::Size(132, 20);
			this->cbo_group->TabIndex = 4;
			// 
			// txt_pass
			// 
			this->txt_pass->Location = System::Drawing::Point(247, 20);
			this->txt_pass->Name = L"txt_pass";
			this->txt_pass->Size = System::Drawing::Size(100, 21);
			this->txt_pass->TabIndex = 1;
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(200, 24);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(41, 12);
			this->label3->TabIndex = 30;
			this->label3->Text = L"密码：";
			// 
			// OtherUserCreateForm
			// 
			this->AcceptButton = this->btnOK;
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(443, 274);
			this->Controls->Add(this->txt_pass);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->cbo_group);
			this->Controls->Add(this->label6);
			this->Controls->Add(this->btnFolderBrowse);
			this->Controls->Add(this->btnOK);
			this->Controls->Add(this->txt_desc);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->txt_name);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->txt_path);
			this->Controls->Add(this->btnQuit);
			this->MaximizeBox = false;
			this->Name = L"OtherUserCreateForm";
			this->ShowInTaskbar = false;
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = L"创建用户";
			this->Load += gcnew System::EventHandler(this, &OtherUserCreateForm::OtherUserCreateForm_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	
		 //加载事件
private: System::Void OtherUserCreateForm_Load(System::Object^  sender, System::EventArgs^  e) {
			 ManagementObjectSearcher^ mos = gcnew ManagementObjectSearcher("select Name from Win32_Group"); 
			 for each(ManagementObject^ mo in mos->Get())   
			 {
				 cbo_group->Items->Add(mo["Name"]->ToString());
			 }
		 }

		 //浏览
private: System::Void btnFolderBrowse_Click(System::Object^  sender, System::EventArgs^  e) {
			 FBD->ShowNewFolderButton = true;
			 if (FBD->ShowDialog()==System::Windows::Forms::DialogResult::OK)
			 {
				 txt_path->Text = FBD->SelectedPath;
			 }
		 }

		 //确定
private: System::Void btnOK_Click(System::Object^  sender, System::EventArgs^  e) {
			 if (String::IsNullOrEmpty(txt_name->Text))
			 {
				 MessageBox::Show("用户名不能为空！");
				 txt_name->Focus();
				 return;
			 }
			 if (String::IsNullOrEmpty(txt_path->Text))
			 {
				 MessageBox::Show("用户主目录不能为空！");
				 txt_path->Focus();
				 return;
			 }
			 //创建用户
			 if(common_net::common_createUser(txt_name->Text,txt_pass->Text,txt_path->Text,cbo_group->Text,txt_desc->Text))
			 {
				 if (MessageBox::Show("创建成功！")==System::Windows::Forms::DialogResult::OK)
				 {
					 this->Close();
				 }	 
			 }
			 else
			 {
				 MessageBox::Show("发生未知错误！");
			 }
		 }

		 //取消
private: System::Void btnQuit_Click(System::Object^  sender, System::EventArgs^  e) {
			 this->Close();
		 }

};
}
