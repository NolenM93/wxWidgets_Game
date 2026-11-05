#include "ArenaPanel.h"

#include <wx/dcbuffer.h>
#include <wx/frame.h>

#include <algorithm>
#include <cmath>

wxBEGIN_EVENT_TABLE(ArenaPanel, wxPanel)
    EVT_PAINT(ArenaPanel::OnPaint)
    EVT_TIMER(wxID_ANY, ArenaPanel::OnTick)
    EVT_SIZE(ArenaPanel::OnSize)
wxEND_EVENT_TABLE()

ArenaPanel::ArenaPanel(wxWindow* parent)
    : wxPanel(parent, wxID_ANY), timer_(this) {
    SetBackgroundStyle(wxBG_STYLE_PAINT);
    simulation_.reset("");
}

void ArenaPanel::loadScript(const std::string& code) {
    simulation_.reset(code);
    Refresh();
}

void ArenaPanel::startSimulation() {
    if (!timer_.IsRunning()) {
        timer_.Start(50);
    }
}

void ArenaPanel::stopSimulation() {
    timer_.Stop();
}

void ArenaPanel::OnTick(wxTimerEvent&) {
    simulation_.tick();
    Refresh();

    if (auto* frame = dynamic_cast<wxFrame*>(wxGetTopLevelParent(this))) {
        wxString status;
        for (const Robot& bot : simulation_.robots()) {
            if (!status.empty()) {
                status += "    ";
            }
            status += wxString::Format("%s  HP %.0f%s",
                                       bot.name.c_str(), bot.health,
                                       bot.shield ? "  [shield]" : "");
        }
        status += wxString::Format("    Tick %d", simulation_.tickCount());
        frame->SetStatusText(status);
    }

    if (!simulation_.hasLivingRobot()) {
        stopSimulation();
    }
}

void ArenaPanel::OnSize(wxSizeEvent&) {
    Refresh();
}

void ArenaPanel::OnPaint(wxPaintEvent&) {
    wxAutoBufferedPaintDC dc(this);
    const wxSize size = GetClientSize();
    dc.SetBackground(wxBrush(wxColour(18, 22, 32)));
    dc.Clear();

    const float scaleX = static_cast<float>(size.GetWidth()) / Simulation::kWidth;
    const float scaleY = static_cast<float>(size.GetHeight()) / Simulation::kHeight;

    dc.SetPen(wxPen(wxColour(42, 50, 68), 1));
    for (int i = 0; i <= 10; ++i) {
        const int x = static_cast<int>(i * 10.0f * scaleX);
        const int y = static_cast<int>(i * 10.0f * scaleY);
        dc.DrawLine(x, 0, x, size.GetHeight());
        dc.DrawLine(0, y, size.GetWidth(), y);
    }

    dc.SetPen(wxPen(wxColour(80, 90, 110), 2));
    dc.SetBrush(*wxTRANSPARENT_BRUSH);
    dc.DrawRectangle(0, 0, size.GetWidth(), size.GetHeight());

    const wxColour colors[] = {wxColour(74, 163, 255), wxColour(255, 92, 92)};
    std::size_t which = 0;
    for (const Robot& bot : simulation_.robots()) {
        const wxColour color = colors[which % 2];
        const int cx = static_cast<int>(bot.position.x * scaleX);
        const int cy = static_cast<int>(bot.position.y * scaleY);
        const int radius = std::max(8, static_cast<int>(std::min(scaleX, scaleY) * 2.4f));

        if (!bot.isAlive()) {
            dc.SetPen(wxPen(wxColour(90, 90, 90), 2));
            dc.SetBrush(wxBrush(wxColour(50, 50, 50)));
        } else {
            dc.SetPen(wxPen(color.ChangeLightness(80), 2));
            dc.SetBrush(wxBrush(color));
        }
        dc.DrawCircle(cx, cy, radius);

        const int barW = radius * 3;
        const int barH = 5;
        const int barX = cx - barW / 2;
        const int barY = (which == 0) ? cy - radius - 16 : cy + radius + 4;
        dc.SetPen(*wxTRANSPARENT_PEN);
        dc.SetBrush(wxBrush(wxColour(40, 40, 40)));
        dc.DrawRectangle(barX, barY, barW, barH);
        const float pct = std::max(0.0f, bot.health / Robot::kMaxHealth);
        const int fillW = static_cast<int>(barW * pct);
        if (fillW > 0) {
            dc.SetBrush(wxBrush(pct > 0.35f ? wxColour(80, 200, 90) : wxColour(220, 70, 70)));
            dc.DrawRectangle(barX, barY, fillW, barH);
        }

        const wxString label = wxString::FromUTF8(bot.name.c_str());
        const int nameY = (which == 0) ? barY - 16 : barY + barH + 2;
        dc.SetTextForeground(wxColour(230, 235, 245));
        dc.DrawText(label, cx - radius, nameY);
        ++which;
    }
}
