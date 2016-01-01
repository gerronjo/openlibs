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
	/// other_table_view_form 摘要
	/// </summary>
	public ref class other_table_view_form : public System::Windows::Forms::Form
	{
	public:
		other_table_view_form(String^ str_)
		{
			InitializeComponent();
			//
			//TODO: 在此处添加构造函数代码
			//
			this->str_ = str_;
		}

	protected:
		/// <summary>
		/// 清理所有正在使用的资源。
		/// </summary>
		~other_table_view_form()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TabControl^  tc_;
	protected: 

	protected: 
	private: System::Windows::Forms::TabPage^  tabPage1;
	private: System::Windows::Forms::TabPage^  tabPage2;
	private: System::Windows::Forms::TabPage^  tabPage3;
	private: System::Windows::Forms::ListView^  lv_;

	private: System::Windows::Forms::ColumnHeader^  columnHeader1;
	private: System::Windows::Forms::ColumnHeader^  columnHeader2;
	private: System::Windows::Forms::ColumnHeader^  columnHeader3;
	private: System::Windows::Forms::ColumnHeader^  columnHeader4;
	private: System::Windows::Forms::ColumnHeader^  columnHeader5;
	private: System::Windows::Forms::ColumnHeader^  columnHeader6;
	private: System::Windows::Forms::TabPage^  tabPage4;
	private: System::Windows::Forms::TextBox^  txtDllFlags;

	private: System::Windows::Forms::TextBox^  txtCheckSum;

	private: System::Windows::Forms::TextBox^  txtSubSystem;

	private: System::Windows::Forms::TextBox^  txtFileAlign;

	private: System::Windows::Forms::TextBox^  txtSecAlign;

	private: System::Windows::Forms::TextBox^  txtHeaderSize;

	private: System::Windows::Forms::TextBox^  txtImageSize;

	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::Label^  label11;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::Label^  label10;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::Label^  label9;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::TextBox^  txtDataBase;

	private: System::Windows::Forms::TextBox^  txtCodeBase;

	private: System::Windows::Forms::Label^  label8;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::TextBox^  txtImageBase;



	private: System::Windows::Forms::TextBox^  txtOEP;

	private: System::Windows::Forms::Label^  label7;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::TextBox^  txtCharac;

	private: System::Windows::Forms::TextBox^  txtSizeOfOH;
	private: System::Windows::Forms::TextBox^  txtNumOfSymbols;



	private: System::Windows::Forms::Label^  label16;
	private: System::Windows::Forms::Label^  label17;
	private: System::Windows::Forms::Label^  label18;
	private: System::Windows::Forms::TextBox^  txtPointerToST;


	private: System::Windows::Forms::TextBox^  txtTimeDateStamp;

	private: System::Windows::Forms::Label^  label19;
	private: System::Windows::Forms::Label^  label20;
	private: System::Windows::Forms::TextBox^  txtNumOfSec;

	private: System::Windows::Forms::TextBox^  txtMachine;

	private: System::Windows::Forms::Label^  label21;
	private: System::Windows::Forms::Label^  label22;
	private: System::Windows::Forms::ListView^  lv_data;
	private: System::Windows::Forms::ColumnHeader^  columnHeader7;
	private: System::Windows::Forms::ColumnHeader^  columnHeader8;
	private: System::Windows::Forms::ColumnHeader^  columnHeader9;
	private: System::Windows::Forms::ColumnHeader^  columnHeader10;

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
			this->tc_ = (gcnew System::Windows::Forms::TabControl());
			this->tabPage1 = (gcnew System::Windows::Forms::TabPage());
			this->txtDllFlags = (gcnew System::Windows::Forms::TextBox());
			this->txtCheckSum = (gcnew System::Windows::Forms::TextBox());
			this->txtSubSystem = (gcnew System::Windows::Forms::TextBox());
			this->txtFileAlign = (gcnew System::Windows::Forms::TextBox());
			this->txtSecAlign = (gcnew System::Windows::Forms::TextBox());
			this->txtHeaderSize = (gcnew System::Windows::Forms::TextBox());
			this->txtImageSize = (gcnew System::Windows::Forms::TextBox());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->label11 = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->txtDataBase = (gcnew System::Windows::Forms::TextBox());
			this->txtCodeBase = (gcnew System::Windows::Forms::TextBox());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->txtImageBase = (gcnew System::Windows::Forms::TextBox());
			this->txtOEP = (gcnew System::Windows::Forms::TextBox());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->tabPage2 = (gcnew System::Windows::Forms::TabPage());
			this->txtCharac = (gcnew System::Windows::Forms::TextBox());
			this->txtSizeOfOH = (gcnew System::Windows::Forms::TextBox());
			this->txtNumOfSymbols = (gcnew System::Windows::Forms::TextBox());
			this->label16 = (gcnew System::Windows::Forms::Label());
			this->label17 = (gcnew System::Windows::Forms::Label());
			this->label18 = (gcnew System::Windows::Forms::Label());
			this->txtPointerToST = (gcnew System::Windows::Forms::TextBox());
			this->txtTimeDateStamp = (gcnew System::Windows::Forms::TextBox());
			this->label19 = (gcnew System::Windows::Forms::Label());
			this->label20 = (gcnew System::Windows::Forms::Label());
			this->txtNumOfSec = (gcnew System::Windows::Forms::TextBox());
			this->txtMachine = (gcnew System::Windows::Forms::TextBox());
			this->label21 = (gcnew System::Windows::Forms::Label());
			this->label22 = (gcnew System::Windows::Forms::Label());
			this->tabPage3 = (gcnew System::Windows::Forms::TabPage());
			this->lv_ = (gcnew System::Windows::Forms::ListView());
			this->columnHeader1 = (gcnew System::Windows::Forms::ColumnHeader());
			this->columnHeader2 = (gcnew System::Windows::Forms::ColumnHeader());
			this->columnHeader3 = (gcnew System::Windows::Forms::ColumnHeader());
			this->columnHeader4 = (gcnew System::Windows::Forms::ColumnHeader());
			this->columnHeader5 = (gcnew System::Windows::Forms::ColumnHeader());
			this->columnHeader6 = (gcnew System::Windows::Forms::ColumnHeader());
			this->tabPage4 = (gcnew System::Windows::Forms::TabPage());
			this->lv_data = (gcnew System::Windows::Forms::ListView());
			this->columnHeader7 = (gcnew System::Windows::Forms::ColumnHeader());
			this->columnHeader8 = (gcnew System::Windows::Forms::ColumnHeader());
			this->columnHeader9 = (gcnew System::Windows::Forms::ColumnHeader());
			this->columnHeader10 = (gcnew System::Windows::Forms::ColumnHeader());
			this->tc_->SuspendLayout();
			this->tabPage1->SuspendLayout();
			this->tabPage2->SuspendLayout();
			this->tabPage3->SuspendLayout();
			this->tabPage4->SuspendLayout();
			this->SuspendLayout();
			// 
			// tc_
			// 
			this->tc_->Controls->Add(this->tabPage1);
			this->tc_->Controls->Add(this->tabPage2);
			this->tc_->Controls->Add(this->tabPage3);
			this->tc_->Controls->Add(this->tabPage4);
			this->tc_->Dock = System::Windows::Forms::DockStyle::Fill;
			this->tc_->Location = System::Drawing::Point(0, 0);
			this->tc_->Name = L"tc_";
			this->tc_->SelectedIndex = 0;
			this->tc_->Size = System::Drawing::Size(597, 297);
			this->tc_->TabIndex = 0;
			this->tc_->SelectedIndexChanged += gcnew System::EventHandler(this, &other_table_view_form::tc__SelectedIndexChanged);
			// 
			// tabPage1
			// 
			this->tabPage1->BackColor = System::Drawing::SystemColors::Window;
			this->tabPage1->Controls->Add(this->txtDllFlags);
			this->tabPage1->Controls->Add(this->txtCheckSum);
			this->tabPage1->Controls->Add(this->txtSubSystem);
			this->tabPage1->Controls->Add(this->txtFileAlign);
			this->tabPage1->Controls->Add(this->txtSecAlign);
			this->tabPage1->Controls->Add(this->txtHeaderSize);
			this->tabPage1->Controls->Add(this->txtImageSize);
			this->tabPage1->Controls->Add(this->label6);
			this->tabPage1->Controls->Add(this->label11);
			this->tabPage1->Controls->Add(this->label5);
			this->tabPage1->Controls->Add(this->label10);
			this->tabPage1->Controls->Add(this->label4);
			this->tabPage1->Controls->Add(this->label9);
			this->tabPage1->Controls->Add(this->label3);
			this->tabPage1->Controls->Add(this->txtDataBase);
			this->tabPage1->Controls->Add(this->txtCodeBase);
			this->tabPage1->Controls->Add(this->label8);
			this->tabPage1->Controls->Add(this->label2);
			this->tabPage1->Controls->Add(this->txtImageBase);
			this->tabPage1->Controls->Add(this->txtOEP);
			this->tabPage1->Controls->Add(this->label7);
			this->tabPage1->Controls->Add(this->label1);
			this->tabPage1->Location = System::Drawing::Point(4, 21);
			this->tabPage1->Name = L"tabPage1";
			this->tabPage1->Padding = System::Windows::Forms::Padding(3);
			this->tabPage1->Size = System::Drawing::Size(589, 272);
			this->tabPage1->TabIndex = 0;
			this->tabPage1->Text = L"Optional Header";
			// 
			// txtDllFlags
			// 
			this->txtDllFlags->Location = System::Drawing::Point(141, 197);
			this->txtDllFlags->Name = L"txtDllFlags";
			this->txtDllFlags->Size = System::Drawing::Size(129, 21);
			this->txtDllFlags->TabIndex = 5;
			// 
			// txtCheckSum
			// 
			this->txtCheckSum->Location = System::Drawing::Point(424, 161);
			this->txtCheckSum->Name = L"txtCheckSum";
			this->txtCheckSum->Size = System::Drawing::Size(129, 21);
			this->txtCheckSum->TabIndex = 5;
			// 
			// txtSubSystem
			// 
			this->txtSubSystem->Location = System::Drawing::Point(141, 160);
			this->txtSubSystem->Name = L"txtSubSystem";
			this->txtSubSystem->Size = System::Drawing::Size(129, 21);
			this->txtSubSystem->TabIndex = 5;
			// 
			// txtFileAlign
			// 
			this->txtFileAlign->Location = System::Drawing::Point(424, 124);
			this->txtFileAlign->Name = L"txtFileAlign";
			this->txtFileAlign->Size = System::Drawing::Size(129, 21);
			this->txtFileAlign->TabIndex = 5;
			// 
			// txtSecAlign
			// 
			this->txtSecAlign->Location = System::Drawing::Point(141, 124);
			this->txtSecAlign->Name = L"txtSecAlign";
			this->txtSecAlign->Size = System::Drawing::Size(129, 21);
			this->txtSecAlign->TabIndex = 5;
			// 
			// txtHeaderSize
			// 
			this->txtHeaderSize->Location = System::Drawing::Point(424, 88);
			this->txtHeaderSize->Name = L"txtHeaderSize";
			this->txtHeaderSize->Size = System::Drawing::Size(129, 21);
			this->txtHeaderSize->TabIndex = 5;
			// 
			// txtImageSize
			// 
			this->txtImageSize->Location = System::Drawing::Point(141, 88);
			this->txtImageSize->Name = L"txtImageSize";
			this->txtImageSize->Size = System::Drawing::Size(129, 21);
			this->txtImageSize->TabIndex = 5;
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(22, 201);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(65, 12);
			this->label6->TabIndex = 4;
			this->label6->Text = L"Dll Flags:";
			// 
			// label11
			// 
			this->label11->AutoSize = true;
			this->label11->Location = System::Drawing::Point(323, 164);
			this->label11->Name = L"label11";
			this->label11->Size = System::Drawing::Size(59, 12);
			this->label11->TabIndex = 4;
			this->label11->Text = L"CheckSum:";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(22, 164);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(65, 12);
			this->label5->TabIndex = 4;
			this->label5->Text = L"SubSystem:";
			// 
			// label10
			// 
			this->label10->AutoSize = true;
			this->label10->Location = System::Drawing::Point(323, 128);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(95, 12);
			this->label10->TabIndex = 4;
			this->label10->Text = L"File Alignment:";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(22, 128);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(113, 12);
			this->label4->TabIndex = 4;
			this->label4->Text = L"Section Alignment:";
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Location = System::Drawing::Point(323, 92);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(77, 12);
			this->label9->TabIndex = 4;
			this->label9->Text = L"Header Size:";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(22, 92);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(71, 12);
			this->label3->TabIndex = 4;
			this->label3->Text = L"Image Size:";
			// 
			// txtDataBase
			// 
			this->txtDataBase->Location = System::Drawing::Point(424, 53);
			this->txtDataBase->Name = L"txtDataBase";
			this->txtDataBase->Size = System::Drawing::Size(129, 21);
			this->txtDataBase->TabIndex = 3;
			// 
			// txtCodeBase
			// 
			this->txtCodeBase->Location = System::Drawing::Point(141, 53);
			this->txtCodeBase->Name = L"txtCodeBase";
			this->txtCodeBase->Size = System::Drawing::Size(129, 21);
			this->txtCodeBase->TabIndex = 3;
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Location = System::Drawing::Point(323, 57);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(65, 12);
			this->label8->TabIndex = 2;
			this->label8->Text = L"Data Base:";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(22, 57);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(65, 12);
			this->label2->TabIndex = 2;
			this->label2->Text = L"Code Base:";
			// 
			// txtImageBase
			// 
			this->txtImageBase->Location = System::Drawing::Point(424, 21);
			this->txtImageBase->Name = L"txtImageBase";
			this->txtImageBase->Size = System::Drawing::Size(129, 21);
			this->txtImageBase->TabIndex = 1;
			// 
			// txtOEP
			// 
			this->txtOEP->Location = System::Drawing::Point(141, 21);
			this->txtOEP->Name = L"txtOEP";
			this->txtOEP->Size = System::Drawing::Size(129, 21);
			this->txtOEP->TabIndex = 1;
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(323, 25);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(71, 12);
			this->label7->TabIndex = 0;
			this->label7->Text = L"Image Base:";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(22, 25);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(77, 12);
			this->label1->TabIndex = 0;
			this->label1->Text = L"Entry Ponit:";
			// 
			// tabPage2
			// 
			this->tabPage2->BackColor = System::Drawing::SystemColors::Window;
			this->tabPage2->Controls->Add(this->txtCharac);
			this->tabPage2->Controls->Add(this->txtSizeOfOH);
			this->tabPage2->Controls->Add(this->txtNumOfSymbols);
			this->tabPage2->Controls->Add(this->label16);
			this->tabPage2->Controls->Add(this->label17);
			this->tabPage2->Controls->Add(this->label18);
			this->tabPage2->Controls->Add(this->txtPointerToST);
			this->tabPage2->Controls->Add(this->txtTimeDateStamp);
			this->tabPage2->Controls->Add(this->label19);
			this->tabPage2->Controls->Add(this->label20);
			this->tabPage2->Controls->Add(this->txtNumOfSec);
			this->tabPage2->Controls->Add(this->txtMachine);
			this->tabPage2->Controls->Add(this->label21);
			this->tabPage2->Controls->Add(this->label22);
			this->tabPage2->Location = System::Drawing::Point(4, 21);
			this->tabPage2->Name = L"tabPage2";
			this->tabPage2->Padding = System::Windows::Forms::Padding(3);
			this->tabPage2->Size = System::Drawing::Size(589, 272);
			this->tabPage2->TabIndex = 1;
			this->tabPage2->Text = L"File Header";
			// 
			// txtCharac
			// 
			this->txtCharac->Location = System::Drawing::Point(141, 155);
			this->txtCharac->Name = L"txtCharac";
			this->txtCharac->Size = System::Drawing::Size(129, 21);
			this->txtCharac->TabIndex = 27;
			// 
			// txtSizeOfOH
			// 
			this->txtSizeOfOH->Location = System::Drawing::Point(439, 109);
			this->txtSizeOfOH->Name = L"txtSizeOfOH";
			this->txtSizeOfOH->Size = System::Drawing::Size(129, 21);
			this->txtSizeOfOH->TabIndex = 26;
			// 
			// txtNumOfSymbols
			// 
			this->txtNumOfSymbols->Location = System::Drawing::Point(141, 109);
			this->txtNumOfSymbols->Name = L"txtNumOfSymbols";
			this->txtNumOfSymbols->Size = System::Drawing::Size(129, 21);
			this->txtNumOfSymbols->TabIndex = 25;
			// 
			// label16
			// 
			this->label16->AutoSize = true;
			this->label16->Location = System::Drawing::Point(22, 158);
			this->label16->Name = L"label16";
			this->label16->Size = System::Drawing::Size(101, 12);
			this->label16->TabIndex = 14;
			this->label16->Text = L"Characteristics:";
			// 
			// label17
			// 
			this->label17->AutoSize = true;
			this->label17->Location = System::Drawing::Point(291, 113);
			this->label17->Name = L"label17";
			this->label17->Size = System::Drawing::Size(143, 12);
			this->label17->TabIndex = 15;
			this->label17->Text = L"Size Of OptionalHeader:";
			// 
			// label18
			// 
			this->label18->AutoSize = true;
			this->label18->Location = System::Drawing::Point(22, 113);
			this->label18->Name = L"label18";
			this->label18->Size = System::Drawing::Size(113, 12);
			this->label18->TabIndex = 16;
			this->label18->Text = L"Number Of Symbols:";
			// 
			// txtPointerToST
			// 
			this->txtPointerToST->Location = System::Drawing::Point(439, 69);
			this->txtPointerToST->Name = L"txtPointerToST";
			this->txtPointerToST->Size = System::Drawing::Size(129, 21);
			this->txtPointerToST->TabIndex = 12;
			// 
			// txtTimeDateStamp
			// 
			this->txtTimeDateStamp->Location = System::Drawing::Point(141, 69);
			this->txtTimeDateStamp->Name = L"txtTimeDateStamp";
			this->txtTimeDateStamp->Size = System::Drawing::Size(129, 21);
			this->txtTimeDateStamp->TabIndex = 13;
			// 
			// label19
			// 
			this->label19->AutoSize = true;
			this->label19->Location = System::Drawing::Point(291, 73);
			this->label19->Name = L"label19";
			this->label19->Size = System::Drawing::Size(143, 12);
			this->label19->TabIndex = 10;
			this->label19->Text = L"Pointer To SymbolTable:";
			// 
			// label20
			// 
			this->label20->AutoSize = true;
			this->label20->Location = System::Drawing::Point(22, 73);
			this->label20->Name = L"label20";
			this->label20->Size = System::Drawing::Size(101, 12);
			this->label20->TabIndex = 11;
			this->label20->Text = L"Time Date Stamp:";
			// 
			// txtNumOfSec
			// 
			this->txtNumOfSec->Location = System::Drawing::Point(439, 33);
			this->txtNumOfSec->Name = L"txtNumOfSec";
			this->txtNumOfSec->Size = System::Drawing::Size(129, 21);
			this->txtNumOfSec->TabIndex = 8;
			// 
			// txtMachine
			// 
			this->txtMachine->Location = System::Drawing::Point(141, 33);
			this->txtMachine->Name = L"txtMachine";
			this->txtMachine->Size = System::Drawing::Size(129, 21);
			this->txtMachine->TabIndex = 9;
			// 
			// label21
			// 
			this->label21->AutoSize = true;
			this->label21->Location = System::Drawing::Point(291, 37);
			this->label21->Name = L"label21";
			this->label21->Size = System::Drawing::Size(119, 12);
			this->label21->TabIndex = 6;
			this->label21->Text = L"Number Of Sections:";
			// 
			// label22
			// 
			this->label22->AutoSize = true;
			this->label22->Location = System::Drawing::Point(22, 37);
			this->label22->Name = L"label22";
			this->label22->Size = System::Drawing::Size(53, 12);
			this->label22->TabIndex = 7;
			this->label22->Text = L"Machine:";
			// 
			// tabPage3
			// 
			this->tabPage3->Controls->Add(this->lv_);
			this->tabPage3->Location = System::Drawing::Point(4, 21);
			this->tabPage3->Name = L"tabPage3";
			this->tabPage3->Padding = System::Windows::Forms::Padding(3);
			this->tabPage3->Size = System::Drawing::Size(589, 272);
			this->tabPage3->TabIndex = 2;
			this->tabPage3->Text = L"Section Table";
			this->tabPage3->UseVisualStyleBackColor = true;
			// 
			// lv_
			// 
			this->lv_->AllowColumnReorder = true;
			this->lv_->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(6) {this->columnHeader1, this->columnHeader2, 
				this->columnHeader3, this->columnHeader4, this->columnHeader5, this->columnHeader6});
			this->lv_->Dock = System::Windows::Forms::DockStyle::Fill;
			this->lv_->FullRowSelect = true;
			this->lv_->Location = System::Drawing::Point(3, 3);
			this->lv_->MultiSelect = false;
			this->lv_->Name = L"lv_";
			this->lv_->Size = System::Drawing::Size(583, 266);
			this->lv_->TabIndex = 0;
			this->lv_->UseCompatibleStateImageBehavior = false;
			this->lv_->View = System::Windows::Forms::View::Details;
			this->lv_->ColumnClick += gcnew System::Windows::Forms::ColumnClickEventHandler(this, &other_table_view_form::lv__ColumnClick);
			// 
			// columnHeader1
			// 
			this->columnHeader1->Text = L"Name";
			// 
			// columnHeader2
			// 
			this->columnHeader2->Text = L"VirtualAddress";
			this->columnHeader2->Width = 104;
			// 
			// columnHeader3
			// 
			this->columnHeader3->Text = L"VirtualSize";
			this->columnHeader3->Width = 86;
			// 
			// columnHeader4
			// 
			this->columnHeader4->Text = L"PointerToRawData";
			this->columnHeader4->Width = 117;
			// 
			// columnHeader5
			// 
			this->columnHeader5->Text = L"SizeOfRawData";
			this->columnHeader5->Width = 99;
			// 
			// columnHeader6
			// 
			this->columnHeader6->Text = L"Characteristics";
			this->columnHeader6->Width = 106;
			// 
			// tabPage4
			// 
			this->tabPage4->BackColor = System::Drawing::SystemColors::Control;
			this->tabPage4->Controls->Add(this->lv_data);
			this->tabPage4->Location = System::Drawing::Point(4, 21);
			this->tabPage4->Name = L"tabPage4";
			this->tabPage4->Padding = System::Windows::Forms::Padding(3);
			this->tabPage4->Size = System::Drawing::Size(589, 272);
			this->tabPage4->TabIndex = 3;
			this->tabPage4->Text = L"Data Directory";
			// 
			// lv_data
			// 
			this->lv_data->AllowColumnReorder = true;
			this->lv_data->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(4) {this->columnHeader7, this->columnHeader8, 
				this->columnHeader9, this->columnHeader10});
			this->lv_data->Dock = System::Windows::Forms::DockStyle::Fill;
			this->lv_data->FullRowSelect = true;
			this->lv_data->Location = System::Drawing::Point(3, 3);
			this->lv_data->MultiSelect = false;
			this->lv_data->Name = L"lv_data";
			this->lv_data->ShowItemToolTips = true;
			this->lv_data->Size = System::Drawing::Size(583, 266);
			this->lv_data->TabIndex = 1;
			this->lv_data->UseCompatibleStateImageBehavior = false;
			this->lv_data->View = System::Windows::Forms::View::Details;
			// 
			// columnHeader7
			// 
			this->columnHeader7->Text = L"Name";
			this->columnHeader7->Width = 197;
			// 
			// columnHeader8
			// 
			this->columnHeader8->Text = L"Relative Virtual Address";
			this->columnHeader8->Width = 167;
			// 
			// columnHeader9
			// 
			this->columnHeader9->Text = L"Virtual Address";
			this->columnHeader9->Width = 112;
			// 
			// columnHeader10
			// 
			this->columnHeader10->Text = L"Size";
			this->columnHeader10->Width = 90;
			// 
			// other_table_view_form
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(597, 297);
			this->Controls->Add(this->tc_);
			this->MaximizeBox = false;
			this->Name = L"other_table_view_form";
			this->ShowInTaskbar = false;
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = L"详细信息";
			this->Load += gcnew System::EventHandler(this, &other_table_view_form::other_table_view_form_Load);
			this->tc_->ResumeLayout(false);
			this->tabPage1->ResumeLayout(false);
			this->tabPage1->PerformLayout();
			this->tabPage2->ResumeLayout(false);
			this->tabPage2->PerformLayout();
			this->tabPage3->ResumeLayout(false);
			this->tabPage4->ResumeLayout(false);
			this->ResumeLayout(false);

		}
#pragma endregion

		 //ListViewItem排序所需，记录当前列
		 static int lv_item_curColumn = -1; 

		 //表示降序还是升序
		 static bool lv_item_bAsc = false; 

		 //
		 static String^ str_ = String::Empty;

		 //
		 static array<String^>^ strs_dataName = gcnew array<String^>{
			 "IMAGE_DIRECTORY_ENTRY_EXPORT",
				 "IMAGE_DIRECTORY_ENTRY_IMPORT",
				 "IMAGE_DIRECTORY_ENTRY_RESOURCE",
				 "IMAGE_DIRECTORY_ENTRY_EXCEPTION",
				 "IMAGE_DIRECTORY_ENTRY_SECURITY",
				 "IMAGE_DIRECTORY_ENTRY_BASERELOC",
				 "IMAGE_DIRECTORY_ENTRY_DEBUG",
				 "IMAGE_DIRECTORY_ENTRY_COPYRIGHT",
				 "IMAGE_DIRECTORY_ENTRY_GLOBALPTR",
				 "IMAGE_DIRECTORY_ENTRY_TLS",
				 "IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG",
				 "IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT",
				 "IMAGE_DIRECTORY_ENTRY_IAT",
				 "IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT",
				 "IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR"
		 };

		 //获得 Optional Header 信息
private: System::Void GetOptionalHeaderInfo(LPTSTR lpFilePath)
		 {
			 //打开文件
			 HANDLE hFile = CreateFile(lpFilePath,
				 GENERIC_WRITE | GENERIC_READ,
				 0,
				 NULL,
				 OPEN_EXISTING,
				 FILE_ATTRIBUTE_NORMAL,
				 NULL);

			 //打开文件失败
			 if(hFile == INVALID_HANDLE_VALUE)
			 {
				 //
				 return;
			 }

			 //获得文件大小
			 DWORD dwFileSize = GetFileSize(hFile , 0 );

			 //映射文件
			 HANDLE hMap = CreateFileMapping(hFile ,
				 0 ,
				 PAGE_READWRITE ,
				 0 ,
				 dwFileSize ,
				 0);

			 //文件映射内存失败
			 if(hMap == INVALID_HANDLE_VALUE)
			 {
				 CloseHandle(hFile);

				 //
				 return ;
			 }

			 //获得映射基址
			 LPBYTE lpBase = (LPBYTE)MapViewOfFile(hMap , FILE_MAP_READ | FILE_MAP_WRITE , 0 , 0 , dwFileSize);

			 //文件映射内存失败
			 if(lpBase == NULL)
			 {
				 CloseHandle(hFile);
				 CloseHandle(hMap);
				 UnmapViewOfFile(lpBase);

				 //
				 return ;
			 }

			 //
			 PIMAGE_DOS_HEADER pImage_dos_header = (PIMAGE_DOS_HEADER)lpBase;

			 //
			 PIMAGE_NT_HEADERS pImage_nt_header = (PIMAGE_NT_HEADERS)((DWORD)lpBase + pImage_dos_header->e_lfanew);

			 //
			 PIMAGE_OPTIONAL_HEADER pImage_optional_header = (PIMAGE_OPTIONAL_HEADER32)(&pImage_nt_header->OptionalHeader);

			 txtOEP->Text = "0x" + (pImage_optional_header->AddressOfEntryPoint).ToString("X8");
			 txtImageBase->Text = "0x" + (pImage_optional_header->ImageBase).ToString("X8");
			 txtCodeBase->Text = "0x" + (pImage_optional_header->BaseOfCode).ToString("X8");
			 txtDataBase->Text = "0x" + (pImage_optional_header->BaseOfData).ToString("X8");
			 txtImageSize->Text = "0x" + (pImage_optional_header->SizeOfImage).ToString("X8");
			 txtHeaderSize->Text = "0x" + (pImage_optional_header->SizeOfHeaders).ToString("X8");
			 txtSecAlign->Text = "0x" + (pImage_optional_header->SectionAlignment).ToString("X8");
			 txtFileAlign->Text = "0x" + (pImage_optional_header->FileAlignment).ToString("X8");
			 txtSubSystem->Text = "0x" + (pImage_optional_header->Subsystem).ToString("X8");
			 txtCheckSum->Text = "0x" + (pImage_optional_header->CheckSum).ToString("X4");
			 txtDllFlags->Text = "0x" + (pImage_optional_header->DllCharacteristics).ToString("X4");

			 CloseHandle(hFile);
			 CloseHandle(hMap);
			 UnmapViewOfFile(lpBase);
		 }

		 //获得 File Header 信息
private: System::Void GetFileHeaderInfo(LPTSTR lpFilePath)
		 {
			 //打开文件
			 HANDLE hFile = CreateFile(lpFilePath,
				 GENERIC_WRITE | GENERIC_READ,
				 0,
				 NULL,
				 OPEN_EXISTING,
				 FILE_ATTRIBUTE_NORMAL,
				 NULL);

			 //打开文件失败
			 if(hFile == INVALID_HANDLE_VALUE)
			 {
				 //
				 return;
			 }

			 //获得文件大小
			 DWORD dwFileSize = GetFileSize(hFile , 0 );

			 //映射文件
			 HANDLE hMap = CreateFileMapping(hFile ,
				 0 ,
				 PAGE_READWRITE ,
				 0 ,
				 dwFileSize ,
				 0);

			 //文件映射内存失败
			 if(hMap == INVALID_HANDLE_VALUE)
			 {
				 CloseHandle(hFile);

				 //
				 return ;
			 }

			 //获得映射基址
			 LPBYTE lpBase = (LPBYTE)MapViewOfFile(hMap , FILE_MAP_READ | FILE_MAP_WRITE , 0 , 0 , dwFileSize);

			 //文件映射内存失败
			 if(lpBase == NULL)
			 {
				 CloseHandle(hFile);
				 CloseHandle(hMap);
				 UnmapViewOfFile(lpBase);

				 //
				 return ;
			 }

			 //
			 PIMAGE_DOS_HEADER pImage_dos_header = (PIMAGE_DOS_HEADER)lpBase;

			 //
			 PIMAGE_NT_HEADERS pImage_nt_header = (PIMAGE_NT_HEADERS)((DWORD)lpBase + pImage_dos_header->e_lfanew);

			 //
			 PIMAGE_FILE_HEADER pImage_file_header = (PIMAGE_FILE_HEADER)(&pImage_nt_header->FileHeader);

			 //
			 txtMachine->Text = "0x" + (pImage_file_header->Machine).ToString("X4");
			 txtNumOfSec->Text = "0x" + (pImage_file_header->NumberOfSections).ToString("X4");
			 txtTimeDateStamp->Text = "0x" + (pImage_file_header->TimeDateStamp).ToString("X8");
			 txtPointerToST->Text = "0x" + (pImage_file_header->PointerToSymbolTable).ToString("X8");
			 txtNumOfSymbols->Text = "0x" + (pImage_file_header->NumberOfSymbols).ToString("X8");
			 txtSizeOfOH->Text = "0x" + (pImage_file_header->SizeOfOptionalHeader).ToString("X4");
			 txtCharac->Text = "0x" + (pImage_file_header->Characteristics).ToString("X4");

			 CloseHandle(hFile);
			 CloseHandle(hMap);
			 UnmapViewOfFile(lpBase);
		 }

		 //获得 section table 信息
private: System::Void GetSectionTableInfo(LPTSTR lpFilePath)
		 {
			 //打开文件
			 HANDLE hFile = CreateFile(lpFilePath,
				 GENERIC_WRITE | GENERIC_READ,
				 0,
				 NULL,
				 OPEN_EXISTING,
				 FILE_ATTRIBUTE_NORMAL,
				 NULL);

			 //打开文件失败
			 if(hFile == INVALID_HANDLE_VALUE)
			 {
				 //
				 return;
			 }

			 //获得文件大小
			 DWORD dwFileSize = GetFileSize(hFile , 0 );

			 //映射文件
			 HANDLE hMap = CreateFileMapping(hFile ,
				 0 ,
				 PAGE_READWRITE ,
				 0 ,
				 dwFileSize ,
				 0);

			 //文件映射内存失败
			 if(hMap == INVALID_HANDLE_VALUE)
			 {
				 CloseHandle(hFile);

				 //
				 return ;
			 }

			 //获得映射基址
			 LPBYTE lpBase = (LPBYTE)MapViewOfFile(hMap , FILE_MAP_READ | FILE_MAP_WRITE , 0 , 0 , dwFileSize);

			 //文件映射内存失败
			 if(lpBase == NULL)
			 {
				 CloseHandle(hFile);
				 CloseHandle(hMap);
				 UnmapViewOfFile(lpBase);

				 //
				 return ;
			 }

			 //
			 PIMAGE_DOS_HEADER pImage_dos_header = (PIMAGE_DOS_HEADER)lpBase;

			 //
			 PIMAGE_NT_HEADERS pImage_nt_header = (PIMAGE_NT_HEADERS)((DWORD)lpBase + pImage_dos_header->e_lfanew);

			 //
			 PIMAGE_SECTION_HEADER pImage_section_header = IMAGE_FIRST_SECTION(pImage_nt_header);

			 //
			 for(int i = 0;i < pImage_nt_header->FileHeader.NumberOfSections; i ++)
			 {
				 //
				 ListViewItem^ item_ = gcnew ListViewItem(gcnew String((LPSTR)pImage_section_header->Name));

				 //
				 item_->SubItems->AddRange(gcnew array<String^>{			    
					 "0x" + (pImage_section_header->VirtualAddress).ToString("X8"),					 
					 "0x" + (pImage_section_header->Misc.VirtualSize).ToString("X8"),
					 "0x" + (pImage_section_header->PointerToRawData).ToString("X8"),
					 "0x" + (pImage_section_header->SizeOfRawData).ToString("X8"),
					 "0x" + (pImage_section_header->Characteristics).ToString("X8")
				 });

				 //
				 lv_->Items->Add(item_);

				 //
				 pImage_section_header ++;
			 }

			 CloseHandle(hFile);
			 CloseHandle(hMap);
			 UnmapViewOfFile(lpBase);
		 }

		 /*
		 #define IMAGE_DIRECTORY_ENTRY_EXPORT          0   // Export Directory
		 #define IMAGE_DIRECTORY_ENTRY_IMPORT          1   // Import Directory
		 #define IMAGE_DIRECTORY_ENTRY_RESOURCE        2   // Resource Directory
		 #define IMAGE_DIRECTORY_ENTRY_EXCEPTION       3   // Exception Directory
		 #define IMAGE_DIRECTORY_ENTRY_SECURITY        4   // Security Directory
		 #define IMAGE_DIRECTORY_ENTRY_BASERELOC       5   // Base Relocation Table
		 #define IMAGE_DIRECTORY_ENTRY_DEBUG           6   // Debug Directory
		 //      IMAGE_DIRECTORY_ENTRY_COPYRIGHT       7   // (X86 usage)
		 #define IMAGE_DIRECTORY_ENTRY_ARCHITECTURE    7   // Architecture Specific Data
		 #define IMAGE_DIRECTORY_ENTRY_GLOBALPTR       8   // RVA of GP
		 #define IMAGE_DIRECTORY_ENTRY_TLS             9   // TLS Directory
		 #define IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG    10   // Load Configuration Directory
		 #define IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT   11   // Bound Import Directory in headers
		 #define IMAGE_DIRECTORY_ENTRY_IAT            12   // Import Address Table
		 #define IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT   13   // Delay Load Import Descriptors
		 #define IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR 14   // COM Runtime descriptor
		 */
		 
		 //获得 Data Directory 信息
private: System::Void GetDataDirectoryInfo(LPTSTR lpFilePath)
		 {
			 //打开文件
			 HANDLE hFile = CreateFile(lpFilePath,
				 GENERIC_WRITE | GENERIC_READ,
				 0,
				 NULL,
				 OPEN_EXISTING,
				 FILE_ATTRIBUTE_NORMAL,
				 NULL);

			 //打开文件失败
			 if(hFile == INVALID_HANDLE_VALUE)
			 {
				 //
				 return;
			 }

			 //获得文件大小
			 DWORD dwFileSize = GetFileSize(hFile , 0 );

			 //映射文件
			 HANDLE hMap = CreateFileMapping(hFile ,
				 0 ,
				 PAGE_READWRITE ,
				 0 ,
				 dwFileSize ,
				 0);

			 //文件映射内存失败
			 if(hMap == INVALID_HANDLE_VALUE)
			 {
				 CloseHandle(hFile);

				 //
				 return ;
			 }

			 //获得映射基址
			 LPBYTE lpBase = (LPBYTE)MapViewOfFile(hMap , FILE_MAP_READ | FILE_MAP_WRITE , 0 , 0 , dwFileSize);

			 //文件映射内存失败
			 if(lpBase == NULL)
			 {
				 CloseHandle(hFile);
				 CloseHandle(hMap);
				 UnmapViewOfFile(lpBase);

				 //
				 return ;
			 }

			 //
			 PIMAGE_DOS_HEADER pImage_dos_header = (PIMAGE_DOS_HEADER)lpBase;

			 //
			 PIMAGE_NT_HEADERS pImage_nt_header = (PIMAGE_NT_HEADERS)((DWORD)lpBase + pImage_dos_header->e_lfanew);

			 //
			 PIMAGE_OPTIONAL_HEADER pImage_optional_header = (PIMAGE_OPTIONAL_HEADER)(&pImage_nt_header->OptionalHeader);

			 //
			 for (int i = 0;i < 15;i ++)
			 {
				 //
				 ListViewItem^ item_ = gcnew ListViewItem(strs_dataName[i]);

				 //
				 item_->SubItems->AddRange(gcnew array<String^>{
					 "0x" + pImage_optional_header->DataDirectory[i].VirtualAddress.ToString("X8"),
					 "0x" + ((DWORD)ImageRvaToVa(pImage_nt_header,lpBase,pImage_nt_header->OptionalHeader.DataDirectory[i].VirtualAddress,NULL)).ToString("X8"),
					 "0x" + pImage_optional_header->DataDirectory[i].Size.ToString("X8"),
				 });

				 //
				 lv_data->Items->Add(item_);
			 }

			 

			 CloseHandle(hFile);
			 CloseHandle(hMap);
			 UnmapViewOfFile(lpBase);
		 }

		 //
private: System::Void other_table_view_form_Load(System::Object^  sender, System::EventArgs^  e) {
				 
			 //
			 GetOptionalHeaderInfo(common_net::common_stringToHGlobalUni(str_));
		 }

		 //
private: System::Void tc__SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
		
			 //
			 if (tc_->SelectedIndex == 0)
			 {
				 GetOptionalHeaderInfo(common_net::common_stringToHGlobalUni(str_));
			 }
			 else if (tc_->SelectedIndex == 1)
			 {
				 GetFileHeaderInfo(common_net::common_stringToHGlobalUni(str_));
			 }
			 else if (tc_->SelectedIndex == 2)
			 {
				 if (lv_->Items->Count > 0)
				 {
					 lv_->Items->Clear();
				 }

				 //
				 GetSectionTableInfo(common_net::common_stringToHGlobalUni(str_));
			 }
			 else
			 {
				 if (lv_data->Items->Count > 0)
				 {
					 lv_data->Items->Clear();
				 }

				 //
				 GetDataDirectoryInfo(common_net::common_stringToHGlobalUni(str_));
			 }
		 }

		 //sort
private: System::Void lv__ColumnClick(System::Object^  sender, System::Windows::Forms::ColumnClickEventArgs^  e) {
			 if(e->Column == lv_item_curColumn)
			 {		 
				 if (lv_item_bAsc)
				 {
					 lv_item_bAsc = false;
				 }
				 else
				 {
					 lv_item_bAsc = true;
				 }

				 if(e->Column == 0)
				 {
					 //字符串排序
					 lv_->ListViewItemSorter = gcnew ListViewItemComparer(e->Column,lv_item_bAsc,1,0);
				 }
				 else
				 {
					 //16进制
					 lv_->ListViewItemSorter = gcnew ListViewItemComparer(e->Column,lv_item_bAsc,0,16);
				 }
			 }
			 else
			 {		
				 lv_item_bAsc = false;

				 if(e->Column == 0)
				 {
					 //字符串排序
					 lv_->ListViewItemSorter = gcnew ListViewItemComparer(e->Column,lv_item_bAsc,1,0);
				 }
				 else
				 {
					 //16进制
					 lv_->ListViewItemSorter = gcnew ListViewItemComparer(e->Column,lv_item_bAsc,0,16);
				 }

				 //记录当前
				 lv_item_curColumn = e->Column;
			 }
		 }
};
}
