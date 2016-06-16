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
//     Console commands
//
//-----------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include <physfs.h>

#include "l_console.h"
#include "cons_cvars.h"
#include "cons_cmds.h"

struct cons_cmd cons_commands[] = {
  {"help", "",&cmd_help},
  {"set",  "",&cmd_set},
  {"mount","",&cmd_mount},
  {"ls",   "",&cmd_ls},
  {"pwd",  "",&cmd_pwd},
  {"cd",   "",&cmd_cd},
  {"cat",  "",&cmd_cat}
};


unsigned int get_cmd_count() {
  return sizeof(cons_commands) / sizeof(struct cons_cmd);
}

void cmd_cat(int argc, char** argv) {
     if(argc!=2) {
        console_printf("Error! insufficient parameters to cat command\n");
        return;
     }
     if(strcmp(argv[1],"-h")==0) {
        console_printf("Usage: cat <filename>\n");
        console_printf("       <filename> the file to dump to console\n");
     }
     char filepath[PATH_MAX];
     if(argv[1][0]=='/') {
        snprintf(filepath,PATH_MAX-1,"%s",argv[1]);
     } else {
        char *cwd = get_cvar_s("cwd");
        if(cwd[strlen(cwd)-1]=='/') {
           snprintf(filepath,PATH_MAX-1,"%s%s",cwd,argv[1]);
        } else {
           snprintf(filepath,PATH_MAX-1,"%s/%s",cwd,argv[1]);
        }
     }
     if(PHYSFS_exists((const char*)filepath)==0) {
        console_printf("Error! File %s not found or not a file\n",filepath);
        return;
     }
     PHYSFS_File* fd = PHYSFS_openRead((const char*)filepath);
     if(fd==NULL) {
        console_printf("Error! %s\n",PHYSFS_getLastError());
        return;
     }
     PHYSFS_uint32 f_size = PHYSFS_fileLength(fd);
     void* f_data = malloc(f_size);
     PHYSFS_read(fd,f_data,f_size,1);
     console_printf("%s\n",(char*)f_data);
     free(f_data);
     PHYSFS_close(fd);
}

void cmd_cd(int argc, char** argv) {
     if(argc!=2) {
        console_printf("Error! insufficient parameters to cd command\n");
        return;
     }
     if(strcmp(argv[1],"-h")==0) {
        console_printf("Usage: cd <path\n");
        console_printf("       <path> path to change to, do not use trailing /\n");
        console_printf("       The path may be relative to CWD or absolute\n");
        return;
     }
     char new_cwd[PATH_MAX];
     if(argv[1][0]=='/') {
        snprintf(new_cwd,PATH_MAX-1,"%s/",argv[1]);
     } else {
        char *cwd = get_cvar_s("cwd");
        if(cwd[strlen(cwd)-1]=='/') {
          snprintf(new_cwd,PATH_MAX-1,"%s%s/",get_cvar_s("cwd"),argv[1]);
        } else {
          snprintf(new_cwd,PATH_MAX-1,"%s/%s/",get_cvar_s("cwd"),argv[1]);
        }
     }
     if(strcmp(argv[1],"/")==0) {
        set_cvar_s("cwd","/");
        return;
     }
     int in_mount=0;
     int i=0;
     char **p;
     for(p = PHYSFS_getSearchPath(); *p != NULL; p++) {
         if(strcmp(new_cwd+1,PHYSFS_getMountPoint(*p))==0) in_mount=1;
         i++; // needed for bugfix in PHYSFS_freeList
     }
     if(i>1) PHYSFS_freeList(p);
     if(in_mount==0) {
        if(PHYSFS_exists((const char*)new_cwd)==0) {
           console_printf("Error! Path %s not found\n",new_cwd);
           return;
        }
        if(PHYSFS_isDirectory((const char*)new_cwd)==0) {
           console_printf("Error! %s is not a directory!\n",new_cwd);
           return;
        }
     }
     set_cvar_s("cwd",new_cwd);
}

void cmd_pwd(int argc, char** argv) {
     if(argc==2) {
        if(strcmp(argv[1],"-h")!=0) {
           console_printf("Error: pwd command takes no parameters\n");
        }
        console_printf("Usage: pwd\n");
        console_printf("       pwd simply returns the current working directory\n");
        console_printf("       Note that this is done by printing the cwd CVar, which may be set to something invalid\n");
        return;
     }
     console_printf("%s\n",get_cvar_s("cwd"));
}

void cmd_ls(int argc, char** argv) {
     char **rc;
     char **i;
     char *wd;
     if(argc==1) {
        wd = get_cvar_s("cwd");
      }

     if(argc==2) {
        if(strcmp(argv[1],"-h")==0) {
           console_printf("Usage: ls <dir>\n");
           console_printf("       <dir> path to list, if not specified, current directory is default\n");
           return;
        }
        wd = argv[1];
     }

     rc = PHYSFS_enumerateFiles(wd);
     for(i=rc; *i !=NULL; i++) {
         if(argc==1) {
            console_printf("%s\n",*i);
         } else {
            if(wd[strlen(wd)-1] != '/') {
               console_printf("%s/%s\n",wd,*i);
            } else {
               console_printf("%s%s\n",wd,*i);
            }
         }
     }
     PHYSFS_freeList(rc);
}

void cmd_mount(int argc, char** argv) {
     PHYSFS_ArchiveInfo **ar_info;
     int i=0;
     if(argc==1) {
        char **p;
        for(p = PHYSFS_getSearchPath(); *p != NULL; p++) {
            console_printf("%s on %s\n",*p,PHYSFS_getMountPoint(*p));
            i++; // needed for bugfix in PHYSFS_freeList
        }
        if(i>1) PHYSFS_freeList(p);
        return;
     }
     if(argc==2) {
        if(strcmp(argv[1],"-h")==0) {
           console_printf("Usage: mount <archive> <mountpoint\n");
           console_printf("       <archive>     path to an archive file to mount\n");
           console_printf("       <mountpoint>  where in the VFS to mount it - if not specified, / is default\n");
           console_printf("       Archives supported:\n");
           for(ar_info = (PHYSFS_ArchiveInfo**)PHYSFS_supportedArchiveTypes(); *ar_info != NULL; ar_info++) {
               console_printf("         %s - %s\n", (*ar_info)->extension, (*ar_info)->description);
           }
           console_printf("       Please note that .pk3 files are identical to .zip files\n");
           console_printf("       It is also possible to mount directories as archives\n");
           console_printf("       Type mount without params to see currently mounted archives\n");
           return;
        } else {
           int retval=0;
           retval = PHYSFS_mount((const char*)argv[1],"",0);
           if(retval==0) {
              console_printf("Error mounting: %s\n",PHYSFS_getLastError());
              return;
           }
        }
        return;
    }
    if(argc==3) {
       int retval=0;
       retval = PHYSFS_mount((const char*)argv[1],argv[2],0);
       if(retval==0) {
          console_printf("Error mounting: %s\n",PHYSFS_getLastError());
          return;
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
        for(i=0; i< (sizeof(cons_commands)/sizeof(struct cons_cmd)); i++) {
            console_printf(" * %s\n",cons_commands[i].cmd_str);
        }
     } else if(argc==2) {
        for(i=0; i< (sizeof(cons_commands)/sizeof(struct cons_cmd)); i++) {
            if(strcmp(cons_commands[i].cmd_str,argv[1])==0) {
               cons_commands[i].cmd_func(2,help_args);
            }
        }
     }
}
