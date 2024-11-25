using namespace std;
#include <iostream>
#include <Windows.h>
#include "UI.h"
#include "Organizer.h"

int main() {
    Organizer organizer;
    organizer.processInputFile();
    organizer.readHospitalData();
    organizer.PrintHospitals();

    return 0;
}