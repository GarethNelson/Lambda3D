//-----------------------------------------------------------------------------
//
// $Id:$
//
// Copyright (C) 2016 by Gareth Nelson (gareth@garethnelson.com)
//
// This file is part of the Lambda engine.
//
// The Lambda engine is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//
// The Lambda engine is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
//
// $Log:$
//
// DESCRIPTION:
//      Initialisation code for the video context
//
//-----------------------------------------------------------------------------

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>
#if defined(__APPLE__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

SDL_Window *screen;
static SDL_GLContext glcontext;
static int screen_w;
static int screen_h;

void v_pre_render() {
     glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}

void v_post_render() {
     SDL_GL_SwapWindow(screen);
}

int v_init() {
    if(SDL_WasInit(0)==0) {
       SDL_Init(0);
    }
    SDL_InitSubSystem(SDL_INIT_TIMER);
    SDL_InitSubSystem(SDL_INIT_VIDEO);
    SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
    SDL_GL_SetSwapInterval(1);

    SDL_DisplayMode disp_mode;
    SDL_GetDesktopDisplayMode(0, &disp_mode);
    
    screen = SDL_CreateWindow("Lambda 3D",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,disp_mode.w,disp_mode.h,
                                              SDL_WINDOW_OPENGL);
    SDL_GL_GetDrawableSize(screen, &screen_w, &screen_h);

    glcontext = SDL_GL_CreateContext(screen);
    return 0;
}
