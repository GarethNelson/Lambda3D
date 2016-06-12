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
//     Console functionality
//
//-----------------------------------------------------------------------------

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include <SDL.h>
#include <SDL_ttf.h>

#include "l_utils.h"
#include "v_init.h"

#define CONSOLE_FONT_SIZE 12
#define CONSOLE_BUF_SIZE 8192
static char CONSOLE_BUF[CONSOLE_BUF_SIZE];
static TTF_Font *font;

// I am aware the below seems completely pointless, there is reason to the madness
static int is_active=1;
int console_is_active() {
    if(is_active==1) return 1;
    return 0;
}

void console_toggle() {
     if(is_active==1) is_active=0;
     if(is_active==0) is_active=1;
}

void console_printf(const char* fmt, ...) {
     char buf[CONSOLE_BUF_SIZE];
     va_list args;
     va_start(args, fmt);
     vsnprintf(buf,CONSOLE_BUF_SIZE,fmt,args);
     va_end(args);
     snprintf(CONSOLE_BUF,CONSOLE_BUF_SIZE,"%s%s",CONSOLE_BUF,buf);
}

void console_init() {
     if(!TTF_WasInit()) {
         TTF_Init();
     }
     font = TTF_OpenFont("../fonts/system.ttf",CONSOLE_FONT_SIZE);
     console_printf("Lambda console ready\n");
}

void console_render() {
     glMatrixMode(GL_PROJECTION);
     glPushMatrix();
     glLoadIdentity();

     screen_res res=get_screen_res();

     int max_lines = res.h/2/TTF_FontHeight(font);
     console_printf("max_lines: %d\n",max_lines);
     glOrtho( 0.0, res.w, res.h, 0.0, 1.0, -1.0 );

     glEnable(GL_BLEND);
     glDisable(GL_TEXTURE_2D);
     glEnable(GL_BLEND);
     glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


     glColor4f(1.0f,1.0f,1.0f,1.0f);
     glBegin(GL_QUADS);
       glVertex2f(0,0);
       glVertex2f(res.w,0);
       glVertex2f(res.w,res.h/2);
       glVertex2f(0,res.h/2);
     glEnd();
     
     SDL_Surface *sdl_output;
     SDL_Color font_bg = { 0xFF, 0xFF, 0xFF, 0 };
     SDL_Color font_fg = { 0x00, 0x00, 0x00, 0 };

     sdl_output = TTF_RenderText_Blended_Wrapped(font,CONSOLE_BUF,font_fg,res.w);
     GLuint tex_out = SDL_GL_LoadTexture(sdl_output);
     glEnable(GL_TEXTURE_2D);
     glEnable(GL_BLEND);
     glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
     glBindTexture(GL_TEXTURE_2D, tex_out);
     glBegin(GL_QUADS);
       glTexCoord2f(0.0f, 0.0f); glVertex2f(0,0);
       glTexCoord2f(1.0f, 0.0f); glVertex2f(sdl_output->w,0);
       glTexCoord2f(1.0f, 1.0f); glVertex2f(sdl_output->w,sdl_output->h);
       glTexCoord2f(0.0f, 1.0f); glVertex2f(0,sdl_output->h);
     glEnd();

     glPopMatrix();
     SDL_FreeSurface(sdl_output);
}



