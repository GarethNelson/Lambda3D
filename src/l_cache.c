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

#include <uthash.h>
#include <SDL.h>

#include "l_cache.h"
#include "l_console.h"
#include "cons_cmds.h"

struct cache_contexts_table_t* cache_ctx_table=NULL;
struct cache_context_t* global_ctx=NULL;

void cmd_lscachectx(int argc, char** argv);
void cmd_evictcachectx(int argc, char** argv);

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
};

void cmd_evictcachectx(int argc, char** argv) {
     if(argc==2) {
        evict_cache_ctx(argv[1]);
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
       console_printf("Dumping cache context %s:\n", argv[1]);
       struct cache_context_t *dump_ctx = cache_ctx(argv[1]);
       struct cache_context_t *entry;
       for(entry=dump_ctx; entry != NULL; entry=entry->hh.next) {
           console_printf(" %s %d %d\n",entry->filename,entry->cache_type,entry->data_size);
       }
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
       free(entry->filename);
       free(entry->data);
       free(entry);
     }
}

struct cache_context_t* cache_vfs_file(struct cache_context_t** ctx, char* filename) {
}

struct cache_context_t* cache_gl_texture(struct cache_context_t** ctx, char* filename) {
}

struct cache_context_t* cached_file(struct cache_context_t** ctx, char* filename) {
}

struct cache_context_t* cached_texture(struct cache_context_t** ctx, char* filename) {
}

struct cache_context_t* cached_obj(struct cache_context_t** ctx, char* filename) {
}
