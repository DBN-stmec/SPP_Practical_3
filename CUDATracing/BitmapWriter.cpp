#include <cstring>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

int writeBMPToFile(float* data, const int w, const int h) {
  FILE* f;
  unsigned char* img = NULL;
  int filesize       = 54 + 3 * w * h;  // w is your image width, h is image height, both int

  img = (unsigned char*)malloc(3 * w * h);
  memset(img, 0, 3 * w * h * sizeof(unsigned char));

  int x, y;
  unsigned char r, g, b;

  for (int i = 0; i < w; i++) {
    for (int j = 0; j < h; j++) {
      x = i;  // y = (h - 1) - j;
      y = j;

      r = (unsigned char)(data[(y * w + x) * 3 + 0] * 255);
      g = (unsigned char)(data[(y * w + x) * 3 + 1] * 255);
      b = (unsigned char)(data[(y * w + x) * 3 + 2] * 255);
      if (r > 255)
        r = 255;
      if (g > 255)
        g = 255;
      if (b > 255)
        b = 255;
      img[(x + (h - y - 1) * w) * 3 + 2] = (unsigned char)(r);
      img[(x + (h - y - 1) * w) * 3 + 1] = (unsigned char)(g);
      img[(x + (h - y - 1) * w) * 3 + 0] = (unsigned char)(b);
    }
  }

  unsigned char bmpfileheader[14] = {'B', 'M', 0, 0, 0, 0, 0, 0, 0, 0, 54, 0, 0, 0};
  unsigned char bmpinfoheader[40] = {40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 24, 0};
  unsigned char bmppad[3]         = {0, 0, 0};

  bmpfileheader[2] = (unsigned char)(filesize);
  bmpfileheader[3] = (unsigned char)(filesize >> 8);
  bmpfileheader[4] = (unsigned char)(filesize >> 16);
  bmpfileheader[5] = (unsigned char)(filesize >> 24);

  bmpinfoheader[4]  = (unsigned char)(w);
  bmpinfoheader[5]  = (unsigned char)(w >> 8);
  bmpinfoheader[6]  = (unsigned char)(w >> 16);
  bmpinfoheader[7]  = (unsigned char)(w >> 24);
  bmpinfoheader[8]  = (unsigned char)(h);
  bmpinfoheader[9]  = (unsigned char)(h >> 8);
  bmpinfoheader[10] = (unsigned char)(h >> 16);
  bmpinfoheader[11] = (unsigned char)(h >> 24);

  f = fopen("img.bmp", "wb");
  fwrite(bmpfileheader, 1, 14, f);
  fwrite(bmpinfoheader, 1, 40, f);
  for (int i = 0; i < h; i++) {
    fwrite(img + (w * (h - i - 1) * 3), 3, w, f);
    fwrite(bmppad, 1, (4 - (w * 3) % 4) % 4, f);
  }

  free(img);
  fclose(f);

  return 0;
}