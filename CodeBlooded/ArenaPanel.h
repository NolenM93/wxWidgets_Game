#pragma once

#include "Simulation.h"

#include <wx/panel.h>
#include <wx/timer.h>

class ArenaPanel : public wxPanel {
public:
    explicit ArenaPanel(wxWindow* parent);

    void loadScript(const std::string& code);
    void startSimulation();
    void stopSimulation();
    bool isRunning() const { return timer_.IsRunning(); }

    const Simulation& simulation() const { return simulation_; }

private:
    void OnPaint(wxPaintEvent& event);
    void OnTick(wxTimerEvent& event);
    void OnSize(wxSizeEvent& event);

    Simulation simulation_;
    wxTimer timer_;

    wxDECLARE_EVENT_TABLE();
};
