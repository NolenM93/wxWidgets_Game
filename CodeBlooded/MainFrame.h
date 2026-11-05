#pragma once

#include "ArenaPanel.h"
#include "EditorPanel.h"

#include <wx/button.h>
#include <wx/frame.h>

class MainFrame : public wxFrame {
public:
    MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
    void StartSimulation();
    void StopSimulation();

private:
    void OnRunSimulation(wxCommandEvent& event);
    void OnStopSimulation(wxCommandEvent& event);

    EditorPanel* editor_ = nullptr;
    ArenaPanel* arena_ = nullptr;

    wxDECLARE_EVENT_TABLE();
};
