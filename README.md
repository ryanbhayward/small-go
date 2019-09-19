# Small Go
Efficient implementation of Go with positional superko for solving games
on up to 8x8 boards. Used in CMPUT 355 and CMPUT 497 at the University of Alberta.

## Installation instructions
Typing `make` will produce the executable in `small-go/bin`. 

`make release` does the same, but performs compiler optimizations.

`make test` runs unit tests after compilation.

## Running
`bin/small-go` will start the program. It implements some of the gtp interface (https://www.gnu.org/software/gnugo/gnugo_19.html), so games can be played in the CLI using a subset those commands.

Some sample positions are located in `small-go/problems`. These use gtp to set up the board and test the solver. For example,

``` bin/small-go < problems/middle.txt```

will create the position:
```
...
.b.
...
```
and solve for white's best move.

## Contributing
Be sure to run `make contribute` to run unit tests, check for memory leaks, and run static analysis and style checking before
issuing a pull request.

This requires cppcheck and cpplint.
