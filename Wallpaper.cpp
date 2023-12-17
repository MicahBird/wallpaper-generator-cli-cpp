#include "lodepng.h"
#include "Wallpaper.h"

#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>
using namespace std;


Wallpaper::Wallpaper(){
    _image = new vector<unsigned char>();
    _hueStart = new double;
    _hueIncrement = new double;
    _wavelength = new double;
    _amplitude = new double;
    _offset = new double;
    _offsetIncrement = new double;
    _sat = new double;
    _light = new double;
    _lightIncrement = new double;
}

Wallpaper::~Wallpaper(){
    delete _image; 
}

void Wallpaper::generateWallpaper() {
    // Seed the random number generator.
    srand(time(0));
    rand();
 
    // These random values are based on Roy Tanck's Wallpaper Generator Algorithm [1].
    // _segments: Number of line segments. It's either a random number between 1 and 9 (50% chance) or 200 for sinewave like effect (50% chance).
    _segments = ( rand() % 100 < 50 ) ? 1 + rand() % 9 : 200;

    // _wavelength: Wavelength for the wave the lines, a random value between WIDTH/20 and WIDTH/5.
    *_wavelength = _width / ( 5 + ( 15 * (rand() / static_cast<double>(RAND_MAX)) ) );

    // _layers: Number of layers, a random number between 3 and 12.
    _layers = 3 + rand() % 10;

    // _amplitude: Amplitude for the wave function, a random value between 0.1* wavelength and the random value of wavelength.
    *_amplitude = ( 0.1 * *_wavelength ) + ( 0.9 * *_wavelength ) * (rand() / static_cast<double>(RAND_MAX));

    // _offset: Offset for each layer, a random value between 0 and _width.
    *_offset = _width * (rand() / static_cast<double>(RAND_MAX));

    // _offsetIncrement: Increment of offset which is used in calculations when drawing the image, a random value between _width/20 and 3*_width/20.
    *_offsetIncrement = _width / 20 + ( _width / 10 ) * (rand() / static_cast<double>(RAND_MAX));
    
    // _hueStart: Starting hue value, a random value between 0 and 360.
    *_hueStart = 360 * (rand() / static_cast<double>(RAND_MAX));

    // _hueIncrement: Increment of hue when iterating through layers, a random value between -20 and 20.
    *_hueIncrement = 20 - ( 40 * (rand() / static_cast<double>(RAND_MAX)) );

    // _sat: Saturation value, a random value between 15 and 50.
    *_sat = 15 + ( 35 * (rand() / static_cast<double>(RAND_MAX)) );

    // _light: Lightness value, a random value between 15 and 60.
    *_light = 15 + ( 45 * (rand() / static_cast<double>(RAND_MAX)) );

    // _lightIncrement: Increment of hue when iterating though layers, a random value between -6 and 6.
    *_lightIncrement = ( rand() % 100 < 50 ) ? ( 2 + ( 4 * (rand() / static_cast<double>(RAND_MAX)) ) ) : -( 2 + ( 4 * (rand() / static_cast<double>(RAND_MAX)) ) );

    /* These commented out values are initial test cases that were used to test the program early in development! Feel Free to uncomment them and try them out!
    * _width = 1920;
    * _height = 1080;

    * // Simple starter test case
    * _segments = 3;
    * _layers = 3;
    * _hueStart = 276.3527026176697;
    * _hueIncrement = -12.94801290303505;
    * _wavelength = 156.18269380696006;
    * _amplitude = 88.32438147758303;
    * _offset = 1485.835090272766;
    * _offsetIncrement = 138.12140986688786;
    * _sat = 47.08667430944574;
    * _light = 23.47177801632887;
    * _lightIncrement = 4.399244666437821;
    

    // Sinewave test case
    * _segments = 200;
    * _layers = 10;
    * _hueStart = 130.05478222346966;
    * _hueIncrement = 13.872929765326889;
    * _wavelength = 98.40714033624617;
    * _amplitude = 20.88477820770464;
    * _offset = 86.82545478839415;
    * _offsetIncrement = 197.35049230157213;
    * _sat = 36.662787504804356;
    * _light = 29.657512073806124;
    * __lightIncrement = -4.912805248960836;

    * // Advanced sinwave test case
    * _segments = 200;
    * _layers = 8;
    * _hueStart = 283.73584096310907;
    * _hueIncrement = -3.579744500098734;
    * _wavelength = 345.81939983876964;
    * _amplitude = 239.5835502852145;
    * _offset = 88.33607705483544;
    * _offsetIncrement = 206.96516853656834;
    * _sat = 17.324596495406862;
    * _light = 49.36458128951048;
    * _lightIncrement = 2.3166077717589575;
    
    * // Color test case
    * _segments = 200;
    * _layers = 7;
    * _hueStart = 308.2377703504164;
    * _hueIncrement = 19.055048488229758;
    * _wavelength = 302.21594753628574;
    * _amplitude = 175.52457523189426;
    * _offset = 277.24449118027917;
    * _offsetIncrement = 153.55343346638824;
    * _sat = 43.326089130157726;
    * _light = 29.738522960445597;
    * _lightIncrement = 5.7004006707436154;

    * // Cout test values for debbuging
    * cout << "Segments: " << _segments << endl;
    * cout << "Layers: " << _layers << endl;
    * cout << "Hue Start: " << _hueStart << endl;
    * cout << "Hue Increment: " << _hueIncrement << endl;
    * cout << "Wavelength: " << _wavelength << endl;
    * cout << "Amplitude: " << _amplitude << endl;
    * cout << "Offset: " << _offset << endl;
    * cout << "Offset Increment: " << _offsetIncrement << endl;
    * cout << "Saturation: " << _sat << endl;
    * cout << "Light: " << _light << endl;
    * cout << "Light Increment: " << _lightIncrement << endl;
    */

    // Create a buffer to hold the image data.
    _image->resize(_width * _height * 4);

    // Convert HSL to RGB.
    hslToRgb(*_hueStart, *_sat, *_light, _r, _g, _b);
    
    // Fill the image with the RGB color.
    // This method of manipulating the image vector is based on the code from LodePNG's "Generates 4-bit PNG with translucent palette." example. [6]
    for (unsigned y = 0; y < _height; y++) {
        for (unsigned x = 0; x < _width; x++) {
            _image->at(4 * _width * y + 4 * x + 0) = _r;
            _image->at(4 * _width * y + 4 * x + 1) = _g;
            _image->at(4 * _width * y + 4 * x + 2) = _b;
            _image->at(4 * _width * y + 4 * x + 3) = 255; // Alpha (opacity)
        }
    }
    
    // Draw the layers
    for (unsigned l = 0; l < _layers; l++) {
        // cout << "---" << endl;
        double h = *_hueStart + ((l + 1) * *_hueIncrement);
        double s = *_sat;
        double v = *_light + ((l + 1) * *_lightIncrement);
        // cout << "Layer: " << l << endl;
        // cout << "Light: " << v << endl;
        // cout << "Hue: " << h << endl;
        // cout << "Saturation: " << s << endl;
        
        // Convert HSL to RGB for the current layer
        unsigned char r, g, b;
        hslToRgb(h, s, v, r, g, b);

        // The offset for the current layer 
        double layerOffset = *_offset + (*_offsetIncrement * l);

        // The base Y-coordinate for the current layer.
        double offsetY = ((l + 0.5) * (_height / _layers));

        double startY = offsetY + (*_amplitude * sin(layerOffset / *_wavelength));
        // cout << "Layer Offset: " << layerOffset << endl;
        // cout << "Offset Y: " << offsetY << endl;
        // cout << "Start Y: " << startY << endl;

        // Set starting postion for the first segment.
        double prevX = 0;
        double prevY = startY;

        // Draw every segment in the current layer.
        for(unsigned int i = 0; i <= _segments; i++){
            double x = (i * (_width / static_cast<double>(_segments)));
            double y = (startY + ( *_amplitude * sin( ( layerOffset + x ) / *_wavelength ) ));
            // cout << "Segment: " << i << endl;
            // cout << "X: " << x << endl;
            // cout << "Y: " << y << endl;
            
            // Cast all the doubles to ints, otherwise the image will incomplete due to floating point comparison errors.
            prevX = static_cast<int>(prevX);
            prevY = static_cast<int>(prevY);
            x = static_cast<int>(x);
            y = static_cast<int>(y);

            drawAndFillSegment(prevX, prevY, x, y, r, g, b);
            prevX = x;
            prevY = y;
        }
    }
}

void Wallpaper::setResolution(unsigned int width, unsigned int height) {
    _width = width;
    _height = height;
}

void Wallpaper::writeImagetoDisk(std::string filename) {
    // Verify that _image is not empty.
    if (_image->empty()) {
        cerr << "Error: Image vector is empty. Please ensure that generateWallpaper() is used before calling this." << endl;
        exit(1);
    }

    // Encode the image to PNG and write it to a file.
    if (lodepng::encode(filename, *_image, _width, _height) != 0) {
        cerr << "Error: Could not encode PNG image. Ensure the filename is valid and try again." << endl;
        exit(1);
    }
}

void Wallpaper::embedImage(string imageInFilename, string embedPosition) {
    // Verify that _image is not empty.
    if (_image->empty()) {
        cerr << "Error: Image vector is empty. Please ensure that generateWallpaper() is used before calling this." << endl;
        exit(1);
    }
    
    // Load the image from disk.
    vector<unsigned char>* imageIn = new vector<unsigned char>;
    unsigned int widthIn, heightIn;
    if (lodepng::decode(*imageIn, widthIn, heightIn, imageInFilename) != 0) {
        cerr << "Error: Could load PNG to embed from disk. Ensure the image is a PNG and that the filename is correct and try again." << endl;
        delete imageIn;
        exit(1);
    }
    
    // Verify that the image to embed is not larger than the wallpaper.
    if (widthIn > _width || heightIn > _height) {
        cerr << "Error: The image to embed is larger than the wallpaper. Please use a smaller image." << endl;
        delete imageIn;
        exit(1);
    }
    
    unsigned int xPos, yPos;
    
    // Calculate the x and y position of the embed image
    if (embedPosition == "lowerright") {
        xPos = _width - widthIn;
        yPos = _height - heightIn;
    } else if (embedPosition == "upperleft") {
        xPos = 0;
        yPos = 0;
    } else {
        // Parse the embed position
        size_t pos = embedPosition.find("x");
        if(pos == string::npos) {
            cerr << "Error: Invalid embed position format. Please use the format WIDTHxHEIGHT|lowerright|upperleft (Ex: 1920x1080|lowerright)" << endl;
            exit(1);
        }
        xPos = stoi(embedPosition.substr(0, pos));
        yPos = stoi(embedPosition.substr(pos + 1));
        // Check if the width and height are valid
        if(xPos <= 0 || yPos <= 0) {
            cerr << "Error: Invalid embed position. Please use positive integers for the width and height." << endl;
            exit(1);
        } else if (xPos > _width || yPos > _height) {
            cerr << "Error: Invalid embed position. Please use a position within the resolution of the image." << endl;
            exit(1);
        }
    }

    // Embed the image.
    for (unsigned int y = 0; y < heightIn; y++) {
        for (unsigned int x = 0; x < widthIn; x++) {
            unsigned int index = 4 * _width * (y + yPos) + 4 * (x + xPos);
            // This if statement is to prevent a out_of_range error if the embed image exceeds the resolution of the wallpaper.
            if (index + 3 >= _image->size()) {
                break;
            }
            _image->at(index + 0) = imageIn->at(4 * widthIn * y + 4 * x + 0);
            _image->at(index + 1) = imageIn->at(4 * widthIn * y + 4 * x + 1);
            _image->at(index + 2) = imageIn->at(4 * widthIn * y + 4 * x + 2);
            _image->at(index + 3) = imageIn->at(4 * widthIn * y + 4 * x + 3);
        }
    }

    delete imageIn;
}

// This function is based on the HSL to RGB on the algorithm from Wikipedia https://en.wikipedia.org/wiki/HSL_and_HSV#To_RGB. [5]
void Wallpaper::hslToRgb(double h, double s, double l, unsigned char& r, unsigned char& g, unsigned char& b) { 
    // Normalize the saturation and hue to be between 0 and 360.
    s /= 100.0;
    l /= 100.0;

    // Calculate chroma. NOTE: The values for multiplying/subtracting must be floats.
    double c = (1.0 - abs(2.0 * l - 1.0)) * s;

    // Calculate the hue prime and temporary values
    double x = c * (1.0 - abs(fmod(h / 60.0, 2) - 1.0));
    double m = l - c / 2.0;

    // If-else tree to determine the RGB values, similar to the Wikipedia pseudocode.
    double rp, gp, bp;
    if (h < 60) {
        rp = c; gp = x; bp = 0;
    } else if (h < 120) {
        rp = x; gp = c; bp = 0;
    } else if (h < 180) {
        rp = 0; gp = c; bp = x;
    } else if (h < 240) {
        rp = 0; gp = x; bp = c;
    } else if (h < 300) {
        rp = x; gp = 0; bp = c;
    } else {
        rp = c; gp = 0; bp = x;
    }

    // Finally calculate the RGB values by adding the temporary values and multiplying by 255.
    r = (rp + m) * 255;
    g = (gp + m) * 255;
    b = (bp + m) * 255;
}

// This function is based on the Bresenham's line algorithm pseudocode from Wikipedia. [4]
void Wallpaper::drawAndFillSegment(double x0, double y0, double x1, double y1, unsigned char r, unsigned char g, unsigned char b) {
    double dx = abs(x1 - x0);
    double sx = x0 < x1 ? 1 : -1;
    double dy = -abs(y1 - y0);
    double sy = y0 < y1 ? 1 : -1;
    double error = dx + dy;
    double e2;

    // Debugging
    // cout << "------" << endl;
    // cout << "x0: " << x0 << endl;
    // cout << "y0: " << y0 << endl;
    // cout << "------" << endl;

    // Loop until x0 and y0 meet x1 and y1.
    while (true) {
        // Fill in the pixel at that point but also fill in the pixels below it until reaching the bottom of the image.
        for (unsigned int y = y0; y < _height; y++) {
            if (x0 < _width && y < _height) {
                _image->at(4 * _width * y + 4 * (int)x0 + 0) = r;
                _image->at(4 * _width * y + 4 * (int)x0 + 1) = g;
                _image->at(4 * _width * y + 4 * (int)x0 + 2) = b;
                _image->at(4 * _width * y + 4 * (int)x0 + 3) = 255;
            }
        }
        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * error;
        if (e2 >= dy) {
            if (x0 == x1) break;
            error += dy;
            x0 += sx;
        }
        if (e2 <= dx) {
            if (y0 == y1) break;
            error += dx;
            y0 += sy;
        }
    }
}
