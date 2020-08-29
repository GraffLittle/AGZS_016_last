#pragma once
#include "MainForm.h"

namespace AGZS {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	ref class TInfoForm : public System::Windows::Forms::Form {
	public: TInfoForm( MainForm^ M_Form,
		String^ user) {
		InitializeComponent();
		MForm = M_Form;
		Text = user;
	}
	void InitializeComponent(void) {
		this->MainPanel = (gcnew System::Windows::Forms::Panel());
		this->ExcelBtn = (gcnew System::Windows::Forms::Button());
		this->InfoTextBox = gcnew System::Windows::Forms::RichTextBox();
		this->MainPanel->SuspendLayout();
		this->SuspendLayout();
		// MainPanel
		this->MainPanel->Controls->Add(this->ExcelBtn);
		this->MainPanel->Dock = System::Windows::Forms::DockStyle::Bottom;
		this->MainPanel->Location = System::Drawing::Point(0, 216);
		this->MainPanel->Name = L"MainPanel";
		this->MainPanel->Size = System::Drawing::Size(51, 51);
		this->MainPanel->TabIndex = 0;
		// ExcelBtn
		this->ExcelBtn->Location = System::Drawing::Point(10, 11);
		this->ExcelBtn->Name = L"ExcelBtn";
		this->ExcelBtn->Size = System::Drawing::Size(158, 23);
		this->ExcelBtn->TabIndex = 0;
		this->ExcelBtn->Text = L"Сохранить заправки в Excel";
		this->ExcelBtn->UseVisualStyleBackColor = true;
		this->ExcelBtn->Click += gcnew System::EventHandler(this, &TInfoForm::ExcelBtn_Click);
		// InfoTextBox
		this->InfoTextBox->Dock = System::Windows::Forms::DockStyle::Fill;
		this->InfoTextBox->Multiline = true;
		this->InfoTextBox->AutoSize = false; // true;
		this->InfoTextBox->Location = System::Drawing::Point(0, 0);
		this->InfoTextBox->Name = L"InfoTextBox";
		this->InfoTextBox->Size = System::Drawing::Size(286, 216);
		this->InfoTextBox->TabIndex = 1;
		InfoTextBox->Text = L"Info:";
		// TInfoForm
		this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
		this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
		this->ClientSize = System::Drawing::Size(286, 264);
		this->Controls->Add(InfoTextBox);
		this->Controls->Add(this->MainPanel);
		this->Text = L"Info";
		this->Location = System::Drawing::Point(1, 1);
		this->Size = System::Drawing::Size(700, 500);
		this->MainPanel->ResumeLayout(false);
		this->ResumeLayout(false);
	}
	protected: MainForm^ MForm;
	private: System::Windows::Forms::Panel^ MainPanel;
	private: System::Windows::Forms::Button^ ExcelBtn;
	private: System::Void ExcelBtn_Click(System::Object^ sender, System::EventArgs^ e) {
		// Default saving in C:\Users\Admin\Documents
		String^ path = Application::StartupPath; // C:\Works\efremov.av_shuya\AGZS\...\Debug
		String^ dtstr = String::Format("{0:D4}{1:D2}{2:D2}_{3:D2}{4:D2}{5:D2}",
			DateTime::Now.Year, DateTime::Now.Month, DateTime::Now.Day,
			DateTime::Now.Hour, DateTime::Now.Minute, DateTime::Now.Second);
		// PathSeparator=';',DirectorySeparatorChar='\'
		String^ fn = path + Path::DirectorySeparatorChar + dtstr + ".xls"; // or ".xlsx" ?
		MessageBox::Show("Сохраняю " + dtstr + ".xls - нажмите ОК.","Excel");
		MForm->SaveExcel(true,fn);
	}
	public:
		System::Windows::Forms::RichTextBox^ InfoTextBox;
	public:
		  void putText(String^ T) { this->InfoTextBox->Text = T; }
		  String^ getText() { return this->InfoTextBox->Text; }
		  void addText(String^ T) { this->InfoTextBox->Text += T; }
		  void FormatText(String^ T) {
			  int f = -1;
			  while ((f = InfoTextBox->Find(T, f+1, RichTextBoxFinds::WholeWord)) >= 0) {
			  InfoTextBox->SelectionFont = gcnew System::Drawing::Font("Verdana", 12, FontStyle::Bold);
			  InfoTextBox->SelectionColor = Color::Red;
			  }
		  }
	};
}
