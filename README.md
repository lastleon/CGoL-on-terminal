### Conways Game Of Life On Terminal

![output](https://github.com/lastleon/CGoL-on-terminal/assets/19859326/42e35adc-c6f9-4a52-a43c-c77b31cdb6ca)



Quick university project implementing Conways Game of Life on the terminal. Only tested on Linux.
Build using `make`, run using `./cgol`.
For further information see `./cgol --help`:
```bash
This program runs Conway's Game of Life on the commandline.

USAGE:
        ./cgol {rows} {cols} [-h] [-r | -c] [-x | -b]

PARAMETERS:
        rows:   Number of rows the board will have. Must be positive integer. Default: 30
        cols:   Number of columns the board will have. Must be positive integer. Default: 30

OPTIONS:
        -h:     Show this help page.
        -r:     Start game with random initialization. (default)
        -c:     Start game with custom initialization. You will be able to edit the board.
        -x:     Board will have skin 'X' (default)
        -b:     Board will have skin 'brackets'
```


(Done in 2021, only now uploaded)
