#pragma once

#include <wx/wx.h>
#include "net.hpp"

class guiApp : public wxApp
{
public:
    virtual bool OnInit(); //wxWidgets window opened
    virtual bool OnClose();

private:

};

class nnFrame : public wxFrame
{
public:
    nnFrame();

private:

    wxStaticText* labelText;
    wxStaticText* outText;

    net* network;
    enum
    {
        ID_TRAIN,
        ID_FEED,
        ID_SAVE,
        ID_LOAD
    };

    void handleTrain(wxCommandEvent& event); //Function to handle network train button
    void handleFeed(wxCommandEvent& event); //Function to handle network feed button
    void handleSave(wxCommandEvent& event); //Function to handle network save button
    void handleLoad(wxCommandEvent& event); //Function to handle network load button

};

wxDECLARE_APP(guiApp);
