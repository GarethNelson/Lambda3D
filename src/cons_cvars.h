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
//     CVars (Console variables) implementation
//
//-----------------------------------------------------------------------------

#ifndef __CONS_CVARS_H_
#define __CONS_CVARS_H_

#include <uthash.h>

// Data types
#define CVAR_STRING 1
#define CVAR_FLOAT  2
#define CVAR_BOOL   3
#define CVAR_INT    4

struct cons_cvar_t {
     char  var_name[32];
     int   var_type;
     char  s_val[1024];
     float f_val;
     int   i_val; // boolean values are just integers of 1 or 0
     UT_hash_handle hh;
};

void dump_cvars(); // dump all cvars to the console
void set_cvar_s(char* name, char* val); // set a cvar to a string
void set_cvar_f(char* name, float val); // set a cvar to a float
void set_cvar_i(char* name, int val);   // set a cvar to an int
void set_cvar_b(char* name, int val);   // set a cvar to a boolean
void toggle_cvar(char* name);           // toggle a boolean cvar

char* get_cvar_s(char* name); // return a copy of the value of a string cvar, must free() after
float get_cvar_f(char* name); // return a float cvar's value
int   get_cvar_i(char* name); // return an int/bool cvar's value

#endif
