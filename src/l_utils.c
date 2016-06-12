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
//     misc utils
//
//-----------------------------------------------------------------------------

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include <SDL.h>
#include <SDL_ttf.h>

#include "l_utils.h"
#include "v_init.h"

screen_res get_screen_res() {
           screen_res retval;
           SDL_GL_GetDrawableSize(screen, &(retval.w), &(retval.h));
           return retval;
}

int power_of_two(int input)
{
    int value = 1;

    while ( value < input ) {
        value <<= 1;
    }
    return value;
}

GLuint SDL_GL_LoadTexture(SDL_Surface *surface)
{
    GLfloat texcoord[4];
    GLuint texture;
    int w, h;
    SDL_Surface *image;
    SDL_Rect area;
    Uint8  saved_alpha;
    SDL_BlendMode saved_mode;

    /* Use the surface width and height expanded to powers of 2 */
 //   w = power_of_two(surface->w);
 //   h = power_of_two(surface->h);
    w = surface->w;
    h = surface->h;
    texcoord[0] = 0.0f;         /* Min X */
    texcoord[1] = 0.0f;         /* Min Y */
    texcoord[2] = (GLfloat)surface->w / w;  /* Max X */
    texcoord[3] = (GLfloat)surface->h / h;  /* Max Y */

    image = SDL_CreateRGBSurface(
            SDL_SWSURFACE,
            w, h,
            32,
#if SDL_BYTEORDER == SDL_LIL_ENDIAN /* OpenGL RGBA masks */
            0x000000FF,
            0x0000FF00,
            0x00FF0000,
            0xFF000000
#else
            0xFF000000,
            0x00FF0000,
            0x0000FF00,
            0x000000FF
#endif
               );
    if ( image == NULL ) {
        return 0;
    }

    /* Save the alpha blending attributes */
    SDL_GetSurfaceAlphaMod(surface, &saved_alpha);
    SDL_SetSurfaceAlphaMod(surface, 0xFF);
    SDL_GetSurfaceBlendMode(surface, &saved_mode);
    SDL_SetSurfaceBlendMode(surface, SDL_BLENDMODE_NONE);

    /* Copy the surface into the GL texture image */
    area.x = 0;
    area.y = 0;
    area.w = surface->w;
    area.h = surface->h;
    SDL_BlitSurface(surface, &area, image, &area);

    /* Restore the alpha blending attributes */
    SDL_SetSurfaceAlphaMod(surface, saved_alpha);
    SDL_SetSurfaceBlendMode(surface, saved_mode);

    /* Create an OpenGL texture for the image */
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D,
             0,
             GL_RGBA,
             w, h,
             0,
             GL_RGBA,
             GL_UNSIGNED_BYTE,
             image->pixels);
    SDL_FreeSurface(image); /* No longer needed */

    return texture;
}


