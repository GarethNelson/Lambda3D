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
//     Definitions and such used for app flow
//
//-----------------------------------------------------------------------------

#ifndef __L_APPFLOW_H_
#define __L_APPFLOW_H_

#define APPSTAGE_STARTUP    1 // Load initial resources and configure stuff
#define APPSTAGE_SPLASH     2 // Display logo
#define APPSTAGE_STARTMENU  3 // Display menu with new game etc
#define APPSTAGE_LOADSCREEN 4 // Display loading screen and load whatever resources
#define APPSTAGE_INGAME     5 // In an actual game
#define APPSTAGE_GAMEOVER   6 // Game over screen

#define APPFLAGS_NORMAL 0 // Normal state
#define APPFLAGS_PAUSED 1 // Paused
#define APPFLAGS_MENU   2 // Menu displayed
#define APPFLAGS_DEBUG  4 // Show debug info

struct app_stage_t {
   int    stage_id;
   char*  stage_name;
   void   (*init)(void* params);
   void   (*cleanup)();
   void   (*update)();
   void   (*render)();
};

// see l_appflow.c
struct app_stage_t* app_stage_table;

// TODO: add time delta
void init_appstage_table();
char* stage_name(int stage, int flags);
void  switch_appstage(int old_stage, int new_stage, int old_flags, int new_flags, void* params); // must be called from l_main only
                                                                                                 // other modules can send events
void  update_app(int stage, int flags);
void  render_app(int stage, int flags);

#endif
