/*
 *  Copyright (C) 2016 Masatoshi Teruya
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a
 *  copy of this software and associated documentation files (the "Software"),
 *  to deal in the Software without restriction, including without limitation
 *  the rights to use, copy, modify, merge, publish, distribute, sublicense,
 *  and/or sell copies of the Software, and to permit persons to whom the
 *  Software is furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 *  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 *  DEALINGS IN THE SOFTWARE.
 *
 *  src/lzfse.c
 *  lua-lzfse
 *  Created by Masatoshi Teruya on 16/07/14.
 */


#include <string.h>
#include <errno.h>
// utilities
#include "lauxhlib.h"
// mbedtls headers
#include "lzfse.h"


LUALIB_API int luaopen_lzfse( lua_State *L )
{
    struct luaL_Reg funcs[] = {
        { NULL, NULL }
    };
    struct luaL_Reg *ptr = funcs;

    // create table
    lua_newtable( L );
    // add functions
    while( ptr->name ){
        lauxh_pushfn2tbl( L, ptr->name, ptr->func );
        ptr++;
    }

    return 1;
}

