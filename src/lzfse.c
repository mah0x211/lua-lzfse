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


#include <stdlib.h>
#include <string.h>
#include <errno.h>
// utilities
#include "lauxhlib.h"
// mbedtls headers
#include "lzfse.h"


static int decode_lua( lua_State *L )
{
    size_t slen = 0;
    const char *src = lauxh_checklstring( L, 1, &slen );
    size_t dlen = slen * 4;
    uint8_t *dst = (void*)malloc( dlen );
    void *aux = NULL;
    const char *errstr = NULL;

    if( !dst ){
        lua_pushnil( L );
        lua_pushstring( L, strerror( errno ) );
        return 2;
    }
    else if( !( aux = malloc( lzfse_decode_scratch_size() ) ) ){
        free( dst );
        lua_pushnil( L );
        lua_pushstring( L, strerror( errno ) );
        return 2;
    }

    dlen = lzfse_decode_buffer( dst, dlen, (uint8_t*)src, slen, aux );
    if( dlen ){
        lua_pushlstring( L, (const char*)dst, dlen );
        free( dst );
        free( aux );
        return 1;
    }

    errstr = "failed to lzfse_decode_buffer()";
    free( aux );
    free( dst );
    lua_pushnil( L );
    lua_pushstring( L, errstr );

    return 2;
}


static int encode_lua( lua_State *L )
{
    size_t slen = 0;
    const char *src = lauxh_checklstring( L, 1, &slen );
    size_t buflen = slen;
    size_t dlen = buflen;
    uint8_t *dst = malloc( dlen );
    void *aux = NULL;
    const char *errstr = NULL;
    void *buf = NULL;

    if( !dst ){
        lua_pushnil( L );
        lua_pushstring( L, strerror( errno ) );
        return 2;
    }
    else if( !( aux = malloc( lzfse_encode_scratch_size() ) ) ){
        free( dst );
        lua_pushnil( L );
        lua_pushstring( L, strerror( errno ) );
        return 2;
    }

    while( 1 )
    {
        dlen = lzfse_encode_buffer( dst, dlen, (uint8_t*)src, slen, aux );

        if( dlen ){
            lua_pushlstring( L, (const char*)dst, dlen );
            free( dst );
            free( aux );
            return 1;
        }

        // probably, need a more buffer space
        buflen <<= 1;
        buf = realloc( dst, buflen );
        if( !buf ){
            errstr = strerror( errno );
            break;
        }
        dst = (uint8_t*)buf;
        dlen = buflen;
    }

    free( aux );
    free( dst );
    lua_pushnil( L );
    lua_pushstring( L, errstr );

    return 2;
}


LUALIB_API int luaopen_lzfse( lua_State *L )
{
    struct luaL_Reg funcs[] = {
        { "encode", encode_lua },
        { "decode", decode_lua },
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

