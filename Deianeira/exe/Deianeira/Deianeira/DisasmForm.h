#pragma once
#include "..\common\common_net.h"
#include "OD_DASM/disasm.h"


namespace Deianeira {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// DisasmForm 摘要
	/// </summary>
	public ref class DisasmForm : public System::Windows::Forms::Form
	{
	public:
		DisasmForm(String^ str_addr,HANDLE hDriver)
		{
			InitializeComponent();
			//
			//TODO: 在此处添加构造函数代码
			//
			txt_addr->Text = str_addr->Replace("0x","");
			this->hDriver = hDriver;
		}

	protected:
		/// <summary>
		/// 清理所有正在使用的资源。
		/// </summary>
		~DisasmForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  btnDisasm;
	protected: 
	private: System::Windows::Forms::ListView^  lv_;
	private: System::Windows::Forms::ColumnHeader^  columnHeader2;
	private: System::Windows::Forms::ColumnHeader^  columnHeader3;
	private: System::Windows::Forms::ColumnHeader^  columnHeader1;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::TextBox^  txt_addr;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::TextBox^  txt_size;
	private: System::Windows::Forms::ContextMenuStrip^  cms;
	private: System::Windows::Forms::ToolStripMenuItem^  cms_refresh;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator1;
	private: System::Windows::Forms::ToolStripMenuItem^  cms_export;
	private: System::ComponentModel::IContainer^  components;

	private:
		/// <summary>
		/// 必需的设计器变量。
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// 设计器支持所需的方法 - 不要
		/// 使用代码编辑器修改此方法的内容。
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->btnDisasm = (gcnew System::Windows::Forms::Button());
			this->lv_ = (gcnew System::Windows::Forms::ListView());
			this->columnHeader2 = (gcnew System::Windows::Forms::ColumnHeader());
			this->columnHeader3 = (gcnew System::Windows::Forms::ColumnHeader());
			this->columnHeader1 = (gcnew System::Windows::Forms::ColumnHeader());
			this->cms = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->cms_refresh = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator1 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->cms_export = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->txt_addr = (gcnew System::Windows::Forms::TextBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->txt_size = (gcnew System::Windows::Forms::TextBox());
			this->cms->SuspendLayout();
			this->SuspendLayout();
			// 
			// btnDisasm
			// 
			this->btnDisasm->Location = System::Drawing::Point(428, 15);
			this->btnDisasm->Name = L"btnDisasm";
			this->btnDisasm->Size = System::Drawing::Size(75, 23);
			this->btnDisasm->TabIndex = 2;
			this->btnDisasm->Text = L"反汇编";
			this->btnDisasm->UseVisualStyleBackColor = true;
			this->btnDisasm->Click += gcnew System::EventHandler(this, &DisasmForm::btnDisasm_Click);
			// 
			// lv_
			// 
			this->lv_->AllowColumnReorder = true;
			this->lv_->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(3) {this->columnHeader2, this->columnHeader3, 
				this->columnHeader1});
			this->lv_->ContextMenuStrip = this->cms;
			this->lv_->FullRowSelect = true;
			this->lv_->Location = System::Drawing::Point(12, 55);
			this->lv_->MultiSelect = false;
			this->lv_->Name = L"lv_";
			this->lv_->ShowItemToolTips = true;
			this->lv_->Size = System::Drawing::Size(557, 264);
			this->lv_->TabIndex = 3;
			this->lv_->UseCompatibleStateImageBehavior = false;
			this->lv_->View = System::Windows::Forms::View::Details;
			// 
			// columnHeader2
			// 
			this->columnHeader2->Text = L"地址";
			this->columnHeader2->Width = 114;
			// 
			// columnHeader3
			// 
			this->columnHeader3->Text = L"二进制";
			this->columnHeader3->Width = 155;
			// 
			// columnHeader1
			// 
			this->columnHeader1->Text = L"汇编";
			this->columnHeader1->Width = 263;
			// 
			// cms
			// 
			this->cms->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {this->cms_refresh, this->toolStripSeparator1, 
				this->cms_export});
			this->cms->Name = L"cms";
			this->cms->Size = System::Drawing::Size(95, 54);
			// 
			// cms_refresh
			// 
			this->cms_refresh->Name = L"cms_refresh";
			this->cms_refresh->Size = System::Drawing::Size(152, 22);
			this->cms_refresh->Text = L"刷新";
			this->cms_refresh->Click += gcnew System::EventHandler(this, &DisasmForm::cms_refresh_Click);
			// 
			// toolStripSeparator1
			// 
			this->toolStripSeparator1->Name = L"toolStripSeparator1";
			this->toolStripSeparator1->Size = System::Drawing::Size(149, 6);
			// 
			// cms_export
			// 
			this->cms_export->Name = L"cms_export";
			this->cms_export->Size = System::Drawing::Size(152, 22);
			this->cms_export->Text = L"导出";
			this->cms_export->Click += gcnew System::EventHandler(this, &DisasmForm::cms_export_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(31, 20);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(41, 12);
			this->label1->TabIndex = 2;
			this->label1->Text = L"地址：";
			// 
			// txt_addr
			// 
			this->txt_addr->Location = System::Drawing::Point(78, 16);
			this->txt_addr->Name = L"txt_addr";
			this->txt_addr->Size = System::Drawing::Size(100, 21);
			this->txt_addr->TabIndex = 0;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(208, 20);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(77, 12);
			this->label2->TabIndex = 2;
			this->label2->Text = L"大小(字节)：";
			// 
			// txt_size
			// 
			this->txt_size->Location = System::Drawing::Point(291, 16);
			this->txt_size->Name = L"txt_size";
			this->txt_size->Size = System::Drawing::Size(100, 21);
			this->txt_size->TabIndex = 1;
			// 
			// DisasmForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(581, 331);
			this->Controls->Add(this->txt_size);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->txt_addr);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->lv_);
			this->Controls->Add(this->btnDisasm);
			this->MaximizeBox = false;
			this->Name = L"DisasmForm";
			this->ShowInTaskbar = false;
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = L"反汇编器";
			this->Load += gcnew System::EventHandler(this, &DisasmForm::DisasmForm_Load);
			this->cms->ResumeLayout(false);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

		 HANDLE hDriver;

		 //ulAddr:地址
		 //ulSize:大小
private: System::Void lv_show(ULONG ulAddr,ULONG ulSize){
			 BOOL bRet = FALSE;
			 DWORD dwRet=0;
			 UINT32 i;		 
			 t_disasm da;
			 memset(&da,0,sizeof(da));

			 DISASM_CLIENT_ID clientID;
			 clientID.ulStartAddress = ulAddr;
			 clientID.ulSize = ulSize;

			 PBYTE pBuffer = new BYTE[ulSize];
			 bRet = DeviceIoControl(hDriver,IOCTL_DISASM,&clientID,sizeof(DISASM_CLIENT_ID),pBuffer,ulSize,&dwRet,NULL);

			 if (!bRet)
			 {
				 MessageBox::Show("反汇编失败！");
				 return;
			 }

			 //发送控制代码获得内核内存拷贝
			 //循环
			 for ( i = 0;i <ulSize;)
			 {
				 //获得反编译了多少个字节数
				 int count=Disasm(&pBuffer[i],10,NULL,&da,DISASM_CODE);
				 //添加数据
				 ListViewItem^ item = gcnew ListViewItem("0x"+(i+ulAddr).ToString("X8"));
				 item->SubItems->AddRange(gcnew array<String^>{gcnew String(da.dump),gcnew String(da.result)});
				 lv_->Items->Add(item);
				 //迭代一下
			     i=i+count;
			 }

			 //释放内存
			 free(pBuffer);
		 }		 

		 //加载事件
private: System::Void DisasmForm_Load(System::Object^  sender, System::EventArgs^  e) {
			 txt_size->Text = "000000C8";
			 lv_show(Convert::ToUInt32(txt_addr->Text,16),Convert::ToUInt32(txt_size->Text,16));
		 }

		 //反汇编
private: System::Void btnDisasm_Click(System::Object^  sender, System::EventArgs^  e) {
			 if (String::IsNullOrEmpty(txt_addr->Text))
			 {
				 return;
			 }
			 if (String::IsNullOrEmpty(txt_size->Text))
			 {
				 return;
			 }

			 lv_->Items->Clear();
			 //刷新
			 lv_show(Convert::ToUInt32(txt_addr->Text,16),Convert::ToUInt32(txt_size->Text,16));
		 }

		 //刷新
private: System::Void cms_refresh_Click(System::Object^  sender, System::EventArgs^  e) {
			 btnDisasm_Click(sender,e);			 
		 }

		 //导出
private: System::Void cms_export_Click(System::Object^  sender, System::EventArgs^  e) {
			 common_net::common_getListViewData(lv_);
		 }
};
}
