# wallpaper-generator-cli-cpp
### (CSCI 200 Final Project)

## Program Description

My final project is command line wallpaper generator that is based on Roy Tanck’s wallpaper generation algorithm [1]. With this tool, users can specify various parameters such as image resolution, if they would like to embed another image within the generated image, and the position of said image. I have previously done a project similar to this, as an exercise for learning JavaScript, however it was terribly inefficient and would take upwards of 10 seconds to generate one wallpaper. But by leveraging the power of C++, this tool provides an efficient and user-friendly way to personalize their wallpaper without requiring extensive design knowledge or specialized software. Surprisingly with the right compiler optimizations, this program can generate a wallpaper in 0.05 seconds, all while being a fraction of the size of the JS implementation (1.8MB vs 211MB with dependencies)! This tool likewise enables users to create new wallpapers programmatically, which opens up a world of opportunities. A potential programmatic use case of this program would be [generating a new wallpaper every time you log into your computer!](#generating-a-new-wallpaper-on-login)

### Examples:
|--------------------------|--------------------------|--------------------------|
| ![Example 1](docs/1.png) | ![Example 2](docs/2.png) | ![Example 2](docs/2.png) |
|--------------------------|--------------------------|--------------------------|

## Installation
To install `wallpaper-generator-cli` simply clone the repo, run `make` and then move the `wallpaper-generator-cli` binary to `~/.local/bin/`!

## Usage
```
Usage: ./wallpaper-generator-cli [OPTIONS]

Options:
  -h,--help                   Print this help message and exit
  -o,--output TEXT REQUIRED   Path of output file as a png (Ex: ./my-epic-wallpaper.png)
  -r,--resolution TEXT        The resolution of the image in the format WIDTHxHEIGHT (Default: 1920x1080)
  -e,--embed TEXT             Path of embed PNG image (Ex: ./my-embed-image.png)
  -p,--embedPosition TEXT     Position of the embedded image in wallpaper in the format WIDTHxHEIGHT|lowerright|upperleft (Default: lowerright)

Examples:
    # Generates a 1920x1080 PNG wallpaper in ~/Pictures
	$ wallpaper-generator-cli -o ~/Pictures/my-epic-wallpaper.png

    # Generates a 1920x1080 wallpaper with a placeholder kitten embedded in the upper left
	$ wallpaper-generator-cli -e kitten.png -p upperleft -o ./my-epic-embedded-wallpaper.png

    # Generates a 4K wallpaper in the current directory 
	$ wallpaper-generator-cli -r 3840x2160 -o ./my-epic-wallpaper.png
```
## Generating a New Wallpaper on Login 

### macOS
Simply copy/paste the following script to `new-wallpaper` and add it as a login item in System Settings > General > Login Items. Be sure to also set the default terminal application to close when the script is complete (Terminal > Settings > Profiles > Shell > When the shell exits > Close if the shell exited cleanly).
```sh
#!/bin/sh
wallpaper-generator-cli -r 3840x2160 -o ~/Pictures/wallpaper.png
```

## Program Documentation

You can generate a 1920x1080 wallpaper by simply running `make` and `wallpaper-generator-cli -o ./my-epic-wallpaper.png` and then opening the `my-epic-wallpaper.png` file! This program uses two external libraries: LodePNG [2] and CLI11 [3] both of which are bundled with the program and are fully portable. The program also has the capability to embed another PNG image within a generated wallpaper, and an example file (kitten.png [7]) is included for this purpose. Simply run `wallpaper-generator-cli -e kitten.png -p upperleft -o ./my-epic-embedded-wallpaper.png` to generate a 1920x1080 wallpaper with a placeholder kitten embedded in the upper left. Finally, it's possible to generate a different resolution wallpaper by using the `-r` flag. For example, to generate a 4K wallpaper in the current directory, run `wallpaper-generator-cli -r 3840x2160 -o ./my-epic-wallpaper.png`. For more options and examples, run `wallpaper-generator-cli -h`. It's also worth noting that the program is expected to not output anything when running successfully.

However, it must be noted that the CLI11 library has a few warnings and due to being designed for C++ 11 instead of C++ 17, however the program compiles and is fully functional regardless. It also must be noted that the program functions differently on Windows and UNIX systems, and the same random values will generate different wallpapers. As for extending the functionality of the program, each class and it's methods are well documented in the `Wallpaper.h` file and in code comments themselves. If I had more time I would have implemented a SFML GUI for faster iterations and more accessible user experience. Although this would necessitate that each private variable would have it's own getters/setters.

## Works Cited

[1] R. Tanck, “Roy Tanck’s Wallpaper Generator Algorithm,” Github. [Online]. Available: https://github.com/roytanck/wallpaper-generator

[2] “LodePNG.” Accessed: Dec. 02, 2023. [Online]. Available: https://lodev.org/lodepng/

[3] CLIUtils, “CLI11: Command line parser for C++11,” Github. [Online]. Available: https://github.com/CLIUtils/CLI11

[4] “Bresenham’s line algorithm,” Wikipedia. Nov. 23, 2023. Accessed: Dec. 02, 2023. [Online]. Available: https://en.wikipedia.org/w/index.php?title=Bresenham%27s_line_algorithm&oldid=1186441766

[5] “HSL and HSV,” Wikipedia. Nov. 18, 2023. Accessed: Dec. 02, 2023. [Online]. Available: https://en.wikipedia.org/w/index.php?title=HSL_and_HSV&oldid=1185698752

[6] “LodePNG Example,” LodePNG. [Online]. Available: https://raw.githubusercontent.com/lvandeve/lodepng/master/examples/example_4bit_palette.cpp

[7] M. James, “placekitten,” placekitten. Accessed: Dec. 02, 2023. [Online]. Available: http://placekitten.com
