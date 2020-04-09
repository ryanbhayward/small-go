# Small Go
Efficient implementation of Go with positional superko for solving games
on up to 8x8 boards. Used in CMPUT 355 and CMPUT 497 at the University of Alberta.

Also includes an implementation of a solver that can solve provided 3x3 
positions with the following assumptions for any legal history leading to
positions:
```
.b.
.b.
.b.
```
results in a score of +9 for black,
```
.b.
bb.
...
```
results in a score of +9 for black,
```
b..
...
...
```
results in a score of -9 for black,
```
bbb
...
...
```
results in a score of -9 for black, and
```
bb.
...
...
```
results in a score of +3 for black.

## Installation instructions
Typing `make` will produce the executable in `small-go/bin`. 

`make release` does the same, but performs compiler optimizations.

`make test` runs unit tests after compilation.

## Running
`bin/small-go` will start the program. It implements some of the gtp interface (https://www.gnu.org/software/gnugo/gnugo_19.html), so games can be played in the CLI using a subset those commands.

Some sample positions are located in `small-go/problems`. These use gtp to set up the board and test the solver. For example,

``` bin/small-go < problems/1.txt```

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
