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
//     The splash screen and logo
//
//-----------------------------------------------------------------------------


#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include "l_utils.h"
#include "cons_cvars.h"
#include <SDL.h>


float yrot=0.0f;
float  xrot=90.0f;
int draw_text=0;
int fade_out=0;
float fade_alpha=0.0f;

void l_splash_logo_update() {
     float delta = get_cvar_f("delta")/1000.0f;
     yrot+=100.0f*delta;
     xrot-=50.0f*delta;
     printf("%f %f %f\n",yrot,xrot,delta);
     if(yrot>360.0) yrot=0.0f;
     if(fade_out==0) {
        if(yrot>350.0) draw_text++;
        if(draw_text>=10) {
          fade_out=1;
        }
     } else {
       fade_alpha += 1.0f*delta;
       if(fade_alpha >= 1.0f) {
          // switch stage here
       }
     }
}

GLuint lambda_polygons=0;
GLuint lambda_outline=0;
GLuint lambda_faces_quads=0;
GLuint lambda_faces_lines=0;
GLuint lambda_grid=0;
GLuint lambda_text=0;
screen_res s_res;
GLuint grid_list() {
       int i=0;
       GLuint retval=glGenLists(1);
       glNewList(retval,GL_COMPILE);
       glBegin(GL_LINES);
       for(i=-20; i<50; i+=1) {
           glVertex2d(-20.0f,i);
           glVertex2d(50.0f,i);
           glVertex2d(i,-20.0f);
           glVertex2d(i,50.0f);
       }
       glEnd();
       glEndList();
       return retval;
}

GLuint text_list() {
       GLuint retval=glGenLists(1);
       glNewList(retval,GL_COMPILE);
       glBegin(GL_QUADS);
         glVertex2f(6.5f,1.0f); // L
         glVertex2f(7.0f,1.0f);
         glVertex2f(7.0f,3.25f);
         glVertex2f(6.5f,3.25f);
         glVertex2f(6.5f,3.25f);
         glVertex2f(12.0+8.5f,3.25f);
         glVertex2f(12.0+8.5f,3.5f);
         glVertex2f(6.5f,3.5f);

         glVertex2f(7.5f,1.0f); // A
         glVertex2f(9.5f,1.0f);
         glVertex2f(9.5f,1.25f);
         glVertex2f(7.5f,1.25f);
         glVertex2f(7.5f,1.0f);
         glVertex2f(8.0f,1.0f);
         glVertex2f(8.0f,3.0f);
         glVertex2f(7.5f,3.0f);
         glVertex2f(9.0f,1.0f);
         glVertex2f(9.5f,1.0f);
         glVertex2f(9.5f,3.0f);
         glVertex2f(9.0f,3.0f);
         glVertex2f(7.5f,1.75f);
         glVertex2f(9.5f,1.75f);
         glVertex2f(9.5f,2.0f);
         glVertex2f(7.5f,2.0f);

         glVertex2f(10.0f,1.0f); // M
         glVertex2f(12.5f,1.0f);
         glVertex2f(12.5f,1.25f);
         glVertex2f(10.0f,1.25f);
         glVertex2f(10.0f,1.0f);
         glVertex2f(10.5f,1.0f);
         glVertex2f(10.5f,3.0f);
         glVertex2f(10.0f,3.0f);
         glVertex2f(11.0f,1.0f);
         glVertex2f(11.5f,1.0f);
         glVertex2f(11.5f,3.0f);
         glVertex2f(11.0f,3.0f);
         glVertex2f(12.0f,1.0f);
         glVertex2f(12.5f,1.0f);
         glVertex2f(12.5f,3.0f);
         glVertex2f(12.0f,3.0f);
         
         glVertex2f(13.0f,1.0f); // B
         glVertex2f(13.5f,1.0f);
         glVertex2f(13.5f,3.0f);
         glVertex2f(13.0f,3.0f);
         
         glVertex2f(13.0f,1.0f);
         glVertex2f(14.5f,1.0f);
         glVertex2f(14.5f,1.25f);
         glVertex2f(13.0f,1.25f);
         
         glVertex2f(13.0f,1.875f);
         glVertex2f(14.5f,1.875f);
         glVertex2f(14.5f,2.125f);
         glVertex2f(13.0f,2.125f);
         
         glVertex2f(13.0f,2.75f);
         glVertex2f(14.5f,2.75f);
         glVertex2f(14.5f,3.0f);
         glVertex2f(13.0f,3.0f);
         
         glVertex2f(14.5f,1.25f);
         glVertex2f(15.0f,1.25f);
         glVertex2f(15.0f,1.75f);
         glVertex2f(14.5f,1.75f);
         
         glVertex2f(14.5f,2.25f);
         glVertex2f(15.0f,2.25f);
         glVertex2f(15.0f,2.75f);
         glVertex2f(14.5f,2.75f); 

         glVertex2f(15.5f,1.0f); // D
         glVertex2f(16.0f,1.0f);
         glVertex2f(16.0f,3.0f);
         glVertex2f(15.5f,3.0f);
         
         glVertex2f(16.0f,1.0f);
         glVertex2f(17.0f,1.0f);
         glVertex2f(17.0f,1.25f);
         glVertex2f(16.0f,1.25f);
         
         glVertex2f(16.0f,2.75f);
         glVertex2f(17.0f,2.75f);
         glVertex2f(17.0f,3.0f);
         glVertex2f(16.0f,3.0f);
         
         glVertex2f(17.5f,1.5f);
         glVertex2f(18.0f,1.5f);
         glVertex2f(18.0f,2.5f);
         glVertex2f(17.5f,2.5f);
         
         glVertex2f(11.0+7.5f,1.0f); // A
         glVertex2f(11.0+9.5f,1.0f);
         glVertex2f(11.0+9.5f,1.25f);
         glVertex2f(11.0+7.5f,1.25f);
         glVertex2f(11.0+7.5f,1.0f);
         glVertex2f(11.0+8.0f,1.0f);
         glVertex2f(11.0+8.0f,3.0f);
         glVertex2f(11.0+7.5f,3.0f);
         glVertex2f(11.0+9.0f,1.0f);
         glVertex2f(11.0+9.5f,1.0f);
         glVertex2f(11.0+9.5f,3.0f);
         glVertex2f(11.0+9.0f,3.0f);
         glVertex2f(11.0+7.5f,1.75f);
         glVertex2f(11.0+9.5f,1.75f);
         glVertex2f(11.0+9.5f,2.0f);
         glVertex2f(11.0+7.5f,2.0f);

       glEnd();

       glBegin(GL_TRIANGLES);
         glVertex2f(14.5f,1.0f);   // B
         glVertex2f(15.0f,1.25f);
         glVertex2f(14.5f,1.25f);
         glVertex2f(14.5f,2.0f);
         glVertex2f(15.0f,1.75f);
         glVertex2f(14.5f,1.75f);
         glVertex2f(14.5,2.0f);
         glVertex2f(15.0f,2.25f);
         glVertex2f(14.5f,2.25f);
         glVertex2f(14.5f,2.75f);
         glVertex2f(15.0f,2.75f);
         glVertex2f(14.5f,3.0f);

         glVertex2f(17.0f,1.25f); // D
         glVertex2f(17.5f,1.25f);
         glVertex2f(17.5f,1.5f);
         glVertex2f(17.0f,1.0f);
         glVertex2f(17.5f,1.25f);
         glVertex2f(17.0f,1.25f);
         glVertex2f(17.5f,1.25f);
         glVertex2f(18.0f,1.5f);
         glVertex2f(17.5f,1.5f);
         glVertex2f(17.5f,2.5f);
         glVertex2f(17.5f,2.75f);
         glVertex2f(17.0f,2.75f);
         glVertex2f(17.5f,2.5f);
         glVertex2f(18.0f,2.5f);
         glVertex2f(17.5f,2.75f);


         glVertex2f(17.0f,2.75f);
         glVertex2f(17.5f,2.75f);
         glVertex2f(17.0f,3.0f);



       glEnd();
       glEndList();
       return retval;
}

GLuint faces_list(GLuint mode) {
       GLuint retval=glGenLists(1);
       glNewList(retval,GL_COMPILE);
       glBegin(mode);
       glVertex3f(0.0f,0.5f,0.0f);  // 0, forward
       glVertex3f(0.0f,0.5f,1.0f);  // 0, backward
       glVertex3f(1.5f,0.5f,1.0f);  // 1, backward
       glVertex3f(1.5f,0.5f,0.0f);  // 1, forward

       glVertex3f(1.5f,0.5f,0.0f); // 1,forward
       glVertex3f(1.5f,0.5f,1.0f); // 1,backward
       glVertex3f(5.25,4.25f,1.0f); // 2, backward
       glVertex3f(5.25,4.25f,0.0f); // 2, forward

       glVertex3f(5.25,4.25f,0.0f); // 2, forward
       glVertex3f(5.25,4.25f,1.0f); // 2, backward
       glVertex3f(6.5f,4.0f,1.0f); // 3, backward
       glVertex3f(6.5f,4.0f,0.0f); // 3, forward

       glVertex3f(6.5f,4.0f,0.0f); // 3, forward
       glVertex3f(6.5f,4.0f,1.0f); // 3, backward
       glVertex3f(7.0f,4.5f,1.0f); // 4, backward
       glVertex3f(7.0f,4.5f,0.0f); // 4, forward
       
       glVertex3f(7.0f,4.5f,0.0f); // 4, forward
       glVertex3f(7.0f,4.5f,1.0f); // 4, backward
       glVertex3f(5.0f,5.0f,1.0f); // 5, backward
       glVertex3f(5.0f,5.0f,0.0f); // 5, forward

       glVertex3f(5.0f,5.0f,0.0f); // 5, forward
       glVertex3f(5.0f,5.0f,1.0f); // 5, backward
       glVertex3f(2.5f,2.5f,1.0f); // 6, backward
       glVertex3f(2.5f,2.5f,0.0f); // 6, forward

       glVertex3f(2.5f,2.5f,0.0f); // 6, forward
       glVertex3f(2.5f,2.5f,1.0f); // 6, backward
       glVertex3f(0.0f,4.75f,1.0f); // 7, backward
       glVertex3f(0.0f,4.75f,0.0f); // 7, forward

       glVertex3f(0.0f,4.75f,0.0f); // 7, forward
       glVertex3f(0.0f,4.75f,1.0f); // 7, backward
       glVertex3f(-1.0f,4.75f,1.0f); // 8, backward
       glVertex3f(-1.0f,4.75f,0.0f); // 8, forward

       glVertex3f(-1.0f,4.75f,0.0f); // 8, forward
       glVertex3f(-1.0f,4.75f,1.0f); // 8, backward
       glVertex3f(2.0f,2.0f,1.0f); // 9, backward
       glVertex3f(2.0f,2.0f,0.0f); // 9, forward

       glVertex3f(2.0f,2.0f,0.0f); // 9, forward
       glVertex3f(2.0f,2.0f,1.0f); // 9, backward
       glVertex3f(1.0f,1.0f,1.0f); // 10, backward
       glVertex3f(1.0f,1.0f,0.0f); // 10, forward

       glVertex3f(1.0f,1.0f,0.0f); // 10, forward
       glVertex3f(1.0f,1.0f,1.0f); // 10, backward
       glVertex3f(0.0f,1.0f,1.0f); // 11, backward
       glVertex3f(0.0f,1.0f,0.0f); // 11, forward

       glVertex3f(0.0f,1.0f,0.0f); // 11, forward
       glVertex3f(0.0f,1.0f,1.0f); // 11, backward
       glVertex3f(0.0f,0.5f,1.0f); // 12, backward
       glVertex3f(0.0f,0.5f,0.0f); // 12, forward
       glEnd();

       glEndList();
       return retval;
}

GLuint outline_list() {
       GLuint retval=glGenLists(1);
       GLuint shape=glGenLists(1);
       glNewList(shape,GL_COMPILE);
       glBegin(GL_LINE_STRIP);
         glVertex3f(0.0f,0.5f,0.0f);  // 0
         glVertex3f(1.5f,0.5f,0.0f);  // 1
         glVertex3f(5.25f,4.25f,0.0f);  // 2
         glVertex3f(6.5f,4.0f,0.0f);  // 3
         glVertex3f(7.0f,4.5f,0.0f);  // 4
         glVertex3f(5.0f,5.0f,0.0f);  // 5
         glVertex3f(2.5f,2.5f,0.0f);  // 6
         glVertex3f(0.0f,4.75f,0.0f);  // 7
         glVertex3f(-1.0f,4.75f,0.0f); // 8
         glVertex3f(2.0f,2.0f,0.0f);  // 9
         glVertex3f(1.0f,1.0f,0.0f);  // 10
         glVertex3f(0.0f,1.0f,0.0f);  // 11
         glVertex3f(0.0f,0.5f,0.0f);  // 12
       glEnd();
       glEndList();
       glNewList(retval,GL_COMPILE);
         glCallList(lambda_faces_lines);
         glCallList(shape);
         glTranslatef(0.0,0.0,1.0);
         glCallList(shape);
         glTranslatef(0.0,0.0,-1.0);
       glEndList();
     return retval;
}

GLuint polygon_list() {
       GLuint retval=glGenLists(1);
       glNewList(retval, GL_COMPILE);
       glBegin(GL_QUADS);
       glVertex2f(0.0f,0.5f);
       glVertex2f(1.0f,0.5f);
       glVertex2f(1.0f,1.0f);
       glVertex2f(0.0f,1.0f);
       
       glVertex2f(1.0f,0.5f);
       glVertex2f(1.5f,0.5f);
       glVertex2f(1.5f,1.0f);
       glVertex2f(1.0f,1.0f);

       glVertex2f(2.0f,2.0f);
       glVertex2f(2.5f,2.5f);
       glVertex2f(0.0f,4.75f);
       glVertex2f(-1.0f,4.75f);

       glVertex2f(2.0f,2.0f);
       glVertex2f(3.0f,2.0f);
       glVertex2f(5.25f,4.25f);
       glVertex2f(5.0f,5.0f);

       glVertex2f(5.25f,4.25f);
       glVertex2f(6.5f,4.0f);
       glVertex2f(7.0f,4.5f);
       glVertex2f(5.0f,5.0f);
     glEnd();

     glBegin(GL_TRIANGLES);
       glVertex2f(1.5f,0.5f);
       glVertex2f(2.0f,1.0f);
       glVertex2f(1.5f,1.0f);

       glVertex2f(1.0f,1.0f);
       glVertex2f(2.0f,1.0f);
       glVertex2f(2.0f,2.0f);

       glVertex2f(2.0f,1.0f);
       glVertex2f(3.0f,2.0f);
       glVertex2f(2.0f,2.0f);
     glEnd();

       glEndList();
     return retval;
}


void l_splash_logo_init_scene() {
     s_res = get_screen_res();
     glMatrixMode(GL_PROJECTION);
     glLoadIdentity();
     glOrtho(0.0,s_res.w,s_res.h,0.0,1.0,-1.0);
     glClearColor(0.0f,0.0f,0.0f,0.0f);
     glEnable(GL_DEPTH_TEST);
     glDepthFunc(GL_LEQUAL); 
     glShadeModel( GL_SMOOTH );
}

void l_splash_logo_render() {
     glDisable(GL_TEXTURE_2D);
     glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
     glMatrixMode(GL_MODELVIEW);
     glPushMatrix();
     glLoadIdentity();

     glTranslatef((s_res.w/2)-600.0,(s_res.h/2)-300.0,0.0);
     glScaled(50,100,0);

     glColor3d(1,1,1);
     if(draw_text==0) glCallList(lambda_grid);

     if(draw_text>=1) {
        glColor3d(1,0,0);
        glCallList(lambda_text);
     }

     if(xrot>0.0f) glRotatef(360.0-xrot,1.0f,0.0f,0.0f);
     glTranslatef(3.0,0.0,0.0);
     glRotatef(360.0-yrot, 0.0f,1.0f,0.0f);
     glTranslatef(-3.0,0.0,0.0);

     if(draw_text==1) {
       glColor3d(1,0,0);
       draw_text=2;
     } else {
       glColor3d(0,0,1);
     }

     glCallList(lambda_polygons);
     glTranslatef(0.0,0.0,1.0);
     glCallList(lambda_polygons);
     glTranslatef(0.0,0.0,-1.0);

     glCallList(lambda_faces_quads);

     glColor3d(1,1,1);

     glCallList(lambda_outline);

     if(fade_out==1) {
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(0,0,0,fade_alpha);
        glBegin(GL_QUADS);
          glVertex2f(0.0f,0.0f);
          glVertex2f(s_res.w,0.0f);
          glVertex2f(s_res.w,s_res.h);
          glVertex2f(0.0f,s_res.h);
        glEnd();
        glDisable(GL_BLEND);
     }

     glPopMatrix();

}

void l_splash_logo_init(void* params) {
     SDL_LogVerbose(SDL_LOG_CATEGORY_SYSTEM,"l_splash init");
    lambda_grid          = grid_list();
    lambda_polygons      = polygon_list();
    lambda_faces_quads   = faces_list(GL_QUADS);
    lambda_faces_lines   = faces_list(GL_LINE_STRIP);
    lambda_outline       = outline_list();
    lambda_text          = text_list();
    l_splash_logo_init_scene();

}
