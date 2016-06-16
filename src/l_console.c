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
#include <physfs.h>

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

void cmd_help(int argc, char** argv);
void cmd_set(int argc, char** argv);
void cmd_mount(int argc, char** argv);

char *commands_str[] = {
     "help",
     "set",
     "mount"
};

void (*commands_func[])(int argc, char** argv) = {
     &cmd_help,
     &cmd_set,
     &cmd_mount
};

// TODO: move commands into another file, make dynamic and shit
void cmd_mount(int argc, char** argv) {
     PHYSFS_ArchiveInfo **ar_info;
     if(argc==1) {
        char **p;
        for(p = PHYSFS_getSearchPath(); *p != NULL; p++) {
            printf("%s on %s\n",*p,PHYSFS_getMountPoint(*p));
        }
        PHYSFS_freeList(p);
        return;
     }
     if(argc==2) {
        if(strcmp(argv[1],"-h")==0) {
           console_printf("Usage: mount <archive> <mountpoint\n");
           console_printf("       <archive>     path to an archive file to mount\n");
           console_printf("       <mountpoint>  where in the VFS to mount it - if not specified, / is default\n");
           console_printf("       Archives supported:\n");
           for(ar_info = PHYSFS_supportedArchiveTypes(); *ar_info != NULL; ar_info++) {
               console_printf("         %s - %s\n", (*ar_info)->extension, (*ar_info)->description);
           }
           console_printf("       Please note that .pk3 files are identical to .zip files\n");
           console_printf("       Type mount without params to see currently mounted archives\n");
        }
     }
}

void cmd_set(int argc, char** argv) {
     if(argc==1) {
        dump_cvars();
        return;
     }
     if(argc != 4) {
        if(argc==2) { 
          if(strcmp(argv[1],"-h")==0) {
          } else {
            console_printf("Error: insufficient parameters to set command\n");
          }
        } else {
           console_printf("Error: insufficient parameters to set command\n");
        }
        console_printf("Usage: set <type> <varname> <value>\n");
        console_printf("       <type>      one of s,i,f or b\n");
        console_printf("       <varname>   variable name to set\n");
        console_printf("       <value>     value to set\n");
        console_printf("       \n");
        console_printf("       Data types supported: string(s), integer(i), float(f), boolean(b)\n");
        console_printf("       For boolean values, valid values are: true, false\n");
        console_printf("       Type set without params to see current CVars\n");
        return;
     }
     switch(argv[1][0]) {
         case 's':
            set_cvar_s(argv[2],argv[3]);
            console_printf("%s=\"%s\"\n",argv[2],argv[3]);
         break;
         case 'i':
            set_cvar_i(argv[2],atoi(argv[3]));
            console_printf("%s=%d\n",argv[2],atoi(argv[3]));
         break;
         case 'f':
            set_cvar_f(argv[2],((float)atof(argv[3])));
            console_printf("%s=%f\n",argv[2],atof(argv[3]));
         break;
         case 'b':
            if(strcmp(argv[3],"true")==0)  set_cvar_b(argv[2],1);
            if(strcmp(argv[3],"false")==0) set_cvar_b(argv[2],0);
            console_printf("%s=%s",argv[2],get_cvar_i(argv[2]));
         break;
         default:
            console_printf("Error: invalid data type %s\n",argv[1]);
         break;
     }
}

void cmd_help(int argc, char** argv) {
     int i;
     char* help_args[2] = {"help","-h"};
     if(argc==1) {
        console_printf("Following commands are available, type help <cmd> for more info:\n");
        for(i=0; i< (sizeof(commands_str)/sizeof(char*)); i++) {
            console_printf(" * %s\n",commands_str[i]);
        }
     } else if(argc==2) {
        for(i=0; i< (sizeof(commands_str)/sizeof(char*)); i++) {
            if(strcmp(commands_str[i],argv[1])==0) {
               commands_func[i](2,help_args);
            }
        }
     }
}



void console_defcmd(int argc, char** argv) {
     int i=0;
     if(strcmp(argv[0],"quit")==0) {
        SDL_Event e;
        e.type = SDL_QUIT;
        SDL_PushEvent(&e);
     } else {
        for(i=0; i< (sizeof(commands_str)/sizeof(char*)); i++) {
          if(strcmp(argv[0],commands_str[i])==0) {
              commands_func[i](argc, argv);
           }
        }
     }
}

void console_runcmd(char* cmdline) {
     fprintf(stderr,"] %s\n",cmdline);
     if(strlen(cmdline)==0) return;
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



