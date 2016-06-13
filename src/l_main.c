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
//      Program entry point
//
//-----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <SDL.h>

#include "v_init.h"
#include "l_console.h"
#include "l_events.h"
#include "l_appflow.h"
#include "cons_cvars.h"
#include "oglconsole.h"

int running=1;
int appstage=APPSTAGE_STARTUP;
int appflags=APPFLAGS_NORMAL;

void handle_l_event(SDL_Event e) {
     console_printf("Handling event %d\n",e.user.code);
     // TODO - actually handle, and free() stuff
}

void handle_events() {
     SDL_Event e;
     while (SDL_PollEvent(&e)) {
            if (OGLCONSOLE_SDLEvent(&e) == 0) switch(e.type) {
               case SDL_QUIT:
                 running = 0;
               break;
               default:
                  if(e.type==get_sdl_type()) {
                     handle_l_event(e); // see l_event.h and l_event.c
                  }
               break;
            }
     }
}

int main(int argc, char* argv[]) {
    setbuf(stdout,NULL);
    printf("\n*** LAMBDA ENGINE STARTUP ***\n\n");

    if(SDL_WasInit(0)==0) {
       SDL_Init(0);
    }
    SDL_InitSubSystem(SDL_INIT_EVENTS);

 
    v_init();
    console_init();

    switch_appstage(0,appstage,0,appflags);
    
    while(running) {
       handle_events();

       update_app(appstage, appflags);

       v_pre_render();

       render_app(appstage, appflags);

       console_render();

       v_post_render();
    }
    return 0;
}
