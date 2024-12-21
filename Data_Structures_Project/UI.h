#ifndef UI_H
#define UI_H

using namespace std;

enum UI_MODE {
	SILENT,
	INTERACTIVE,
};

class UI
{
private:
	UI_MODE mode;
	string fileName;
	string outFileName;
  
public:
	//Constructor
	UI() :mode(SILENT){}

	//GUI startup function
	void Start();

	bool fileExists(string& filename);

	//Filename getters
	string getInputFileName() { return fileName; }
	string getOutputFileName() { return outFileName; }

	//Formatted output function
	void Output(int timestep, Hospital** h, int nOfHosp, priQueue<Car*>* backCars, ModifiedPriQ* outCars, LinkedQueue<Patient*>* finished, priQueue<Car*>* checkup);
	
	~UI();
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
			Sleep(100);
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
			Sleep(100);
		}
	}

	//Getting input file name
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
			fileName = name;
			break;
		}
		else
		{
			cout << "Error: File " << name << " does not exist. Please try again." << endl;
		}
	}

	//Getting output file name
	printf("\033c");
	cout << "Please enter the name of the file you would like to save to: " << endl;
	cin >> outFileName;
	printf("\033c");
}

//Checks whether the filename inserted by the user exists or not
bool UI::fileExists(string& filename)
{
	ifstream file;
	file.open(filename + ".txt", ios::in);
	return file.is_open(); // Return true if the file can be opened, false otherwise
}

//Prints lists with proper formatting
void UI::Output(int timestep, Hospital** h, int nOfHosp, priQueue<Car*>* backCars, ModifiedPriQ* outCars, LinkedQueue<Patient*>* finished, priQueue<Car*>* checkup)
{
	if (mode == INTERACTIVE)
	{
		for (int i = 0; i < nOfHosp; i++)
		{
			Sleep(100);
			printf("\033c");
			cout << "Current Timestep: " << timestep << endl;
			cout << *h[i];
			cout << "-------------------------------------------------" << endl;
			cout << outCars->getCount() << " ==> Out cars: " << *outCars << endl;
			cout << backCars->getCount() << " <== Back cars: " << *backCars << endl;
			cout << checkup->getCount() << " Checkup cars: " << *checkup << endl;
			cout << "-------------------------------------------------" << endl;
			cout << finished->getCount() << " finished patients: " << *finished << endl;
			cout << "Press any key to display next hospital" << endl;
			_getch();
		}
	}
}

UI::~UI()
{
	cout << "Simulation ends, Output file created." << endl;
}

#endif
