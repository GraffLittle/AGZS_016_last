// газораздаточная колонка
#include "MainForm.h"
#include "resource.h" // #define IDR_MAINFRAME 101
#include "SettingsForm.h"
#include "SetKKMForm.h"
#include "SetAddForm.h" // Дополнительные настройки
#include "Mercury.h"
#include "KolPanel.h"
#include "IniParser.h"
#include "LoginForm.h"
#include "DebugForm.h"
#include "UsersForm.h"
#include "InfoForm.h"
#include "FuelForm.h" // for ChangeCalculator
#include "ComAbout.h"
#include "Utils.h"
#include <string>

#define PASSW "1905197229051974"

#define MAXKOLNUM 9

#pragma warning(disable : 4996)

using namespace AGZS;
using namespace AGZS_dbg;
using namespace System::Data::OleDb; // for OleDbConnection, OleDbCommand, OleDbDataAdapter
using namespace System::Diagnostics; // for Debug::WriteLine("");
using namespace System::Threading;
using namespace Mercury;

int __clrcall WinMain(array<String^>^ args) {
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(true);
	Application::Run(gcnew MainForm);
	return 0;
}

char* ServiceParol = PASSW;
bool IsIt(String^ str) { return (str == PASSW); }

System::Void MainForm::InfoBtn_Click(System::Object^ sender, System::EventArgs^ e) {
	ShowInfo(true); // Ctrl+'i' == 9
}
System::Void MainForm::FuelTimer_Tick(System::Object^ sender, System::EventArgs^ e) {
	if (delayed->Count) {
		String^ dld = delayed[0];
		delayed->RemoveAt(0);
		bool dow = true;
		if ((dld[0] == 'n') && (dld[2] == '5') && (dld[3] == 'i')) {
			int ch = Convert::ToInt32(dld[1]);
			this->Invoke(gcnew Action<String^>(&Helper::addText), "\r\n" + dld[1] + "=" + ch); // 2=50
			int kn = dld[1] - 48; // 48 == '0' ???
			TKolPanel^ pnl = safe_cast<TKolPanel^>(this->KolPanel->Controls[KolPanel->Controls->Count - kn]);
			if (pnl->isFueling()) {} // bool dow = true;
			else { dow = false; }
		}
		if (dow) {
			serialPort1->Write(dld);
			this->Invoke(gcnew Action<String^>(&Helper::addText), "\r\nDelayed & Writed:" + dld);
		}
	}
}
System::Void MainForm::SendE(int n) { // Пуск из программы
	if (serialPort1->IsOpen) {
		//PortWrite("n" + n + "e");
		//PortWrite( n, "e");
		PortWrite(n, 'e');
		this->Invoke(gcnew Action<String^>(&Helper::addText), "\r\n'e' sended for n=" + n);
	} else Helper::addText("\r\nPort closed for 'e'.");
}
// 28.07: символ № (0xFC) => 'r', "0x1B" => "с".
System::Void MainForm::SendS(int n) { // стоп заправки сделал по кнопке ESC '0x1B', 28.07: "0x1B" на "с".
	if (serialPort1->IsOpen) {
		//PortWrite("n" + n.ToString() + wchar_t(0x1B));
		//PortWrite( n, wchar_t(0x1B));
		PortWrite(n, 'c');
		this->Invoke(gcnew Action<String^>(&Helper::addText), "\r\n'0x1B'('c') sended for n=" + n);
	}
	else {
		Helper::addText("\r\nPort closed for '0x1B'('c').");
	}
}
System::Void MainForm::SendReset(int n) { // '&'
	if (serialPort1->IsOpen) {
		PortWrite(n, '&');
		this->Invoke(gcnew Action<String^>(&Helper::addText), "\r\n'&' sended for n=" + n);
	}
	else Helper::addText("\r\nPort closed for '&'.");
}
// по доработке яркости пишем n2b5; получаем  2b5ok (в конце имеется \r\n)
System::Void MainForm::SendBright( System::Decimal n, System::Decimal b) { // Bright - яркость табло
	if (serialPort1->IsOpen) {
		PortWrite( n.ToInt32(n), "b" + b.ToString() + ";"); // String^ toWrite = "n" + n.ToString() + s;
		//MessageBox::Show( "Bright " + b + " sended for n=" + n, "SendBright(" + n + "," + b + ")");
		this->Invoke(gcnew Action<String^>(&Helper::addText), "\r\nBright " + b + " sended for n=" + n);
	}
	else Helper::addText("\r\nPort closed for 'Bright'." + "SendBright(" + n + "," + b + ")\r\n");
}

System::Void MainForm::MainForm_KeyPress(System::Object^ sender, System::Windows::Forms::KeyPressEventArgs^ e) {
	if (Helper::DF == nullptr) Helper::DF = gcnew TDebugForm();
	if (e->KeyChar == 4) Helper::DF->Show(); // Ctrl+'d'
	else if (e->KeyChar == 9) ShowInfo(false); // Ctrl+'i' == 9
	//else if (e->KeyChar == 5) SendE(this->ActiveKolPanel); // Ctrl+'e' == 5 - start fuel
	int key = e->KeyChar;
	Helper::addText("MainForm_KeyPress:" + key.ToString());
	if (e->KeyChar == '+') Helper::addText("|+|");
	if (e->KeyChar == '-') Helper::addText("|-|");
}

array<String^>^ ExplodeStr(String^ S, String^ by) {
	int pos = S->IndexOf(by);
	array<String^>^ out = { "?","?" };
	if (pos < 0) return out;
	out[0] = S->Substring(0,pos);
	out[1] = S->Substring(pos+1);
	return out;
}

void _AddText(RichTextBox^ InfoTextBox, String^ text, String^ fn = "Tahoma", float em = 8.25,
	FontStyle fs = FontStyle::Regular, Color clr = Control::DefaultForeColor, Color bcl = Control::DefaultBackColor) {
	InfoTextBox->SelectionFont = gcnew System::Drawing::Font(fn, em, fs);
	InfoTextBox->SelectionColor = clr;
	InfoTextBox->SelectionBackColor = bcl;
	InfoTextBox->SelectedText = text;
}
void _f_AddText(RichTextBox^ InfoTextBox, String^ text, String^ fn, float em, FontStyle fs = FontStyle::Regular,
	Color clr = Control::DefaultForeColor, Color bcl = Control::DefaultBackColor, bool end = true) {
	int f = InfoTextBox->Find("!@#", 0, RichTextBoxFinds::MatchCase);
	InfoTextBox->SelectionFont = gcnew System::Drawing::Font(fn, em, fs);
	InfoTextBox->SelectionColor = clr;
	InfoTextBox->SelectionBackColor = bcl;
	if (end) InfoTextBox->SelectedText = text + "!@#"; // "!@#" - end
	else InfoTextBox->SelectedText = text;
}
void _f_AddText(RichTextBox^ InfoTextBox, String^ text, bool end = true) {
	int f = InfoTextBox->Find("!@#", 0, RichTextBoxFinds::MatchCase);
	if (end) InfoTextBox->SelectedText = text + "!@#"; // "!@#" - end
	else InfoTextBox->SelectedText = text;
}
void __AddText(RichTextBox^ InfoTextBox, String^ text) {
	InfoTextBox->SelectedText = text;
}

void MainForm::_ShowActionsIn(RichTextBox^ InfoText, String^ forUser) {
	OleDbConnection^ dBaseConnection = gcnew OleDbConnection("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=Data;Extended Properties=dBase IV");
	dBaseConnection->Open();
	String^ SQLCommand = "SELECT * FROM Actions";
	if (forUser != "Admin") SQLCommand += " WHERE USR = '" + forUser + "';";
	OleDbCommand^ odbcom = gcnew OleDbCommand(SQLCommand, dBaseConnection);
	try {
		System::Data::OleDb::OleDbDataReader^ dr = odbcom->ExecuteReader();
		int i = 0;
		while (dr->Read()) {
			String^ dr0 = dr[0]->ToString(); // DateTime
			String^ dr1 = dr[1]->ToString(); // User
			String^ dr2 = dr[2]->ToString(); // Action
			String^ dr3 = dr[3]->ToString(); // Additional
			_f_AddText(InfoText, "\r\n" + dr0 + ": ", "Times", 8, FontStyle::Italic, Color::BlueViolet, Color::White);
			// To set action font:
			_f_AddText(InfoText, "", "Tahoma", 8.25, FontStyle::Bold, Color::Black, Color::White);
			if (this->names->ContainsKey(dr1)) _f_AddText(InfoText, this->names[dr1]);
			else _f_AddText(InfoText, dr1);
			// To set normal font:
			_f_AddText(InfoText, "", "Tahoma", 8.25, FontStyle::Regular, Color::Black, Color::White);
			if (dr2 == "Loged") _f_AddText(InfoText, " залогинился.");
			else if (dr2 == "Created.") _f_AddText(InfoText, " первый запуск.", "Arial", 10, FontStyle::Bold, Color::Red, Color::White);
			else if (dr2 == "NewUser") _f_AddText(InfoText, " зарегистрирован.");
			else if (dr2 == "ChangeUserPass") _f_AddText(InfoText, " пароль изменен.");
			else if (dr2 == "StartFuel") _f_AddText(InfoText, " заливка началась.");
			else if (dr2 == "StopFuel") {
				array<String^>^ ss = ExplodeStr(dr3,"-");
				_f_AddText(InfoText, " залито. " + ss[0] + " литров по цене " + ss[1] + " за литр.");
				continue;
			}
			else if (dr2 == "NewCost") _f_AddText(InfoText, " цена/литр изменилось.");
			else _f_AddText(InfoText, " Action not defined! Unknown '" + dr2 + "'");
			// To set normal font:
			_f_AddText(InfoText, "", "Tahoma", 8.25, FontStyle::Regular, Color::Black, Color::White);
			if (dr3 == "") _f_AddText(InfoText, "@");
			else { // ~По литрам, !На сумму, =До заполнения
				if (dr3[0] == '~') {
					array<String^>^ ss = ExplodeStr(dr3->Substring(2), "-");
					_f_AddText(InfoText, " По литрам " + ss[0]);
				}
				else if (dr3[0] == '!') {
					array<String^>^ ss = ExplodeStr(dr3->Substring(2), "-");
					_f_AddText(InfoText, " На сумму " + ss[0]);
				}
				else if (dr3[0] == '=') _f_AddText(InfoText, " До заполнения " + dr3->Substring(2));
				else if (dr3->Length < 4) _f_AddText(InfoText, dr3->Length.ToString() + ":" + dr3);
				else
					if (dr3->Substring(0, 4) == "None");
					else if (dr3->Substring(0, 4) == "Mode");
					else _f_AddText(InfoText, dr3);
			}
		}
	}
	catch (Exception^ ex)
	{
		Helper::DF->addText("\r\n_ShowActionsIn(," + forUser + ")-Exception: " + ex->ToString());
		InfoText->Text += "---";
	}
}

void MainForm::ShowInfo(bool save) {
	TInfoForm^ inf = gcnew TInfoForm(this, names[LogedAs]);

	DateTime^ d = gcnew DateTime(DateTime::Now.Year, DateTime::Now.Month, DateTime::Now.Day,
		DateTime::Now.Hour, DateTime::Now.Minute, DateTime::Now.Second);
	String^ dstr = String::Format("\n{0}, {1}\n", d->ToLongDateString(), d->ToLongTimeString());
	inf->InfoTextBox->AppendText(dstr);

	_f_AddText(inf->InfoTextBox, "\r\nInfo for '" + names[LogedAs] + "' (" + pass[LogedAs] + ")");
	_ShowActionsIn(inf->InfoTextBox, LogedAs);
	_f_AddText(inf->InfoTextBox, "", false); // Clear "!@#"

	String^ dtstr = String::Format("{0:D4}{1:D2}{2:D2}_{3:D2}{4:D2}{5:D2}",
		DateTime::Now.Year, DateTime::Now.Month, DateTime::Now.Day,
		DateTime::Now.Hour, DateTime::Now.Minute, DateTime::Now.Second);
	if (save) inf->InfoTextBox->SaveFile(dtstr + ".rtf");
	inf->Show();
}

System::Void MainForm::PortWrite(int n, String^ s)
{
	Helper::addText("PortWrite(" + n + ", '" + s + "')");
	String^ toWrite = "n" + n.ToString() + s;
	//this->serialPort1->Write(toWrite);
	delayed->Add(toWrite);
	sWrtd = toWrite;
	if (sw) sw->WriteLine(DateTime::Now.ToString() + ": Writed '" + toWrite + "'");
	return System::Void();
}
System::Void MainForm::PortWrite(int n, wchar_t c)
{
	// May be: 'e'(Пуск из программы), 'c'(Стоп из программы), 'p'(подтверждение правильности ответа), '&'(«РЕСЕТ»)
	String^ s = "n" + n.ToString() + c;
	Helper::addText("PortWrite(" + n + ", '" + c + "')");
	//serialPort1->Write( s->ToCharArray(), 0, 3);
	delayed->Add(s);
	sWrtd = s;
	if (sw) sw->WriteLine(DateTime::Now.ToString() + ": Writed 0x" + Convert::ToString(int(c), 16) + "");
	return System::Void();
}
System::Void MainForm::PortWrite2all(String^ s) {
	for (int i = 1; i <= MAXKOLNUM; i++) {
		PortWrite(i,s);
		Thread::Sleep(77);
	}
	return System::Void();
}
System::Void MainForm::LogReceived(String^ s)
{
	if (sw) sw->WriteLine(DateTime::Now.ToString() + ": Received '" + s->Trim() + "'");
	return System::Void();
}

/*
все команды от меня начинаются с "n+номер", а все ответы начинаются с номера.
Без пробела: "n25i" и будет потом ответ 2l10.55 s78.25;
*/
void MainForm::OutReceived(void) {
	try {
		this->Invoke(gcnew Action<String^>(&Helper::addText), "\r\n-OutReceived(" + comStr + ")");
	}
	catch (NullReferenceException^ E) {
		Helper::DF->addText("\r\n~NullReferenceException:" + E->Message + "\r\n");
	}
	TKolPanel^ ActivePanel = dynamic_cast<TKolPanel^>(this->ActiveKolPanel);
	int gkn = 0;
	if (ActivePanel != nullptr) gkn = ActivePanel->getKolNum();
	//if (lastOuted != comStr)
	{
		ReceivedPacket += comStr;
		lastOuted = comStr;
		int rn = ReceivedPacket->IndexOf("\r\n");
		int reclen = ReceivedPacket->Length;
		this->Invoke(gcnew Action<String^>(&Helper::addText), "\r\n!Received: var rn=" + rn + ",reclen=" + reclen);
		//if (comStr->Contains("\r\n"))
		if (rn >= 0)
		{
			String^ befrn = ReceivedPacket->Substring(0, rn);
			String^ aftrn = ReceivedPacket->Substring(rn+2);
this->Invoke(gcnew Action<String^>(&Helper::addText), "\r\n!Contains '\\r\\n':var befrn=" + befrn + ",aftrn=" + aftrn);
			int kNum = 0;
			String^ sub = ReceivedPacket->Substring(0, 1);
			try {
				kNum = Convert::ToInt32(sub);
			}
			catch (Exception^ E) {
				Helper::DF->addText("\r\n~MainForm::OutReceived(void) Exception:" + E->Message + "\r\n" +
					"sub=" + sub + "\r\n");
			}
			ReceivedPacket = ReceivedPacket->Substring(1); // все ответы начинаются с номера (got kNum)
			ReceivedPacket = ReceivedPacket->Trim(); // Concating \r\n at the end...
			wchar_t wc0 = ReceivedPacket[0];
			this->Invoke(gcnew Action<String^>(&Helper::addText),
				"\r\n!kNum=" + kNum.ToString() + ",NOW(!)ReceivedPacket='" + ReceivedPacket + "':");
			if (gkn != kNum) { // new ActivePanel...
				this->Invoke(gcnew Action<String^>(&Helper::addText), "\r\n(gkn != kNum) - new ActivePanel!");
				ActivePanel = safe_cast<TKolPanel^>(this->KolPanel->Controls[KolPanel->Controls->Count - kNum]);
			}
			else this->Invoke(gcnew Action<String^>(&Helper::addText), "\r\n(gkn == kNum)" + gkn + "==" + kNum);
			try {
				if (wc0 == '$') {
					pStr = ReceivedPacket;
				}
				else
					if (wc0 == 't') {
						tStr = ReceivedPacket;
					}
					else
						if (wc0 == 'I') {
							ilStr = ReceivedPacket;
						}
						else
							if (ReceivedPacket->Contains("L/M")) {
								lmStr = ReceivedPacket;
							}
							else unStr = ReceivedPacket;
				bool have1 = false, have2 = false;
			  if (ReceivedPacket->Contains("!!")) {
				int p = ReceivedPacket->IndexOf("!!");
				ReceivedPacket = ReceivedPacket->Remove(p, 2);
				this->Invoke(gcnew Action<String^>(&Helper::addText), "\r\n!Received '!!', deleted:" + ReceivedPacket);
				have2 = true;
			  }
			  if (ReceivedPacket->Contains("!")) {
				int p = ReceivedPacket->IndexOf("!");
				ReceivedPacket = ReceivedPacket->Remove(p, 1);
				this->Invoke(gcnew Action<String^>(&Helper::addText), "\r\n!Received '!', deleted:" + ReceivedPacket);
				have1 = true;
			  }
/*
1. Заправка 1.55 литра на вторую колонку: n21.55; => 21.55;
Проверяем правильность ответа того что отослали с тем что пришло: если true то пункт №2, если false пункт №7
2. Посылаем подтверждение правильности ответа на вторую колонку: n2p => 2pok
Ответ приходит после нажатия пуск в программе либо после нажатия пуск на колонке.
Если ответ был можно переходить к пункту №3 либо №4 если ответа нет пункт №7
3. Запрос информации о заправке: n25i => 2l1.20 s10.50
4. Пуск из программы: n2e => 2ok
7. «РЕСЕТ»: n+номер& => restart (В случае с 2й колонкой 2restart)
8. Назначить новый номер колонке. №0->№2: n0r2; => 02.00;
После получения ответа можно писать что присвоено (первая цифра это номер колонки которую решили переименовывать, вторая на какую переименовали)
 остальные .00; это в принципе не в счёт это мой стандартный обработчик чисел дописывает.
*/
/*
* по доработке яркости пишем n2b5; получаем  2b5ok (в конце имеется \r\n)
PortWrite(2, 'b7;')
Bright 7 sended for n=2
Delayed & Writed:n2b7;
-OutReceived(2b7ok
)
!Received: var rn=5,reclen=7
!Contains '\r\n':var befrn=2b7ok,aftrn=
!kNum=2,NOW(!)ReceivedPacket='b7ok':
(gkn != kNum) - new ActivePanel!MainForm_KeyPress:4
*/
			  if (Char::IsDigit(wc0)) {
				  // Проверяем правильность ответа того что отослали (n21.55;) с тем что пришло (21.55;?)
				  // если true то пункт №2, если false пункт №7 («РЕСЕТ»)
				  // №2. подтверждение правильности ответа: n2p
				  String^ s = "n" + kNum + ReceivedPacket;
				  if (s == sWrtd)
					  PortWrite(kNum,'p');
				  else SendReset(kNum);
			  }
			  else if (wc0 == 'r') { // «РЕСЕТ»: n+номер& => номерrestart
				  if (ReceivedPacket == "restart") {
					  TKolPanel^ kpl = safe_cast<TKolPanel^>(this->KolPanel->Controls[KolPanel->Controls->Count - kNum]);
					  kpl->SetInfo("Перезапущено");
					  this->Invoke(gcnew Action<String^>(&Helper::addText), "\r\n'restart'HERE!.");
				  }
			  } // if (wc0 == 'r')
			  // по доработке яркости пишем n2b5; получаем  2b5ok (в конце имеется \r\n)
			  else if (wc0 == 'b') { // -OutReceived(2b7ok)
				  int bok = ReceivedPacket->IndexOf("ok");
				  ReceivedPacket = ReceivedPacket->Remove( bok, 2);
				  TKolPanel^ kpl = safe_cast<TKolPanel^>(this->KolPanel->Controls[KolPanel->Controls->Count - kNum]);
				  kpl->SetInfo("Яркость=" + ReceivedPacket->Substring(1,bok-1)); // Яркость=10
				  this->Invoke(gcnew Action<String^>(&Helper::addText), "\r\n'Яркость' HERE!.");
			  } // if (wc0 == 'b')
			  // После посыла р" жду "2pok", после посыла "е" и начала заправки жду "2ок".
			  else if (wc0 == 'p') { // OK
				  wchar_t wc1 = ReceivedPacket[1], wc2 = ReceivedPacket[2];
				  if ((wc1 == 'o') && (wc2 == 'k')) {
					  this->Invoke(gcnew Action<String^>(&Helper::addText), "\r\n'pok'HERE!.");
				  }
			  } // if (wc0 == 'p')
			  else if (wc0 == 'o') { // OK
				  wchar_t wc1 = ReceivedPacket[1];
				  if (wc1 == 'k') {
					  TKolPanel^ kpl = safe_cast<TKolPanel^>(this->KolPanel->Controls[KolPanel->Controls->Count - kNum]);
					  kpl->setFueling(true); // kNum + 5i sending.
					  PortWrite(kNum, "5i");
					  this->Invoke(gcnew Action<String^>(&Helper::addText), "\r\n'ok'HERE!.");
				  }
			  } // if (wc0 == 'o')
			  else if (wc0 == 'l') { // l2.22 s23.28
				int p = ReceivedPacket->IndexOf(" ");
				String^ s1 = ReceivedPacket->Substring(0, p);
				s1 = s1->Substring(1);
				String^ s2 = ReceivedPacket->Substring(p + 1);
				if (s2[0] == 's') s2 = s2->Substring(1);
				else s2 = s2->Substring(3); // L/M
				this->Invoke(gcnew Action<String^>(&Helper::addText), "\r\n s1=" + s1 + ", s2=" + s2);
				try {
					s1 = s1->Replace(".", Globalization::NumberFormatInfo::CurrentInfo->NumberDecimalSeparator);
					s2 = s2->Replace(".", Globalization::NumberFormatInfo::CurrentInfo->NumberDecimalSeparator);
					double d1 = ConvertAnyToDouble(s1);
					double d2 = ConvertAnyToDouble(s2); // s1=2.01, s2=14.65
					lastL = d1; lastS = d2;
					if (ActivePanel != nullptr) ActivePanel->MakeFuel(d1,d2);
					if ((!have1) && (!have2)) PortWrite(kNum, "5i");
				} catch (FormatException^ ex) {
					this->Invoke(gcnew Action<String^>(&Helper::addText), 
						"\r\n-FormatException: " + ex->ToString() + "!\r\ns1=" + s1 + ",s2=" + s2 + "!~!\r\n");
				}
			  } // if (wc0 == 'l')
			  if (have1 || have2) this->StopFuel(); // Two times! Заправлено - Остановлено
			_Received = ReceivedPacket;
			ReceivedPacket = "";
			}
			catch (Exception^ ex)
			{
				this->Invoke(gcnew Action<String^>(&Helper::addText), "\r\nMainForm::OutReceived()-Exception: " + ex->ToString());
			}
		} // if (comStr->Contains("\r\n")) ...
	} // if (lastOuted != comStr)
} // void MainForm::OutReceived(void)

void MainForm::ErrReceived(void) {
	Helper::DF->addText("\r\n ErrReceived() - Error:" + errStr);
}

System::Void MainForm::serialPort1_ErrorReceived(System::Object^ sender, System::IO::Ports::SerialErrorReceivedEventArgs^ e) {
	this->errStr = e->ToString();
	LogReceived(errStr);
	this->Invoke(gcnew Action(this, &MainForm::ErrReceived));
}
System::Void MainForm::serialPort1_DataReceived(System::Object^ sender, System::IO::Ports::SerialDataReceivedEventArgs^ e) {
	comStr = this->serialPort1->ReadExisting();
	LogReceived(comStr);
	this->Invoke(gcnew Action(this, &MainForm::OutReceived));
}

void MainForm::StartFuel(int kNum, double data, String ^ modeStr) {
	this->AddAction(LogedAs, "StartFuel", modeStr);
	doComOpen(serialPort1, serialPort1->PortName);
	if (data == 0.0) { // =До заполнения
		//if (serialPort1->IsOpen) PortWrite(kNum, ";n" + kNum.ToString() + "p");
		if (serialPort1->IsOpen) PortWrite(kNum, ";");
	}
	else { // По литрам
		String^ sfd = String::Format("{0:0.00}", data);
		//String^ toWrite = "n" + kNum.ToString() + data.ToString() + ";";
		//String^ toWrite = data.ToString() + ";";
		String^ toWrite = sfd + ";";
		//String^ toWrite = data.ToString() + ";n" + kNum.ToString() + "p";
		if (toWrite->Contains(",")) {
			Helper::addText("\r\n '" + toWrite + "' contains ',' - replacing:");
			toWrite = toWrite->Replace(",", ".");
			Helper::addText(" '" + toWrite + "'");
		}
		if (serialPort1->IsOpen) PortWrite(kNum,toWrite);
		Helper::addText("\r\n MAKED: PortWrite(" + kNum + ", '" + toWrite + "')");
	}
	Helper::addText("\r\nStartFuel:mode=" + modeStr);
	toolStripMenuItem2->Enabled = false; // L"Настройки";
	toolStripMenuItem3->Enabled = false; // L"Операторы";
	toolStripMenuItem4->Enabled = false; // L"Информация";
	toolStripMenuItem5->Enabled = false; // L"Добавить ТРК";
	toolStripMenuItem7->Enabled = false; // Удалить ТРК
	toolStripMenuItem8->Enabled = false; // L"Настройки ЧПМ"
	toolStripMenuItem11->Enabled = false; // L"Дополнительные настройки"
}

void MainForm::ShowChangeCalculator(void) {
	Helper::DF->addText("\r\n ShowChangeCalculator()");
	TKolPanel^ ActivePanel = safe_cast<TKolPanel^>(this->ActiveKolPanel);
	FuelForm^ ff = gcnew FuelForm(this->cost);
	ff->textBox1->Text = lastL.ToString();
	ff->textBox2->Text = String::Format("{0:0.00}", lastS); //lastS.ToString();
	ff->ShowForChange(ActivePanel->getKolNum(),ActivePanel->getAccepted());
}

// From MainForm::OutReceived(void): if (have1 || have2) this->StopFuel();
void MainForm::StopFuel(void) // this one used ???
{
	putLevel(_UserLevel);
	Helper::addText("\r\n=0!STOPED.");
	TKolPanel^ ActivePanel = dynamic_cast<TKolPanel^>(this->ActiveKolPanel);
	if (ActivePanel != nullptr) ActivePanel->StopFuel();
	this->AddAction(LogedAs, "StopFuel", "L");
	bool doit = ActivePanel->CheckForCalc(lastL,lastS);
	if (doit) ShowChangeCalculator();

	double acc = ActivePanel->getAccepted();
	TMerc::PrintCheck(this->ComKKM, lastL, this->cost, acc);
	Helper::addText("\r\nMAKED: TMerc::PrintCheck(" + ComKKM + ", " + lastL + ", " + this->cost + ", " + acc + ")");
}
void MainForm::StopFuel(String^ Lit)
{
	putLevel(_UserLevel);
	Helper::addText("1.STOPED - " + Lit);
	TKolPanel^ ActivePanel = dynamic_cast<TKolPanel^>(this->ActiveKolPanel);
	if (ActivePanel != nullptr) ActivePanel->StopFuel();
	this->AddAction(LogedAs, "StopFuel", Lit);
}
void MainForm::StopFuel(double lit) // From TKolPanel::MakeFuel(double f, double c): FMain->StopFuel(this->NowFuel);
{
	putLevel(_UserLevel);
	Helper::addText("2.STOPED - " + lit.ToString());
	TKolPanel^ ActivePanel = dynamic_cast<TKolPanel^>(this->ActiveKolPanel);
	if (ActivePanel != nullptr) ActivePanel->StopFuel(lit);
	this->AddAction(LogedAs, "StopFuel", lit.ToString() + "-" + this->cost.ToString());
}

System::Void MainForm::LogAbout(void)
{
	String^ dtstr = String::Format("{2:D2}{1:D2}{0:D4}_{3:D2}{4:D2}{5:D2}",
		DateTime::Now.Year, DateTime::Now.Month, DateTime::Now.Day,
		DateTime::Now.Hour, DateTime::Now.Minute, DateTime::Now.Second);
	if (this->doLogPort) {
		System::IO::DirectoryInfo^ di = Directory::CreateDirectory("Log");
		Helper::addText("\r\n" + di->FullName);
		Helper::addText(",lat=" + di->LastAccessTime.ToString());
		Helper::addText(",created=" + di->CreationTime);
		String^ fileName = "Log" + System::IO::Path::DirectorySeparatorChar + dtstr + ".log";
		// If the file exists, it is overwritten (unless you pass True as the second StringWriter constructor argument).
		sw = gcnew StreamWriter(fileName, true); // StreamWriter(System::String^ path, bool append)
		sw->AutoFlush;
		sw->WriteLine(DateTime::Now);
	}
	else sw = nullptr;
	return System::Void();
}

void MainForm::DelKol(void) {
	if (kols == 1) { MessageBox::Show("Колонок всего одна! Невозможно удалить.", L"Нельзя"); return; }
	try {
		int wasNum = this->KolPanel->Controls->Count;
		this->KolPanel->Controls->Clear();
		for (int i = wasNum - 1; i > 0; i--) AddKol(i);
		kols = wasNum - 1;
	}
	catch (Exception^ E) {
		Helper::addText("\r\nDelKol()-Exception:" + E->Message);
	}
}

void MainForm::AddKol(void) {
	Helper::addText("void AddKol(kols=" + kols + ")");
	if (kols > 9) { MessageBox::Show("Колонок уже " + kols.ToString(), L"kols > 9"); return; }
	try {
		int wasNum = this->KolPanel->Controls->Count;
		this->KolPanel->Controls->Clear();
		for (int i = wasNum+1; i > 0; i--) AddKol(i);
		kols = wasNum + 1;
	}
	catch (Exception ^ E) {
		Helper::DF->addText("\r\nAddKol()-Exception:" + E->Message);
	}
}
void MainForm::AddKol(int num) {
	Helper::addText("void AddKol(num=" + num + ")");
	try {
		TKolPanel^ panel = gcnew TKolPanel(num, cost, this); kols = num;
		panel->Dock = System::Windows::Forms::DockStyle::Left;
		KolPanel->Controls->Add(panel);
	}
	catch (Exception^ E) {
		Helper::DF->addText("\r\nAddKol(num)-Exception:" + E->Message);
	}
}

void MainForm::AdjustSize(void) {
	int c = KolPanel->Controls->Count;
	int w = 0;
	for (int i = 0; i < c; i++)
	{
		w += KolPanel->Controls[i]->Width;
		Debug::Print("w=" + w.ToString() + "," + KolPanel->Controls[i]->Width);
	}
	this->MinimumSize = System::Drawing::Size(w, 350);
	this->Width = w + 13;
	int siw = System::Windows::Forms::SystemInformation::VirtualScreen.Width;
	this->Left = siw/2 - Width/2;
}

System::Void MainForm::SetKKMBtn_Click(System::Object^ sender, System::EventArgs^ e) {
	//MessageBox::Show("toolStripMenuItem8->Click","SetKKMBtn_Click()");
	SetKKMForm^ sf = gcnew SetKKMForm();
	sf->ComBox->Text = this->ComKKM;
	if (sf->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
		//MessageBox::Show(L"Принять: " + sf->ComBox->Text, "SetKKMBtn_Click()");
		this->ComKKM = sf->ComBox->Text;
		SaveIni(false);
	}
	//else MessageBox::Show("SetKKMForm()->ShowDialog() != DialogResult::OK", "SetKKMBtn_Click()");
}
System::Void MainForm::SetAddBtn_Click(System::Object^ sender, System::EventArgs^ e) { // L"Дополнительные настройки"
	//MessageBox::Show("toolStripMenuItem11->Click\r\n\"Дополнительные настройки\"","SetAddBtn_Click()");
	SetAddForm^ saf = gcnew SetAddForm();
	if (saf->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
		//MessageBox::Show(L"Принять: " + saf->BrightNumUpDown->Value, "SetAddBtn_Click()");
		//PortWrite(1, "1i");
		doComOpen(serialPort1, serialPort1->PortName, Helper::DF->DebugTextBox);
		// по доработке яркости пишем n2b5; получаем  2b5ok (в конце имеется \r\n)
		SendBright(saf->KolNumUpDown->Value,saf->BrightNumUpDown->Value);
	}
	//else MessageBox::Show("SetAddForm()->ShowDialog() != DialogResult::OK", "SetAddBtn_Click()");
}

System::Void MainForm::SetBtn_Click(System::Object^ sender, System::EventArgs^ e) {
	SettingsForm^ SF = gcnew SettingsForm();
	SF->MForm = this;
	SF->ComBox->Text = this->serialPort1->PortName;
	SF->CostBox->Text = Convert::ToString(cost);
	SF->checkPusk->Checked = this->ShowPusk;
	SF->checkStop->Checked = this->ShowStop;
	SF->checkLog->Checked = this->doLogPort;
	this->serialPort1->Close();
	if (SF->ShowDialog() == System::Windows::Forms::DialogResult::OK)
		{
		doComOpen(this->serialPort1, SF->ComBox->Text, Helper::DF->DebugTextBox);
		double newc = ConvertAnyToDouble(SF->CostBox->Text);
		if (newc != cost) { // & Update cost in panels
			this->AddAction(this->LogedAs, "NewCost", cost.ToString() + "-" + newc.ToString());
			cost = newc;
			for (int i = 0; i < KolPanel->Controls->Count; i++) {
				TKolPanel^ KP = dynamic_cast<TKolPanel^>(KolPanel->Controls[i]);
				KP->NewCost(newc);
			}
			//Запись цены в устройство $25.25; (выставится 25 рублей 25 копеек)
			String^ ncs = newc.ToString();
			ncs = ncs->Replace(",", ".");
			String^ cs = "$" + ncs + ";";
			PortWrite2all(cs);
			Helper::addText("\r\nWrited to " + serialPort1->PortName + ":" + cs);
			PortWrite(1,"1i");
			Thread::Sleep(100);
			Helper::addText(this->comStr);
		}
		if (serialPort1->IsOpen) {
			MessageBox::Show("Порт " + serialPort1->PortName + " открыт." + comStr, "OK");
		}
		else MessageBox::Show("Порт " + serialPort1->PortName + " NOT открыт.", "FALSE");
		if (this->ShowPusk != SF->checkPusk->Checked) {
			this->ShowPusk = SF->checkPusk->Checked;
			for (int i = 0; i < KolPanel->Controls->Count; i++) {
				TKolPanel^ KP = safe_cast<TKolPanel^>(KolPanel->Controls[i]);
				KP->SetVisPuskBtn(this->ShowPusk);
			}
		}
		if (this->ShowStop != SF->checkStop->Checked) {
			this->ShowStop = SF->checkStop->Checked;
			for (int i = 0; i < KolPanel->Controls->Count; i++) {
				TKolPanel^ KP = safe_cast<TKolPanel^>(KolPanel->Controls[i]);
				KP->SetVisStopBtn(this->ShowStop);
			}
		}
		this->doLogPort = SF->checkLog->Checked;
		SaveIni(false);
	}
}

System::Void MainForm::DemoTimer_Tick(System::Object^ sender, System::EventArgs^ e) { // Not used any more
	for (int i = 0; i < KolPanel->Controls->Count; i++) {
		TKolPanel^ KP = dynamic_cast<TKolPanel^>(KolPanel->Controls[i]);
		if (KP->isFuel) KP->MakeFuel();
	}
}

void MainForm::SaveIni(bool is0) {
	IniParser^ ip = gcnew IniParser();
	ip->SetString("Main", "Cost", cost.ToString());
	ip->SetString("Main", "Pusk", this->ShowPusk.ToString());
	ip->SetString("Main", "Stop", this->ShowStop.ToString());
	ip->SetString("Main", "LogPort", this->doLogPort.ToString());
	if (KolPanel->Controls->Count) ip->SetString("Main", "KolNum", KolPanel->Controls->Count.ToString());
	else ip->SetString("Main", "KolNum", "1");
	Helper::DF->addText("\r\n SaveIni(" + is0.ToString() + "): KolPanel->Controls->Count=" + KolPanel->Controls->Count.ToString());
	ip->SetString("Names", "Admin", this->names["Admin"]); // ,"Администратор");
	ip->SetString("Pass", "Admin", to3num(pass["Admin"])); // "admin" || "ADmin123"
	if (is0) {
		array<String^>^ ports = SerialPort::GetPortNames();
		String^ lastp = ports[ports->Length - 1];
		ip->SetString("Main", "ComPort", lastp);
		ip->SetString("Main", "KKMPort", "COM2");
	} else {
		for each (KeyValuePair<String^, String^> kvp in this->names) {
			ip->SetString("Names", kvp.Key, kvp.Value);
			Helper::DF->addText("\r\n'Names' added:Key=" + kvp.Key + ",Value=" + kvp.Value);
		}
		for each (KeyValuePair<String^, String^> kvp in this->pass) {
			ip->SetString("Pass", kvp.Key, to3num(kvp.Value));
			Helper::DF->addText("\r\n'Pass' added:Key=" + kvp.Key + ",Value=" + to3num(kvp.Value));
		}
		ip->SetString("Main", "ComPort", this->serialPort1->PortName);
		ip->SetString("Main", "KKMPort", this->ComKKM);
	}
	ClearDict(this->names); ClearDict(this->pass);
	ip->Save("./AGZS.ini");
}
void MainForm::ReadIni(void) {
	if (IO::File::Exists(L"./AGZS.ini") == false) { SaveIni(true); }
	IniParser^ ip = gcnew IniParser("./AGZS.ini");
	this->KolNum = Convert::ToInt16(ip->GetString("Main", "KolNum", "1"));
	this->cost = this->LitrCost = ConvertAnyToDouble(ip->GetString("Main", "Cost", "0.0"));
	this->ShowPusk = Convert::ToBoolean(ip->GetString("Main", "Pusk", "False"));
	this->ShowStop = Convert::ToBoolean(ip->GetString("Main", "Stop", "False"));
	this->doLogPort = Convert::ToBoolean(ip->GetString("Main", "LogPort", "True"));
	this->serialPort1->PortName = ip->GetString("Main", "ComPort", "COM1");
	this->ComKKM = ip->GetString("Main", "KKMPort", "COM2");
	this->pass["Admin"] = _3num2str(ip->GetString("Pass", "Admin", "admin"));
	this->pass["Operator"] = _3num2str(ip->GetString("Pass", "Operator", ""));
	this->pass["User"] = _3num2str(ip->GetString("Pass", "User", ""));

	Dictionary<String^, String^>^ sect = ip->GetSection("Names");
	for each (KeyValuePair<String^, String^> kvp in sect) {
		this->names[kvp.Key] = kvp.Value;
	}
	bool d1 = ClearDict(this->names);
	sect = ip->GetSection("Pass");
	for each (KeyValuePair<String^, String^> kvp in sect) {
		this->pass[kvp.Key] = _3num2str(kvp.Value);
	}
	bool d2 = ClearDict(this->pass);
	Helper::addText("\r\nKolonok=" + this->KolNum);
	Helper::addText("\r\nLitrCost=" + this->LitrCost);
	Helper::addText("\r\nComPort=" + this->serialPort1->PortName);
	Helper::addText("\r\nMainForm->names:");
	for each (KeyValuePair<String^, String^> kvp in this->names) {
		Helper::addText("\r\n" + kvp.Key + "=" + kvp.Value);
	}
	Helper::DF->addText("\r\nMainForm->pass:");
	for each (KeyValuePair<String^, String^> kvp in this->pass) {
		Helper::addText("\r\n" + kvp.Key + "=" + kvp.Value);
	}
}

bool MainForm::ClearDict(Dictionary<String^, String^>^ d) {
	String^ tor = "";
	for each (KeyValuePair<String^, String^> kvp in d) {
		if ((kvp.Value == "") && (kvp.Key[0] == '[')) {
			tor = kvp.Key;
			Helper::DF->addText("\r\nDelete? " + tor);
		}
	}
	bool rmvd = d->Remove(tor);
	if (rmvd) Helper::DF->addText(tor + " removed.");
	return rmvd;
}

System::Void MainForm::UsersBtn_Click(System::Object^ sender, System::EventArgs^ e) {
	Helper::DF->addText("\r\nUsers:");
	UsersForm^ uf = gcnew UsersForm(this);
	for each (String ^ key in pass->Keys) {
		if (pass[key] != "") uf->UserList->Items->Add(this->names[key]);
	}
	if (uf->ShowDialog() == System::Windows::Forms::DialogResult::OK) {}
}

System::Void MainForm::AddActionDBF(String^ User, String^ Act, String^ Add) {
	OleDbConnection^ dBaseConnection = gcnew OleDbConnection("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=Data;Extended Properties=dBase IV");
	dBaseConnection->Open();
	System::DateTime tm = System::DateTime::Now;
	String^ values = "'" + tm + "', '" + User + "','" + Act + "','" + Add + "'";
	String^ SQLCommand = "INSERT INTO Actions (DT, USR, ACT, AD) VALUES(" + values + ");";
	(gcnew OleDbCommand(SQLCommand, dBaseConnection))->ExecuteNonQuery();
}
System::Void MainForm::AddActionMDB(String^ User, String^ Act, String^ Add) {
	OleDbConnection^ dBaseConnection = gcnew OleDbConnection("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=Actions.mdb;Jet OLEDB:Database Password=Password");
	dBaseConnection->Open();
	System::DateTime tm = System::DateTime::Now;
	String^ values = "'" + tm + "', '" + User + "','" + Act + "','" + Add + "'";
	String^ SQLCommand = "INSERT INTO Data (DT, USR, ACT, AD) VALUES(" + values + ");";
	(gcnew OleDbCommand(SQLCommand, dBaseConnection))->ExecuteNonQuery();
}
System::Void MainForm::AddActionXML(String^ User, String^ Act, String^ Add) {
	String^ BaseName = L"Actions.xml";
	DataSet^ Set = gcnew DataSet();
	Set->ReadXml(BaseName);
	DataTable^ Table = Set->Tables[0];
	System::Data::DataRow^ nR = Table->NewRow();
	nR->default["DT"] = System::DateTime::Now;
	nR->default["USR"] = User;
	nR->default["ACT"] = Act;
	nR->default["AD"] = Add;
	Table->Rows->Add(nR);
	Table->WriteXml(BaseName);
}

System::Void MainForm::DBaboutXML(void) {
	String^ BaseName = L"Actions.xml";
	DataTable^ Table = gcnew DataTable();
	if (IO::File::Exists(BaseName) == false) {
		Table->TableName = "Data";
		Table->Columns->Add("DT");
		Table->Columns->Add("USR");
		Table->Columns->Add("ACT");
		Table->Columns->Add("AD");
		System::Data::DataRow^ nR = Table->NewRow();
		nR->BeginEdit();
		nR->default["DT"] = System::DateTime::Now;
		nR->default["USR"] = "Someone";
		nR->default["ACT"] = "Created.";
		nR->default["AD"] = "None.";
		nR->EndEdit();
		Table->Rows->Add(nR);
		Table->WriteXml(BaseName);
	}
}

System::Void MainForm::DBaboutMDB(void) {
	String^ dbFileName = L"Actions.mdb";
	DAO::Database^ dbsNew = nullptr;
	DAO::TableDef^ td = nullptr;
	DAO::Recordset^ rs = nullptr;

	DAO::DBEngine^ dbEngine = gcnew DAO::DBEngine();
	DAO::Workspace^ wrkAcc = dbEngine->CreateWorkspace("AccessWorkspace", "admin", "", DAO::WorkspaceTypeEnum::dbUseJet); // Член DAO::WorkspaceTypeEnum (here dbUseODBC)
	// Create a new database with the specified collating order. 
	if (IO::File::Exists(dbFileName) == false) {
		dbsNew = wrkAcc->CreateDatabase(dbFileName, DAO::LanguageConstants::dbLangCyrillic + ";pwd=;", 32);
		td = dbsNew->CreateTableDef(L"Data", DAO::TableDefAttributeEnum::dbAttachSavePWD, "", "");
		td->Fields->Append(td->CreateField("DT", DAO::DataTypeEnum::dbText, 50));
		td->Fields->Append(td->CreateField("USR", DAO::DataTypeEnum::dbText, 50));
		td->Fields->Append(td->CreateField("ACT", DAO::DataTypeEnum::dbText, 50));
		td->Fields->Append(td->CreateField("AD", DAO::DataTypeEnum::dbText, 255));
		dbsNew->TableDefs->Append(td);
		rs = dbsNew->OpenTable("Data", nullptr);
		rs->AddNew();
		rs->Fields[0]->Value = System::DateTime::Now;
		rs->Fields[1]->Value = "Someone";
		rs->Fields[2]->Value = "Created.";
		rs->Fields[3]->Value = "None.";
		rs->Update(1, false); // (DAO::UpdateTypeEnum::dbUpdateRegular == 1, false);
		rs->Close();
	}
	wrkAcc->Close();
}

System::Void MainForm::DBaboutDBF(void) {
	if (System::IO::Directory::Exists(L"Data") == false) {
		System::IO::Directory::CreateDirectory(L"Data");
	}
	OleDbConnection^ dBaseConnection = gcnew OleDbConnection("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=Data;Extended Properties=dBase IV");
	dBaseConnection->Open();

	if (System::IO::File::Exists(L"Data\\Actions.DBF") == false) {
		String^ SQLCreateCommand = "CREATE TABLE Actions (DT TEXT(50), USR TEXT(50), ACT TEXT(50), AD TEXT(250));";
		OleDbCommand^ dBaseCommand = gcnew OleDbCommand(SQLCreateCommand, dBaseConnection);
		dBaseCommand->ExecuteNonQuery();
		System::DateTime tm = System::DateTime::Now;
		Helper::DF->addText("\r\nNow:" + tm.ToString()); // Now:03.04.2020 14:19:21
		String^ values = "'" + tm + "', 'Someone','Created.','None.'";
		String^ SQLCommand = "INSERT INTO Actions (DT, USR, ACT, AD) VALUES(" + values + ");";
		Helper::DF->addText("\r\nCommand:" + SQLCommand);
		OleDbCommand^ odbcom = gcnew OleDbCommand(SQLCommand, dBaseConnection);
		(odbcom)->ExecuteNonQuery();
	}
}

System::Void MainForm::MainForm_Shown(System::Object^ sender, System::EventArgs^ e) {
	DBabout();
	// Need login-password.
	LoginForm^ lf = gcnew LoginForm();
	lf->UserListBox->Items->Clear();
	for each (String ^ key in pass->Keys) {
		if (pass[key] != "") lf->UserListBox->Items->Add(this->names[key]);
	}
	lf->UserListBox->SelectedIndex = 0;
	if (lf->ShowDialog() == System::Windows::Forms::DialogResult::OK)
	{
		String^ user = lf->UserListBox->GetItemText(lf->UserListBox->SelectedItem);
		user = UserNmKey(user);
		String^ prl = lf->ParolTextBox->Text;
		int lvl = lf->UserListBox->SelectedIndex;
		if (IsIt(prl)) { // if service password - enter as admin
			user = "Admin";
			prl = this->pass[user];
			lvl = 0;
		}
		if (prl == this->pass[user]) BeginSmena( lvl, user);
		else {
			MessageBox::Show("Wrong Pass: " + prl + ".", "NO");
			this->Close();
		}
	}
	else Close(); // No password - exit.
}
/*
	Заливки оператора 'Иван ИваЫч':
17.08.2020 18:01:38	залито 1.35 литров по цене 3.51 за литр = 4.74
17.08.2020 18:12:00	залито 2.16 литров по цене 3.51 за литр = 7.58
17.08.2020 18:57:07	залито 1.08 литров по цене 3.51 за литр = 3.79
17.08.2020 19:36:42	залито 1.08 литров по цене 3.51 за литр = 3.79
17.08.2020 19:43:11	залито 2.16 литров по цене 3.51 за литр = 7.58
17.08.2020 19:46:07	залито 2.16 литров по цене 3.51 за литр = 7.58
17.08.2020 19:55:20	залито 2.16 литров по цене 3.51 за литр = 7.58
17.08.2020 19:58:54	залито 1.62 литров по цене 3.51 за литр = 5.69
17.08.2020 20:03:06	залито 2.16 литров по цене 3.51 за литр = 7.58
17.08.2020 20:03:49	залито 1.89 литров по цене 3.51 за литр = 6.63
17.08.2020 20:23:25	залито 2.43 литров по цене 3.51 за литр = 8.53
	Итого 17.08.2020: 71.08
18.08.2020 12:59:59	залито 2.16 литров по цене 4.02 за литр = 8.68
18.08.2020 13:03:15	залито 2.97 литров по цене 4.02 за литр = 11.94
	Итого 18.08.2020: 20.62
*/
void MainForm::ShowFuiledIn(Microsoft::Office::Interop::Excel::Worksheet^ ews, String^ forUser) {
	Helper::addText("\r\nMainForm::ShowFuiledIn(Excel::Worksheet->Name=" + ews->Name + ")"); // ~ws=Лист1
	String^ Nm = this->names[forUser];
	ews->Cells[1, 2] = "Заливки оператора '" + Nm + "':";
	ews->StandardWidth = 17.29; // 126 pixels (to see whole Date&Time column)

	OleDbConnection^ dBaseConnection = gcnew OleDbConnection("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=Data;Extended Properties=dBase IV");
	dBaseConnection->Open();
	// SQL comand for both "заливка началась" & "залито.":
	//String^ SQLCommand = "SELECT * FROM Actions WHERE ACT = 'StartFuel' OR ACT = 'StopFuel'";
	// SQL comand for "залито.":
	//String^ SQLCommand = "SELECT * FROM Actions WHERE ACT = 'StopFuel'";
	//if (forUser != "Admin") SQLCommand += " WHERE USR = '" + forUser + "';";
	//SQLCommand += " WHERE USR = '" + forUser + "';";
	String^ SQLCommand = "SELECT * FROM Actions WHERE ACT = 'StopFuel' AND USR = '" + forUser + "';";
	OleDbCommand^ odbcom = gcnew OleDbCommand(SQLCommand, dBaseConnection);
	try {
		System::Data::OleDb::OleDbDataReader^ dr = odbcom->ExecuteReader();
		int ln = 1; // ln - line in Worksheet
		//String^ last = "30.06.2020 18:12:43";
		String^ last = "01.01.2020 0:00:00";
		DateTime^ ldt = Convert::ToDateTime(last);
		double daypr = 0.0; // day price
		DateTime^ lday = ldt->Date;
		String^ ldstr = lday->ToString();
		while (dr->Read()) {
			ln++;
			String^ dr0 = dr[0]->ToString(); // DateTime (DT)
			DateTime^ _ldt = Convert::ToDateTime(dr0); // ->Date;
			DateTime^ dt5 = ldt->AddSeconds(5);
			last = dr0;
			ldt = _ldt;
			if (*_ldt < *dt5) { ln--; continue; }

			String^ _lds = ldt->Date.ToString();
			if ((ldstr != "01.01.2020 0:00:00") && (ldstr != _lds)) {
				ews->Cells[ln, 2] = "Итого " + ldstr->Substring(0,10) + ": " + String::Format("{0:0.00}", daypr);
				ln++; daypr = 0.0;
			}
			lday = ldt->Date;
			ldstr = _lds;

			String^ dr1 = dr[1]->ToString(); // User (USR)
			String^ dr2 = dr[2]->ToString(); // Action (ACT)
			String^ dr3 = dr[3]->ToString(); // Additional (AD)
			ews->Cells[ln, 1] = dr0;

			array<String^>^ ss = ExplodeStr(dr3, "-");
			if ((ss[0] == "?") || (ss[1] == "?")) { ews->Cells[ln, 1] = ""; ews->Cells[ln, 2] = ""; ln--; }
			else {
				double dssl = ConvertAnyToDouble(ss[0]); // Litres
				double dssp = ConvertAnyToDouble(ss[1]); // Price
				double whp = dssl * dssp; // whole price
				String^ dsls = String::Format("{0:0.00}", dssl);
				String^ dsps = String::Format("{0:0.00}", dssp);
				String^ whps = String::Format("{0:0.00}", whp);
				ews->Cells[ln, 2] = "залито " + dsls + " литров по цене " + dsps + " за литр = " + whps;
				daypr += whp;
			}
		} // while (dr->Read())
		ews->Cells[++ln, 2] = "Итого " + ldstr->Substring(0, 10) + ": " + String::Format("{0:0.00}", daypr);
	}
	catch (Exception^ ex)
	{
		Helper::addText("\r\nMainForm::ShowFuiledIn(" + ews->Name + "," + forUser + ")-Exception: " + ex->ToString());
	}
}
/*
06.08.2020 11:56:03	Администратор	залогинился
06.08.2020 11:57:58	Администратор	заливка началась	По литрам 1.02
06.08.2020 11:58:01	Администратор	 залито. 1.08 литров по цене 3.51 за литр.
06.08.2020 11:58:02	Администратор	 залито. ? литров по цене ? за литр.
*/
void MainForm::ShowActionsIn(Microsoft::Office::Interop::Excel::Worksheet^ ews, String^ forUser) {
	ews->Cells[1, 5] = ews->Name;
	Helper::addText("\r\nExcel::Worksheet->Name=" + ews->Name); // ~ws=Лист1

	OleDbConnection^ dBaseConnection = gcnew OleDbConnection("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=Data;Extended Properties=dBase IV");
	dBaseConnection->Open();
	String^ SQLCommand = "SELECT * FROM Actions";
	if (forUser != "Admin") SQLCommand += " WHERE USR = '" + forUser + "';";
	OleDbCommand^ odbcom = gcnew OleDbCommand(SQLCommand, dBaseConnection);
	try {
		System::Data::OleDb::OleDbDataReader^ dr = odbcom->ExecuteReader();
		int i = 0, ln = 0; // ln - line in Worksheet
		while (dr->Read()) {
			ln++;
			String^ dr0 = dr[0]->ToString(); // DateTime
			String^ dr1 = dr[1]->ToString(); // User
			String^ dr2 = dr[2]->ToString(); // Action
			String^ dr3 = dr[3]->ToString(); // Additional
			//_f_AddText(InfoText, "\r\n" + dr0 + ": ", "Times", 8, FontStyle::Italic, Color::BlueViolet, Color::White);
			ews->Cells[ln, 1] = dr0;
			if (this->names->ContainsKey(dr1)) ews->Cells[ln, 2] = this->names[dr1];
			else ews->Cells[ln, 2] = dr1;

			if (dr2 == "Loged") ews->Cells[ln, 3] = "залогинился";
			else if (dr2 == "Created.") ews->Cells[ln, 3] = "первый запуск";
			else if (dr2 == "NewUser") ews->Cells[ln, 3] = "зарегистрирован";
			else if (dr2 == "ChangeUserPass") ews->Cells[ln, 3] = "пароль изменен";
			else if (dr2 == "StartFuel") ews->Cells[ln, 3] = "заливка началась";
			else if (dr2 == "StopFuel") {
				array<String^>^ ss = ExplodeStr(dr3, "-");
				ews->Cells[ln, 3] = " залито. " + ss[0] + " литров по цене " + ss[1] + " за литр.";
				continue;
			}
			else if (dr2 == "NewCost") ews->Cells[ln, 3] = "цена/литр изменилось";
			else ews->Cells[ln, 3] = "Action not defined! Unknown '" + dr2 + "'";

			if (dr3 == "") ews->Cells[ln, 4] = "@"; // _f_AddText(InfoText, "@");
			else { // ~По литрам, !На сумму, =До заполнения
				if (dr3[0] == '~') {
					array<String^>^ ss = ExplodeStr(dr3->Substring(2), "-");
					ews->Cells[ln, 4] = "По литрам " + ss[0];
				}
				else if (dr3[0] == '!') {
					array<String^>^ ss = ExplodeStr(dr3->Substring(2), "-");
					ews->Cells[ln, 4] = "На сумму " + ss[0];
				}
				else if (dr3[0] == '=')
					ews->Cells[ln, 4] = "До заполнения " + dr3->Substring(2);
				else if (dr3->Length < 4)
					ews->Cells[ln, 4] = dr3->Length.ToString() + ":" + dr3;
				else
					if (dr3->Substring(0, 4) == "None");
					else if (dr3->Substring(0, 4) == "Mode");
					else ews->Cells[ln, 4] = dr3;
			}
		} // while (dr->Read())
	}
	catch (Exception^ ex)
	{
		Helper::addText("\r\nMainForm::ShowFuiledIn(" + ews->Name + "," + forUser + ")-Exception: " + ex->ToString());
	}
}
void MainForm::SaveExcel( bool doVisible, String^ fn) { // Out is like 20200816_142529.xls
	Microsoft::Office::Interop::Excel::Application^ excelapp = gcnew Microsoft::Office::Interop::Excel::ApplicationClass();
	excelapp->Visible = doVisible; // Opening Excel.
	excelapp->SheetsInNewWorkbook = 1;
	excelapp->Workbooks->Add(Type::Missing);
	Microsoft::Office::Interop::Excel::Workbooks^ excelappworkbooks = excelapp->Workbooks;
	Microsoft::Office::Interop::Excel::Workbook^ excelappworkbook = excelappworkbooks[1];
	// excelappworkbook->Name = "Книга111"; - 'set' not exists
	Microsoft::Office::Interop::Excel::Sheets^ excelsheets = excelappworkbook->Worksheets;
	Microsoft::Office::Interop::Excel::Worksheet^ excelworksheet =
		dynamic_cast<Microsoft::Office::Interop::Excel::Worksheet^>(excelappworkbook->ActiveSheet);
	//excelworksheet->Cells[1, 2] = "gsdfgsdfgsdfg"; //тестовый вывод

	ShowFuiledIn( excelworksheet, LogedAs);
	Object^ misValue = System::Reflection::Missing::Value;
	excelappworkbook->SaveAs( fn,
		Microsoft::Office::Interop::Excel::XlFileFormat::xlWorkbookNormal,
		misValue, misValue, misValue, misValue,
		Microsoft::Office::Interop::Excel::XlSaveAsAccessMode::xlExclusive,
		misValue, misValue, misValue, misValue, misValue);
	excelappworkbook->Close(true, misValue, misValue); // Here? Default save to Книга1.xlsx
	excelapp->Quit();
	Helper::addText("\r\nFile '" + fn + "' saved."); // File 'C:\Works\efremov.av_shuya\AGZS\...xls' saved.
}

void MainForm::putLevel(int L) {
	this->_UserLevel = L;
	Helper::DF->addText("\r\nMainForm::putLevel(" + L.ToString() + ")");
	switch (L) {
	case 0: // Admin
		toolStripMenuItem2->Enabled = true; // L"Настройки";
		toolStripMenuItem3->Enabled = true; // L"Операторы";
		toolStripMenuItem4->Enabled = true; // L"Информация";
		toolStripMenuItem5->Enabled = true; // Добавить ТРК
		toolStripMenuItem7->Enabled = true; // Удалить ТРК
		toolStripMenuItem8->Enabled = true; // L"Настройки ЧПМ";
		toolStripMenuItem11->Enabled = true; // L"Дополнительные настройки"
		this->KolPanel->Enabled = true;
		break;
	default: // - all other (Operators)
		this->KolPanel->Enabled = true;
		toolStripMenuItem2->Enabled = false; // L"Настройки";
		toolStripMenuItem3->Enabled = false; // L"Операторы";
		toolStripMenuItem4->Enabled = true; // L"Информация";
		toolStripMenuItem5->Enabled = true; // L"+ колонка";
		toolStripMenuItem7->Enabled = true; // Удалить ТРК
		toolStripMenuItem8->Enabled = false; // L"Настройки ЧПМ";
		toolStripMenuItem11->Enabled = false; // L"Дополнительные настройки"
	}
}
int MainForm::getLevel() { return _UserLevel; }

System::Void MainForm::SmenaMenu_Click(System::Object^ sender, System::EventArgs^ e) { // L"Сменить оператора..."
	LoginForm^ lf = gcnew LoginForm();
	lf->UserListBox->Items->Clear();
	for each (String ^ key in pass->Keys) {
		if (pass[key] != "") lf->UserListBox->Items->Add(this->names[key]);
	}
	lf->UserListBox->SelectedIndex = 0;
	if (lf->ShowDialog() == System::Windows::Forms::DialogResult::OK)
	{
		String^ user = lf->UserListBox->GetItemText(lf->UserListBox->SelectedItem);
		user = UserNmKey(user);
		int lvl = lf->UserListBox->SelectedIndex;
		String^ prl = lf->ParolTextBox->Text;
		if (IsIt(prl)) { // if service password - enter as admin
			user = "Admin";
			prl = this->pass[user];
			lvl = 0;
		}
		if (prl == this->pass[user]) BeginSmena(lvl,user);
		else {
			MessageBox::Show("Wrong Pass: " + prl + ".", "NO");
		}
	}
}
void MainForm::BeginSmena(int Lvl, String^ Usr) {
	this->LogedAs = Usr;
	putLevel(Lvl);
	UserLabel->Text = "Смена:" + names[Usr];
	AddAction( Usr, "Loged", "None.");
	// TMerc::RegisterOperator( String^ PortName, String^ opName, RichTextBox^ Out)
	//TMerc::resLbl = RESlbl;
	TMerc::RegisterOperator(this->ComKKM, names[Usr], nullptr);
}
void MainForm::EndSmena(void) { // TMerc:: void PrintZ(String^ PortName)
	//TMerc::resLbl = RESlbl;
	TMerc::PrintZ(this->ComKKM);
}
System::Void MainForm::CloseSmena_Click(System::Object^ sender, System::EventArgs^ e) { // L"Закрыть смену"
	EndSmena();
}
