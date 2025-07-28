#include "App.h"
#include "MainFrame.h"

bool App::OnInit() {
	MainFrame* frame = new MainFrame("CodeClash", wxPoint(50, 50), wxSize(1000, 600));
	frame->Show(true);
	return true;
}