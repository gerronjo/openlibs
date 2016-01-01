#pragma once

namespace Deianeira {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Diagnostics;

	/// <summary>
	/// EvenglogForm 摘要
	/// </summary>
	public ref class EventlogForm : public System::Windows::Forms::Form
	{
	public:
		EventlogForm(EventLogEntry^ ele_)
		{
			InitializeComponent();
			//
			//TODO: 在此处添加构造函数代码
			//
			this->ele_ = ele_;
		}

	protected:
		/// <summary>
		/// 清理所有正在使用的资源。
		/// </summary>
		~EventlogForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TabControl^  tabControl1;
	protected: 
	private: System::Windows::Forms::TabPage^  tabPage1;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::RichTextBox^  rtb_;
	private: System::Windows::Forms::Label^  label9;
	private: System::Windows::Forms::Label^  label8;
	private: System::Windows::Forms::Label^  label7;
	private: System::Windows::Forms::Label^  lbl_comp;
	private: System::Windows::Forms::Label^  lbl_user;
	private: System::Windows::Forms::Label^  lbl_type;




	private: System::Windows::Forms::Label^  lbl_time;

	private: System::Windows::Forms::Label^  lbl_date;
	private: System::Windows::Forms::Label^  lbl_id;
	private: System::Windows::Forms::Label^  lbl_category;

	private: System::Windows::Forms::Label^  lbl_source;








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
			this->lbl_id = (gcnew System::Windows::Forms::Label());
			this->lbl_category = (gcnew System::Windows::Forms::Label());
			this->lbl_source = (gcnew System::Windows::Forms::Label());
			this->lbl_comp = (gcnew System::Windows::Forms::Label());
			this->lbl_user = (gcnew System::Windows::Forms::Label());
			this->lbl_type = (gcnew System::Windows::Forms::Label());
			this->lbl_time = (gcnew System::Windows::Forms::Label());
			this->lbl_date = (gcnew System::Windows::Forms::Label());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->rtb_ = (gcnew System::Windows::Forms::RichTextBox());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->tabControl1->SuspendLayout();
			this->tabPage1->SuspendLayout();
			this->SuspendLayout();
			// 
			// tabControl1
			// 
			this->tabControl1->Controls->Add(this->tabPage1);
			this->tabControl1->Location = System::Drawing::Point(12, 12);
			this->tabControl1->Name = L"tabControl1";
			this->tabControl1->SelectedIndex = 0;
			this->tabControl1->Size = System::Drawing::Size(367, 394);
			this->tabControl1->TabIndex = 0;
			// 
			// tabPage1
			// 
			this->tabPage1->Controls->Add(this->lbl_id);
			this->tabPage1->Controls->Add(this->lbl_category);
			this->tabPage1->Controls->Add(this->lbl_source);
			this->tabPage1->Controls->Add(this->lbl_comp);
			this->tabPage1->Controls->Add(this->lbl_user);
			this->tabPage1->Controls->Add(this->lbl_type);
			this->tabPage1->Controls->Add(this->lbl_time);
			this->tabPage1->Controls->Add(this->lbl_date);
			this->tabPage1->Controls->Add(this->label9);
			this->tabPage1->Controls->Add(this->label8);
			this->tabPage1->Controls->Add(this->label7);
			this->tabPage1->Controls->Add(this->rtb_);
			this->tabPage1->Controls->Add(this->label6);
			this->tabPage1->Controls->Add(this->label5);
			this->tabPage1->Controls->Add(this->label4);
			this->tabPage1->Controls->Add(this->label3);
			this->tabPage1->Controls->Add(this->label2);
			this->tabPage1->Controls->Add(this->label1);
			this->tabPage1->Location = System::Drawing::Point(4, 21);
			this->tabPage1->Name = L"tabPage1";
			this->tabPage1->Padding = System::Windows::Forms::Padding(3);
			this->tabPage1->Size = System::Drawing::Size(359, 369);
			this->tabPage1->TabIndex = 0;
			this->tabPage1->Text = L"事件详细信息";
			this->tabPage1->UseVisualStyleBackColor = true;
			// 
			// lbl_id
			// 
			this->lbl_id->AutoSize = true;
			this->lbl_id->Location = System::Drawing::Point(229, 61);
			this->lbl_id->Name = L"lbl_id";
			this->lbl_id->Size = System::Drawing::Size(0, 12);
			this->lbl_id->TabIndex = 4;
			// 
			// lbl_category
			// 
			this->lbl_category->AutoSize = true;
			this->lbl_category->Location = System::Drawing::Point(211, 39);
			this->lbl_category->Name = L"lbl_category";
			this->lbl_category->Size = System::Drawing::Size(0, 12);
			this->lbl_category->TabIndex = 4;
			// 
			// lbl_source
			// 
			this->lbl_source->AutoSize = true;
			this->lbl_source->Location = System::Drawing::Point(211, 17);
			this->lbl_source->Name = L"lbl_source";
			this->lbl_source->Size = System::Drawing::Size(0, 12);
			this->lbl_source->TabIndex = 4;
			// 
			// lbl_comp
			// 
			this->lbl_comp->AutoSize = true;
			this->lbl_comp->Location = System::Drawing::Point(88, 107);
			this->lbl_comp->Name = L"lbl_comp";
			this->lbl_comp->Size = System::Drawing::Size(0, 12);
			this->lbl_comp->TabIndex = 3;
			// 
			// lbl_user
			// 
			this->lbl_user->AutoSize = true;
			this->lbl_user->Location = System::Drawing::Point(77, 84);
			this->lbl_user->Name = L"lbl_user";
			this->lbl_user->Size = System::Drawing::Size(0, 12);
			this->lbl_user->TabIndex = 3;
			// 
			// lbl_type
			// 
			this->lbl_type->AutoSize = true;
			this->lbl_type->Location = System::Drawing::Point(77, 61);
			this->lbl_type->Name = L"lbl_type";
			this->lbl_type->Size = System::Drawing::Size(0, 12);
			this->lbl_type->TabIndex = 3;
			// 
			// lbl_time
			// 
			this->lbl_time->AutoSize = true;
			this->lbl_time->Location = System::Drawing::Point(77, 39);
			this->lbl_time->Name = L"lbl_time";
			this->lbl_time->Size = System::Drawing::Size(0, 12);
			this->lbl_time->TabIndex = 3;
			// 
			// lbl_date
			// 
			this->lbl_date->AutoSize = true;
			this->lbl_date->Location = System::Drawing::Point(77, 17);
			this->lbl_date->Name = L"lbl_date";
			this->lbl_date->Size = System::Drawing::Size(0, 12);
			this->lbl_date->TabIndex = 3;
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Location = System::Drawing::Point(152, 61);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(71, 12);
			this->label9->TabIndex = 2;
			this->label9->Text = L"事件 ID(I):";
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Location = System::Drawing::Point(152, 39);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(53, 12);
			this->label8->TabIndex = 2;
			this->label8->Text = L"类别(R):";
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(152, 17);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(53, 12);
			this->label7->TabIndex = 2;
			this->label7->Text = L"来源(S):";
			// 
			// rtb_
			// 
			this->rtb_->Location = System::Drawing::Point(19, 166);
			this->rtb_->Name = L"rtb_";
			this->rtb_->ReadOnly = true;
			this->rtb_->Size = System::Drawing::Size(317, 185);
			this->rtb_->TabIndex = 1;
			this->rtb_->Text = L"";
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(17, 142);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(53, 12);
			this->label6->TabIndex = 0;
			this->label6->Text = L"描述(D):";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(17, 107);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(65, 12);
			this->label5->TabIndex = 0;
			this->label5->Text = L"计算机(O):";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(17, 84);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(53, 12);
			this->label4->TabIndex = 0;
			this->label4->Text = L"用户(U):";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(17, 61);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(53, 12);
			this->label3->TabIndex = 0;
			this->label3->Text = L"类型(E):";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(17, 39);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(53, 12);
			this->label2->TabIndex = 0;
			this->label2->Text = L"时间(M):";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(17, 17);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(53, 12);
			this->label1->TabIndex = 0;
			this->label1->Text = L"日期(A):";
			// 
			// EvenglogForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(391, 417);
			this->Controls->Add(this->tabControl1);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->MaximizeBox = false;
			this->Name = L"EvenglogForm";
			this->ShowInTaskbar = false;
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = L"信息属性";
			this->Load += gcnew System::EventHandler(this, &EventlogForm::EventlogForm_Load);
			this->tabControl1->ResumeLayout(false);
			this->tabPage1->ResumeLayout(false);
			this->tabPage1->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion

		 EventLogEntry^ ele_;

		 //窗体的加载事件
private: System::Void EventlogForm_Load(System::Object^  sender, System::EventArgs^  e) {
			 //日期
			 lbl_date->Text = ele_->TimeWritten.ToString()->Split(' ')[0];
			 //时间
			 lbl_time->Text = ele_->TimeWritten.ToString()->Split(' ')[1];
			 //类型
			 lbl_type->Text = ele_->EntryType.ToString();
			 //用户
			 lbl_user->Text = ele_->UserName;
			 //计算机
			 lbl_comp->Text = ele_->MachineName;
			 //来源
			 lbl_source->Text = ele_->Source;
			 //类别
			 lbl_category->Text = ele_->Category;
			 //事件ID
			 lbl_id->Text = ele_->EventID.ToString();

			 rtb_->Text = ele_->Message+"\n\n"+"有关更多信息，请参阅在 http://go.microsoft.com/fwlink/events.asp 的帮助和支持中心。";
		 }

};
}
