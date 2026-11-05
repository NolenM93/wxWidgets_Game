#include "MainFrame.h"

#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/statusbr.h>

enum {
    ID_RunSim = 1,
    ID_StopSim
};

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_BUTTON(ID_RunSim, MainFrame::OnRunSimulation)
    EVT_BUTTON(ID_StopSim, MainFrame::OnStopSimulation)
wxEND_EVENT_TABLE()

MainFrame::MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
    : wxFrame(nullptr, wxID_ANY, title, pos, size) {
    auto* root = new wxPanel(this);
    editor_ = new EditorPanel(root);
    arena_ = new ArenaPanel(root);

    auto* runButton = new wxButton(root, ID_RunSim, "Run Simulation");
    auto* stopButton = new wxButton(root, ID_StopSim, "Stop");

    auto* buttons = new wxBoxSizer(wxHORIZONTAL);
    buttons->Add(runButton, 0, wxRIGHT, 8);
    buttons->Add(stopButton, 0);

    auto* hint = new wxStaticText(
        root, wxID_ANY,
        "Edit the script, then Run. Both robots execute the same commands each tick.");

    auto* left = new wxBoxSizer(wxVERTICAL);
    left->Add(editor_, 1, wxEXPAND);
    left->Add(hint, 0, wxLEFT | wxRIGHT | wxBOTTOM, 8);
    left->Add(buttons, 0, wxLEFT | wxRIGHT | wxBOTTOM, 8);

    auto* rootSizer = new wxBoxSizer(wxHORIZONTAL);
    rootSizer->Add(left, 1, wxEXPAND | wxALL, 4);
    rootSizer->Add(arena_, 2, wxEXPAND | wxALL, 8);
    root->SetSizer(rootSizer);

    auto* frameSizer = new wxBoxSizer(wxVERTICAL);
    frameSizer->Add(root, 1, wxEXPAND);
    SetSizer(frameSizer);

    CreateStatusBar();
    SetStatusText("Write a bot script, then click Run Simulation.");
    SetMinSize(wxSize(900, 560));
}

void MainFrame::StartSimulation() {
    arena_->stopSimulation();
    arena_->loadScript(editor_->getScript());
    arena_->startSimulation();
    SetStatusText("Simulation running.");
}

void MainFrame::StopSimulation() {
    arena_->stopSimulation();
    SetStatusText("Simulation stopped.");
}

void MainFrame::OnRunSimulation(wxCommandEvent&) {
    StartSimulation();
}

void MainFrame::OnStopSimulation(wxCommandEvent&) {
    StopSimulation();
}
