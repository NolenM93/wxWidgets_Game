#include "EditorPanel.h"

#include <wx/sizer.h>
#include <wx/stattext.h>

const char* EditorPanel::DefaultScript() {
    return
        "# Code Blooded bot script\n"
        "# Commands run once per tick for every robot.\n"
        "#\n"
        "# moveTo(x, y)\n"
        "# moveToNearest()\n"
        "# activateShield()\n"
        "# takeDamage(amount)\n"
        "# attackNearest(range, damage)\n"
        "\n"
        "moveToNearest()\n"
        "attackNearest(8, 4)\n";
}

EditorPanel::EditorPanel(wxWindow* parent)
    : wxPanel(parent, wxID_ANY) {
    auto* title = new wxStaticText(this, wxID_ANY, "Bot Script");
    textCtrl_ = new wxTextCtrl(this, wxID_ANY, DefaultScript(),
                               wxDefaultPosition, wxDefaultSize,
                               wxTE_MULTILINE | wxTE_DONTWRAP);

    wxFont font = textCtrl_->GetFont();
    font.SetFamily(wxFONTFAMILY_TELETYPE);
    font.SetPointSize(11);
    textCtrl_->SetFont(font);

    auto* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(title, 0, wxLEFT | wxRIGHT | wxTOP, 8);
    sizer->Add(textCtrl_, 1, wxEXPAND | wxALL, 8);
    SetSizer(sizer);
}

std::string EditorPanel::getScript() const {
    return std::string(textCtrl_->GetValue().utf8_str());
}

void EditorPanel::setScript(const std::string& script) {
    textCtrl_->SetValue(wxString::FromUTF8(script.c_str()));
}
