#include <stdio.h>
#include <stdlib.h>
#include "lib/png.h"
#include <string.h>
#include <errno.h>

int png_extractGIF(const char *png_filename, const char *gif_filename) {
  //argv[1] = extract the uiuc chunk from the png file
  //argv[2] = save the data from the uiuc chunk into the GIF file
  /*
    The entire data contents of the uiuc chunk is the GIF file -- 
    just write the entire contents out as the new GIF file.  
    There is no need to add any signature 
    or any understand any GIF-specific stuff.
  */
  
  //open png_file as readable to find the chunk.type == uiuc
  PNG *png = PNG_open(png_filename, "r");
  if (!png) {return ERROR_INVALID_FILE;}
  //printf("png_extractGIF: PNG header opened\n");
  // initialize needed variable
  uint32_t uiuc_len;
  unsigned char *uiuc_data;
  

  //open gif_filename file and set that able to write
  FILE *gif_fp;
  gif_fp = fopen(gif_filename, "w");
  if(!gif_fp) {return ERROR_INVALID_FILE;}
  //printf("png_extractGIF: GIF file opened\n");


  while (1) {
    // Read chunk and ensure we get a valid result (exit on error):
    PNG_Chunk chunk;
    if (PNG_read(png, &chunk) == 0) {
      PNG_close(png);
      return ERROR_INVALID_CHUNK_DATA;
    }

    // find chunk.type == 'uiuc'
    if (strcmp(chunk.type, "uiuc") == 0) {
      //move data's in initialized variables
      uiuc_len = chunk.len;
      uiuc_data = malloc(uiuc_len + 1);
      memcpy(uiuc_data, chunk.data, chunk.len);
      //uiuc_data = chunk.data;
      // fwrite(uiuc_data, uiuc_len, 1, gif_fp);
      // free(uiuc_data);
      // fclose(gif_fp);

      // PNG_close(png);
      //free chunk?
      PNG_free_chunk(&chunk);
      // break;
      //return 0;
    }

    // Free the memory associated with the chunk we just read:
    PNG_free_chunk(&chunk);
  }

  // write uiuc_data's into gif_file
  fwrite(uiuc_data, uiuc_len, 1, gif_fp);
  free(uiuc_data);
  fclose(gif_fp);

  PNG_close(png);
  return 0;
  //return 255;  // Change the to a zero to indicate success, when your implementaiton is complete.
}


int main(int argc, char *argv[]) {
  // Ensure the correct number of arguments:
  if (argc != 3) {
    printf("Usage: %s <PNG File> <GIF Name>\n", argv[0]);
    return ERROR_INVALID_PARAMS;
  }

  return png_extractGIF(argv[1], argv[2]);
}
