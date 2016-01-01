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
	/// OtherShareCreateForm 摘要
	/// </summary>
	public ref class OtherShareCreateForm : public System::Windows::Forms::Form
	{
	public:
		OtherShareCreateForm(void)
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
		~OtherShareCreateForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  btnQuit;
	protected: 

	protected: 
	private: System::Windows::Forms::Label^  label1;


	private: System::Windows::Forms::TextBox^  txt_name;

	private: System::Windows::Forms::FolderBrowserDialog^  FBD;
	private: System::Windows::Forms::TextBox^  txt_path;

	private: System::Windows::Forms::Label^  label2;



	private: System::Windows::Forms::TextBox^  txt_desc;

	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::TextBox^  txt_max;


	private: System::Windows::Forms::Button^  btnOK;
	private: System::Windows::Forms::Button^  btnFolderBrowse;

	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::Label^  label7;


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
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->txt_name = (gcnew System::Windows::Forms::TextBox());
			this->FBD = (gcnew System::Windows::Forms::FolderBrowserDialog());
			this->txt_path = (gcnew System::Windows::Forms::TextBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->txt_desc = (gcnew System::Windows::Forms::TextBox());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->txt_max = (gcnew System::Windows::Forms::TextBox());
			this->btnOK = (gcnew System::Windows::Forms::Button());
			this->btnFolderBrowse = (gcnew System::Windows::Forms::Button());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// btnQuit
			// 
			this->btnQuit->Location = System::Drawing::Point(336, 238);
			this->btnQuit->Name = L"btnQuit";
			this->btnQuit->Size = System::Drawing::Size(75, 23);
			this->btnQuit->TabIndex = 8;
			this->btnQuit->Text = L"取消";
			this->btnQuit->UseVisualStyleBackColor = true;
			this->btnQuit->Click += gcnew System::EventHandler(this, &OtherShareCreateForm::btnQuit_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(32, 30);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(41, 12);
			this->label1->TabIndex = 1;
			this->label1->Text = L"名称：";
			// 
			// txt_name
			// 
			this->txt_name->Location = System::Drawing::Point(79, 26);
			this->txt_name->Name = L"txt_name";
			this->txt_name->Size = System::Drawing::Size(100, 21);
			this->txt_name->TabIndex = 0;
			// 
			// txt_path
			// 
			this->txt_path->Location = System::Drawing::Point(79, 61);
			this->txt_path->Name = L"txt_path";
			this->txt_path->Size = System::Drawing::Size(295, 21);
			this->txt_path->TabIndex = 1;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(32, 65);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(41, 12);
			this->label2->TabIndex = 4;
			this->label2->Text = L"路径：";
			// 
			// txt_desc
			// 
			this->txt_desc->Location = System::Drawing::Point(79, 139);
			this->txt_desc->Multiline = true;
			this->txt_desc->Name = L"txt_desc";
			this->txt_desc->Size = System::Drawing::Size(295, 90);
			this->txt_desc->TabIndex = 6;
			this->txt_desc->Text = L"(\"空\"表示没有描述)";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(21, 103);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(65, 12);
			this->label4->TabIndex = 8;
			this->label4->Text = L"最大连接：";
			// 
			// txt_max
			// 
			this->txt_max->Location = System::Drawing::Point(80, 99);
			this->txt_max->Name = L"txt_max";
			this->txt_max->Size = System::Drawing::Size(100, 21);
			this->txt_max->TabIndex = 4;
			// 
			// btnOK
			// 
			this->btnOK->Location = System::Drawing::Point(242, 238);
			this->btnOK->Name = L"btnOK";
			this->btnOK->Size = System::Drawing::Size(75, 23);
			this->btnOK->TabIndex = 7;
			this->btnOK->Text = L"确定";
			this->btnOK->UseVisualStyleBackColor = true;
			this->btnOK->Click += gcnew System::EventHandler(this, &OtherShareCreateForm::btnOK_Click);
			// 
			// btnFolderBrowse
			// 
			this->btnFolderBrowse->Location = System::Drawing::Point(380, 60);
			this->btnFolderBrowse->Name = L"btnFolderBrowse";
			this->btnFolderBrowse->Size = System::Drawing::Size(31, 23);
			this->btnFolderBrowse->TabIndex = 2;
			this->btnFolderBrowse->Text = L"...";
			this->btnFolderBrowse->UseVisualStyleBackColor = true;
			this->btnFolderBrowse->Click += gcnew System::EventHandler(this, &OtherShareCreateForm::btnFolderBrowse_Click);
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(32, 139);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(41, 12);
			this->label6->TabIndex = 14;
			this->label6->Text = L"描述：";
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(186, 103);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(173, 12);
			this->label7->TabIndex = 15;
			this->label7->Text = L"(\"空\"表示没有限制，默认为空)";
			// 
			// OtherShareCreateForm
			// 
			this->AcceptButton = this->btnOK;
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(443, 274);
			this->Controls->Add(this->label7);
			this->Controls->Add(this->label6);
			this->Controls->Add(this->btnFolderBrowse);
			this->Controls->Add(this->btnOK);
			this->Controls->Add(this->txt_max);
			this->Controls->Add(this->txt_desc);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->txt_path);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->txt_name);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->btnQuit);
			this->MaximizeBox = false;
			this->Name = L"OtherShareCreateForm";
			this->ShowInTaskbar = false;
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = L"创建共享";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

		 //浏览
private: System::Void btnFolderBrowse_Click(System::Object^  sender, System::EventArgs^  e) {
			 FBD->ShowNewFolderButton = true;
			 if (FBD->ShowDialog()==System::Windows::Forms::DialogResult::OK)
			 {
				 txt_path->Text = FBD->SelectedPath;
			 }
		 }

		 //OK
private: System::Void btnOK_Click(System::Object^  sender, System::EventArgs^  e) {
			 String^ str_msg = String::Empty;

			 if (String::IsNullOrEmpty(txt_name->Text))
			 {
				 MessageBox::Show("名称不能为空！");
				 txt_name->Focus();
				 return;
			 }	 
			 if (String::IsNullOrEmpty(txt_path->Text))
			 {
				 MessageBox::Show("路径不能为空！");
				 txt_path->Focus();
				 return;
			 }
			 //在这里创建
			 //DiskDrive = 0x0
			 if (String::IsNullOrEmpty(txt_max->Text))
			 {
				 str_msg = common_net::common_createShare(txt_name->Text,txt_path->Text,0x0,nullptr,
					 (String::IsNullOrEmpty(txt_desc->Text)==true?nullptr:txt_max->Text));
			 }
			 else
			 {
				 str_msg = common_net::common_createShare(txt_name->Text,txt_path->Text,0x0,Convert::ToInt32(txt_max->Text),
					 (String::IsNullOrEmpty(txt_desc->Text)==true?nullptr:txt_desc->Text));
			 }

			 //判断返回信息
			 if (str_msg!="Success")
			 {
				 MessageBox::Show(str_msg);
			 }
			 else
			 {
				 if (MessageBox::Show("创建成功！")==System::Windows::Forms::DialogResult::OK)
				 {
					 this->Close();
				 }	
			 }
		 }

		 //取消
private: System::Void btnQuit_Click(System::Object^  sender, System::EventArgs^  e) {
			 this->Close();
		 }

};
}
