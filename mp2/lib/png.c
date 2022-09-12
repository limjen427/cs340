#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#include "crc32.h"
#include "png.h"

const int ERROR_INVALID_PARAMS = 1;
const int ERROR_INVALID_FILE = 2;
const int ERROR_INVALID_CHUNK_DATA = 3;
const int ERROR_NO_UIUC_CHUNK = 4;


/**
 * Opens a PNG file for reading (mode == "r" or mode == "r+") or writing (mode == "w").
 * 
 * (Note: The function follows the same function prototype as `fopen`.)
 * 
 * When the file is opened for reading this function must verify the PNG signature.  When opened for
 * writing, the file should write the PNG signature.
 * 
 * This function must return NULL on any errors; otherwise, return a new PNG struct for use
 * with further fuctions in this library.
 */
PNG * PNG_open(const char *filename, const char *mode) {
  FILE* fp;
  PNG* png;
  unsigned char sign_val[9] ="\x89\x50\x4e\x47\x0d\x0a\x1a\x0a\x00";

  if (strcmp(mode, "r") == 0 || strcmp(mode, "r+") == 0) {
    fp = fopen(filename, mode);
    if (!fp) {
      return NULL;
    }

    unsigned char buffer[8];
    fread(buffer, sizeof(char), 8, fp);
    for (int i = 0; i < 8; i++) {
      if (buffer[i] != sign_val[i]) {
        fclose(fp);
        return NULL;
      }
    }
  }
  else if (strcmp(mode, "w") == 0) {
    fp = fopen(filename, mode);
    if (!fp) {
      return NULL;
    }

    fwrite(sign_val, sizeof(char), 9, fp);
  }

  png = (PNG*) malloc(sizeof(PNG));
  png->file = fp;
  png->chunk_len = 8;
  return png;
}


/**
 * Reads the next PNG chunk from `png`.
 * 
 * If a chunk exists, a the data in the chunk is populated in `chunk` and the
 * number of bytes read (the length of the chunk in the file) is returned.
 * Otherwise, a zero value is returned.
 * 
 * Any memory allocated within `chunk` must be freed in `PNG_free_chunk`.
 * Users of the library must call `PNG_free_chunk` on all returned chunks.
 */
size_t PNG_read(PNG *png, PNG_Chunk *chunk) {
  FILE* fp = png->file;

  if (!feof(fp)) {
    fseek(fp, png->chunk_len, SEEK_SET);

    //file_len = ftell(fp)
    //read first 4 bytes to find the size
    uint32_t *len_store = malloc(sizeof(uint32_t));
    fread(len_store, sizeof(uint32_t), 1, fp);
    chunk->len = ntohl(*len_store);
    free(len_store);

    //read next 4 bytes to find the chunk type
    char *type_store = malloc(5 * sizeof(char)); //5->4
    fread(type_store, sizeof(char), 4, fp);
    for (int i = 0; i < 4; i++) {
      chunk->type[i] = type_store[i];
    }
    chunk->type[4] = '\0';  //comment out this
    free(type_store);

    //read next chunk->len bytes to find the chunk data
    chunk->data = malloc((chunk->len + 1) * sizeof(unsigned char)); //chunk->len + 1 to chunk->len
    fread(chunk->data, sizeof(unsigned char), chunk->len, fp);

    //read next 4 bytes to find the chunk crc
    uint32_t *crc_store = malloc(sizeof(uint32_t));
    fread(crc_store, sizeof(uint32_t), 1, fp);
    chunk->crc = ntohl(*crc_store);
    free(crc_store);

    png->chunk_len = png->chunk_len + sizeof(uint32_t) + 4 * sizeof(char) + chunk->len + sizeof(uint32_t);
  }
  //PNG_free_chunk(chunk);
  return (sizeof(uint32_t) + 4 * sizeof(char) + chunk->len + sizeof(uint32_t));

}


/**
 * Writes a PNG chunk to `png`.
 * 
 * Returns the number of bytes written. 
 */
size_t PNG_write(PNG *png, PNG_Chunk *chunk) {
  FILE* fp = png->file;
  fseek(fp, png->chunk_len, SEEK_SET);

  //length
  //if length should be converted to htonl, should the sizeof and png->chunk_len be changed?
  uint32_t *len_store = malloc(sizeof(uint32_t));
  *len_store = htonl(chunk->len);
  fwrite(len_store, sizeof(uint32_t), 1, fp);
  free(len_store);

  //type
  fwrite(chunk->type, 4 * sizeof(char), 1, fp);

  //*data
  fwrite(chunk->data, chunk->len, 1, fp);

  //crc
  unsigned char *type_data = malloc(chunk->len + sizeof(chunk->type));
  memcpy(type_data, chunk->type, sizeof(chunk->type));
  memcpy(type_data + 4, chunk->data, chunk->len);
  printf("crc_data before: %u\n", *type_data);
  uint32_t crc = 0;
  crc32(type_data, sizeof(type_data), &crc);
  uint32_t *crc_store = malloc(sizeof(uint32_t));
  *crc_store = htonl(crc);
  fwrite(crc_store, sizeof(uint32_t), 1, fp);
  free(type_data);
  free(crc_store);

  png->chunk_len = png->chunk_len + sizeof(uint32_t) + 4 * sizeof(char) + chunk->len + sizeof(uint32_t);
  //PNG_free_chunk(chunk);
  return (sizeof(uint32_t) + 4 * sizeof(char) + chunk->len + sizeof(uint32_t));
  //return 0;
}


/**
 * Frees all memory allocated by this library related to `chunk`.
 */
void PNG_free_chunk(PNG_Chunk *chunk) {
  free(chunk->data);
}


/**
 * Closes the PNG file and frees all memory related to `png`.
 */
void PNG_close(PNG *png) {
  fclose(png->file);
  free(png);
}