#pragma once
#include <wx/panel.h>
#include <wx/timer.h>
#include "Robot.h"
#include "ScriptEngine.h"

class ArenaPanel : public wxPanel
{private:
	std::vector<Robot> robots;// List of robots in the arena
	ScriptEngine engine;// Script engine to manage robot scripts
	wxTimer timer;// Timer for simulation updates

	void OnPaint(wxPaintEvent& event);// Handle paint events to draw the arena and robots
	void OnTick(wxTimerEvent& event);// Handle timer ticks to update the simulation
	
public:
	ArenaPanel(wxWindow* parent);// Constructor to initialize the arena panel
	void loadScript(const std::string& code);// Load a script into the script engine
	void startSimulation();// Start the simulation by initializing the script engine and starting the timer

	wxDECLARE_EVENT_TABLE();// Declare event table for handling events
};

