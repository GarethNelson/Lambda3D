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

#include <SDL.h>
#include <SDL_ttf.h>

#include "l_utils.h"
#include "v_init.h"

static char CONSOLE_BUF[8192]; // 8kb should be enough for anybody

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

void console_init() {
     if(!TTF_WasInit()) {
         TTF_Init();
     }
}

void console_render() {
}

void console_printf(const char* fmt, ...) {
}

