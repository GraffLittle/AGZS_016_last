#pragma once

namespace AGZS {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Сводка для SetAddForm
	/// </summary>
	public ref class SetAddForm : public System::Windows::Forms::Form
	{
	public:
		SetAddForm(void)
		{
			InitializeComponent();
			//
			//TODO: добавьте код конструктора
			//
		}

	protected:
		/// <summary>
		/// Освободить все используемые ресурсы.
		/// </summary>
		~SetAddForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::GroupBox^ BoardGroupBox;
	public: System::Windows::Forms::NumericUpDown^ BrightNumUpDown;
	private:

	protected:

	protected:

	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::Button^ button1;
	private: System::Windows::Forms::Label^ label2;
	public: System::Windows::Forms::NumericUpDown^ KolNumUpDown;
	private:


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
			this->BoardGroupBox = (gcnew System::Windows::Forms::GroupBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->BrightNumUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->KolNumUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->BoardGroupBox->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->BrightNumUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->KolNumUpDown))->BeginInit();
			this->SuspendLayout();
			// 
			// BoardGroupBox
			// 
			this->BoardGroupBox->Controls->Add(this->BrightNumUpDown);
			this->BoardGroupBox->Controls->Add(this->label1);
			this->BoardGroupBox->Location = System::Drawing::Point(15, 45);
			this->BoardGroupBox->Name = L"BoardGroupBox";
			this->BoardGroupBox->Size = System::Drawing::Size(259, 57);
			this->BoardGroupBox->TabIndex = 0;
			this->BoardGroupBox->TabStop = false;
			this->BoardGroupBox->Text = L"Табло";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(7, 21);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(53, 13);
			this->label1->TabIndex = 0;
			this->label1->Text = L"Яркость:";
			// 
			// BrightNumUpDown
			// 
			this->BrightNumUpDown->Location = System::Drawing::Point(67, 21);
			this->BrightNumUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 10, 0, 0, 0 });
			this->BrightNumUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 0 });
			this->BrightNumUpDown->Name = L"BrightNumUpDown";
			this->BrightNumUpDown->Size = System::Drawing::Size(47, 21);
			this->BrightNumUpDown->TabIndex = 1;
			this->BrightNumUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 0 });
			// 
			// button1
			// 
			this->button1->DialogResult = System::Windows::Forms::DialogResult::OK;
			this->button1->Location = System::Drawing::Point(99, 229);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 23);
			this->button1->TabIndex = 1;
			this->button1->Text = L"Принять";
			this->button1->UseVisualStyleBackColor = true;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(12, 20);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(88, 13);
			this->label2->TabIndex = 2;
			this->label2->Text = L"Для колонки №";
			// 
			// KolNumUpDown
			// 
			this->KolNumUpDown->Location = System::Drawing::Point(106, 18);
			this->KolNumUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 9, 0, 0, 0 });
			this->KolNumUpDown->Name = L"KolNumUpDown";
			this->KolNumUpDown->Size = System::Drawing::Size(45, 21);
			this->KolNumUpDown->TabIndex = 3;
			// 
			// SetAddForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(286, 264);
			this->Controls->Add(this->KolNumUpDown);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->BoardGroupBox);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedToolWindow;
			this->Name = L"SetAddForm";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"Дополнительные настройки";
			this->BoardGroupBox->ResumeLayout(false);
			this->BoardGroupBox->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->BrightNumUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->KolNumUpDown))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	};
}
