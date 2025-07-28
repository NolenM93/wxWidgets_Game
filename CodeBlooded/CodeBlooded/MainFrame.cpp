#include "MainFrame.h"

enum { ID_RunSim = 1 };

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
	EVT_MENU(ID_RunSim, MainFrame::OnRunSimulation)
wxEND_EVENT_TABLE()

MainFrame::MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
	: wxFrame(nullptr, wxID_ANY, title, pos, size)
{
	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

	editor = new EditorPanel(this);
	arena = new ArenaPanel(this);

	sizer->Add(editor, 1, wxEXPAND | wxALL, 5);
	sizer->Add(arena, 2, wxEXPAND | wxALL, 5);

	wxButton* runButton = new wxButton(this, ID_RunSim, "Run Simulation");
	sizer->Add(runButton, 0, wxALIGN_CENTER | wxALL, 5);
	SetSizer(sizer);
}

void MainFrame::OnRunSimulation(wxCommandEvent& event) {
	arena->loadScript(editor->getScript());
	arena->startSimulation();
}