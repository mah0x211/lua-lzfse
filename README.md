# lua-lzfse

[![test](https://github.com/mah0x211/lua-lzfse/actions/workflows/test.yml/badge.svg)](https://github.com/mah0x211/lua-lzfse/actions/workflows/test.yml)

LZFSE compression and decompression library bindings.


## Dependencies

- lzfse: https://github.com/lzfse/lzfse


## Installation

```sh
luarocks install lzfse
```


## Encode

### out, err = lzfse.encode( str [, retry] )

encoding function uses memory space for buffers of at  at least `slen + lzfse_encode_scratch_size()` and most `slen * retry + lzfse_encode_scratch_size()`.


**Parameters**

- `str:string`: source string.
- `retry:uint8`: number of encoding retries. (`default: 4`)

**Returns**

1. `out:string`: LZFSE encoded-string on success, or a nil on failure.
2. `err:string`: error message.


## Decode

### out, err = lzfse.decode( str [, retry] )

dencode function uses memory space for buffers of at  at least `slen + lzfse_decode_scratch_size()` and most `slen * retry + lzfse_decode_scratch_size()`.

**Parameters**

- `str:string`: LZFSE encoded-string.
- `retry:uint8`: number of decoding retries. (`default: 4`)

**Returns**

1. `out:string`: decoded-string on success, or a nil on failure.
2. `err:string`: error message.

