#ifndef UI_H
#define UI_H
using namespace std;

class Organizer;

enum UI_MODE {
	SILENT,
	INTERACTIVE,
};

class UI
{
private:
	UI_MODE mode;
	Organizer* org;
public:
	UI(Organizer* organizer):mode(SILENT), org(organizer) {}
	void Start();
	void Output();
	void printPage(int hospitalID);
	
};


void UI::Start()
{
	//Choosing mode
	mode = SILENT;
	printf("\033c");
	cout << "Choose the mode you would like to use:" << endl;
	cout << "SILENT		";
	cout << "<----";
	cout << endl;
	cout << "INTERACTIVE";
	cout << endl;

	while (GetAsyncKeyState(VK_RETURN) == 0)
	{
		if (GetAsyncKeyState(VK_UP))
		{
			mode = SILENT;
			printf("\033c");
			cout << "Choose the mode you would like to use:" << endl;
			cout << "SILENT		";
			cout << "<----";
			cout << endl;
			cout << "INTERACTIVE";
			cout << endl;
		}

		if (GetAsyncKeyState(VK_DOWN))
		{
			mode = INTERACTIVE;
			printf("\033c");
			cout << "Choose the mode you would like to use:" << endl;
			cout << "SILENT			";

			cout << endl;
			cout << "INTERACTIVE	";
			cout << "<----";
			cout << endl;
		}
	}
	//Getting file input name
	printf("\033c");
	string name;
	cout << "Please enter the name of the file you would like to open: ";
	cin >> name;
	cout << "Opening file " << name << "...";
}

void UI::Output()
{

}

void UI::printPage(int hospitalID)
{

}

#endif