#ifndef WALLPAPER_H
#define WALLPAPER_H

#include <vector>
#include <string>

class Wallpaper {
    public:
        /**
         * @brief Construct a new Wallpaper object
         * @desc It initializes the _image vector.
         */
        Wallpaper();
        /**
         * @brief Destroy the Wallpaper object
         * @desc It deletes private data members that are allocated on the free store.
         */
        ~Wallpaper();
        /**
         * @brief This function generates the wallpaper.
         * @desc It generates the wallpaper by assigning random values to private variables and looping through all the layers and segments and calling the drawAndFillSegment function.
         */
        void generateWallpaper();
        /**
         * @brief Set the resolution of the wallpaper.
         * @param width The width of the wallpaper.
         * @param height The height of the wallpaper.
         */
        void setResolution(unsigned int width, unsigned int height);
        /**
         * @brief Writes the _image vector to a png file on disk.
         * @param filename The filename of the png file that will be written to disk.
         */
        void writeImagetoDisk(std::string filename);
        /**
         * @brief Embeds an image in the _image vector.
         * @param imageInFilename The filename of the image that will be embedded.
         * @param embedPosition A string that specifies the position of the embedded image. It can be either "lowerright", "upperleft", or in the format WIDTHxHEIGHT.
         */
        void embedImage(std::string imageInFilename, std::string embedPosition);
    private:
        /**
         * @brief This function converts an HSL color to an RGB color. It's based on the HSL and HSV Wikipedia article. [5]
         * @param h The hue of the color.
         * @param s The saturation of the color.
         * @param l The lightness of the color.
         * @param r The red value of the color that needs to be converted.
         * @param g The green value of the color that needs to be converted.
         * @param b The blue value of the color that needs to be converted.
         */
        void hslToRgb(double h, double s, double l, unsigned char& r, unsigned char& g, unsigned char& b);
        /**
         * @brief This function draws a line (segment) from (x0, y0) to (x1, y1) and fills in the pixels below it. The function is based on the Bresenham's line algorithm. [3]
         * @param x0 The x-coordinate of the first point.
         * @param y0 The y-coordinate of the first point.
         * @param x1 The x-coordinate of the second point.
         * @param y1 The y-coordinate of the second point.
         * @param r The red value of the color. 
         * @param g The green value of the color.
         * @param b The blue value of the color.
         */
        void drawAndFillSegment(double x0, double y0, double x1, double y1, unsigned char r, unsigned char g, unsigned char b);
        
        // Below are all private variables.
        std::vector<unsigned char>* _image;
        unsigned char _r, _g, _b;
        unsigned _width, _height;
        unsigned short int _segments, _layers;
        double* _hueStart;
        double* _hueIncrement;
        double* _wavelength;
        double* _amplitude;
        double* _offset;
        double* _offsetIncrement;
        double* _sat;
        double* _light;
        double* _lightIncrement;
};

#endif 
