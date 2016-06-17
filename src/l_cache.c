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

struct cache_contexts_table_t* cache_ctx_table=NULL;
struct cache_context_t** global_ctx=NULL;

void init_cache() {
     SDL_LogVerbose(SDL_LOG_CATEGORY_SYSTEM,"Creating global cache context");
     global_ctx = create_cache_ctx("GLOBAL");
}

struct cache_context_t** create_cache_ctx(char* name) {
     struct cache_contexts_table_t *ctx=NULL;
     HASH_FIND_STR(cache_ctx_table, name, ctx);
     if(ctx==NULL) {
        SDL_LogVerbose(SDL_LOG_CATEGORY_SYSTEM,"Cache context %s not found, creating",name);
        ctx = (struct cache_contexts_table_t*)malloc(sizeof(struct cache_contexts_table_t));
        ctx->ctx = NULL;
        ctx->ctx_desc = strdup(name);
        HASH_ADD_STR(cache_ctx_table,ctx_desc,ctx);
     }
     return ctx;
}

struct cache_context_t** cache_ctx(char* name) {
}

void evict_cache_ctx(char* name) {
}

void evict_cache_entry(struct cache_context_t** ctx, char* filename) {
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
