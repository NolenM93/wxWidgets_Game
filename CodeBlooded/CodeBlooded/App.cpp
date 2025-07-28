#include "App.h"
#include "MainFrame.h"

bool App::OnInit() // Initialize the application
{
	MainFrame* frame = new MainFrame("CodeClash", wxPoint(50, 50), wxSize(1000, 600));
	frame->Show(true);
	return true;
}