using namespace std;

#include <iostream>
#include <string>
#include "Organizer.h" // Include the Organizer class header

using namespace std;

int main() {
    // Create an instance of Organizer
    Organizer organizer;

    // Input file name
    string inputFileName = "hospital_data.txt"; // Replace with the actual file name

    // Process the input file
    organizer.processInputFile(inputFileName);

    // Read hospital data
    organizer.readHospitalData();

    // Print hospitals and the distance matrix
    organizer.PrintHospitals();

    return 0;
}
