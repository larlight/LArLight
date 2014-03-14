#ifndef MAKEPNG_H_NHQFGWDJ
#define MAKEPNG_H_NHQFGWDJ

#include <vector>
#include <string>
#include <png.h>
#include <iostream>


class MakePng
{
public:
  typedef enum colortype {
    gray = 1, // Grayscale, 1 byte per pixel
    rgb  = 3, // color, red-green-blue, 3 bytes per pixel
    rgba = 4,  // color and alpha channel, 4 bytes per pixel  
    palette = 10, // 1 byte per pixel, using palette of RGB
    palette_alpha = 11 // 1 byte per pixel using palette of RGBA
  } Color_Mode_t;

  MakePng(int width, int height, Color_Mode_t colormode, // colors: either 1 or 3 colors for grayscale or bitmap.
      const std::vector<unsigned char>& palette=std::vector<unsigned char>(),
      const std::vector<unsigned char>& palette_transparency=std::vector<unsigned char>() );
  void AddRow(const std::vector<unsigned char>& data); // normalized
  void Finish();
  unsigned char* getData() { return (unsigned char*) outdata; };
  size_t         getDataLen() { return outdatalen; };
  void           writeToFile(const std::string& filename);
  std::string    writeToUniqueFile(const std::string& path);
  std::string    getBase64Encoded(); 
  ~MakePng();
  

private:
  void AddData(png_structp png_ptr, png_bytep   data, png_size_t  length);
  friend void my_user_write_data( png_structp png_ptr,  png_bytep   data,   png_size_t  length);
  int width;
  int height;
  Color_Mode_t colormode;
  png_structp png_ptr;
  png_infop   info_ptr;
  size_t bytes_per_row;
  // png_bytep rowdata;
  int rows_done;
  
  png_bytep   outdata;
  size_t  outdatalen;
  
};

void BuildThumbnail(const std::string& pathname, const std::string& thumbname);

#endif /* end of include guard: MAKEPNG_H_NHQFGWDJ */
