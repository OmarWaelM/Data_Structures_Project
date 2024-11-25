#ifndef UI_H
#define UI_H
using namespace std;
#include <conio.h>

enum UI_MODE {
	SILENT,
	INTERACTIVE,
};

class UI
{
private:
	UI_MODE mode;
	string inFileName;
	string outFileName;
public:
	UI():mode(SILENT) {}
	void Start();
	void Output(int timestep, Hospital** h, int nOfHosp, priQueue<Car*>* backCars, ModifiedPriQ<Car*>* outCars, LinkedQueue<Patient*>* finished);
};


void UI::Start()
{
	//Choosing mode
	mode = SILENT;
	printf("\033c");
	cout << "Choose the mode you would like to use (Use Arrow Keys):" << endl;
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
			cout << "Choose the mode you would like to use (Use Arrow Keys):" << endl;
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
			cout << "Choose the mode you would like to use (Use Arrow Keys):" << endl;
			cout << "SILENT			";

			cout << endl;
			cout << "INTERACTIVE	";
			cout << "<----";
			cout << endl;
		}
	}
	//Getting input file name
	printf("\033c");
	cout << "Please enter the name of the file you would like to open: ";
	cin >> inFileName;
	//Getting output file name
	printf("\033c");
	cout << "Please enter the name of the file you would like to save to: " << endl;
	cin >> outFileName;
	printf("\033c");
}

void UI::Output(int timestep, Hospital** h, int nOfHosp, priQueue<Car*>* backCars, ModifiedPriQ<Car*>* outCars, LinkedQueue<Patient*>* finished)
{
	for (int i = 0; i < nOfHosp; i++)
	{
		cout << "Current Timestep: " << timestep << endl;
		cout << *h[i];
		cout << "-------------------------------------------------" << endl;
		cout << outCars->getCount() << " ==> Out cars: " << *outCars << endl;
		cout << backCars->getCount() << " <== Back cars: " << *backCars << endl;
		cout << "-------------------------------------------------" << endl;
		cout << finished->getCount() << " finished patients: " << *finished << endl;
		cout << "Press any key to display next hospital" << endl;
		_getch();
	}
}

#endif