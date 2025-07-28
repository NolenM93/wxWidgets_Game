#pragma once
#include <wx/panel.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>
class EditorPanel : public wxPanel
{private:
	wxTextCtrl* textCtrl;
public:
	EditorPanel(wxWindow* parent);
	std::string getScript() const;
};

