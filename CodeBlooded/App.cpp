#include "App.h"
#include "MainFrame.h"

#include <wx/utils.h>

wxIMPLEMENT_APP(App);

bool App::OnInit() {
    SetAppName("Code Blooded");
    auto* frame = new MainFrame("Code Blooded", wxDefaultPosition, wxSize(1100, 700));
    frame->Show(true);

    // Used by docs/screenshot capture under Xvfb. Not required for normal play.
    if (wxGetEnv("CODEBLOODED_AUTO_RUN", nullptr)) {
        frame->CallAfter(&MainFrame::StartSimulation);
    }
    return true;
}
