#pragma once

//All UI library includes
#include <imgui.h>
#include <SDL.h>
#include <glad/glad.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>

#include "net.hpp"


namespace GUI
{


void initScreen(unsigned int w, unsigned int h);

}