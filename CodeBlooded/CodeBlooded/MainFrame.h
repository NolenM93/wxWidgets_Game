#pragma once
#include <wx/frame.h>
#include "EditorPanel.h"
#include "ArenaPanel.h"
#include <wx/sizer.h>
#include <wx/button.h>

class MainFrame : public wxFrame
{private:
	EditorPanel* editor;// Editor panel for script editing
	ArenaPanel* arena;// Arena panel for displaying the simulation
	void OnRunSimulation(wxCommandEvent& event);// Handle the Run Simulation button click event
	
public:
	MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size);// Constructor to initialize the main frame
	

	wxDECLARE_EVENT_TABLE();// Declare event table for handling events
};

