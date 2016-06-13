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
#include <string.h>
#include <wordexp.h>

#include <SDL.h>
#include <SDL_ttf.h>

#include "l_utils.h"
#include "v_init.h"
#include "oglconsole.h"
#include "cons_cvars.h"

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
     fprintf(stderr,"%s",buf);
}

// TODO: move commands into another file, make dynamic and shit
void cmd_set(int argc, char** argv) {
     if(argc==1) {
        dump_cvars();
        return;
     }
     if(argc != 4) {
        console_printf("Error: insufficient parameters to set command\n");
        console_printf("Usage: set <type> <varname> <value>\n");
        console_printf("       <type>      one of s,i,f or b\n");
        console_printf("       <varname>   variable name to set\n");
        console_printf("       <value>     value to set\n");
        console_printf("       \n");
        console_printf("       Data types supported: string(s), integer(i), float(f), boolean(b)\n");
        console_printf("       For boolean values, valid values are: true, false\n");
     }
     
}

void console_defcmd(int argc, char** argv) {
     if(strcmp(argv[0],"quit")==0) {
        SDL_Event e;
        e.type = SDL_QUIT;
        SDL_PushEvent(&e);
     } else if(strcmp(argv[0],"set")==0) {
        cmd_set(argc, argv);
     }
}

void console_runcmd(char* cmdline) {
     int i;
     char **argv = NULL;
     int argc;
     wordexp_t p;
     wordexp(cmdline, &p, 0);
     argc = p.we_wordc;
     argv = calloc(argc, sizeof(char*));
     for(i=0; i < p.we_wordc; i++) {
         argv[i] = strdup(p.we_wordv[i]);
     }
     wordfree(&p);
     console_defcmd(argc, argv);
     for(i=0; i < p.we_wordc; i++) {
         free(argv[i]);
     }
     free(argv);
}

void cmdCB(OGLCONSOLE_Console console, char *cmd) {
     console_runcmd(cmd);
}

void console_log_func(void* userdata, int category, SDL_LogPriority priority, const char* message) {
     console_printf("%s\n", message);
}

void console_init() {
     screen_res res = get_screen_res();
     glViewport( 0, 0, ( GLsizei )res.w, ( GLsizei )res.h );
     OGLCONSOLE_Create();
     OGLCONSOLE_EnterKey(cmdCB);
     SDL_LogSetAllPriority(SDL_LOG_PRIORITY_VERBOSE);
     SDL_LogSetOutputFunction(&console_log_func, NULL);
     
     SDL_LogInfo(SDL_LOG_CATEGORY_SYSTEM,"SDL logging started!");
     console_printf("Lambda console ready\n\n");
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



