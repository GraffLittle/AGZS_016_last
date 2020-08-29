#pragma once

namespace AGZS {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::IO::Ports;

	/// <summary>
	/// Сводка для SetKKMForm
	/// </summary>
	public ref class SetKKMForm : public System::Windows::Forms::Form
	{
	public:
		SetKKMForm(void)
		{
			InitializeComponent();
			//
			//TODO: добавьте код конструктора
			//
			array<String^>^ ports = SerialPort::GetPortNames();
			ComBox->Items->Clear();
			ComBox->Items->AddRange(ports);
			ComBox->SelectedItem = ComBox->Items[ComBox->Items->Count - 1];
			ComBox->Text = ports[ports->Length - 1];
		}

	protected:
		/// <summary>
		/// Освободить все используемые ресурсы.
		/// </summary>
		~SetKKMForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^ OKbtn;
	public: System::Windows::Forms::ComboBox^ ComBox;
	private:

	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::Label^ DTlbl;
	private: System::Windows::Forms::Label^ Vlbl;
	private: System::Windows::Forms::Label^ RESlbl;
	private: System::Windows::Forms::RichTextBox^ OutRichTextBox;

	private: System::Windows::Forms::Button^ InfoBtn;
	protected:

	private:
		/// <summary>
		/// Обязательная переменная конструктора.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Требуемый метод для поддержки конструктора — не изменяйте 
		/// содержимое этого метода с помощью редактора кода.
		/// </summary>
		void InitializeComponent(void)
		{
			this->OKbtn = (gcnew System::Windows::Forms::Button());
			this->ComBox = (gcnew System::Windows::Forms::ComboBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->DTlbl = (gcnew System::Windows::Forms::Label());
			this->Vlbl = (gcnew System::Windows::Forms::Label());
			this->RESlbl = (gcnew System::Windows::Forms::Label());
			this->OutRichTextBox = (gcnew System::Windows::Forms::RichTextBox());
			this->InfoBtn = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// OKbtn
			// 
			this->OKbtn->DialogResult = System::Windows::Forms::DialogResult::OK;
			this->OKbtn->Location = System::Drawing::Point(179, 238);
			this->OKbtn->Name = L"OKbtn";
			this->OKbtn->Size = System::Drawing::Size(75, 23);
			this->OKbtn->TabIndex = 0;
			this->OKbtn->Text = L"Принять";
			this->OKbtn->UseVisualStyleBackColor = true;
			// 
			// ComBox
			// 
			this->ComBox->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->ComBox->FormattingEnabled = true;
			this->ComBox->Location = System::Drawing::Point(67, 6);
			this->ComBox->Name = L"ComBox";
			this->ComBox->Size = System::Drawing::Size(121, 21);
			this->ComBox->TabIndex = 1;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(12, 9);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(48, 13);
			this->label1->TabIndex = 2;
			this->label1->Text = L"Порт №";
			// 
			// DTlbl
			// 
			this->DTlbl->AutoSize = true;
			this->DTlbl->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->DTlbl->Location = System::Drawing::Point(12, 36);
			this->DTlbl->Name = L"DTlbl";
			this->DTlbl->Size = System::Drawing::Size(94, 15);
			this->DTlbl->TabIndex = 3;
			this->DTlbl->Text = L"Дата-Время ЧПМ";
			this->DTlbl->Click += gcnew System::EventHandler(this, &SetKKMForm::DTlbl_Click);
			// 
			// Vlbl
			// 
			this->Vlbl->AutoSize = true;
			this->Vlbl->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->Vlbl->Location = System::Drawing::Point(12, 58);
			this->Vlbl->Name = L"Vlbl";
			this->Vlbl->Size = System::Drawing::Size(69, 15);
			this->Vlbl->TabIndex = 4;
			this->Vlbl->Text = L"Версия ЧПМ";
			this->Vlbl->Click += gcnew System::EventHandler(this, &SetKKMForm::Vlbl_Click);
			// 
			// RESlbl
			// 
			this->RESlbl->AutoSize = true;
			this->RESlbl->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->RESlbl->Location = System::Drawing::Point(112, 82);
			this->RESlbl->Name = L"RESlbl";
			this->RESlbl->Size = System::Drawing::Size(167, 15);
			this->RESlbl->TabIndex = 5;
			this->RESlbl->Text = L"Результат выполнения команд";
			// 
			// OutRichTextBox
			// 
			this->OutRichTextBox->Location = System::Drawing::Point(12, 103);
			this->OutRichTextBox->Name = L"OutRichTextBox";
			this->OutRichTextBox->Size = System::Drawing::Size(408, 129);
			this->OutRichTextBox->TabIndex = 6;
			this->OutRichTextBox->Text = L"";
			// 
			// InfoBtn
			// 
			this->InfoBtn->Location = System::Drawing::Point(13, 80);
			this->InfoBtn->Name = L"InfoBtn";
			this->InfoBtn->Size = System::Drawing::Size(91, 23);
			this->InfoBtn->TabIndex = 7;
			this->InfoBtn->Text = L"Информация";
			this->InfoBtn->UseVisualStyleBackColor = true;
			this->InfoBtn->Click += gcnew System::EventHandler(this, &SetKKMForm::InfoBtn_Click);
			// 
			// SetKKMForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(432, 273);
			this->Controls->Add(this->InfoBtn);
			this->Controls->Add(this->OutRichTextBox);
			this->Controls->Add(this->RESlbl);
			this->Controls->Add(this->Vlbl);
			this->Controls->Add(this->DTlbl);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->ComBox);
			this->Controls->Add(this->OKbtn);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"SetKKMForm";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = L"Настройки ЧПM";
			this->Load += gcnew System::EventHandler(this, &SetKKMForm::SetKKMForm_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void DTlbl_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void Vlbl_Click(System::Object^ sender, System::EventArgs^ e);
private: System::Void SetKKMForm_Load(System::Object^ sender, System::EventArgs^ e) {}
private: System::Void InfoBtn_Click(System::Object^ sender, System::EventArgs^ e);
};
}
