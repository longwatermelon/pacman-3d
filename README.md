# pacman-3d
Pacman in first person

# Controls
* Left, right, down arrow keys: rotate
* r / up arrow: restart

https://user-images.githubusercontent.com/73869536/172951042-95a23f5d-2e87-47eb-9986-0917676c73d5.mp4

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
