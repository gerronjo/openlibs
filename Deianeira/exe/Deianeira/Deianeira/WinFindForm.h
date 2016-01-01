#pragma once
#include "..\common\common.h"

namespace Deianeira {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// WinFindForm 摘要
	/// </summary>
	public ref class WinFindForm : public System::Windows::Forms::Form
	{
	public:
		WinFindForm(Form^ f,ImageList^ il_)
		{
			InitializeComponent();
			//
			//TODO: 在此处添加构造函数代码
			//
			this->f = f;
			this->imageList2 = il_;
		}

	protected:
		/// <summary>
		/// 清理所有正在使用的资源。
		/// </summary>
		~WinFindForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::GroupBox^  groupBox1;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Button^  btnOK;
	private: System::Windows::Forms::Button^  btnQuit;


	private: System::Windows::Forms::Label^  label7;
	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::TextBox^  txt_handle;

	private: System::Windows::Forms::Label^  lbl_rect;

	private: System::Windows::Forms::Label^  lbl_style;

	private: System::Windows::Forms::Label^  lbl_className;

	private: System::Windows::Forms::Label^  lbl_name;
	private: System::Windows::Forms::CheckBox^  chk_hide;
	private: System::Windows::Forms::PictureBox^  pic_;


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
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->pic_ = (gcnew System::Windows::Forms::PictureBox());
			this->chk_hide = (gcnew System::Windows::Forms::CheckBox());
			this->lbl_rect = (gcnew System::Windows::Forms::Label());
			this->lbl_style = (gcnew System::Windows::Forms::Label());
			this->lbl_className = (gcnew System::Windows::Forms::Label());
			this->lbl_name = (gcnew System::Windows::Forms::Label());
			this->txt_handle = (gcnew System::Windows::Forms::TextBox());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->btnOK = (gcnew System::Windows::Forms::Button());
			this->btnQuit = (gcnew System::Windows::Forms::Button());
			this->groupBox1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pic_))->BeginInit();
			this->SuspendLayout();
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->pic_);
			this->groupBox1->Controls->Add(this->chk_hide);
			this->groupBox1->Controls->Add(this->lbl_rect);
			this->groupBox1->Controls->Add(this->lbl_style);
			this->groupBox1->Controls->Add(this->lbl_className);
			this->groupBox1->Controls->Add(this->lbl_name);
			this->groupBox1->Controls->Add(this->txt_handle);
			this->groupBox1->Controls->Add(this->label7);
			this->groupBox1->Controls->Add(this->label6);
			this->groupBox1->Controls->Add(this->label5);
			this->groupBox1->Controls->Add(this->label4);
			this->groupBox1->Controls->Add(this->label3);
			this->groupBox1->Controls->Add(this->label2);
			this->groupBox1->Controls->Add(this->label1);
			this->groupBox1->Location = System::Drawing::Point(4, 12);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(247, 232);
			this->groupBox1->TabIndex = 0;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"窗口查找程序";
			// 
			// pic_
			// 
			this->pic_->Location = System::Drawing::Point(95, 69);
			this->pic_->Name = L"pic_";
			this->pic_->Size = System::Drawing::Size(32, 32);
			this->pic_->TabIndex = 5;
			this->pic_->TabStop = false;
			this->pic_->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &WinFindForm::pic__MouseDown);
			this->pic_->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &WinFindForm::pic__MouseMove);
			this->pic_->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &WinFindForm::pic__MouseUp);
			// 
			// chk_hide
			// 
			this->chk_hide->AutoSize = true;
			this->chk_hide->Location = System::Drawing::Point(133, 77);
			this->chk_hide->Name = L"chk_hide";
			this->chk_hide->Size = System::Drawing::Size(108, 16);
			this->chk_hide->TabIndex = 4;
			this->chk_hide->Text = L"隐藏 Deianeira";
			this->chk_hide->UseVisualStyleBackColor = true;
			this->chk_hide->CheckedChanged += gcnew System::EventHandler(this, &WinFindForm::chk_hide_CheckedChanged);
			// 
			// lbl_rect
			// 
			this->lbl_rect->AutoSize = true;
			this->lbl_rect->Location = System::Drawing::Point(63, 205);
			this->lbl_rect->Name = L"lbl_rect";
			this->lbl_rect->Size = System::Drawing::Size(0, 12);
			this->lbl_rect->TabIndex = 3;
			// 
			// lbl_style
			// 
			this->lbl_style->AutoSize = true;
			this->lbl_style->Location = System::Drawing::Point(63, 183);
			this->lbl_style->Name = L"lbl_style";
			this->lbl_style->Size = System::Drawing::Size(0, 12);
			this->lbl_style->TabIndex = 3;
			// 
			// lbl_className
			// 
			this->lbl_className->AutoSize = true;
			this->lbl_className->Location = System::Drawing::Point(63, 161);
			this->lbl_className->Name = L"lbl_className";
			this->lbl_className->Size = System::Drawing::Size(0, 12);
			this->lbl_className->TabIndex = 3;
			// 
			// lbl_name
			// 
			this->lbl_name->AutoSize = true;
			this->lbl_name->Location = System::Drawing::Point(63, 139);
			this->lbl_name->Name = L"lbl_name";
			this->lbl_name->Size = System::Drawing::Size(0, 12);
			this->lbl_name->TabIndex = 3;
			// 
			// txt_handle
			// 
			this->txt_handle->Location = System::Drawing::Point(65, 113);
			this->txt_handle->Name = L"txt_handle";
			this->txt_handle->Size = System::Drawing::Size(100, 21);
			this->txt_handle->TabIndex = 2;
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(6, 205);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(35, 12);
			this->label7->TabIndex = 1;
			this->label7->Text = L"矩形:";
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(6, 183);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(35, 12);
			this->label6->TabIndex = 1;
			this->label6->Text = L"样式:";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(6, 161);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(23, 12);
			this->label5->TabIndex = 1;
			this->label5->Text = L"类:";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(6, 139);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(35, 12);
			this->label4->TabIndex = 1;
			this->label4->Text = L"标题:";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(6, 117);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(53, 12);
			this->label3->TabIndex = 1;
			this->label3->Text = L"句柄(A):";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(7, 79);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(83, 12);
			this->label2->TabIndex = 1;
			this->label2->Text = L"查找程序工具:";
			// 
			// label1
			// 
			this->label1->Location = System::Drawing::Point(8, 17);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(233, 49);
			this->label1->TabIndex = 0;
			this->label1->Text = L"将“查找程序工具”拖至窗口上以选择该窗口，然后释放鼠标按钮。或者输入窗口句柄(以十六进制的格式)。";
			// 
			// btnOK
			// 
			this->btnOK->Location = System::Drawing::Point(257, 24);
			this->btnOK->Name = L"btnOK";
			this->btnOK->Size = System::Drawing::Size(59, 23);
			this->btnOK->TabIndex = 1;
			this->btnOK->Text = L"确定";
			this->btnOK->UseVisualStyleBackColor = true;
			this->btnOK->Click += gcnew System::EventHandler(this, &WinFindForm::btnOK_Click);
			// 
			// btnQuit
			// 
			this->btnQuit->Location = System::Drawing::Point(257, 55);
			this->btnQuit->Name = L"btnQuit";
			this->btnQuit->Size = System::Drawing::Size(59, 23);
			this->btnQuit->TabIndex = 2;
			this->btnQuit->Text = L"取消";
			this->btnQuit->UseVisualStyleBackColor = true;
			this->btnQuit->Click += gcnew System::EventHandler(this, &WinFindForm::btnQuit_Click);
			// 
			// WinFindForm
			// 
			this->AcceptButton = this->btnOK;
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(328, 256);
			this->Controls->Add(this->btnQuit);
			this->Controls->Add(this->btnOK);
			this->Controls->Add(this->groupBox1);
			this->MaximizeBox = false;
			this->Name = L"WinFindForm";
			this->ShowInTaskbar = false;
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = L"查找窗口";
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &WinFindForm::WinFindForm_FormClosing);
			this->Load += gcnew System::EventHandler(this, &WinFindForm::WinFindForm_Load);
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pic_))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion

	     //
		 static bool isMouseDowned = false;
		 static Form^ f = nullptr;
		 static ImageList^ imageList2 = nullptr;

		 //
		 static HWND hwnd = NULL;
		 static HWND g_hwnd = NULL;
		 static HDC hdc = NULL;
		 static int oldRop2 = 0;
		 static HPEN hpen = NULL;
		 static HGDIOBJ oldPen = NULL;

		 //
private: System::Void WinFindForm_Load(System::Object^  sender, System::EventArgs^  e) {
			 //
			 pic_->Image = imageList2->Images[0];
			 //
			 hdc = GetWindowDC(GetDesktopWindow());
		 }

	     //
private: System::Void chk_hide_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 //
			if (chk_hide->Checked)
			{
				f->Hide();
			}
			else
			{
				f->Show();
			}
		 }

		 //
private: System::Void btnOK_Click(System::Object^  sender, System::EventArgs^  e) {
			 this->Close();
		 }

		 //
private: System::Void btnQuit_Click(System::Object^  sender, System::EventArgs^  e) {
			 //
			 this->Close();
		 }

		 //
private: System::Void pic__MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 //
			 isMouseDowned = true;
			 //
			 pic_->Image = imageList2->Images[1];
			 //
			 this->Cursor = gcnew System::Windows::Forms::Cursor(System::Reflection::Assembly::GetExecutingAssembly()->GetManifestResourceStream ("win.cur"));
		 }

		 //
private: System::Void pic__MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 //
			 isMouseDowned = false;
			 //
			 pic_->Image = imageList2->Images[0];
			 //
			 this->Cursor = Cursors::Default;

			 //处理鼠标弹上的话的情况
			 //原理和下面一样，要消除掉画出来的矩形线条就必须在重新画一次
			 RECT rect;
			 //
			 GetWindowRect(g_hwnd, &rect);
			 //
			 ::Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
			 //
			 SetROP2(hdc, oldRop2);
			 //
			 SelectObject(hdc, oldPen);
			 //
			 DeleteObject(hpen);

			 //
			 g_hwnd = NULL;
		 }

		 //
private: System::Void pic__MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 //防止鼠标在图片框上面移动的时候得出结果
			 if (!isMouseDowned)
			 {
				 return;
			 }

			 //
			 TCHAR tcText[MAX_PATH] = { 0 };
			 POINT p;
			 RECT rect;
	 
			 //获得鼠标位置
			 GetCursorPos(&p);
			 //句柄
			 hwnd = WindowFromPoint(p);

			 //如果上一次保存的句柄与当前得到的句柄相等的话，就说明鼠标没有移动到新的窗体
			 if (g_hwnd == hwnd)
			 {
				 //直接返回
				 return;
			 }

			 //第一次的时候，g_hwnd不会等于hwnd，因为为NULL，所以要排除掉
			 if (g_hwnd != NULL && g_hwnd != hwnd)
			 {
				 //满足上面的条件的话，说明在新的窗体来了，这里重新获得上一次窗体的rect
				 GetWindowRect(g_hwnd, &rect);
				 //因为hdc没有变化，而上一次画了矩形，这次在画的话，由于异或的关系，先前的矩形线条就会被清空
				 ::Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
				 //通知hdc
				 SetROP2(hdc, oldRop2);
				 //
				 SelectObject(hdc, oldPen);
				 //
				 DeleteObject(hpen);	
			 }
	 
			 //赋值保存
			 g_hwnd = hwnd;

			 //
			 oldRop2 = SetROP2(hdc, R2_NOTXORPEN);
			 //
			 GetWindowRect(hwnd, &rect);
			 //创建画笔
			 hpen = CreatePen(PS_INSIDEFRAME, 4, RGB(0,0,255));
			 //
			 oldPen = SelectObject(hdc, hpen);
			 //画矩形
			 ::Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);	 
			  
			 //
			 txt_handle->Text = "0x" + ((ULONG)hwnd).ToString("X8");
			 //
			 GetWindowText(hwnd,tcText,MAX_PATH);
			 //
			 lbl_name->Text = gcnew String(tcText);
			 //
			 ZeroMemory(tcText,0);
			 //
			 GetClassName(hwnd,tcText,MAX_PATH);
			 //
			 lbl_className->Text = gcnew String(tcText);
			 //
			 //窗口样式
			 LONG ulStyle = GetWindowLong(hwnd, GWL_STYLE);

			 lbl_style->Text = "0x" + ulStyle.ToString("X8");
		 }

		 //
private: System::Void WinFindForm_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e) {
			 if (chk_hide->Checked)
			 {
				 f->Show();
			 }

			 //释放hdc
			 ReleaseDC(GetDesktopWindow(), hdc);
		 }
};
}
