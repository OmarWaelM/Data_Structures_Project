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
	UI(Organizer* or):mode(INTERACTIVE), org(or) {}
	void Start();
	void Output();
	void printPage(int hospitalID);
	
};


void UI::Start(Organizer* org)
{
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
}

void UI::Output()
{

}

void UI::printPage(int hospitalID)
{

}

#endif