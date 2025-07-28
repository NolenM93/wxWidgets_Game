#pragma once
#include <wx/panel.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>
class EditorPanel : public wxPanel
{private:
	wxTextCtrl* textCtrl;// Text control for editing scripts
public:
	EditorPanel(wxWindow* parent);// Constructor to initialize the editor panel
	std::string getScript() const;// Get the script text from the editor
};

