#pragma once
#include <wx/panel.h>
#include <wx/timer.h>
#include "Robot.h"
#include "ScriptEngine.h"

class ArenaPanel : public wxPanel
{private:
	std::vector<Robot> robots;
	ScriptEngine engine;
	wxTimer timer;

	void OnPaint(wxPaintEvent& event);
	void OnTick(wxTimerEvent& event);
	
public:
	ArenaPanel(wxWindow* parent);
	void loadScript(const std::string& code);
	void startSimulation();

	wxDECLARE_EVENT_TABLE();
};

