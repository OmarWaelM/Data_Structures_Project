#include "UI.h"
#include "Organizer.h"
#include "Patient.h"
#include "Car.h"
#include "Hospital.h"
#include "LinkedQueue.h"
#include "priQueue.h"
#include "ModifiedQ.h"
#include "ModifiedPriQ.h"
#include <conio.h>
#include <Windows.h>

void UI::Start()
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