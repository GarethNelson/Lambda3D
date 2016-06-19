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

#include <physfs.h>
#include <SDL.h>

#include "v_init.h"
#include "l_console.h"
#include "l_events.h"
#include "l_appflow.h"
#include "l_cache.h"
#include "r_2d.h"
#include "cons_cvars.h"
#include "oglconsole.h"
#include "l_splash_logo.h"

int running=1;

void handle_l_event(SDL_Event e) {
     l_ev_switch_req *req;
     int new_stage=get_cvar_i("appstage");
     int new_flags=get_cvar_i("appflags");
     void* new_params;
     switch(e.user.code) {
        case L_EV_SWITCH_REQ:
          req = ((l_ev_switch_req*)e.user.data2);
          if(req->new_stage != 0) new_stage = req->new_stage;
          new_flags  |=  (req->set_flags);
          new_flags  &= ~(req->unset_flags);
          new_params  = req->new_stage_params;
          switch_appstage(get_cvar_i("appstage"),new_stage,get_cvar_i("appflags"),new_flags,new_params);
        break;
        case L_EV_CVAR_CHANGE:
        break;
     }
     free(e.user.data2);
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

static char* mount_file=NULL; // file to mount supplied from command line
static char* exe_name=NULL;

void print_usage() {
     fprintf(stderr,"usage: %s [-f filename] [-h]\n",exe_name);
     fprintf(stderr,"       -f mount an archive to the VFS root before starting the engine\n");
     fprintf(stderr,"       -h show this usage information\n");
}

void handle_opts(int argc, char** argv) {
     extern char *optarg;
     extern int optind;
     int c=0;

     exe_name = strdup(argv[0]);

     while((c = getopt(argc, argv, "f:h")) != -1) {
         switch(c) {
             case 'f':
                mount_file = strdup(optarg);
             break;
             case 'h':
                print_usage();
                exit(0);
             break;
         }
     }
}

int main(int argc, char** argv) {
    handle_opts(argc, argv);

    setbuf(stdout,NULL);
    printf("\n*** LAMBDA ENGINE STARTUP ***\n\n");

    if(SDL_WasInit(0)==0) {
       SDL_Init(0);
    }
    SDL_InitSubSystem(SDL_INIT_EVENTS);
 
    v_init();
    
    console_init();

    if(PHYSFS_init(argv[0])==0) {
       SDL_LogError(SDL_LOG_CATEGORY_SYSTEM,"Could not setup PhysFS - VFS will not be operational!");
       exit(1);
    } else {
       SDL_LogInfo(SDL_LOG_CATEGORY_SYSTEM,"Loaded PhysFS - VFS will be operational");
    }

    if(mount_file != NULL) {
      int retval=PHYSFS_mount((const char*)mount_file,"",0);
      if(retval==0) {
         SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,"Could not mount file %s",mount_file);
      }
    }

    init_cache();
    init_appstage_table();

    switch_appstage(0,APPSTAGE_STARTUP,0,APPFLAGS_NORMAL,NULL);
    set_cvar_s("cwd","/");

    console_runscript("/autoexec.cfg");

    int stage, flags;
    float seconds;
    float fps;
    int Frames=0;
    int T0=0;
    int last_frame=SDL_GetTicks();
    int t = SDL_GetTicks();
    float delta;
    set_cvar_f("fps",60);
    set_cvar_f("delta",16);

    switch_appstage(APPSTAGE_STARTUP,APPSTAGE_SPLASH,APPFLAGS_NORMAL,APPFLAGS_NORMAL,NULL);
    while(running) {
       delta = t-last_frame;
       last_frame = t;
       handle_events();

       stage = get_cvar_i("appstage");
       flags = get_cvar_i("appflags");
       update_app(stage, flags);

       v_pre_render();

       render_app(stage, flags);
       console_render();

       v_post_render();
       Frames++; {
           t = SDL_GetTicks();
           if(t - T0 >= 5000) {
              seconds = (t - T0) / 1000.0;
              fps = Frames / seconds;
           }
       }
       set_cvar_f("fps",fps);
       set_cvar_f("delta",delta);
    }
    return 0;
}
