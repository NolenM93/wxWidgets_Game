#pragma once
#include <wx/frame.h>
#include "EditorPanel.h"
#include "ArenaPanel.h"
#include <wx/sizer.h>
#include <wx/button.h>

class MainFrame : public wxFrame
{private:
	EditorPanel* editor;
	ArenaPanel* arena;
	void OnRunSimulation(wxCommandEvent& event);
	
public:
	MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
	

	wxDECLARE_EVENT_TABLE();
};

