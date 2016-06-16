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
//     Definitions of events
//
//-----------------------------------------------------------------------------

#ifndef __L_EVENTS_H_
#define __L_EVENTS_H_

// types of events used in event.user.code for SDL
#define L_EV_SWITCH_REQ  1 // switch app stage requested
#define L_EV_CVAR_CHANGE 2 // CVar changed

// pass one of these as a pointer in data2
typedef struct l_ev_switch_req {
   int   new_stage;        // 0 to keep the current stage and only change flags
   int   set_flags;        // flags to set
   int   unset_flags;      // flags to unset
   void* new_stage_params; // params for the new stage
} l_ev_switch_req;

typedef struct l_ev_cvar_change {
   char* cvar_name; // we only set the name, if the value is needed, it can be retrieved
} l_ev_cvar_change;

int  get_sdl_type();
void ev_send(int ev_type, void* ev);

#endif
