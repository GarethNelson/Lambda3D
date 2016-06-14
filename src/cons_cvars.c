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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "l_console.h"
#include "l_events.h"

#include "cons_cvars.h"


struct cons_cvar_t* cvars_ht=NULL;

void dump_cvars() {
     struct cons_cvar_t* entry=NULL;
     for(entry=cvars_ht; entry!=NULL; entry=entry->hh.next) {
         switch(entry->var_type) {
            case CVAR_STRING:
               console_printf("%s=\"%s\"\n",entry->var_name,entry->s_val);
            break;
            case CVAR_FLOAT:
               console_printf("%s=%f\n",entry->var_name,entry->f_val);
            break;
            case CVAR_BOOL:
               if(entry->i_val==1) {
                 console_printf("%s=True\n",entry->var_name);
               } else if(entry->i_val==0) {
                 console_printf("%s=False\n",entry->var_name);
               } else {
                 console_printf("Error: %s has invalid boolean value %d\n",entry->var_name,entry->i_val);
               }
            break;
            case CVAR_INT:
               console_printf("%s=%d\n",entry->var_name,entry->i_val);
            break;
         }
     }
}

static void send_cvar_ev(char* name) {
       struct l_ev_cvar_change *change_ev = malloc(sizeof(struct l_ev_cvar_change));
       change_ev->cvar_name = strdup(name);
       ev_send(L_EV_CVAR_CHANGE,change_ev);
       free(change_ev);
}

void set_cvar_s(char* name, char* val) {
     struct cons_cvar_t* entry=NULL;
     HASH_FIND_STR(cvars_ht,name,entry);
     if(entry==NULL) {
        entry = malloc(sizeof(struct cons_cvar_t));
        snprintf(entry->var_name,32,"%s",name);
        entry->var_type = CVAR_STRING;
        snprintf(entry->s_val,1024,"%s",val);
        HASH_ADD_STR(cvars_ht,var_name,entry);
     } else {
        entry->var_type = CVAR_STRING;
        snprintf(entry->s_val,1024,"%s",val);
     }
     send_cvar_ev(name);
}

void set_cvar_f(char* name, float val) {
     struct cons_cvar_t* entry=NULL;
     HASH_FIND_STR(cvars_ht,name,entry);
     if(entry==NULL) {
        entry = malloc(sizeof(struct cons_cvar_t));
        snprintf(entry->var_name,32,"%s",name);
        entry->var_type = CVAR_FLOAT;
        entry->f_val    = val;
        HASH_ADD_STR(cvars_ht,var_name,entry);
     } else {
        entry->var_type = CVAR_FLOAT;
        entry->f_val    = val;
     }
     send_cvar_ev(name);
}

void set_cvar_i(char* name, int val) {
     struct cons_cvar_t* entry=NULL;
     HASH_FIND_STR(cvars_ht,name,entry);
     if(entry==NULL) {
        entry = malloc(sizeof(struct cons_cvar_t));
        snprintf(entry->var_name,32,"%s",name);
        entry->var_type = CVAR_INT;
        entry->i_val    = val;
        HASH_ADD_STR(cvars_ht,var_name,entry);
     } else {
        entry->var_type = CVAR_INT;
        entry->i_val    = val;
     }
     send_cvar_ev(name);
}

void set_cvar_b(char* name, int val) {
     struct cons_cvar_t* entry=NULL;
     HASH_FIND_STR(cvars_ht,name,entry);
     if(entry==NULL) {
        entry = malloc(sizeof(struct cons_cvar_t));
        snprintf(entry->var_name,32,"%s",name);
        entry->var_type = CVAR_BOOL;
        entry->i_val    = val;
        HASH_ADD_STR(cvars_ht,var_name,entry);
     } else {
        entry->var_type = CVAR_BOOL;
        entry->i_val    = val;
     }
     send_cvar_ev(name);
}

void toggle_cvar(char* name) {
     struct cons_cvar_t* entry=NULL;
     HASH_FIND_STR(cvars_ht,name,entry);
     if(entry==NULL) {
        entry = malloc(sizeof(struct cons_cvar_t));
        snprintf(entry->var_name,32,"%s",name);
        entry->var_type = CVAR_BOOL;
        entry->i_val    = 0;
        HASH_ADD_STR(cvars_ht,var_name,entry);
     } else {
        entry->var_type = CVAR_BOOL;
        entry->i_val    = !(entry->i_val);
     }
     send_cvar_ev(name);
}

char* get_cvar_s(char* name) {
      struct cons_cvar_t* entry=NULL;
      HASH_FIND_STR(cvars_ht,name,entry);
      if(entry==NULL) return NULL;
      if(entry->var_type != CVAR_STRING) return NULL;
      return entry->s_val;
}

float get_cvar_f(char* name) {
      struct cons_cvar_t* entry=NULL;
      HASH_FIND_STR(cvars_ht,name,entry);
      if(entry==NULL) return 0.0f;
      if(entry->var_type != CVAR_FLOAT) return 0.0f;
      return entry->f_val;
}

int   get_cvar_i(char* name) {
      struct cons_cvar_t* entry=NULL;
      HASH_FIND_STR(cvars_ht,name,entry);
      if(entry==NULL) return 0;
      if((entry->var_type != CVAR_INT) & (entry->var_type != CVAR_BOOL)) return 0;
      return entry->i_val;
}

