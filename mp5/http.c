#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#include "http.h"

// void find_list(HTTPRequest *req, char* key, char* value) {
//   keyList *cur = req->head;

//   while (cur != NULL) {
//     if (cur->key != NULL && strcmp(cur->key, key) == 0) {
//       return;
//     }
//     cur = cur->next; 
//   }

//   //no matching key founded
//   keyList *new_head = malloc(sizeof(keyList));
//   new_head->key = key;
//   new_head->value = value;
//   new_head->next = req->head;
//   req->head = new_head;
//   return;
// }

/**
 * httprequest_parse_headers
 * 
 * Populate a `req` with the contents of `buffer`, returning the number of bytes used from `buf`.
 */
char *tmp = NULL;
ssize_t httprequest_parse_headers(HTTPRequest *req, char *buffer, ssize_t buffer_len) {
  req->head = NULL;
  tmp = (char *)malloc(buffer_len+1);
  //printf("!!!!!!!!!!!!!!!!!!!!!!tmp size!!!!!!!!!!!!!!!!!!!!!: %d\n", buffer_len);
  memcpy(tmp, buffer, buffer_len);
  tmp[buffer_len] = '\0';
  //printf("buffer: %s, tmp: %s\n", buffer, tmp);
  char *store_rn; // line
  char *store_h; // subtok
  char *store_f; //subtok

  char *tok = strtok_r(tmp, "\r\n", &store_rn);
  //free(tmp);
  //printf("tok: %s\n", tok);

  req->action = strtok_r(tok, " ", &store_f);
  //char *subtok = strtok_r(tok, " ", &store_f);
  //req->action = subtok;
  //printf("action: %s::\n", req->action);
  req->path = strtok_r(NULL, " ", &store_f);
  //subtok = strtok_r(NULL, " ", &store_f);
  //req->path = subtok;
  //printf("path: %s::\n", req->path);
  req->version = strtok_r(NULL, " ", &store_f);
  //subtok = strtok_r(NULL, " ", &store_f);
  //req->version = subtok;
  //printf("verison: %s::\n", req->version);

  tok = strtok_r(NULL, "\r\n", &store_rn);
  //printf("before while first header start. tok: %s\n", tok);
  while (tok != NULL) {
    // part to parse with ": "
    char *key = strtok_r(tok, ": ", &store_h);
    //char *sub_head = strtok_r(tok, ": ", &store_h);
    //char *key = sub_head;
    //printf("key%d: %s\n", i, key);
    char *value = strtok_r(NULL, ": ", &store_h);
    //sub_head = strtok_r(NULL, ": ", &store_h);
    //char *value = sub_head;
    //printf("value%d: %s\n", i, value);

    keyList *new_head = malloc(sizeof(keyList));
    new_head->key = key;
    new_head->value = value;
    new_head->next = req->head;
    req->head = new_head;

    tok = strtok_r(NULL, "\r\n", &store_rn);
    if (strcmp(key, "Content-Length") == 0) {
      break;
    }
    //printf("tok%d: %s\n", i, tok);
  }

  //printf("after header tok?: %s\n", tok);
  const char* check = httprequest_get_header(req, "Content-Length");
  if (check == NULL) {
    req->payload = NULL;
  }
  else {
    //int len_payload = atoi(check);
    //printf("len of payload: %d\n", len_payload);
    printf("tok payload: %s\n", tok);
    req->payload = tok;
    printf("payload: %s\n", req->payload);
    //memcpy(req->payload, tok, len_payload);
  }


  // free(tmp);
  return buffer_len;
  //free(tmp);
}


/**
 * httprequest_read
 * 
 * Populate a `req` from the socket `sockfd`, returning the number of bytes read to populate `req`.
 */
ssize_t httprequest_read(HTTPRequest *req, int sockfd) {
  char *tmp = (char *)malloc(1024);
  ssize_t buffer_len = read(sockfd, tmp, 1024);
  //realloc(tmp, buffer_len);
  char *buffer = (char*)malloc(buffer_len+1);
  memcpy(buffer, tmp, buffer_len);
  buffer[buffer_len] = '\0';
  free(tmp); // matter(1034) 1st

  httprequest_parse_headers(req, buffer, buffer_len);

  //Content-Length를 key 에서 찾으면 value에 있는 애가 payload 길이
  //\r\n\r\n 다음으로 쭉 payload아이이니까 memcpy(dest, src, value of content-length key)해줘야함. 
  free(buffer); //matter(616) 2nd
  return buffer_len;
}


/**
 * httprequest_get_action
 * 
 * Returns the HTTP action verb for a given `req`.
 */
const char *httprequest_get_action(HTTPRequest *req) {
  return req->action;
}


/**
 * httprequest_get_header
 * 
 * Returns the value of the HTTP header `key` for a given `req`.
 */
const char *httprequest_get_header(HTTPRequest *req, const char *key) {
  keyList *cur = req->head;

  while (cur != NULL) {
    if (strcmp(cur->key, key) == 0) {
      return cur->value;
    }
    cur = cur->next;
  }
  return NULL;
}


/**
 * httprequest_get_path
 * 
 * Returns the requested path for a given `req`.
 */
const char *httprequest_get_path(HTTPRequest *req) {
  return req->path;
}


/**
 * httprequest_destroy
 * 
 * Destroys a `req`, freeing all associated memory.
 */
void httprequest_destroy(HTTPRequest *req) {
  keyList *cur = req->head;
  keyList *tmp2;

  while (cur != NULL) {
    tmp2 = cur->next;
    free(cur);
    cur = tmp2;
  }
  cur = NULL;
  free(tmp);
}