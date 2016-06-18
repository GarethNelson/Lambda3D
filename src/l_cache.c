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
//     Implementation of the cache layer
//
//-----------------------------------------------------------------------------

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

#include <uthash.h>
#include <SDL.h>
#include <physfs.h>
#include <SOIL.h>

#ifdef __MACH__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include "l_cache.h"
#include "l_console.h"
#include "cons_cvars.h"
#include "cons_cmds.h"

struct cache_contexts_table_t* cache_ctx_table=NULL;
struct cache_context_t* global_ctx=NULL;

void cmd_lscachectx(int argc, char** argv);
void cmd_evictcachectx(int argc, char** argv);
void cmd_evict(int argc, char** argv);
void cmd_cacheload(int argc, char** argv);

static struct cons_cmd cache_commands[] = {
  {"lscachectx","Lists cache contexts or the contents of a particular cache context",
    {"context",NULL,NULL},
    {"the context to list, optional argument",NULL,NULL},
    {"If no parameters are supplied, all cache contexts are listed",
     "Please note that if the context does not exist, it will be created",
     "When listing a cache context's contents, the output columns will be: filename, cache type, size"},
    &cmd_lscachectx},
  {"evictcachectx","Evicts a cache context - this wipes it completely and deregisters it",
    {"context",NULL,NULL},
    {"the context to evict",NULL,NULL},
    {"Note that if the context is currently in use by the engine, this might cause a segfault",
     "You have been warned!",
     NULL},
    &cmd_evictcachectx},
  {"evict","Evicts a single entry from a cache context",
   {"context",
    "filename",
    NULL},
   {"the context to evict from",
    "the filename of the entry to evict",
    NULL},
   {"Note that evicting textures from cache can cause rendering glitches",
    "Please also note that evicting NULL is not supported - NULL is required for implementation reasons",
    NULL},
   &cmd_evict},
  {"cacheload","Loads a file from the VFS into a cache context",
   {"context",
    "filename",
    NULL},
   {"the context to load into",
    "the filename of the file to load - may be a relative path",
    NULL},
   {"Please note that you can not overwrite an existing entry without first evicting it",
    NULL,
    NULL},
   &cmd_cacheload},
};

void cmd_cacheload(int argc, char** argv) {
     if(argc==3) {
       struct cache_context_t *ctx = cache_ctx(argv[1]);
       char*  filename             = argv[2];
       char*  cwd                  = get_cvar_s("cwd");
       char   filepath[PATH_MAX];
       if(filename[0]=='/') {
          snprintf(filepath,PATH_MAX-1,"%s",filename);
       } else {
          if(cwd[strlen(cwd)-1]=='/') {
             snprintf(filepath,PATH_MAX-1,"%s%s",cwd,filename);
          } else {
             snprintf(filepath,PATH_MAX-1,"%s/%s",cwd,filename);
          }
       }
       cache_vfs_file(&ctx,filepath);
     } else {
       console_printf("Error! insufficient parameters to cacheload command\n");
     }
}

void cmd_evict(int argc, char** argv) {
     if(argc==3) {
       struct cache_context_t *ctx       = cache_ctx(argv[1]);
       char*  filename                   = argv[2];
       if(strcmp(argv[2],"NULL")==0) {
          console_printf("Error! Can not evict NULL\n");
          return;
       }
       evict_cache_entry(&ctx,filename);
     } else {
       console_printf("Error! insufficient parameters to evict command\n");
     }
}

void cmd_evictcachectx(int argc, char** argv) {
     if(argc==2) {
        evict_cache_ctx(argv[1]);
     } else {
        console_printf("Error! insufficient parameters to evictcachectx command\n");
     }
}

void cmd_lscachectx(int argc, char** argv) {
    if(argc==1) {
       console_printf("Currently registered cache contexts:\n");
       struct cache_contexts_table_t *ctx;
       for(ctx=cache_ctx_table; ctx != NULL; ctx=ctx->hh.next) {
           console_printf(" %s\n",ctx->ctx_desc);
       }
       return;
    } else if(argc==2) {
       unsigned int total_size=0;
       unsigned int total_items=0;
       console_printf("Dumping cache context %s:\n", argv[1]);
       struct cache_context_t *dump_ctx = cache_ctx(argv[1]);
       struct cache_context_t *entry;
       for(entry=dump_ctx; entry != NULL; entry=entry->hh.next) {
           console_printf(" %s %d %d\n",entry->filename,entry->cache_type,entry->data_size);
           total_items++;
           total_size+= entry->data_size;
       }
       console_printf("%d items, total of %d bytes\n",total_items,total_size);
       return;
    } else {
       console_printf("Error! invalid parameters to lscachectx command\n");
    }
}

void init_cache() {
     SDL_LogVerbose(SDL_LOG_CATEGORY_SYSTEM,"Creating global cache context");
     global_ctx = cache_ctx("GLOBAL");
     SDL_LogVerbose(SDL_LOG_CATEGORY_SYSTEM,"Adding cache commands to console");
     int i=0;
     for(i=0; i < (sizeof(cache_commands)/sizeof(struct cons_cmd)); i++) {
         add_command(cache_commands[i]);
     }
}

struct cache_context_t* cache_ctx(char* name) {
     struct cache_contexts_table_t *ctx=NULL;
     HASH_FIND_STR(cache_ctx_table, name, ctx);
     if(ctx==NULL) {
        SDL_LogVerbose(SDL_LOG_CATEGORY_SYSTEM,"Cache context %s not found, creating",name);
        
 

        ctx = (struct cache_contexts_table_t*)malloc(sizeof(struct cache_contexts_table_t));
        ctx->ctx = NULL;
        ctx->ctx_desc = strdup(name);
        HASH_ADD_STR(cache_ctx_table,ctx_desc,ctx);

        struct cache_context_t *null_entry = (struct cache_context_t*)malloc(sizeof(struct cache_context_t));
        null_entry->cache_type = CACHE_TYPE_NULL;
        null_entry->filename   = strdup("NULL");
        null_entry->data_size  = 5;
        null_entry->data       = (void*)strdup("NULL");
        
        HASH_ADD_STR(ctx->ctx,filename,null_entry);
     }
     return ctx->ctx;
}

void evict_cache_ctx(char* name) {
     SDL_LogVerbose(SDL_LOG_CATEGORY_SYSTEM,"Evicting cache context %s",name);
     struct cache_context_t *ctx = cache_ctx(name);
     struct cache_context_t *s, *tmp;
     HASH_ITER(hh, ctx, s, tmp) {
         SDL_LogVerbose(SDL_LOG_CATEGORY_SYSTEM,"Evicting %s from context %s",s->filename,name);
         evict_cache_entry(&ctx,s->filename);
     }
     struct cache_contexts_table_t *ctx_entry=NULL;
     HASH_FIND_STR(cache_ctx_table, name, ctx_entry);
     SDL_LogVerbose(SDL_LOG_CATEGORY_SYSTEM,"Removing context %s from table",name);
     HASH_DEL(cache_ctx_table,ctx_entry);
     free(ctx_entry);
}

void evict_cache_entry(struct cache_context_t** ctx, char* filename) {
     SDL_LogVerbose(SDL_LOG_CATEGORY_SYSTEM,"Evicting %s from cache",filename);
     struct cache_context_t *entry=NULL;
     HASH_FIND_STR(*ctx,filename,entry);
     if(entry==NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_SYSTEM,"%s already evicted from context!",filename);
     } else {
       HASH_DEL(*ctx,entry);
       if(entry->cache_type == CACHE_TYPE_GL) {
          SDL_LogVerbose(SDL_LOG_CATEGORY_SYSTEM,"Unloading evicted OpenGL texture %d",entry->gl_tex_id);
          glDeleteTextures(1, &(entry->gl_tex_id));
       }
       free(entry->filename);
       free(entry->data);
       free(entry);
     }
}

struct cache_context_t* cache_vfs_file(struct cache_context_t** ctx, char* filename) {
     SDL_LogVerbose(SDL_LOG_CATEGORY_SYSTEM,"Caching file %s as VFS object",filename);
     if(PHYSFS_exists((const char*)filename)==0) {
        SDL_LogError(SDL_LOG_CATEGORY_SYSTEM,"%s does not exist!",filename);
        return NULL;
     }
     PHYSFS_File* fd = PHYSFS_openRead(filename);
     if(fd==NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_SYSTEM,"error opening %s: %s",filename,PHYSFS_getLastError());
        return NULL;
     }
     PHYSFS_uint32 f_size = PHYSFS_fileLength(fd);
     void* f_data         = malloc(f_size);
     PHYSFS_read(fd,f_data,(PHYSFS_uint32)f_size,1);
     PHYSFS_close(fd);

     struct cache_context_t *entry = (struct cache_context_t*)malloc(sizeof(struct cache_context_t));
     entry->cache_type = CACHE_TYPE_VFS;
     entry->filename   = strdup(filename);
     entry->data_size  = f_size;
     entry->data       = f_data;
     HASH_ADD_STR(*ctx,filename,entry);
     SDL_LogVerbose(SDL_LOG_CATEGORY_SYSTEM,"Loaded %s into VFS cache",filename);
     return entry;
}

struct cache_context_t* cache_gl_texture(struct cache_context_t** ctx, char* filename) {
     SDL_LogVerbose(SDL_LOG_CATEGORY_SYSTEM,"Caching file %s as OpenGL texture",filename);
     if(PHYSFS_exists((const char*)filename)==0) {
        SDL_LogError(SDL_LOG_CATEGORY_SYSTEM,"%s does not exist!",filename);
        return NULL;
     }
     PHYSFS_File* fd = PHYSFS_openRead(filename);
     if(fd==NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_SYSTEM,"error opening %s: %s",filename,PHYSFS_getLastError());
        return NULL;
     }
     PHYSFS_uint32 f_size = PHYSFS_fileLength(fd);
     void* f_data         = malloc(f_size);
     PHYSFS_read(fd,f_data,(PHYSFS_uint32)f_size,1);
     PHYSFS_close(fd);

     struct cache_context_t *entry = (struct cache_context_t*)malloc(sizeof(struct cache_context_t));
     entry->cache_type = CACHE_TYPE_GL;
     entry->filename   = strdup(filename);
     entry->data_size  = f_size;
     entry->data       = f_data;
     entry->gl_tex_id  = SOIL_load_OGL_texture_from_memory((const unsigned char*)f_data,f_size,SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,0);

     HASH_ADD_STR(*ctx,filename,entry);
     SDL_LogVerbose(SDL_LOG_CATEGORY_SYSTEM,"Loaded %s into texture cache",filename);
     return entry;
}

struct cache_context_t* cached_file(struct cache_context_t** ctx, char* filename) {
     struct cache_context_t* entry=NULL;
     HASH_FIND_STR(*ctx,filename,entry);
     if(entry==NULL) {
        entry = cache_vfs_file(ctx,filename);
     }
     return entry;
}

struct cache_context_t* cached_texture(struct cache_context_t** ctx, char* filename) {
}

struct cache_context_t* cached_obj(struct cache_context_t** ctx, char* filename) {
}
