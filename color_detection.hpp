/*Formula_from_http://hooktail.org/computer/index.php?RGB%A4%AB%A4%E9HSV%A4%D8%A4%CE%CA%D1%B4%B9%A4%C8%C9%FC%B8%B5*/

#pragma once
#ifndef JPEG_COLOR_DETECTION_HPP
#define JPEG_COLOR_DETECTION_HPP
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <jpeglib.h>

struct rgb_cls{
public:
  int b;
  int g;
  int r;
};

struct hsv_cls{
public:
  int h;
  int s;
  int v;
};

struct image_cls{
public:
  unsigned int height;
  unsigned int width;
  hsv_cls **hsv;
  hsv_cls **r_hsv;
  rgb_cls **rgb;
};



class find_color_mass {
private:
  image_cls img_;
  JSAMPARRAY origin_jpeg_img_;
  JSAMPARRAY mask_jpeg_img_;
  struct jpeg_decompress_struct jpeg_cinfo_;
public:
    
    int read_jpeg_image(char[]);
    
    void init_variation();
    
    void convert_rgb_to_hsv();
    
    float find_hsv_mass(hsv_cls,hsv_cls,hsv_cls);
    
    void convert_hsv_to_rgb();
    
    float equilibrium_filter();
    
    void write_jpeg_image(char[]);
    
    void destroy_all();
    
};

#endif