#include "EditorPanel.h"

EditorPanel::EditorPanel(wxWindow* parent)// Constructor to initialize the editor panel
	: wxPanel(parent, wxID_ANY)
{
	textCtrl = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_PROCESS_ENTER);
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(textCtrl, 1, wxEXPAND | wxALL, 5);
	SetSizer(sizer);
}

std::string EditorPanel::getScript() const // Get the script text from the editor
{
	return textCtrl->GetValue().ToStdString();
}