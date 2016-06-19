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
//     Definitions and such used for app flow
//
//-----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL.h>

#include "l_console.h"
#include "l_appflow.h"
#include "cons_cvars.h"
#include "r_2d.h"
#include "l_splash_logo.h"

void default_init(void* params) {
     console_printf("Default init called for stage\n");
}

void default_cleanup() {
     console_printf("Default cleanup called for stage\n");
}

void default_update() {
}

void default_render() {
}

struct app_stage_t* app_stage_table=NULL;
static unsigned int stage_count=0;

static struct app_stage_t app_stages[] = {
  {APPSTAGE_STARTUP,   "Startup",         &default_init,&default_cleanup,&default_update,&default_render},
  {APPSTAGE_SPLASH,    "Splash screen",   &l_splash_logo_init,&default_cleanup,&l_splash_logo_update,&l_splash_logo_render},
  {APPSTAGE_STARTMENU, "Start menu",      &default_init,&default_cleanup,&default_update,&default_render},
  {APPSTAGE_LOADSCREEN,"Loading screen",  &default_init,&default_cleanup,&default_update,&default_render},
  {APPSTAGE_INGAME,    "Ingame",          &default_init,&default_cleanup,&default_update,&default_render},
  {APPSTAGE_GAMEOVER,  "Game over screen",&default_init,&default_cleanup,&default_update,&default_render},
};

void add_appstage(struct app_stage_t stage) {
     stage_count++;
     app_stage_table = realloc((void*)app_stage_table,stage_count*sizeof(struct app_stage_t));
     memcpy(&(app_stage_table[stage_count-1]), &stage, sizeof(struct app_stage_t));
}

void init_appstage_table() {
     int i=0;
     for(i=0; i< (sizeof(app_stages)/sizeof(struct app_stage_t)); i++) {
         SDL_LogInfo(SDL_LOG_CATEGORY_SYSTEM,"Adding stage: %s",app_stages[i].stage_name);
         add_appstage(app_stages[i]);
     }
}

void switch_appstage(int old_stage, int new_stage, int old_flags, int new_flags, void* params) {
     char* old_s=stage_name(old_stage, old_flags);
     char* new_s=stage_name(new_stage, new_flags);
     console_printf("Switching from [%s] to [%s]\n",old_s,new_s);
     if(old_stage != new_stage) {
        int i=0;
        for(i=0; i<stage_count; i++) {
            if(app_stage_table[i].stage_id==old_stage) {
               app_stage_table[i].cleanup();
            }
        }
        for(i=0; i<stage_count; i++) {
            if(app_stage_table[i].stage_id==new_stage) {
               app_stage_table[i].init(params);
            }
        }
     }
     set_cvar_s("s_stage",new_s);
     set_cvar_i("appstage",new_stage);
     set_cvar_i("appflags",new_flags);
     free(old_s); free(new_s);
}

// used for console stuff
char* stage_name(int stage, int flags) {
      char retval[1024];
      char stage_str[32];
      char flags_str[32];
      memset((void*)stage_str,0,32);
      memset((void*)flags_str,0,32);
      switch(stage) {
         case APPSTAGE_STARTUP:
           snprintf(stage_str,32,"STARTUP");
         break;
         case APPSTAGE_SPLASH:
           snprintf(stage_str,32,"SPLASH");
         break;
         case APPSTAGE_STARTMENU:
           snprintf(stage_str,32,"STARTMENU");
         break;
         case APPSTAGE_LOADSCREEN:
           snprintf(stage_str,32,"LOADSCREEN");
         break;
         default:
           snprintf(stage_str,32,"INVALID");
         break;
      }
      if((flags & APPFLAGS_PAUSED) != 0) strncat(flags_str,"PAUSED ", 32);
      if((flags & APPFLAGS_MENU)   != 0) strncat(flags_str,"MENU ", 32);
      if((flags & APPFLAGS_DEBUG)  != 0) strncat(flags_str,"DEBUG ",32);
      snprintf(retval,1024,"Stage:%s, Flags:%s",stage_str,flags_str);

      return strdup(retval);
}

void update_app(int stage, int flags) {
     int i=0;
     for(i=0; i < stage_count; i++) {
         if(app_stage_table[i].stage_id==stage) {
            app_stage_table[i].update();
         }
     }

}


char debug_info[32];
void render_app(int stage, int flags) {
     int i=0;
     for(i=0; i < stage_count; i++) {
         if(app_stage_table[i].stage_id==stage) {
            app_stage_table[i].render();
         }
     }

     if((flags & APPFLAGS_DEBUG) != 0) {
       snprintf(debug_info,32,"FPS: %f, Delta: %f",get_cvar_f("fps"),get_cvar_f("delta"));
       draw_debug_text(debug_info,0,0);
     }
}

