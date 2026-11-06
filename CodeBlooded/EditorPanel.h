#pragma once

#include <wx/panel.h>
#include <wx/textctrl.h>

#include <string>

class EditorPanel : public wxPanel {
public:
    static const char* DefaultScript();

    explicit EditorPanel(wxWindow* parent);

    std::string getScript() const;
    void setScript(const std::string& script);

private:
    wxTextCtrl* textCtrl_ = nullptr;
};
