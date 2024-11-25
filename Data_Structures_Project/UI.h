#ifndef UI_H
#define UI_H
using namespace std;

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
	string fileName;
public:
	UI() :mode(SILENT){}
	void Start();
	bool fileExists(string& filename);
	void setInputFileName(string& filename);
	string getInputFileName();
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

	// Loop until the user provides a valid file
	while (1)
	{
		cout << "Please enter the name of the file you would like to open: ";
		cin >> name; 

		// Check if the file exists
		if (fileExists(name))
		{
			cout << "Opening file " << name << "...";
			setInputFileName(name);
			
			break;
		}
		else
		{
			cout << "Error: File " << name << " does not exist. Please try again." << endl;
		}
	}
}

//Checks whether the filename inserted by the user exists or not
bool UI::fileExists(string& filename)
{
	ifstream file;
	file.open(filename + ".txt", ios::in);
	return file.is_open(); // Return true if the file can be opened, false otherwise
}
//Sends the input file name to the Organizer
void UI::setInputFileName(string& filename)
{
	fileName = filename;
}

string UI::getInputFileName()
{
	return fileName;
}

void UI::Output()
{

}

void UI::printPage(int hospitalID)
{

}

#endif
