#include "SettingsForm.h"
#include "MainForm.h"
#include "DebugForm.h"
#include "ComAbout.h"
#include "Utils.h"
#include "stdlib.h"

namespace AGZS {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::IO;
	using namespace System::IO::Ports;
	using namespace System::Threading;

System::Void SettingsForm::SettingsForm_Shown(System::Object^ sender, System::EventArgs^ e) {
	Helper::addText("\r\nSettingsForm::SettingsForm_Shown()");
	if (!this->SerialComPort->IsOpen) {
			Helper::addText("\r\nSettingsForm::SerialComPort not IsOpen!");
			doComOpen(this->SerialComPort, SerialComPort->PortName, Helper::DF->DebugTextBox);
	}
	if (this->SerialComPort->IsOpen) Helper::addText("\r\nSettingsForm::SerialComPort->IsOpen: " + this->SerialComPort->PortName);
}
System::Void SettingsForm::SettingsForm_Load(System::Object^ sender, System::EventArgs^ e) {
	Helper::addText("\r\nSettingsForm::SettingsForm_Load()");
	MainForm^ MF = dynamic_cast<MainForm^>(this->MForm);
	this->ComBox->Text = MF->serialPort1->PortName;
	if (!this->SerialComPort->IsOpen)
		doComOpen(this->SerialComPort, MF->serialPort1->PortName,Helper::DF->DebugTextBox);
	if (this->SerialComPort->IsOpen) Helper::addText("\r\nSettingsForm::SerialComPort->IsOpen: " + this->SerialComPort->PortName);
}
System::Void SettingsForm::SettingsForm_FormClosed(System::Object^ sender, System::Windows::Forms::FormClosedEventArgs^ e) {
	StopInfo();
	this->SerialComPort->Close();
}

System::String^ correctDouble(System::String^ s) { return s; }
System::String^ correctPoint(System::String^ s) {
	String^ out = s;
	if (out->Contains(",")) {
		Helper::addText("\r\n '" + out + "' contains ',' - replacing:");
		out = out->Replace(",", ".");
		Helper::addText(" '" + out + "'");
	}
	return out;
}
System::String^ correctMLinL(System::String^ s) {
	String^ out = s;
	try {
		out = out->Replace(".", Globalization::NumberFormatInfo::CurrentInfo->NumberDecimalSeparator);
		out = out->Replace(",", Globalization::NumberFormatInfo::CurrentInfo->NumberDecimalSeparator);
		double dbl = ConvertAnyToDouble(out);
		dbl = dbl / 1000;
		//out = dbl.ToString();
		out = String::Format("{0:0.00}", dbl);
	}
	catch (FormatException^ ex) {
		Helper::DF->addText("\r\n-FormatException: " + ex->ToString() + "!\r\n toWrite=" + out + "!~!\r\n");
	}
	return out;
}

// Юстировка
// АГЗС, [05.08.20 20:04] Залить это n210;            залито реально n2k9;
System::Void SettingsForm::button1_Click(System::Object^ sender, System::EventArgs^ e) { // Залить
// Павел Маленький, [05.08.20 20:52] Короче: кол-во; = > ответ = > ;р = > ИФСЁ. (without sending 'e')
	Helper::addText("\r\n~Залить-" + textBox1->Text);
	//Значения милилитров пишем тысячами а отправляем в порт делённое на 1000 значение
	String^ toWrite = textBox1->Text;
	toWrite = correctPoint(correctMLinL(toWrite)) + ";";
	kSended = false;
	PortWrite(toWrite);
	Helper::addText("\r\nMAKED: SerialComPort->Write(" + toWrite + ")-" + this->SerialComPort->ReadExisting());
}
System::Void SettingsForm::button2_Click(System::Object^ sender, System::EventArgs^ e) { // Залито
	String^ toWrite = textBox2->Text;
	Helper::addText("\r\n~Залитo!-" + toWrite);
	/*Калибровка точности колонки k9.789; (k маленькая, колличество реально залитого топлива в мерник)*/
	toWrite = correctPoint(correctMLinL(toWrite));
	toWrite = "k" + toWrite + ";"; // "n" + setKolNumStr + 
	kSended = true;
	PortWrite(toWrite);
	Helper::addText("\r\nMAKED: SerialComPort->Write(" + toWrite + ")-" + this->SerialComPort->ReadExisting());
}

// Обновить
System::Void SettingsForm::StartInfo(void) {
	this->UpdateBtn->Enabled = false;
	UpdateBtn->Text = "Обновляю...";
	lbl1i->Enabled = false;
	lbl2i->Enabled = false;
	lbl3i->Enabled = false;
	lbl4i->Enabled = false;
	this->pStr = ""; this->tStr = ""; this->ilStr = ""; this->lmStr = "";
	this->_InfoStage = 0;
	this->InfTimer->Start();
}
System::Void SettingsForm::StopInfo(void) {
	this->InfTimer->Stop();
	this->InfTimer->Enabled = false;
	this->UpdateBtn->Enabled = true;
	UpdateBtn->Text = "Обновить";
	lbl1i->Enabled = true;
	lbl2i->Enabled = true;
	lbl3i->Enabled = true;
	lbl4i->Enabled = true;
	lblDT->Text = "Обновлено: " + DateTime::Now.ToString();
}

System::Void SettingsForm::button3_Click(System::Object^ sender, System::EventArgs^ e) { // UpdateBtn
	StartInfo();
	return;
	// 1i = P/L (lblPL), 2i = T (lblT), 3i = I/L (lblIL), 4i = L/M (lblLM)
	lblPL->Text = "цену за литр";
	lblT->Text = "температура сжиженного газа";
	lblIL->Text = "импульсов на литр";
	lblLM->Text = "расход литров в минуту";
	if (!SerialComPort->IsOpen) doComOpen(SerialComPort, SerialComPort->PortName);
	if (SerialComPort->IsOpen) {
		String^ dtstr = String::Format("{2:D2}.{1:D2}.{0:D4}-{3:D2}:{4:D2}:{5:D2}",
			DateTime::Now.Year, DateTime::Now.Month, DateTime::Now.Day,
			DateTime::Now.Hour, DateTime::Now.Minute, DateTime::Now.Second);
		Helper::addText("\r\n~Info started: " + dtstr);
		String^ was = lblPL->Text;
		if (check1i->Checked) {
			Helper::DF->addText("\r\n>check1i->Checked");
			PortWrite("1i");
			Thread::Sleep(750);
			lblPL->Text = pStr;
		}
		was = lblT->Text;
		if (check2i->Checked) {
			Helper::DF->addText("\r\n>check2i->Checked");
			PortWrite("2i");
			Thread::Sleep(750);
			lblPL->Text = tStr;
		}
		was = lblIL->Text;
		if (check3i->Checked) {
			Helper::DF->addText("\r\n>check3i->Checked");
			PortWrite("3i");
			Thread::Sleep(800);
			lblPL->Text = ilStr;
		}
		was = lblLM->Text;
		if (check4i->Checked) {
			Helper::DF->addText("\r\n>check4i->Checked");
			PortWrite("4i");
			Thread::Sleep(800);
			lblPL->Text = lmStr;
		}
		dtstr = String::Format("{0:D4}.{1:D2}.{2:D2}-{3:D2}:{4:D2}:{5:D2}",
			DateTime::Now.Year, DateTime::Now.Month, DateTime::Now.Day,
			DateTime::Now.Hour, DateTime::Now.Minute, DateTime::Now.Second);
		//lblDT->Text = "Обновлено: " + dtstr;
		lblDT->Text = "Обновлено: " + DateTime::Now.ToString();
	}
	else lblDT->Text = "CoM '" + SerialComPort->PortName + "' NE открыт.";
	StopInfo();
}

// 1i = P/L (lblPL), 2i = T (lblT), 3i = I/L (lblIL), 4i = L/M (lblLM)
System::Void SettingsForm::lbl1i_Click(System::Object^ sender, System::EventArgs^ e) {
	pStr = "";
	PortWrite("1i");
	lblDT->Text = "Обновлено: " + DateTime::Now.ToString();
}
System::Void SettingsForm::lbl2i_Click(System::Object^ sender, System::EventArgs^ e) {
	tStr = "";
	PortWrite("2i");
	lblDT->Text = "Обновлено: " + DateTime::Now.ToString();
}
System::Void SettingsForm::lbl3i_Click(System::Object^ sender, System::EventArgs^ e) {
	ilStr = "";
	PortWrite("3i");
	lblDT->Text = "Обновлено: " + DateTime::Now.ToString();
}
System::Void SettingsForm::lbl4i_Click(System::Object^ sender, System::EventArgs^ e) {
	lmStr = "";
	PortWrite("4i");
	lblDT->Text = "Обновлено: " + DateTime::Now.ToString();
}

void SettingsForm::OutReceived(void) {
	// Павел Маленький, [05.08.20 20:52] Короче: кол-во; = > ответ = > ;р = > ИФСЁ. (without sending 'e') - USTIR
	this->Invoke(gcnew Action<String^>(&Helper::addText), "\r\n-SettingsForm::OutReceived(" + comStr + ")");
	if (lastOuted != comStr) {
		ReceivedPacket += comStr;
		lastOuted = comStr;
		int rn = ReceivedPacket->IndexOf("\r\n");
		//if (comStr->Contains("\r\n"))
		if (rn >= 0)
		{
			_Received = ReceivedPacket;
			//String^ befrn = ReceivedPacket->Substring(0, rn);
			//String^ aftrn = ReceivedPacket->Substring(rn + 2);
			//this->Invoke(gcnew Action<String^>(&Helper::addText), "\r\n!Contains '\\r\\n':var befrn=" + befrn + ",aftrn=" + aftrn);
			int kNum = 0;
			String^ sub = ReceivedPacket->Substring(0, 1);
			try {
				kNum = Convert::ToInt32(sub);
			}
			catch (Exception^ E) {
				this->Invoke(gcnew Action<String^>(&Helper::addText),
					"\r\n~SettingsForm::OutReceived(void) Exception:" + E->Message);
			}
			ReceivedPacket = ReceivedPacket->Substring(1); // все ответы начинаются с номера (got kNum)
			ReceivedPacket = ReceivedPacket->Trim(); // Concating \r\n at the end...
			int p = ReceivedPacket->IndexOf("!");
			if (p >= 0) UstGroupBox->Text = "Юстировка: заправка закончена.";
			try {
				wchar_t wc0 = ReceivedPacket[0];
				if (wc0 == '$') {
					pStr = ReceivedPacket;
					lblPL->Text = pStr;
				}
				else
					if (wc0 == 't') {
						tStr = ReceivedPacket;
						lblT->Text = tStr;
					}
					else
						if (wc0 == 'I') {
							ilStr = ReceivedPacket;
							lblIL->Text = ilStr;
						}
						else
							if (ReceivedPacket->Contains("L/M")) {
								lmStr = ReceivedPacket;
								lblLM->Text = lmStr;
							}
							else unStr = ReceivedPacket;
				if (Char::IsDigit(wc0)) {
					// Проверяем правильность ответа того что отослали (n21.55;) с тем что пришло (21.55;?)
					// подтверждение правильности ответа: n2p
					//String^ s = "n" + kNum + ReceivedPacket;
					//if (s == sWrtd)
					// Приходит и на "к"!
					if (kSended)
						UstGroupBox->Text = "Юстировка: откалибровано.";
					else PortWrite("p");
					//else SendReset(kNum);
						//UstGroupBox->Text = "Юстировка: нажмите ПУСК на колонке";
				}
				else if ((wc0 == 'r') && (ReceivedPacket == "restart")) {
					ResetLbl->Visible = true;
					ResetLbl->Text = "Перезапущено:" + kNum;
					this->Invoke(gcnew Action<String^>(&Helper::addText), "\r\n" + kNum + " 'restart' HERE!.");
				}
				// После посыла р" жду "2pok", после посыла "е" и начала заправки жду "2ок".
				else if (wc0 == 'p') { // OK
					wchar_t wc1 = ReceivedPacket[1], wc2 = ReceivedPacket[2];
					if ((wc1 == 'o') && (wc2 == 'k')) {
						UstGroupBox->Text = "Юстировка: нажмите ПУСК на колонке";
						this->Invoke(gcnew Action<String^>(&Helper::addText), "\r\n'pok'HERE!.");
					}
				} // if (wc0 == 'p')
				else if (wc0 == 'o') { // OK
					wchar_t wc1 = ReceivedPacket[1];
					if (wc1 == 'k') {
						UstGroupBox->Text = "Юстировка: заправка началась";
						this->Invoke(gcnew Action<String^>(&Helper::addText), "\r\n'ok'HERE!.");
					}
				} // if (wc0 == 'o')
			}
			catch (Exception^ ex)
			{
				this->Invoke(gcnew Action<String^>(&Helper::addText), "\r\n>SettingsForm::OutReceived-Exception: " + ex->ToString());
			}
			ReceivedPacket = "";
		} // if (comStr->Contains("\r\n"))
	} // if (lastOuted != comStr)
} // void SettingsForm::OutReceived(void)
void SettingsForm::ErrReceived(void) {
	this->Invoke(gcnew Action<String^>(&Helper::addText), "\r\n-SettingsForm::ErrReceived:" + errStr);
}
System::Void SettingsForm::SerialComPort_DataReceived(System::Object^ sender, System::IO::Ports::SerialDataReceivedEventArgs^ e) {
	comStr = this->SerialComPort->ReadExisting();
	LogReceived(comStr);
	this->Invoke(gcnew Action(this, &SettingsForm::OutReceived));
}
System::Void SettingsForm::SerialComPort_ErrorReceived(System::Object^ sender, System::IO::Ports::SerialErrorReceivedEventArgs^ e) {
	this->errStr = e->ToString();
	LogReceived(errStr);
	this->Invoke(gcnew Action(this, &SettingsForm::ErrReceived));
}
System::Void SettingsForm::InfTimer_Tick(System::Object^ sender, System::EventArgs^ e) {
	if (!this->SerialComPort->IsOpen) {
		this->Invoke(gcnew Action<String^>(&Helper::addText), "\r\n=SerialComPort closed.");
		return;
	}
	this->Invoke(gcnew Action<String^>(&Helper::addText), "<IT:Stage=" + _InfoStage + ">");
	bool is1i = check1i->Checked;
	bool is2i = check2i->Checked;
	bool is3i = check3i->Checked;
	bool is4i = check4i->Checked;
	if (!_InfoStage) {
		if (is1i) {
			PortWrite("1i");
			this->Invoke(gcnew Action<String^>(&Helper::addText), "\r\n-Writed '1i'");
		}
		_InfoStage = 1; return;
	}
	if (_InfoStage == 1) {
		if ((is1i) && (this->pStr == "")) return;
		else {
			if (is2i) {
				PortWrite("2i");
				this->Invoke(gcnew Action<String^>(&Helper::addText), "-Writed '2i'");
			}
			_InfoStage = 2; return;
		}
	}
	if (_InfoStage == 2) {
		if ((is2i) && (this->tStr == "")) return;
		else {
			if (is3i) {
				PortWrite("3i");
				this->Invoke(gcnew Action<String^>(&Helper::addText), "-Writed '3i'");
			}
			_InfoStage = 3; return;
		}
	}
	if (_InfoStage == 3) {
		if ((is3i) && (this->ilStr == "")) return;
		else {
			if (is4i) {
				PortWrite("4i");
				this->Invoke(gcnew Action<String^>(&Helper::addText), "-Writed '4i'");
			}
			_InfoStage = 4; return;
		}
	}
	if (_InfoStage == 4) {
		StopInfo();
	}
}
System::Void SettingsForm::LogReceived(String^ s)
{
	MainForm^ MF = safe_cast<MainForm^>(this->MForm);
	if (MF->sw) MF->sw->WriteLine(DateTime::Now.ToString() + ": Received '" + s->Trim() + "'");
	return System::Void();
}
System::Void SettingsForm::ComOpenButn_Click(System::Object^ sender, System::EventArgs^ e) { // Открыть порт
	doComOpen(SerialComPort, ComBox->Text);
	if (SerialComPort->IsOpen)
		MessageBox::Show( "Порт " + SerialComPort->PortName + " открыт.", "Открытие");
}

System::Void SettingsForm::PortWrite(String^ s)
{
	String^ toWrite = "n" + setKolNumStr + s;
	Helper::addText("\r\nSettingsForm::PortWrite('" + s + "'): setKolNumStr=" + setKolNumStr + ",writed:" + toWrite);
	if (!SerialComPort->IsOpen) doComOpen(SerialComPort, SerialComPort->PortName);
	this->SerialComPort->Write(toWrite);
	MainForm^ MF = safe_cast<MainForm^>(this->MForm);
	if (MF->sw) MF->sw->WriteLine(DateTime::Now.ToString() + ": Writed '" + s + "'");
	return System::Void();
}
System::Void SettingsForm::PortWriteLikeis(String^ s)
{
	Helper::addText("\r\nSettingsForm::PortWriteLikeis('" + s + "')");
	if (!SerialComPort->IsOpen) doComOpen(SerialComPort, SerialComPort->PortName);
	this->SerialComPort->Write(s);
	MainForm^ MF = safe_cast<MainForm^>(this->MForm);
	if (MF->sw) MF->sw->WriteLine(DateTime::Now.ToString() + ": Writed '" + s + "'");
	return System::Void();
}

System::Void SettingsForm::CastBtn_Click(System::Object^ sender, System::EventArgs^ e) { // Назначить N => N
	wchar_t N = 'r';
	String^ toWrite = "n" + comboBox1->Text + N + comboBox2->Text + ";";
	PortWriteLikeis(toWrite);
}
// «РЕСЕТ»: n+номер& => restart: В случае с 2й колонкой 2restart
System::Void SettingsForm::ResetBtn_Click(System::Object^ sender, System::EventArgs^ e) {
	String^ toWrite = "n" + KolNumBox->Text + "&";
	ResetLbl->Visible = true;
	ResetLbl->Text = toWrite;
	PortWriteLikeis(toWrite);
}

System::Void SettingsForm::SendE(int n) { // Пуск из программы
	if (SerialComPort->IsOpen) {
		PortWrite("e");
		this->Invoke(gcnew Action<String^>(&Helper::addText), "\r\n'e' sended for n=" + n);
	}
	else Helper::addText("\r\nPort closed for 'e'.");
}
// 28.07: символ № (0xFC) => 'r', "0x1B" => "с".
System::Void SettingsForm::SendS(int n) { // стоп заправки сделал по кнопке ESC '0x1B', 28.07: "0x1B" на "с".
	if (SerialComPort->IsOpen) {
		PortWrite("c");
		this->Invoke(gcnew Action<String^>(&Helper::addText), "\r\n'0x1B'('c') sended for n=" + n);
	}
	else {
		Helper::addText("\r\nPort closed for '0x1B'('c').");
	}
}
System::Void SettingsForm::SendReset(int n) { // '&'
	if (SerialComPort->IsOpen) {
		PortWrite("&");
		this->Invoke(gcnew Action<String^>(&Helper::addText), "\r\n'&' sended for n=" + n);
	}
	else Helper::addText("\r\nPort closed for '&'.");
}

}
