#include <stdio.h>
#include <stdlib.h>
#include "lib/png.h"
#include <string.h>

int png_hideGIF(const char *png_filename, const char *gif_filename) {
  /*
    hide a GIF file within a uiuc chunk in a provided PNG file
    sample/mickey.png mickey.gif
    argv[1] = normal PNG file that needs to be modified to hide an GIF image
    argv[2] = GIF to be hidden
    add additional PNG_Chunk between "IHDR" and "IEND"
    add chunk between "IDAT" and "IEND" called uiuc
  */

  //open png_file writable to add gif_file in png_file
  PNG *png = PNG_open(png_filename, "r+");
  //if (!png) {return ERROR_INVALID_FILE;}
  printf("png_hideGIF: PNG header opened\n");

  //open gif_file readable to extract data's from it and add in png_file
  FILE *gif_fp = fopen(gif_filename, "r");
  if (!gif_fp) {return ERROR_INVALID_FILE;}
  printf("png_hideGIF: GIF header opened\n");

  //chunk and it's bytes
  PNG_Chunk uiuc_chunk;
  size_t bytes; // should be 3585468
  PNG_Chunk IEND_chunk;
  


  //store gif values into uiuc_chunk
  fseek(gif_fp, 0, SEEK_END);
  uiuc_chunk.len = ftell(gif_fp); //len
  fseek(gif_fp, 0, SEEK_SET);

  uiuc_chunk.type[0] = 'u';       //type
  uiuc_chunk.type[1] = 'i';
  uiuc_chunk.type[2] = 'u';
  uiuc_chunk.type[3] = 'c';
  uiuc_chunk.type[4] = 0x00;

  uiuc_chunk.data = malloc(uiuc_chunk.len + 1); //data
  unsigned char *gif_data = malloc(uiuc_chunk.len + 1);
  fread(gif_data, sizeof(unsigned char), uiuc_chunk.len, gif_fp);
  memcpy(uiuc_chunk.data, gif_data, uiuc_chunk.len);


  while (1) {
    PNG_Chunk chunk;
    //fseek(png->file, 0, SEEK_SET);
    if (PNG_read(png, &chunk) == 0) {
      PNG_close(png);
      fclose(gif_fp);
      return ERROR_INVALID_CHUNK_DATA;
    }
    //find chunk.type == "IEND"
    if (strcmp(chunk.type, "IEND") == 0) {
      IEND_chunk = chunk;
      uint32_t IEND_len = sizeof(uint32_t) + 4 * sizeof(char) + IEND_chunk.len + sizeof(uint32_t);

      //put uiuc chunk here using 
      //fseek(png->file, -1 * sizeof(IEND_chunk) , SEEK_CUR);
      png->chunk_len = png->chunk_len - (IEND_len);
      bytes = PNG_write(png, &uiuc_chunk);
      printf("PNG chunk %s written (%lu bytes)\n", uiuc_chunk.type, bytes);
      
      //restore IEND_chunk into file
      bytes = PNG_write(png, &IEND_chunk);
      printf("PNG chunk %s written (%lu bytes)\n", IEND_chunk.type, bytes);

      PNG_free_chunk(&chunk);
      break;
    }

    PNG_free_chunk(&chunk);
  }
  free(uiuc_chunk.data);

  PNG_close(png);
  printf("png_hideGIF: PNG header closed\n");
  fclose(gif_fp);
  printf("png_hideGIF: GIF file closed\n");
  return 0;
  //return 255;  // Change the to a zero to indicate success, when your implementaiton is complete.
}

int main(int argc, char *argv[]) {
  // Ensure the correct number of arguments:
  if (argc != 3) {
    printf("Usage: %s <PNG File> <GIF File>\n", argv[0]);
    return ERROR_INVALID_PARAMS;
  }

  return png_hideGIF(argv[1], argv[2]);
}
