#include <iostream>
#include <cctype>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>

using namespace std;

const string MyClientsFile = "MyClients.txt";//Global Variable


void TransacitonScreen();
void LoginScreen();

struct stClientData {

	string Name, AccountNumber = "", PIN, PhoneNumber = "";
	double  Balance = 0;
	bool MarkForDelete = false;

};

stClientData Admin;//the Client data when he login to ATM system;


enum enTransactionsScreen
{
	QuickWithdraw = 1, NormalWithdraw = 2, eDeposit = 3, CheckBalance = 4, Logout = 5
};

// ------------


vector <string> SpliteEachWordInString(string S1, string Delimeter) {

	vector <string> vString;

	short Position = 0;
	string Word;


	while ((Position = S1.find(Delimeter)) != string::npos) {

		Word = S1.substr(0, Position);

		if (Word != "") {
			vString.push_back(Word);
		}

		S1.erase(0, Position + Delimeter.length());
	}

	if (S1 != "") {
		vString.push_back(S1);
	}

	return vString;
}

stClientData ConvertLineToRecord(string Line) {

	vector <string> ClientData;
	stClientData Client;
	ClientData = SpliteEachWordInString(Line, "#//#");

	Client.AccountNumber = ClientData[0];
	Client.PIN = ClientData[1];
	Client.Name = ClientData[2];
	Client.PhoneNumber = ClientData[3];
	Client.Balance = stod(ClientData[4]);


	return Client;
}

string ConvertRecordToLine(stClientData Client, string Seperator = "#//#")
{
	string stClientRecord = "";
	stClientRecord += Client.AccountNumber + Seperator;
	stClientRecord += Client.PIN + Seperator;
	stClientRecord += Client.Name + Seperator;
	stClientRecord += Client.PhoneNumber + Seperator;
	stClientRecord += to_string(Client.Balance);
	return stClientRecord;
}

vector <stClientData> LoadFileDataToVector(string FileName) {

	fstream MyFile;
	vector <stClientData> ClientData;

	MyFile.open(FileName, ios::in);

	if (MyFile.is_open()) {

		string Line;
		stClientData Client;

		while (getline(MyFile, Line)) {

			Client = ConvertLineToRecord(Line);
			ClientData.push_back(Client);
		}

		MyFile.close();
	}

	return ClientData;
}

vector <stClientData> LoadVectorDataToFile(vector <stClientData>& vClients, string FileName) {

	string Line;

	fstream MyFile;

	MyFile.open(FileName, ios::out);

	if (MyFile.is_open()) {

		for (stClientData& Client : vClients) {

			if (Client.MarkForDelete == false) {

				Line = ConvertRecordToLine(Client, "#//#");
				MyFile << Line << endl;
			}
		}

		MyFile.close();
	}

	return vClients;
}


bool FindClientByAccountNumber(string UserName, string PIN, stClientData& Client) {

	vector <stClientData> FileData = LoadFileDataToVector(MyClientsFile);

	for (stClientData& C : FileData) {

		if (C.AccountNumber == UserName && C.PIN == PIN) {

			Client = C;
			return true;
		}
	}

	return false;
}



//  Transactions Screen :

bool ValidateClient(stClientData& Client) {

	if (FindClientByAccountNumber(Client.AccountNumber, Client.PIN, ::Admin)) {
		return true;
	}
	else {
		return false;
	}
}


void GoBackToTransactionsScreen() {

	cout << "\n\nPress any key to go to Transactions Screen ..." << endl;
	system("pause>0");

	TransacitonScreen();
}

// Deposit :
int AmountOfOperation() {

	int Num = 0;

	do
	{
		cout << "\nPlease Enter an Amount multiple 5's ? ";
		cin >> Num;

	} while (Num % 5 != 0);

	return Num;
}

bool  ChangeClientBalancy(vector <stClientData>& FileData, string Target, int Amount) {


	char DoTheOperation = 'y';

	cout << "\nAre u shire that you want to do this Transaction ? y/n ";
	cin >> DoTheOperation;

	if (toupper(DoTheOperation) == 'Y') {

		for (stClientData& C : FileData) {

			if (C.AccountNumber == Target) {

				C.Balance = C.Balance + Amount;
				LoadVectorDataToFile(FileData, MyClientsFile);
				cout << "\nThe Transaction is Done Successfully, New Balanc is : " << C.Balance;

			}
		}
		return true;
	}

	return false;
}

double ReadDepositAmount() {

	double Amount = 0;

	while (Amount <= 0) {
		cout << "\nPlease Enter a Positive Amount ?? ";
		cin >> Amount;
	}

	return Amount;
}

void ShowDepositScreen() {

	cout << "\n=================================\n";
	cout << "*** Deposit Screen ***";
	cout << "\n\=================================\n";

	double Amount = ReadDepositAmount();

	vector <stClientData> vClients = LoadFileDataToVector(MyClientsFile);
	ChangeClientBalancy(vClients, ::Admin.AccountNumber, Amount); // to change user balance in file

	::Admin.Balance += Amount; // to change user balance in screen 

}

//Withdraw screen

// Quick Withdraw 

short ReadQuickDrawChoice() {

	short Num = 0;
	do
	{
		cout << "\nChoose What to WithDraw From [1] to [9] ? ";
		cin >> Num;

	} while (Num < 0 || Num > 9);

	return Num;
}

void ShowQucikWithDrawScreen() {

	cout << "\n=======================================\n";
	cout << "\t***Quick Withdraw Screen ***";
	cout << "\n\=======================================\n";

	cout << "\t[1] 20 \t\t";
	cout << "\t[2] 50\n";
	cout << "\t[3] 100 \t";
	cout << "\t[4] 200\n";
	cout << "\t[5] 400 \t";
	cout << "\t[6] 600\n";
	cout << "\t[7] 800 \t";
	cout << "\t[8] 1000\n";
	cout << "\t[9] Exit\n";
	cout << "\n\=======================================\n";

}

short DoQuickWithDrawChoice(short Choise) {

	switch (Choise)
	{
	case 1:
		return 20;

	case 2:
		return 50;

	case 3:
		return 100;

	case 4:
		return 200;

	case 5:
		return 400;

	case 6:
		return 600;

	case 7:
		return 800;

	case 8:
		return 1000;

	default:
		return 0;

		break;
	}
}

void QucikWithDrawScreen() {

	ShowQucikWithDrawScreen();

	cout << "\nYour Balance is : " << ::Admin.Balance;

	short UserOption = ReadQuickDrawChoice();

	// to return to Main menue
	if (UserOption == 9)
		return;


	short Amount = DoQuickWithDrawChoice(UserOption);

	while (Amount > ::Admin.Balance) {

		cout << "\nThe Amount is Extend the Total Balance, You can withdraw up to : " << ::Admin.Balance << endl;
		GoBackToTransactionsScreen();
		return;
	}

	vector <stClientData> vClients = LoadFileDataToVector(MyClientsFile);
	ChangeClientBalancy(vClients, ::Admin.AccountNumber, Amount * -1); // to change user balance in file

	::Admin.Balance -= Amount; // to change user balance in screen 
}


// Normal WithDraw
void ShowWithDrawScreen() {

	cout << "\n===================================\n";
	cout << "\t***Normal Withdraw Screen ***";
	cout << "\n\===================================\n";


	int Amount = AmountOfOperation();

	while (Amount > ::Admin.Balance) {

		cout << "\nThe Amount is Extend the Total Balance, You can withdraw up to : " << ::Admin.Balance << endl;
		Amount = AmountOfOperation();

	}

	vector <stClientData> vClients = LoadFileDataToVector(MyClientsFile);
	ChangeClientBalancy(vClients, ::Admin.AccountNumber, Amount * -1); // to change user balance in file

	::Admin.Balance -= Amount; // to change user balance in screen 

}

// Check Balance 

void CheckBalanceScreen() {

	cout << "\n===================================\n";
	cout << "\t*** Balance Screen ***";
	cout << "\n\===================================\n";


	cout << "Your Balance is : " << ::Admin.Balance << endl;

}
void DoingTransactionsChoice(enTransactionsScreen Choice) {

	switch (Choice) {

	case enTransactionsScreen::QuickWithdraw:
		system("cls");
		QucikWithDrawScreen();
		GoBackToTransactionsScreen();
		break;

	case enTransactionsScreen::NormalWithdraw:
		system("cls");
		ShowWithDrawScreen();
		GoBackToTransactionsScreen();
		break;

	case enTransactionsScreen::eDeposit:
		system("cls");
		ShowDepositScreen();
		GoBackToTransactionsScreen();
		break;

	case enTransactionsScreen::CheckBalance:
		system("cls");
		CheckBalanceScreen();
		GoBackToTransactionsScreen();
		break;

	case enTransactionsScreen::Logout:
		system("cls");
		LoginScreen();
		break;

	}
}

short ReadTransactionsScreenNumber() {

	short Num = 0;
	do {
		cout << "Chosse what do you want to do ?? [1/5] ";
		cin >> Num;

	} while (Num < 1 || Num > 5);

	return Num;
}
void showTransactionsScreen() {

	system("cls");

	cout << "\n==================================================\n";
	cout << "\t\t Transactions Screen \t\t\t";
	cout << "\n==================================================\n\n";
	cout << "\t" << "[1] Quick Withdraw" << endl;
	cout << "\t" << "[2] Normal Withdraw" << endl;
	cout << "\t" << "[3] Deposit" << endl;
	cout << "\t" << "[4] Check Balance" << endl;
	cout << "\t" << "[5] Log Out" << endl;
	cout << "\n==================================================\n\n";
}
void TransacitonScreen() {

	showTransactionsScreen();
	DoingTransactionsChoice((enTransactionsScreen)ReadTransactionsScreenNumber());
}

void LoginScreen() {

	bool LoginFaild = false;
	stClientData Client;

	do
	{
		system("cls");

		cout << "=======================\n";
		cout << "     Login Screen";
		cout << "\n=======================\n";

		if (LoginFaild) {
			cout << "\nInvalid UserName Or Password, Try Again\n";
		}

		cout << "Please Enter UserName ??";
		getline(cin >> ws, Client.AccountNumber);

		cout << "\nPlease Enter Password ??";
		cin >> Client.PIN;

		LoginFaild = !ValidateClient(Client);

	} while (LoginFaild);

	TransacitonScreen();
}
int main() {

	LoginScreen();
	return 0;
}


