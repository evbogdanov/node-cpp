const cpp = require('./build/Release/node_cpp')

const
    x = 1.2,
    y = 3.4

console.log(`cpp.sum(${x}, ${y}) === ${cpp.sum(x, y)}`)
console.log(`cpp.twice(${x}) === ${cpp.twice(x)}`)
