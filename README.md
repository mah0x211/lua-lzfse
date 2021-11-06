# lua-lzfse

LZFSE compression and decompression library bindings.


## Dependencies

- lzfse: https://github.com/lzfse/lzfse


## Installation

```sh
luarocks install lzfse
```


## Encode

### out, err = lzfse.encode( str )

**Parameters**

- `str:string`: source string


**Returns**

1. `out:string`: LZFSE encoded-string on success, or a nil on failure.
2. `err:string`: error message.



## Decode

### out, err = lzfse.decode( str )

**Parameters**

- `str:string`: LZFSE encoded-string


**Returns**

1. `out:string`: decoded-string on success, or a nil on failure.
2. `err:string`: error message.


