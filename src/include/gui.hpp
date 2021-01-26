#pragma once

#include <wx/wx.h>

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
    enum
    {
        ID_TEST = 0
    };
};

wxDECLARE_APP(guiApp);
