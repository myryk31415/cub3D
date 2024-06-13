# cub3D - My First RayCaster

## Table of Contents
- [cub3D - My First RayCaster](#cub3d---my-first-raycaster)
	- [Table of Contents](#table-of-contents)
- [Project](#project)
	- [Features](#features)
	- [Map File (.cub)](#map-file-cub)
	- [Installation](#installation)

# Project
Cub3d is a raycaster render engine that can be used to display square grid maps in 3d space. We ([@antonweizmann](https://www.github.com/antonweizmann)) also implemented sprites. It is inspired by the first FPS: [Wolfenstein 3D](http://users.atw.hu/wolf3d/).

![image](https://github.com/myryk31415/cub3D/blob/main/readme/Screenshot_13-Jun_22-00-07_Cub3D.png)

![image](https://github.com/myryk31415/cub3D/blob/main/readme/Screenshot_13-Jun_22-01-01_Cub3D.png)

## Features
- 3D graphical representation using ray-casting.
- Smooth window management with MLX42.
- Different wall textures based on orientation (North, South, East, West).
- Customizable floor and ceiling colors.
- Keyboard controls for navigation:
  - Arrow keys to look around.
  - `W`, `A`, `S`, `D` keys to move.
  - `ESC` to close the window and quit.
- Mouse input to look around (not up and down due to limitations of the raycasting engine)
- Wall collision
- Sprites

## Map File (.cub)
- There is an example included in the repository. (./map1.cub)
- Must contain only these characters: `0` (empty space), `1` (wall), `B` (barrel), `N`, `S`, `E`, `W` (player start positions and orientation).
- Must be closed/surrounded by walls.
- Format:
  - Wall textures: `NO`, `SO`, `WE`, `EA` followed by the path to the texture file.
  - Sprite texture: `P` followed by path to sprite texture
  - Colors: `F` (floor), `C` (ceiling) followed by three RGB values (0-255).
  - Example:
    ```
    NO ./path_to_the_north_texture
    SO ./path_to_the_south_texture
    WE ./path_to_the_west_texture
    EA ./path_to_the_east_texture
	
    F 220,100,0
    C 225,30,0

    111111
    100101
    101001
    1100N1
    111111
    ```

## Installation
- Clone the repository
- Run the following commands in the terminal
	- `make init`
	- `make`
	- `/cub3d ./path_to_map` (./map1.cub)
- Enjoy :)
