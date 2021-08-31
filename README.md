# Fabamaq

Game Development Challenge - Fabamaq

Necessary libraries:
* SDL2
* Glew
* Glm
* Freetype

On linux you can install these libraries with one of the following commands:
```
sudo apt-get install libglew-dev libsdl2-dev libsdl2-image-dev libglm-dev libfreetype6-dev  # Debian, Ubuntu
sudo dnf install glew-devel SDL2-devel SDL2_image-devel glm-devel freetype-devel   # Fedora
sudo pacman -S glew sdl2 sdl2_image glm freetype2 # Arch
```

To compile write **make clean all**

The executable will be located in the /build/executables directory and to run it type **./main**

## Controls

* 'P' - Start round/Pause game
* 'I' - Add 1 credit
* 'O' - Take out all credits
