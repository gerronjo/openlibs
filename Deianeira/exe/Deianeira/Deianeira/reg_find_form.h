#pragma once

namespace Deianeira {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// reg_find_form 摘要
	/// </summary>
	public ref class reg_find_form : public System::Windows::Forms::Form
	{
	public:
		reg_find_form(void)
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
		~reg_find_form()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::TextBox^  txt_name;
	protected: 

	private: System::Windows::Forms::GroupBox^  groupBox1;
	private: System::Windows::Forms::CheckBox^  chk_value;

	private: System::Windows::Forms::CheckBox^  chk_name;

	private: System::Windows::Forms::CheckBox^  chk_item;

	private: System::Windows::Forms::CheckBox^  chk_whole;
	private: System::Windows::Forms::CheckBox^  chk_ignore;



	private: System::Windows::Forms::Button^  btnFind;
	private: System::Windows::Forms::Button^  btnQuit;



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
			this->txt_name = (gcnew System::Windows::Forms::TextBox());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->chk_value = (gcnew System::Windows::Forms::CheckBox());
			this->chk_name = (gcnew System::Windows::Forms::CheckBox());
			this->chk_item = (gcnew System::Windows::Forms::CheckBox());
			this->chk_whole = (gcnew System::Windows::Forms::CheckBox());
			this->chk_ignore = (gcnew System::Windows::Forms::CheckBox());
			this->btnFind = (gcnew System::Windows::Forms::Button());
			this->btnQuit = (gcnew System::Windows::Forms::Button());
			this->groupBox1->SuspendLayout();
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(10, 13);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(77, 12);
			this->label1->TabIndex = 0;
			this->label1->Text = L"查找目标(N):";
			// 
			// txt_name
			// 
			this->txt_name->Location = System::Drawing::Point(91, 9);
			this->txt_name->Name = L"txt_name";
			this->txt_name->Size = System::Drawing::Size(222, 21);
			this->txt_name->TabIndex = 0;
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->chk_value);
			this->groupBox1->Controls->Add(this->chk_name);
			this->groupBox1->Controls->Add(this->chk_item);
			this->groupBox1->Location = System::Drawing::Point(12, 35);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(301, 78);
			this->groupBox1->TabIndex = 2;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"查看";
			// 
			// chk_value
			// 
			this->chk_value->AutoSize = true;
			this->chk_value->Location = System::Drawing::Point(10, 56);
			this->chk_value->Name = L"chk_value";
			this->chk_value->Size = System::Drawing::Size(66, 16);
			this->chk_value->TabIndex = 3;
			this->chk_value->Text = L"数据(D)";
			this->chk_value->UseVisualStyleBackColor = true;
			// 
			// chk_name
			// 
			this->chk_name->AutoSize = true;
			this->chk_name->Location = System::Drawing::Point(10, 38);
			this->chk_name->Name = L"chk_name";
			this->chk_name->Size = System::Drawing::Size(54, 16);
			this->chk_name->TabIndex = 2;
			this->chk_name->Text = L"值(V)";
			this->chk_name->UseVisualStyleBackColor = true;
			// 
			// chk_item
			// 
			this->chk_item->AutoSize = true;
			this->chk_item->Location = System::Drawing::Point(10, 20);
			this->chk_item->Name = L"chk_item";
			this->chk_item->Size = System::Drawing::Size(54, 16);
			this->chk_item->TabIndex = 1;
			this->chk_item->Text = L"项(K)";
			this->chk_item->UseVisualStyleBackColor = true;
			// 
			// chk_whole
			// 
			this->chk_whole->AutoSize = true;
			this->chk_whole->Location = System::Drawing::Point(12, 121);
			this->chk_whole->Name = L"chk_whole";
			this->chk_whole->Size = System::Drawing::Size(90, 16);
			this->chk_whole->TabIndex = 4;
			this->chk_whole->Text = L"全字匹配(W)";
			this->chk_whole->UseVisualStyleBackColor = true;
			// 
			// chk_ignore
			// 
			this->chk_ignore->AutoSize = true;
			this->chk_ignore->Location = System::Drawing::Point(106, 121);
			this->chk_ignore->Name = L"chk_ignore";
			this->chk_ignore->Size = System::Drawing::Size(84, 16);
			this->chk_ignore->TabIndex = 5;
			this->chk_ignore->Text = L"忽略大小写";
			this->chk_ignore->UseVisualStyleBackColor = true;
			// 
			// btnFind
			// 
			this->btnFind->Location = System::Drawing::Point(324, 8);
			this->btnFind->Name = L"btnFind";
			this->btnFind->Size = System::Drawing::Size(96, 23);
			this->btnFind->TabIndex = 6;
			this->btnFind->Text = L"查找下一个(F)";
			this->btnFind->UseVisualStyleBackColor = true;
			this->btnFind->Click += gcnew System::EventHandler(this, &reg_find_form::btnFind_Click);
			// 
			// btnQuit
			// 
			this->btnQuit->Location = System::Drawing::Point(324, 35);
			this->btnQuit->Name = L"btnQuit";
			this->btnQuit->Size = System::Drawing::Size(96, 23);
			this->btnQuit->TabIndex = 7;
			this->btnQuit->Text = L"取消";
			this->btnQuit->UseVisualStyleBackColor = true;
			this->btnQuit->Click += gcnew System::EventHandler(this, &reg_find_form::btnQuit_Click);
			// 
			// reg_find_form
			// 
			this->AcceptButton = this->btnFind;
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(430, 148);
			this->Controls->Add(this->btnQuit);
			this->Controls->Add(this->btnFind);
			this->Controls->Add(this->chk_ignore);
			this->Controls->Add(this->chk_whole);
			this->Controls->Add(this->groupBox1);
			this->Controls->Add(this->txt_name);
			this->Controls->Add(this->label1);
			this->MaximizeBox = false;
			this->Name = L"reg_find_form";
			this->ShowInTaskbar = false;
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = L"查找";
			this->Load += gcnew System::EventHandler(this, &reg_find_form::reg_find_form_Load);
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	
		 //
private: System::Void reg_find_form_Load(System::Object^  sender, System::EventArgs^  e) {
			 //

		 }

		 //
private: System::Void btnFind_Click(System::Object^  sender, System::EventArgs^  e) {
			//

		 }

		 //
private: System::Void btnQuit_Click(System::Object^  sender, System::EventArgs^  e) {		
			 //

		 }

};
}
