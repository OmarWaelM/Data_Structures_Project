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
	Car C1(15, 2, SC, 15);
	C1.AssignPatient(&P1);
	cout << "Patient1: " << P1 << endl;
	cout << "Car1: " << C1 << endl;
	return 0;
}