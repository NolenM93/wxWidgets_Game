#include "ArenaPanel.h"
#include <wx/dcbuffer.h>

wxBEGIN_EVENT_TABLE(ArenaPanel, wxPanel)
	EVT_PAINT(ArenaPanel::OnPaint)
	EVT_TIMER(wxID_ANY, ArenaPanel::OnTick)
wxEND_EVENT_TABLE()

ArenaPanel::ArenaPanel(wxWindow* parent)
    : wxPanel(parent, wxID_ANY), timer(this) {
    SetBackgroundStyle(wxBG_STYLE_PAINT);
}

void ArenaPanel::loadScript(const std::string& code) {
    robots.clear();
    robots.emplace_back("Alpha", Vec2(10, 10));
    robots.emplace_back("Beta", Vec2(90, 90));
    engine.loadScript(code);
}

void ArenaPanel::startSimulation() {
    timer.Start(100); // 100ms per tick
}

void ArenaPanel::OnTick(wxTimerEvent& event) {
    for (auto& bot : robots) {
        engine.execute(bot, robots);
    }
    Refresh();
}

void ArenaPanel::OnPaint(wxPaintEvent& event) {
    wxAutoBufferedPaintDC dc(this);
    dc.Clear();

    for (const auto& bot : robots) {
        dc.SetBrush(*wxBLUE_BRUSH);
        dc.DrawCircle(bot.position.x * 5, bot.position.y * 5, 10);
        dc.DrawText(bot.name, bot.position.x * 5 - 10, bot.position.y * 5 - 20);
    }
}

