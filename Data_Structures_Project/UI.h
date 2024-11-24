#ifndef UI_H
#define UI_H
using namespace std;
#include "Organizer.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

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
	UI(Organizer* organizer) :mode(SILENT), org(organizer) {}
	void Start(Organizer& organizer);
	bool fileExists(const string& filename);
	void sendInputFile(const string& filename, Organizer& organizer);
	void Output();
	void printPage(int hospitalID);

};


void UI::Start(Organizer& organizer)
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

	// Loop until the user provides a valid file
	while (1)
	{
		cout << "Please enter the name of the file you would like to open: ";
		cin >> name; 

		// Check if the file exists
		if (fileExists(name))
		{
			cout << "Opening file " << name << "...";

			// Pass the file name to sendInputFile
			sendInputFile(name, organizer);
			break;
		}
		else
		{
			cout << "Error: File " << name << " does not exist. Please try again." << endl;
		}
	}
}

//Checks whether the filename inserted by the user exists or not
bool UI::fileExists(const string& filename)
{
	ifstream file(filename);
	return file.is_open(); // Return true if the file can be opened, false otherwise
}
//Sends the input file name to the Organizer
void UI::sendInputFile(const string& filename, Organizer& organizer)
{
	// Pass the file name to the Organizer to process it
	organizer.processInputFile(filename);
}

void UI::Output()
{

}

void UI::printPage(int hospitalID)
{

}

#endif
