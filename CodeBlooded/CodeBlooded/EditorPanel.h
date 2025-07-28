#pragma once
#include <wx/panel.h>
#include <wx/textctrl.h>

class EditorPanel : public wxPanel
{private:
	wxTextCtrl* textCtrl;
public:
	EditorPanel(wxWindow* parent);
	std::string getScript() const;
};

