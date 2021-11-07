local unpack = unpack or table.unpack
local lzfse = require('lzfse')
local assert = require('assertex')
local testcase = require('testcase')

function testcase.encode_decode()
    -- test that encode string
    local src = 'hello world lzfse compression library'
    local enc = assert(lzfse.encode(src, 255))

    -- test that decode string
    local dec = assert(lzfse.decode(enc, 255))
    assert.equal(dec, src)

    -- test that throws an error with invalid argument
    for _, v in ipairs({
        {
            args = {
                1,
            },
            err = '#1 .+ [(]string expected, got number',
        },
        {
            args = {
                "foo",
                "bar",
            },
            err = '#2 .+ [(]number expected, got string',
        },
        {
            args = {
                "foo",
                -1,
            },
            err = '#2 .+ [(]uint8_t expected, got an out of range value',
        },
        {
            args = {
                "foo",
                256,
            },
            err = '#2 .+ [(]uint8_t expected, got an out of range value',
        },
    }) do
        local err = assert.throws(function()
            lzfse.encode(unpack(v.args))
        end)
        assert.match(err, v.err, false)

        err = assert.throws(function()
            lzfse.decode(unpack(v.args))
        end)
        assert.match(err, v.err, false)
    end
end
