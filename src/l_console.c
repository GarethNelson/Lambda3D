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
#include <limits.h>

#include <SDL.h>
#include <physfs.h>

#include "l_utils.h"
#include "v_init.h"
#include "oglconsole.h"
#include "cons_cvars.h"
#include "cons_cmds.h"

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
extern struct cons_cmd *cons_commands;

void console_defcmd(int argc, char** argv) {
     int i=0;
     if(strcmp(argv[0],"quit")==0) {
        SDL_Event e;
        e.type = SDL_QUIT;
        SDL_PushEvent(&e);
     } else {
        for(i=0; i< get_cmd_count(); i++) {
          if(strcmp(argv[0],cons_commands[i].cmd_str)==0) {
              cons_commands[i].cmd_func(argc, argv);
           }
        }
     }
}

void console_runcmd(char* cmdline) {
     if(strlen(cmdline)==0) return;
     fprintf(stderr,"] %s\n",cmdline);
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

char* tabCB(OGLCONSOLE_Console console, char *partial) {
      char* cmd_name;
      int i;
      for(i=0; i < get_cmd_count(); i++) {
          cmd_name = cons_commands[i].cmd_str;
          if(strncmp(partial,cmd_name,strlen(partial))==0) {
             return(cmd_name+strlen(partial));
          }
      }
      return "test";
}

void console_log_func(void* userdata, int category, SDL_LogPriority priority, const char* message) {
     console_printf("%s\n", message);
}

void console_runscript(char* filename) {
     PHYSFS_File* fd = PHYSFS_openRead(filename);
     if(fd==NULL) {
        console_printf("SCRIPT %s: Error! %s\n",filename,PHYSFS_getLastError());
        return;
     }
     PHYSFS_uint32 f_size = PHYSFS_fileLength(fd);
     void* f_data = malloc(f_size);
     PHYSFS_read(fd,f_data,(PHYSFS_uint32)f_size,1);

     char *cmdline;
     char *s;
     char *tofree = s = strdup((char*)f_data);
     while((cmdline = strsep(&s,"\n")) != NULL) {
        if(strlen(cmdline)>1) {
           console_printf("SCRIPT %s: %s\n",filename,cmdline);
           console_runcmd(cmdline);
        }
        
     }
     free(tofree);
     free(f_data);
     PHYSFS_close(fd);
 
}

void console_init() {
     screen_res res = get_screen_res();
     glViewport( 0, 0, ( GLsizei )res.w, ( GLsizei )res.h );
     OGLCONSOLE_Create();
     OGLCONSOLE_EnterKey(cmdCB);
     OGLCONSOLE_TabComplete(tabCB);
     SDL_LogSetAllPriority(SDL_LOG_PRIORITY_VERBOSE);
     SDL_LogSetOutputFunction(&console_log_func, NULL);
     
     SDL_LogInfo(SDL_LOG_CATEGORY_SYSTEM,"SDL logging started!");
     console_printf("Lambda console ready\n\n");
     init_cmd_table();

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



