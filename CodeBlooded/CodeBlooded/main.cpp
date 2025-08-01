#include <wx/wx.h>
#include "MainFrame.h"

class MyApp : public wxApp {
public:
    virtual bool OnInit() override {
        MainFrame* frame = new MainFrame("CodeBlooded", wxDefaultPosition, wxSize(800, 600));
        frame->Show(true);
        return true;
    }
};

wxIMPLEMENT_APP(MyApp);