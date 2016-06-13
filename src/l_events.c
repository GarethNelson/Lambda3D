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
//     Definitions of events
//
//-----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>

#include <SDL.h>

#include "l_events.h"
#include "l_console.h"

static int sdl_type_id=-1;

void ev_send(int ev_type, void* ev) {
     if(sdl_type_id == -1) {
        sdl_type_id = SDL_RegisterEvents(1);
     }
     SDL_Event event;
     SDL_zero(event);
     event.type = sdl_type_id;
     event.user.code = ev_type;
     event.user.data1 = NULL; // reserved for future use
     event.user.data1 = ev;
     SDL_PushEvent(&event);
}

int get_sdl_type() {
       return sdl_type_id;
}
