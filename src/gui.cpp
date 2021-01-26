#include "include/gui.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "include/stb_image.h"

bool guiApp::OnInit()
{
    nnFrame* f = new nnFrame();
    f->Show(true);
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
    wxJPEGHandler* jpgHandle = new wxJPEGHandler(); //Object used to save and load JPEG images
    wxImage::AddHandler(jpgHandle); //Start JPG loading

    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL); //Sizer object to auto place GUI widgets

    wxPanel* pan = new wxPanel(this); //Panel containing all widgets
    
    int w;
    int h;
    int ch;
   // unsigned char* imgDat = stbi_load("moon0.jpg", &w, &h, &ch, 0);
    //wxImage* img = new wxImage(w, h, imgDat, false);
    //stbi_image_free(imgDat);
    wxBitmap* bmp = new wxBitmap("moon0.jpg", wxBITMAP_TYPE_JPEG_RESOURCE);
    pan->AddChild(bmp);

    sizer->Add(bmp);
    
    
    pan->Show(true);
    SetSizer(sizer);

    Bind(wxEVT_MENU, [=](wxCommandEvent& e) {
        wxMessageBox("Hello!");
    }, ID_TEST);
}

