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
//      Implements basic 2D geometry
//
//-----------------------------------------------------------------------------

#include <stdlib.h>
#include <stdio.h>

#include <SDL.h>
#include <SOIL.h>
#include <physfs.h>
#include <oglconsole.h>

#include "l_utils.h"
#include "l_cache.h"

#ifdef __MACH__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

void enter_2d() {
     screen_res res = get_screen_res();
     glMatrixMode(GL_PROJECTION);
     glLoadIdentity();
     glOrtho(0.0,res.w,res.h, 0.0, 1.0, -1.0);
}

// simple wrapper around OGLCONSOLE_DrawString
void draw_debug_text(char* text, double x, double y) {
     double w = CHAR_PIXEL_W;
     double h = CHAR_PIXEL_H;
     glColor3d(0,1,0);
     glEnable(GL_TEXTURE_2D);
     glBindTexture(GL_TEXTURE_2D, OGLCONSOLE_glFontHandle);

     screen_res res = get_screen_res();
     
     glMatrixMode(GL_PROJECTION);
     glPushMatrix();
     glLoadIdentity();
     glOrtho(0,res.w,0,res.h,-1,1);

     glMatrixMode(GL_MODELVIEW);
     glPushMatrix();
     glLoadIdentity();
     glBegin(GL_QUADS);
       OGLCONSOLE_DrawString(text,x,y+(res.h-h),w,h,0);
     glEnd();
     glPopMatrix();

     glMatrixMode(GL_PROJECTION);
     glPopMatrix();
}

int load_gl_texture_file(char* cache_ctx_name, char* vfs_filename) {
    SDL_LogInfo(SDL_LOG_CATEGORY_RENDER,"Loading texture %s into context %s",vfs_filename,cache_ctx_name);
    struct cache_context_t *ctx     = cache_ctx(cache_ctx_name);
    struct cache_context_t *tex_ctx = cache_gl_texture(&ctx,vfs_filename);
    if(tex_ctx == NULL) {
       SDL_LogError(SDL_LOG_CATEGORY_RENDER,"Could not load texture %s",vfs_filename);
       return -1;
    }
    SDL_LogVerbose(SDL_LOG_CATEGORY_RENDER,"Loaded texture %s with OpenGL ID %d",vfs_filename,tex_ctx->gl_tex_id);
    return tex_ctx->gl_tex_id;
}


