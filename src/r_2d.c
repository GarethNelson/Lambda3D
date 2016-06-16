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

int load_gl_texture_file(char* vfs_filename) {
  SDL_LogInfo(SDL_LOG_CATEGORY_RENDER,"Loading texture %s",vfs_filename);

  PHYSFS_File* fd = PHYSFS_openRead(vfs_filename);
  if(fd==NULL) {
     SDL_LogError(SDL_LOG_CATEGORY_RENDER,"Error loading texture %s",vfs_filename);
     return -1;
  }
  PHYSFS_uint32 tex_size = PHYSFS_fileLength(fd);
  void* tex_buf = malloc(tex_size);
  PHYSFS_read(fd,tex_buf,(PHYSFS_uint32)tex_size,1);
  PHYSFS_close(fd);
  
  int tex_id = SOIL_load_OGL_texture_from_memory((const unsigned char*)tex_buf,tex_size,SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,0);
  free(tex_buf);
  SDL_LogVerbose(SDL_LOG_CATEGORY_RENDER,"Loaded texture %s with OpenGL ID %d",vfs_filename,tex_id);
  return tex_id;

}


