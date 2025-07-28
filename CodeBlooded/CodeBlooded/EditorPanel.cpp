#include "EditorPanel.h"

EditorPanel::EditorPanel(wxWindow* parent)
	: wxPanel(parent, wxID_ANY)
{
	textCtrl = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_PROCESS_ENTER);
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(textCtrl, 1, wxEXPAND | wxALL, 5);
	SetSizer(sizer);
}

std::string EditorPanel::getScript() const {
	return textCtrl->GetValue().ToStdString();
}