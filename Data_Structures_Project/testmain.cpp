using namespace std;
#include <iostream>
#include "UI.h"
#include "Organizer.h"

int main() {
    Organizer organizer;
    organizer.processInputFile();
    organizer.readHospitalData();
    organizer.PrintHospitals();

    return 0;
}