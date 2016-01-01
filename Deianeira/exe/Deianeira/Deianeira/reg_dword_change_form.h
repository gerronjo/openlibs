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
	/// reg_dword_change_form 摘要
	/// </summary>
	public ref class reg_dword_change_form : public System::Windows::Forms::Form
	{
	public:
		reg_dword_change_form(ListViewItem^ item_)
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
		~reg_dword_change_form()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TextBox^  txt_name;
	protected: 
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::TextBox^  txt_value;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Button^  btnQuit;
	private: System::Windows::Forms::Button^  btnOK;
	private: System::Windows::Forms::GroupBox^  groupBox1;
	private: System::Windows::Forms::RadioButton^  rdo_10;

	private: System::Windows::Forms::RadioButton^  rdo_16;


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
			this->txt_name = (gcnew System::Windows::Forms::TextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->txt_value = (gcnew System::Windows::Forms::TextBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->btnQuit = (gcnew System::Windows::Forms::Button());
			this->btnOK = (gcnew System::Windows::Forms::Button());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->rdo_10 = (gcnew System::Windows::Forms::RadioButton());
			this->rdo_16 = (gcnew System::Windows::Forms::RadioButton());
			this->groupBox1->SuspendLayout();
			this->SuspendLayout();
			// 
			// txt_name
			// 
			this->txt_name->Location = System::Drawing::Point(14, 25);
			this->txt_name->Name = L"txt_name";
			this->txt_name->ReadOnly = true;
			this->txt_name->Size = System::Drawing::Size(297, 21);
			this->txt_name->TabIndex = 5;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(12, 9);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(77, 12);
			this->label1->TabIndex = 1;
			this->label1->Text = L"数值名称(N):";
			// 
			// txt_value
			// 
			this->txt_value->Location = System::Drawing::Point(14, 74);
			this->txt_value->Name = L"txt_value";
			this->txt_value->Size = System::Drawing::Size(115, 21);
			this->txt_value->TabIndex = 0;
			this->txt_value->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &reg_dword_change_form::txt_value_KeyPress);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(12, 58);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(77, 12);
			this->label2->TabIndex = 4;
			this->label2->Text = L"数值数据(V):";
			// 
			// btnQuit
			// 
			this->btnQuit->Location = System::Drawing::Point(236, 135);
			this->btnQuit->Name = L"btnQuit";
			this->btnQuit->Size = System::Drawing::Size(75, 23);
			this->btnQuit->TabIndex = 4;
			this->btnQuit->Text = L"取消";
			this->btnQuit->UseVisualStyleBackColor = true;
			this->btnQuit->Click += gcnew System::EventHandler(this, &reg_dword_change_form::btnQuit_Click);
			// 
			// btnOK
			// 
			this->btnOK->Location = System::Drawing::Point(144, 135);
			this->btnOK->Name = L"btnOK";
			this->btnOK->Size = System::Drawing::Size(75, 23);
			this->btnOK->TabIndex = 3;
			this->btnOK->Text = L"确定";
			this->btnOK->UseVisualStyleBackColor = true;
			this->btnOK->Click += gcnew System::EventHandler(this, &reg_dword_change_form::btnOK_Click);
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->rdo_10);
			this->groupBox1->Controls->Add(this->rdo_16);
			this->groupBox1->Location = System::Drawing::Point(144, 58);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(167, 65);
			this->groupBox1->TabIndex = 7;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"基数";
			// 
			// rdo_10
			// 
			this->rdo_10->AutoSize = true;
			this->rdo_10->Location = System::Drawing::Point(21, 38);
			this->rdo_10->Name = L"rdo_10";
			this->rdo_10->Size = System::Drawing::Size(77, 16);
			this->rdo_10->TabIndex = 2;
			this->rdo_10->Text = L"十进制(D)";
			this->rdo_10->UseVisualStyleBackColor = true;
			// 
			// rdo_16
			// 
			this->rdo_16->AutoSize = true;
			this->rdo_16->Checked = true;
			this->rdo_16->Location = System::Drawing::Point(21, 16);
			this->rdo_16->Name = L"rdo_16";
			this->rdo_16->Size = System::Drawing::Size(89, 16);
			this->rdo_16->TabIndex = 1;
			this->rdo_16->TabStop = true;
			this->rdo_16->Text = L"十六进制(H)";
			this->rdo_16->UseVisualStyleBackColor = true;
			this->rdo_16->CheckedChanged += gcnew System::EventHandler(this, &reg_dword_change_form::rdo_16_CheckedChanged);
			// 
			// reg_dword_change_form
			// 
			this->AcceptButton = this->btnOK;
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(323, 167);
			this->Controls->Add(this->groupBox1);
			this->Controls->Add(this->btnQuit);
			this->Controls->Add(this->btnOK);
			this->Controls->Add(this->txt_value);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->txt_name);
			this->Controls->Add(this->label1);
			this->MaximizeBox = false;
			this->Name = L"reg_dword_change_form";
			this->ShowInTaskbar = false;
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = L"编辑 DWORD 值";
			this->Load += gcnew System::EventHandler(this, &reg_dword_change_form::reg_dword_change_form_Load);
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

		 //
		 static ListViewItem^ item_ = nullptr;

		 //取消
private: System::Void btnQuit_Click(System::Object^  sender, System::EventArgs^  e) {		
			 //
			 this->Close();
		 }

		 //确定
private: System::Void btnOK_Click(System::Object^  sender, System::EventArgs^  e) {
			 try
			 {
				 //
				 RegistryKey^ rk_ = (RegistryKey^)item_->Tag;
				 //
				 if (String::IsNullOrEmpty(txt_value->Text))
				 {
					 //
					 rk_->SetValue(txt_name->Text,0,RegistryValueKind::DWord);
				 }
				 else
				 {
					 //
					 rk_->SetValue(txt_name->Text,(rdo_16->Checked ? Convert::ToUInt32(txt_value->Text,16) : Convert::ToUInt32(txt_value->Text)),RegistryValueKind::DWord);
				 }			 
				 //更新item
				 item_->SubItems[2]->Text = "0x" + Convert::ToInt32(rk_->GetValue(txt_name->Text)->ToString()).ToString("x8");;
				 //
				 this->Close();
			 }
			 catch(Exception^ ex){ }
		 }

		 //
private: System::Void reg_dword_change_form_Load(System::Object^  sender, System::EventArgs^  e) {
			 //
			 txt_name->Text = item_->Text;
			 //
			 RegistryKey^ rk_ = (RegistryKey^)item_->Tag;
			 //
			 txt_value->Text = Convert::ToInt32(rk_->GetValue(txt_name->Text)->ToString()).ToString("x");
		 }

		 //
private: System::Void rdo_16_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 //
			 if (rdo_16->Checked)
			 {
				 //
				 txt_value->Text = (Convert::ToUInt32(txt_value->Text)).ToString("x");
			 }
			 else
			 {
				 //
				 txt_value->Text = (Convert::ToUInt32(txt_value->Text,16)).ToString();
			 }
		 }

		 //拦截字符的输入
private: System::Void txt_value_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) {
			 //
			 if (rdo_16->Checked)
			 {
				 //只运行6个字母和数字，8位
			 }
			 else
			 {
				 //只允许数字，10位
			 }
		 }
};
}
