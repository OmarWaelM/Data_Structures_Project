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
	return 0;
}