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

#include <errno.h>
#include <stdlib.h>
#include <string.h>
// utilities
#include "lauxhlib.h"
// mbedtls headers
#include "lzfse.h"

static int decode_lua(lua_State *L)
{
    size_t slen  = 0;
    uint8_t *src = (uint8_t *)lauxh_checklstring(L, 1, &slen);
    int retry    = (int)lauxh_optuint8(L, 2, 4);
    void *sbuf   = lua_newuserdata(L, lzfse_decode_scratch_size());
    size_t dlen  = slen;

    lua_settop(L, 0);
    do {
        uint8_t *dst = lua_newuserdata(L, dlen);
        size_t len   = lzfse_decode_buffer(dst, dlen, src, slen, sbuf);

        lua_settop(L, 0);
        if (!len) {
            break;
        } else if (len != dlen) {
            lua_pushlstring(L, (const char *)dst, len);
            return 1;
        }
        // probably, need more buffer space
        dlen += slen;
    } while (--retry > 0);

    lua_pushnil(L);
    lua_pushstring(L, "failed to lzfse_decode_buffer()");
    return 2;
}

static int encode_lua(lua_State *L)
{
    size_t slen  = 0;
    uint8_t *src = (uint8_t *)lauxh_checklstring(L, 1, &slen);
    int retry    = (int)lauxh_optuint8(L, 2, 4);
    void *sbuf   = lua_newuserdata(L, lzfse_encode_scratch_size());
    size_t dlen  = slen;

    lua_settop(L, 0);
    do {
        uint8_t *dst = lua_newuserdata(L, dlen);
        size_t len = lzfse_encode_buffer(dst, dlen, (uint8_t *)src, slen, sbuf);

        lua_settop(L, 0);
        if (len) {
            lua_pushlstring(L, (const char *)dst, len);
            return 1;
        }
        // probably, need more buffer space
        dlen += slen;
    } while (--retry > 0);

    lua_pushnil(L);
    lua_pushstring(L, "failed to lzfse_encode_buffer()");
    return 2;
}

LUALIB_API int luaopen_lzfse(lua_State *L)
{
    struct luaL_Reg funcs[] = {
        {"encode", encode_lua},
        {"decode", decode_lua},
        {NULL,     NULL      }
    };
    struct luaL_Reg *ptr = funcs;

    // create table
    lua_newtable(L);
    // add functions
    while (ptr->name) {
        lauxh_pushfn2tbl(L, ptr->name, ptr->func);
        ptr++;
    }

    return 1;
}
