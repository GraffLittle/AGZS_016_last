#include "SetKKMForm.h" // Настройки ЧПM
#include "Mercury.h"

using namespace AGZS;
using namespace Mercury;

System::Void SetKKMForm::DTlbl_Click(System::Object^ sender, System::EventArgs^ e) {
	TMerc::resLbl = this->RESlbl;
	TMerc::GetMercDT(ComBox->Text, DTlbl);
}
System::Void SetKKMForm::Vlbl_Click(System::Object^ sender, System::EventArgs^ e) {
	TMerc::resLbl = this->RESlbl;
	TMerc::GetMercVersion(ComBox->Text, Vlbl);
}
System::Void SetKKMForm::InfoBtn_Click(System::Object^ sender, System::EventArgs^ e) {
	OutRichTextBox->Text = "Считываю из ЧПМ...";
	TMerc::StartDataAquire(ComBox->Text,OutRichTextBox);
}
