# PEW PEW GAMEBOY
My first game for Gameboy, made in C with GBDK.

have fun!

## Dependencies Required
You need to have @andreasjhkarlsson [GBDK-N](https://github.com/andreasjhkarlsson/gbdk-n) development kit installed on `/opt` folder.<br>
Folder name must be `gbdk-n-master`.

Also you need CMake to run the Makefile (duh).

To run the generated `.gb` file, you need a Gameboy Emulator, like [BGB](http://bgb.bircd.org/).

## Usage

### Building the rom
```bash
$ make
```
a `build` directory with the rom inside will be created.

### Remove unnecessary files
```bash
$ make clean
```
