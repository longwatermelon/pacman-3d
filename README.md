# pacman-3d
The most unpleasant pacman gameplay you'll experience

# Controls
* Left, right, down arrow keys: rotate
* r: restart

# Building
Depends on sdl2, sdl2_image, sdl2_ttf

## Linux
```
git clone https://github.com/longwatermelon/pacman-3d
cd pacman-3d
make
./a.out
```

## MacOS
```
git clone https://github.com/longwatermelon/pacman-3d
cd pacman-3d
make INCLUDE=-I$(brew --prefix)/include LIBRARIES=-L$(brew --prefix)/lib FLAGS=-Wno-error=unused-command-line-argument
./a.out
```
