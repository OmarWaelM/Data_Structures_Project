#pragma once
using namespace std;

enum UI_MODE {
	SILENT,
	INTERACTIVE,
};

class UI
{
private:
	UI_MODE mode;
	Organizer* org;

public:
	UI(Organizer* organizer):mode(INTERACTIVE), org(organizer) {}
	void Start();
	void Output();
	void printPage(int hospitalID);
	
};

