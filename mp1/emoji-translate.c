#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "emoji.h"
#include "emoji-translate.h"

//called to initialize an emoji object
void emoji_init(emoji_t *emoji) {
  //emoji_t emoji = malloc(strlen(emoji));
  emoji->source = NULL;
  emoji->trans = NULL;
}

//adds a translation to the emoji object. For example, “😊” might translate to “happy”.
void emoji_add_translation(emoji_t *emoji, const unsigned char *source, const unsigned char *translation) {
  emoji_t *cur_res = emoji; // emoji->source?

  while (cur_res != NULL) {
    //linked list go all the way to the end
    if (cur_res->source != NULL && strcmp(cur_res->source, source) == 0) {
      // find if the source(emoji) is already stored in linked list
      return;
    }
    cur_res = cur_res->next;
  }

  //source linked list
  emoji_t *src_list = malloc(sizeof(emoji_t)); //////??????  - sizeof(emoji->head)
  src_list->source = source;
  src_list->next = emoji->source; // buffer->next = emoji->head;
  emoji->source = src_list; // emoji->head = buffer;

  //trans linked list
  emoji_t *trans_list = malloc(sizeof(emoji_t));
  trans_list->trans = translation;
  trans_list->next = emoji->trans;
  emoji->trans = trans_list;

  return;
}

//must translate the contents of the file specified by fileName all translation rules added so far. 
//When matching rules, always choose the longest matching rule based on the length of the source.
// Translates the emojis contained in the file `fileName`.
const unsigned char *emoji_translate_file_alloc(emoji_t *emoji, const char *fileName) {
  FILE *fp;

  //open
  fp = fopen(fileName, "r+");
  if (!fp) {return NULL;}

  //find file size
  fseek(fp, 0L, SEEK_END);
  long fp_size = ftell(fp);
  //return pointer to begining of the file
  fseek(fp, 0L, SEEK_SET);

  //make template to store translated version
  char* trans_file = calloc(1, fp_size+1);

  //read file and put all the data in "trans_file"
  fread(trans_file, fp_size, 1, fp);

  fclose(fp);
  //make temp variable with twice greater of the size than fp
  long store_size = 2 * fp_size;
  unsigned char *store = malloc(store_size);
  //memcpy or strcpy to *store from trans_file
  memcpy(store, trans_file, store_size);
  free(trans_file);
  
  for (int i = 0; i < fp_size - 1; i++) {
    //store translation here
    unsigned char *trans_store = malloc(50);
    //store emoji here
    unsigned char *buf_emoji = malloc(5);

    //find emoji by bytes
    if (store[i] == 0xf0 && store[i+1] == 0x9f) {
      //copy emoji bytes to buffer
      buf_emoji[0] = store[i];
      buf_emoji[1] = store[i+1];
      buf_emoji[2] = store[i+2];
      buf_emoji[3] = store[i+3];
      buf_emoji[4] = '\0';
      //printf("4. reach here?(cpy emoji) buf_emoji: %d, from file: %d\n", buf_emoji[2], store[i+2]);

      //find emoji in src_list and store index
      emoji_t *cur_src = emoji;
      int src_i = 1;
      while(cur_src != NULL) {
        if (cur_src->source != NULL && strcmp(cur_src->source, buf_emoji) == 0) {
          break;
        }
        src_i++;
        cur_src = cur_src->next;
      }
      //go through # of index in trans_list
      //when reach
      emoji_t *cur_trans = emoji;
      while(cur_trans != NULL) {
        if (src_i == 1) {
          strcpy(trans_store, cur_trans->trans);
          break;
        }
        src_i--;
        cur_trans = cur_trans->next;
      }
    }

    //copy front of emoji
    long tmp_size = fp_size / 2;
    unsigned char *front = malloc(tmp_size);
    strncpy(front, store, i);
    //printf("1. reach here?(strcpy front part). front: %s\n", front);
    //copy back of emoji
    unsigned char *back = malloc(tmp_size);
    strncpy(back, store + (i+4), fp_size - (i+3));
    //printf("2. reach here?(strcpy back part). back: %s\n", back);

    // free(store);
    // unsigned char *store = malloc(store_size);
    memset(store, '\0', sizeof(store_size));
    //concat temp + front
    strcat(store,front);
//      printf("12. reach here?(store front). store: %s | front: %s\n", temp, front);
    //string concat temp + translation part
    strcat(store, trans_store);
//      printf("13. reach here?(store translation). store: %s | translation: %s\n", temp, trans_store);
    //concat temp(with translation) + back part
    strcat(store, back);
//      printf("14. reach here?(store back). store: %s | back: %s\n", temp, back);
    //memset(store, '\0', sizeof(store_size));

    free(front);
    free(back);

    // ------------- didn't free@!!!!!! trans_store!!!!!!!
    //free(trans_store);
    free(buf_emoji);
    //free(trans_store);
  }
  //find the string length
  // long len = strlen(store);
  // unsigned char *tranlate_ver = malloc(len);
  // strcpy(tranlate_ver, store);
  //make another malloc
  //restore
  //return 

  //fclose(fp);
  return store;
  //return tranlate_ver;
  free(store);
  //free(tranlate_ver);
  //return trans_file;
  
}

//called to destroy the emoji object and any memory allocated by your emoji library.
void emoji_destroy(emoji_t *emoji) {
  emoji_t *cur_src = emoji;
  emoji_t *cur_trans = emoji;
  emoji_t *tmp_src;
  emoji_t *tmp_trans;

  while (cur_src != NULL) {
    tmp_src = cur_src->next;
    free(cur_src);
    cur_src = tmp_src;
  }

  while (cur_trans != NULL) {
    tmp_trans = cur_trans->next;
    free(cur_trans);
    cur_trans = tmp_trans;
  }


}
