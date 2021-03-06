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

#ifndef __L_CACHE_H_
#define __L_CACHE_H_

#include <uthash.h>

#define CACHE_TYPE_NULL  0 // there's nothing here!
#define CACHE_TYPE_VFS   1 // whole files from the VFS
#define CACHE_TYPE_GL    2 // OpenGL textures
#define CACHE_TYPE_OBJ   3 // arbitary objects

// each cache context is just a hashtable
struct cache_context_t {
   int           cache_type;  // one of the above defines
   int           gl_tex_id;   // for use with OpenGL only
   char*         filename;    // filename the object was loaded from, especially relevant for VFS cache
   unsigned int  data_size;   // size of the data in bytes
   void* data;                // the actual data
   UT_hash_handle hh;
};

// a hashtable that stores all contexts
struct cache_contexts_table_t {
   struct cache_context_t *ctx;       // the cache_context_t hashtable
   char*             ctx_desc;  // human-readable description of this hash context
   UT_hash_handle hh;
};

void init_cache();

// all functions return NULL on error (including not found)

// try and get the specified context (name==ctx_desc), if not found then create it
struct cache_context_t* cache_ctx(char* name);

// evict a cache context, wiping all objects cached and wiping the context itself from the main table
void evict_cache_ctx(char* name);

// evict a single cache entry
void evict_cache_entry(struct cache_context_t** ctx, char* filename);

// cache a file from the VFS and return a pointer to the hashtable entry
struct cache_context_t* cache_vfs_file(struct cache_context_t** ctx, char* filename);

// cache an OpenGL texture from the VFS and return a pointer to the hashtable entry
struct cache_context_t* cache_gl_texture(struct cache_context_t** ctx, char* filename);

// If file is in cache, return the entry, otherwise cache it and then return the entry
struct cache_context_t* cached_file(struct cache_context_t** ctx, char* filename);

// If OpenGL texture of specified name is in cache, return entry, otherwise cache it and then return the entry
struct cache_context_t* cached_texture(struct cache_context_t** ctx, char* filename);

// If object with specified filename is in cache, return entry, otherwise return NULL
// Filename is used here as a key, need not be a real VFS path
struct cache_context_t* cached_obj(struct cache_context_t** ctx, char* filename);


#endif
