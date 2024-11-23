#include <iostream>
#include "Organizer.h"
using namespace std;

int main() 
{
	int x;
	cout << "before clear" << endl;
	cin >> x;
	printf("\033c");
	cout << "after clear" << endl;
	Patient P1(1, 5, 2, 15, EP, 5);
	Patient P2(2, 7, 2, 20, SP);
	Patient P3(3, 7, 2, 20, NP);
	Patient P4(4, 7, 2, 20, NP);

	Car C1(15, 2, SC, 15);
	Hospital H1(2);
	H1.addCarToList(&C1);
	H1.addPatientToList(&P1);
	H1.addPatientToList(&P2);
	H1.addPatientToList(&P3);
	H1.addPatientToList(&P4);

	C1.AssignPatient(&P1);
	cout << H1 << endl;

	return 0;
}