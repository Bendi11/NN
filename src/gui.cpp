#include "include/gui.hpp"

bool guiApp::OnInit()
{
    nnFrame* f = new nnFrame();
    f->Show(true);
    f->Maximize(true);
    wxMessageBox("TEST");
    return true;
}

bool guiApp::OnClose()
{
    return true;
}

nnFrame::nnFrame()
    : wxFrame(NULL, wxID_ANY, "Hello World")
{
    wxPanel* pan = new wxPanel(this);
    pan->Show(true);
    wxButton* butt = new wxButton(pan, ID_TEST, "Test Button");
    wxStaticText* txt = new wxStaticText(pan, wxID_ANY, "This is a test text box", wxPoint(50, 50));
    butt->Bind(wxEVT_BUTTON, [=](wxEvent& e) {
        wxMessageBox("Hello!");
    }, ID_TEST);
    
    wxMenu *menuFile = new wxMenu;
    wxMenuBar *menuBar = new wxMenuBar;
    menuFile->Append(ID_TEST, "Display test text box");
    menuBar->Append(menuFile, "&File");
    
    SetMenuBar( menuBar );

    Bind(wxEVT_MENU, [=](wxCommandEvent& e) {
        wxMessageBox("Hello!");
    }, ID_TEST);
}

