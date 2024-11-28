#include <iostream>
#include <Windows.h>
#include <string>
#include <fstream>
#include <iomanip>
#include <conio.h>
#include <fstream>
#include "LinkedQueue.h"
#include "priQueue.h"
#include "ModifiedQ.h"
#include "ModifiedPriQ.h"
#include "Patient.h"
#include "Car.h"
#include "Hospital.h"
#include "UI.h"
#include "Organizer.h"

using namespace std;

int main(void)
{
    Organizer organizer;

    // Process the input file to populate the queues
    organizer.processInputFile();

    //Print the hospitals list to verify the data
    organizer.PrintHospitalsList();

    // Print the patients list to verify the data
    organizer.printPatientsList();

    // Print the cancellation list to verify the data
    organizer.printCancellationList();

    return 0;
}