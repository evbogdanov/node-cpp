const
    assert   = require('assert'),
    node_cpp = require('../build/Release/node_cpp')

describe('node_cpp', () => {
    describe('sum', () => {
        it('should fail with wrong number of arguments', () => {
            assert.throws(
                () => node_cpp.sum(1, 2, 3),
                /Wrong number of arguments/
            )
        })

        it('should fail with invalid arguments', () => {
            assert.throws(
                () => node_cpp.sum('one', true),
                /Wrong arguments/
            )
        })

        it('should work with two numeric arguments', () => {
            assert.equal(node_cpp.sum(1, 2), 3)
        })
    })

    describe('twice', () => {
        it('should fail without an argument', () => {
            assert.throws(
                () => node_cpp.twice(),
                /Where is my numeric argument/
            )
        })

        it('should fail with invalid argument', () => {
            assert.throws(
                () => node_cpp.twice('foo'),
                /Where is my numeric argument/
            )
        })

        it('should work with one numeric argument', () => {
            assert.equal(node_cpp.twice(10), 20)
        })
    })

    describe('twiceAsync', () => {
        it('hopefully should work', (done) => {
            node_cpp.twiceAsync(10, twice => {
                assert.equal(twice, 20)
                done()
            })
        })
    })
})
