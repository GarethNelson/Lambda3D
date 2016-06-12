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
#include "oglconsole.h"

// I am aware the below seems completely pointless, there is reason to the madness
static int is_active=1;
int console_is_active() {
    if(is_active==1) return 1;
    return 0;
}

void console_toggle() {
     if(is_active==1) is_active=0;
     if(is_active==0) is_active=1;
     OGLCONSOLE_SetVisibility(is_active);
}

void console_printf(const char* fmt, ...) {
     char buf[1024];
     memset((void*)buf,0,1024);
     va_list args;
     va_start(args, fmt);
     vsnprintf(buf,1024,fmt,args);
     va_end(args);
     OGLCONSOLE_Print(buf);
}

void console_init() {
     screen_res res = get_screen_res();
     glViewport( 0, 0, ( GLsizei )res.w, ( GLsizei )res.h );
     OGLCONSOLE_Create();
     console_printf("Lambda console ready\n");
}

void console_render() {
     glMatrixMode(GL_PROJECTION);
     glPushMatrix();
     screen_res res = get_screen_res();
     glOrtho( 0.0, res.w, res.h, 0.0, 1.0, -1.0 );
     glViewport( 0, 0, ( GLsizei )res.w, ( GLsizei )res.h );
     OGLCONSOLE_Draw();
     glPopMatrix();
}



