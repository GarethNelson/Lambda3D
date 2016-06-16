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
//     Console commands
//
//-----------------------------------------------------------------------------

#ifndef __CONS_CMDS_H_
#define __CONS_CMDS_H_
void cmd_help(int argc, char** argv);
void cmd_set(int argc, char** argv);
void cmd_mount(int argc, char** argv);
void cmd_ls(int argc, char** argv);
void cmd_pwd(int argc, char** argv);
void cmd_cd(int argc, char** argv);
void cmd_cat(int argc, char** argv);

struct cons_cmd {
    char*   cmd_str;
    char*   help_str;
    void  (*cmd_func)(int argc, char** argvv);
};

unsigned int get_cmd_count();

#endif
