#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::Diagnostics;

namespace Deianeira {

	/// <summary>
	/// AboutForm 摘要
	///
	/// 警告: 如果更改此类的名称，则需要更改
	///          与此类所依赖的所有 .resx 文件关联的托管资源编译器工具的
	///          “资源文件名”属性。否则，
	///          设计器将不能与此窗体的关联
	///          本地化资源正确交互。
	/// </summary>
	public ref class AboutForm : public System::Windows::Forms::Form
	{
	public:
		AboutForm(void)
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
		~AboutForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::PictureBox^  pictureBox1;
	protected: 
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Button^  btnOK;
	private: System::Windows::Forms::Label^  lbl_blog;
	private: System::Windows::Forms::Label^  lbl_email;



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
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(AboutForm::typeid));
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->btnOK = (gcnew System::Windows::Forms::Button());
			this->lbl_blog = (gcnew System::Windows::Forms::Label());
			this->lbl_email = (gcnew System::Windows::Forms::Label());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->BeginInit();
			this->SuspendLayout();
			// 
			// pictureBox1
			// 
			this->pictureBox1->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"pictureBox1.Image")));
			this->pictureBox1->Location = System::Drawing::Point(21, 12);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(70, 71);
			this->pictureBox1->TabIndex = 0;
			this->pictureBox1->TabStop = false;
			// 
			// label1
			// 
			this->label1->Location = System::Drawing::Point(111, 12);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(181, 44);
			this->label1->TabIndex = 1;
			this->label1->Text = L"Deianeira 1.0 beta\r\n\r\n版权所有 (C) 2010-2011 乱码";
			// 
			// btnOK
			// 
			this->btnOK->Location = System::Drawing::Point(290, 12);
			this->btnOK->Name = L"btnOK";
			this->btnOK->Size = System::Drawing::Size(75, 23);
			this->btnOK->TabIndex = 2;
			this->btnOK->Text = L"确定";
			this->btnOK->UseVisualStyleBackColor = true;
			this->btnOK->Click += gcnew System::EventHandler(this, &AboutForm::btnOK_Click);
			// 
			// lbl_blog
			// 
			this->lbl_blog->AutoSize = true;
			this->lbl_blog->Font = (gcnew System::Drawing::Font(L"宋体", 9, System::Drawing::FontStyle::Underline, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(134)));
			this->lbl_blog->ForeColor = System::Drawing::Color::Blue;
			this->lbl_blog->Location = System::Drawing::Point(111, 71);
			this->lbl_blog->Name = L"lbl_blog";
			this->lbl_blog->Size = System::Drawing::Size(95, 12);
			this->lbl_blog->TabIndex = 3;
			this->lbl_blog->Text = L"www.badteen.net";
			this->lbl_blog->Click += gcnew System::EventHandler(this, &AboutForm::lbl_blog_Click);
			// 
			// lbl_email
			// 
			this->lbl_email->AutoSize = true;
			this->lbl_email->Font = (gcnew System::Drawing::Font(L"宋体", 9, System::Drawing::FontStyle::Underline, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(134)));
			this->lbl_email->ForeColor = System::Drawing::Color::Blue;
			this->lbl_email->Location = System::Drawing::Point(255, 71);
			this->lbl_email->Name = L"lbl_email";
			this->lbl_email->Size = System::Drawing::Size(101, 12);
			this->lbl_email->TabIndex = 4;
			this->lbl_email->Text = L"badteen@sina.com";
			this->lbl_email->Click += gcnew System::EventHandler(this, &AboutForm::lbl_email_Click);
			// 
			// AboutForm
			// 
			this->AcceptButton = this->btnOK;
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(382, 102);
			this->Controls->Add(this->lbl_email);
			this->Controls->Add(this->lbl_blog);
			this->Controls->Add(this->btnOK);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->pictureBox1);
			this->MaximizeBox = false;
			this->Name = L"AboutForm";
			this->ShowInTaskbar = false;
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = L"关于 Deianeira";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

		 //确定
private: System::Void btnOK_Click(System::Object^  sender, System::EventArgs^  e) {
			 this->Close();
		 }

		 //博客
private: System::Void lbl_blog_Click(System::Object^  sender, System::EventArgs^  e) {
			 try
			 {
				 Process::Start("http://www.badteen.net");
			 }
			 catch(Exception^ ex){ MessageBox::Show(ex->Message); }
		 }

		 //邮箱
private: System::Void lbl_email_Click(System::Object^  sender, System::EventArgs^  e) {
			 try
			 {
				 Process::Start("mailto:badteen@sina.com");
			 }
			 catch(Exception^ ex){ MessageBox::Show(ex->Message); }
		 }
};
}
