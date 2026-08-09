# Raylib Snake

## Getting Started

Make sure you have `cmake` and `raylib` installed on your computer.

Next, generate the build system using `cmake`.

```sh
# I provide the export compile commands variable for lsp purposes
$ cmake -S . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
# For lsp...
$ ln -s build/compile_commands.json .
```

Finally, you can build and run the game.

```sh
$ cmake --build build && ./build/game
```
