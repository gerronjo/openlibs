#pragma once

namespace Deianeira {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace Microsoft::VisualBasic::FileIO;

	/// <summary>
	/// FileRenameForm 摘要
	/// </summary>
	public ref class FileRenameForm : public System::Windows::Forms::Form
	{
	public:
		FileRenameForm(ListViewItem^ item_)
		{
			InitializeComponent();
			//
			//TODO: 在此处添加构造函数代码
			//
			this->item_ = item_;
		}
		FileRenameForm(TreeNode^ tn_)
		{
			InitializeComponent();
			//
			//TODO: 在此处添加构造函数代码
			//
			this->tn_ = tn_;
		}
	protected:
		/// <summary>
		/// 清理所有正在使用的资源。
		/// </summary>
		~FileRenameForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TextBox^  txtName;
	protected: 

	private: System::Windows::Forms::Button^  btnOK;
	private: System::Windows::Forms::Button^  btnQuit;
	protected: 



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
			this->txtName = (gcnew System::Windows::Forms::TextBox());
			this->btnOK = (gcnew System::Windows::Forms::Button());
			this->btnQuit = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// txtName
			// 
			this->txtName->Location = System::Drawing::Point(12, 12);
			this->txtName->Name = L"txtName";
			this->txtName->Size = System::Drawing::Size(272, 21);
			this->txtName->TabIndex = 0;
			// 
			// btnOK
			// 
			this->btnOK->Location = System::Drawing::Point(128, 39);
			this->btnOK->Name = L"btnOK";
			this->btnOK->Size = System::Drawing::Size(75, 23);
			this->btnOK->TabIndex = 1;
			this->btnOK->Text = L"确定";
			this->btnOK->UseVisualStyleBackColor = true;
			this->btnOK->Click += gcnew System::EventHandler(this, &FileRenameForm::btnOK_Click);
			// 
			// btnQuit
			// 
			this->btnQuit->Location = System::Drawing::Point(209, 39);
			this->btnQuit->Name = L"btnQuit";
			this->btnQuit->Size = System::Drawing::Size(75, 23);
			this->btnQuit->TabIndex = 2;
			this->btnQuit->Text = L"取消";
			this->btnQuit->UseVisualStyleBackColor = true;
			this->btnQuit->Click += gcnew System::EventHandler(this, &FileRenameForm::btnQuit_Click);
			// 
			// FileRenameForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(297, 70);
			this->Controls->Add(this->btnQuit);
			this->Controls->Add(this->btnOK);
			this->Controls->Add(this->txtName);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->MaximizeBox = false;
			this->Name = L"FileRenameForm";
			this->ShowInTaskbar = false;
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = L"重命名";
			this->Load += gcnew System::EventHandler(this, &FileRenameForm::FileRenameForm_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

		 ListViewItem^ item_;
		 TreeNode^ tn_;

		 //窗体加载事件
private: System::Void FileRenameForm_Load(System::Object^  sender, System::EventArgs^  e) {
			 if (item_!=nullptr)
			 {
				 this->txtName->Text = item_->SubItems[0]->Text;
				 txtName->SelectAll();
			 }
			 else
			 {
				 this->txtName->Text = tn_->Text;
				 txtName->SelectAll();
			 }
		 }

		 //确定
private: System::Void btnOK_Click(System::Object^  sender, System::EventArgs^  e) {
			 try
			 {
				 if (item_!=nullptr)
				 {
					 if (item_->SubItems[2]->Text == "文件夹")
					 {
						 FileSystem::RenameDirectory(item_->Tag->ToString(),txtName->Text);
					 }
					 else
					 {
						 FileSystem::RenameFile(item_->Tag->ToString(),txtName->Text);
					 }

					 item_->SubItems[0]->Text = txtName->Text;
				 }
				 else
				 {
					 FileSystem::RenameDirectory(tn_->Tag->ToString(),txtName->Text);

					 tn_->Text = txtName->Text;
				 }

				 this->Close();
			 }
			 catch (Exception^ ex)
			 {
				 MessageBox::Show(ex->Message);
			 }	 
		 }

		 //取消
private: System::Void btnQuit_Click(System::Object^  sender, System::EventArgs^  e) {
			 this->Close();
		 }

	};
}
