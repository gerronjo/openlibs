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
	/// OtherUserChangeForm 摘要
	/// </summary>
	public ref class OtherUserChangeForm : public System::Windows::Forms::Form
	{
	public:
		OtherUserChangeForm(String^ str_)
		{
			InitializeComponent();
			//
			//TODO: 在此处添加构造函数代码
			//
			this->str_name = str_;
		}

	protected:
		/// <summary>
		/// 清理所有正在使用的资源。
		/// </summary>
		~OtherUserChangeForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::TextBox^  txt_pass1;
	protected: 

	private: System::Windows::Forms::Button^  btnOK;

	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::TextBox^  txt_pass2;

	private: System::Windows::Forms::Button^  btnQuit;

	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Label^  label4;

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
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->txt_pass1 = (gcnew System::Windows::Forms::TextBox());
			this->btnOK = (gcnew System::Windows::Forms::Button());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->txt_pass2 = (gcnew System::Windows::Forms::TextBox());
			this->btnQuit = (gcnew System::Windows::Forms::Button());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(12, 22);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(65, 12);
			this->label1->TabIndex = 0;
			this->label1->Text = L"新密码(N):";
			// 
			// txt_pass1
			// 
			this->txt_pass1->Location = System::Drawing::Point(124, 19);
			this->txt_pass1->Name = L"txt_pass1";
			this->txt_pass1->PasswordChar = '*';
			this->txt_pass1->Size = System::Drawing::Size(247, 21);
			this->txt_pass1->TabIndex = 0;
			// 
			// btnOK
			// 
			this->btnOK->Location = System::Drawing::Point(104, 174);
			this->btnOK->Name = L"btnOK";
			this->btnOK->Size = System::Drawing::Size(75, 23);
			this->btnOK->TabIndex = 2;
			this->btnOK->Text = L"确定";
			this->btnOK->UseVisualStyleBackColor = true;
			this->btnOK->Click += gcnew System::EventHandler(this, &OtherUserChangeForm::btnOK_Click);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(12, 54);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(77, 12);
			this->label2->TabIndex = 0;
			this->label2->Text = L"确认密码(C):";
			// 
			// txt_pass2
			// 
			this->txt_pass2->Location = System::Drawing::Point(124, 51);
			this->txt_pass2->Name = L"txt_pass2";
			this->txt_pass2->PasswordChar = '*';
			this->txt_pass2->Size = System::Drawing::Size(247, 21);
			this->txt_pass2->TabIndex = 1;
			// 
			// btnQuit
			// 
			this->btnQuit->Location = System::Drawing::Point(204, 174);
			this->btnQuit->Name = L"btnQuit";
			this->btnQuit->Size = System::Drawing::Size(75, 23);
			this->btnQuit->TabIndex = 3;
			this->btnQuit->Text = L"取消";
			this->btnQuit->UseVisualStyleBackColor = true;
			this->btnQuit->Click += gcnew System::EventHandler(this, &OtherUserChangeForm::btnQuit_Click);
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(11, 146);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(341, 12);
			this->label3->TabIndex = 4;
			this->label3->Text = L"如果您单击“取消”，密码将不会更改，并且将不会丢失数据。";
			// 
			// label4
			// 
			this->label4->Location = System::Drawing::Point(40, 85);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(341, 51);
			this->label4->TabIndex = 5;
			this->label4->Text = L"如果您单击“确定”，将发生如下情况：\r\n\r\n此用户账号将立即失去对他的所有加密的文件，保存密码和\r\n个人安全证书的访问权。";
			// 
			// OtherUserChangeForm
			// 
			this->AcceptButton = this->btnOK;
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(383, 209);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->btnQuit);
			this->Controls->Add(this->btnOK);
			this->Controls->Add(this->txt_pass2);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->txt_pass1);
			this->Controls->Add(this->label1);
			this->MaximizeBox = false;
			this->Name = L"OtherUserChangeForm";
			this->ShowInTaskbar = false;
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = L"设置密码";
			this->Load += gcnew System::EventHandler(this, &OtherUserChangeForm::OtherUserChangeForm_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

		 //用户名称
		 String^ str_name;

		//加载事件
private: System::Void OtherUserChangeForm_Load(System::Object^  sender, System::EventArgs^  e) {
			 this->Text="为"+str_name+this->Text;
		 }

		 //确定
private: System::Void btnOK_Click(System::Object^  sender, System::EventArgs^  e) {
			 if (String::IsNullOrEmpty(txt_pass1->Text))
			 {
				 MessageBox::Show("新密码不能为空！");
				 txt_pass1->Focus();
				 return;
			 }
			 if (String::IsNullOrEmpty(txt_pass2->Text))
			 {
				 MessageBox::Show("确认密码不能为空！");
				 txt_pass2->Focus();
				 return;
			 }
			 if (txt_pass1->Text!=txt_pass2->Text)
			 {
				 MessageBox::Show("密码未被确认！");
				 txt_pass1->Clear();
				 txt_pass2->Clear();
				 txt_pass1->Focus();
				 return;
			 }
			 if (common_net::common_changeUserPassword(str_name,txt_pass2->Text))
			 {
				 if (MessageBox::Show("密码已设置！")==System::Windows::Forms::DialogResult::OK)
				 {
					 this->Close();
				 }	 
			 }
			 else
			 {
				 MessageBox::Show("密码设置失败！");
			 }
		 }

		 //取消
private: System::Void btnQuit_Click(System::Object^  sender, System::EventArgs^  e) {
			 this->Close();
		 }

};
}
