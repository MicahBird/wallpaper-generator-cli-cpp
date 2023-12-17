/* CSCI 200: Final Project - CLI Wallpaper Generator
 *
 * Author: Micah Bird
 *
 * Resources used:
 *  [1] R. Tanck, “Roy Tanck’s Wallpaper Generator Algorithm,” Github. [Online]. Available: https://github.com/roytanck/wallpaper-generator
 *
 *  [2] “LodePNG.” Accessed: Dec. 02, 2023. [Online]. Available: https://lodev.org/lodepng/
 *
 *  [3] CLIUtils, “CLI11: Command line parser for C++11,” Github. [Online]. Available: https://github.com/CLIUtils/CLI11
 *
 *  [4] “Bresenham’s line algorithm,” Wikipedia. Nov. 23, 2023. Accessed: Dec. 02, 2023. [Online]. Available: https://en.wikipedia.org/w/index.php?title=Bresenham%27s_line_algorithm&oldid=1186441766
 *
 *  [5] “HSL and HSV,” Wikipedia. Nov. 18, 2023. Accessed: Dec. 02, 2023. [Online]. Available: https://en.wikipedia.org/w/index.php?title=HSL_and_HSV&oldid=1185698752
 *
 *  [6] “LodePNG Example,” LodePNG. [Online]. Available: https://raw.githubusercontent.com/lvandeve/lodepng/master/examples/example_4bit_palette.cpp
 *
 *  [7] M. James, “placekitten,” placekitten. Accessed: Dec. 02, 2023. [Online]. Available: http://placekitten.com
 * 
 * Run the program with the -h flag to see the help menu.
 */

#include "CLI11.hpp"
#include "Wallpaper.h"

#include <iostream>
#include <string>
using namespace std;

int main(int argc, char** argv) {
    CLI::App app{"Wallpaper Generator CLI - Generate a random wallpaper in the command line"};
  
    // Define the CLI options
    string outputFilename;
    // Mark the option as required
    app.add_option("-o,--output", outputFilename, "Path of output file as a png (Ex: ./my-epic-wallpaper.png)") -> required();
   
    string resolution = "1920x1080";
    app.add_option("-r,--resolution", resolution, "The resolution of the image in the format WIDTHxHEIGHT (Default: 1920x1080)");
    
    string embedImageLocation;
    app.add_option("-e,--embed", embedImageLocation, "Path of embed PNG image (Ex: ./my-embed-image.png)");

    string embedPosition = "lowerright";
    app.add_option("-p,--embedPosition", embedPosition, "Position of the embedded image in wallpaper in the format WIDTHxHEIGHT|lowerright|upperleft (Default: lowerright)");
  
    // Set the help menu
    app.footer("Examples:\n    # Generates a 1920x1080 PNG wallpaper in ~/Pictures\n	$ wallpaper-generator-cli -o ~/Pictures/my-epic-wallpaper.png\n\n    # Generates a 1920x1080 wallpaper with a placeholder kitten embedded in the upper left\n	$ wallpaper-generator-cli -e kitten.png -p upperleft -o ./my-epic-embedded-wallpaper.png\n\n    # Generates a 4K wallpaper in the current directory \n	$ wallpaper-generator-cli -r 3840x2160 -o ./my-epic-wallpaper.png\n");
    
    CLI11_PARSE(app, argc, argv);
    
    // Parse the resolution and check if it's valid
    unsigned int width, height;
    size_t pos = resolution.find("x");
    if(pos == string::npos) {
        cerr << "Error: Invalid resolution format. Please use the format WIDTHxHEIGHT (Ex: 1920x1080)" << endl;
        return 1;
    }
    width = stoi(resolution.substr(0, pos));
    height = stoi(resolution.substr(pos + 1));
    // Check if the width and height are valid
    if(width <= 0 || height <= 0) {
        cerr << "Error: Invalid resolution. Please use positive integers for the width and height." << endl;
        return 1;
    } else if (width > 10000 || height > 10000) {
        cerr << "Error: Invalid resolution. Please use a width and height less than 10000." << endl;
        return 1;
    }
    
    // Check output filename to see if it ends with .png
    if(outputFilename.substr(outputFilename.length() - 4) != ".png") {
        outputFilename += ".png";
    }

    // Create a Wallpaper object
    Wallpaper wallpaper;
    
    // Set the resolution
    wallpaper.setResolution(width, height);

    // Generate the wallpaper
    wallpaper.generateWallpaper();
    
    // Embed the image if the user specified an image
    if(!embedImageLocation.empty()) {
        // Embed the image
        wallpaper.embedImage(embedImageLocation, embedPosition);
    }
  
    // Write the image to disk
    wallpaper.writeImagetoDisk(outputFilename);

    return 0;
}
    
