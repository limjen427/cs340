#include "http.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <pthread.h>

/*
  1. read request
  2. read version to response buffer
  3. create a file path
  4. open the file with that path
  5. if file ptr is null -> 404
  6. not null -> 200
  7. append content type / payload
*/
void *client_thread(void *vptr) {
  int fd = *((int *)vptr); // file descriptor
  HTTPRequest *req = (HTTPRequest *) malloc(sizeof(HTTPRequest));

  ssize_t file_len = httprequest_read(req, fd);

  if (strcmp(req->path, "/") == 0) {
    req->path = "/index.html";
  }

  int res_len = 0;
  long size = 0;
  char *response = (char *)malloc(10000);
  memcpy(response, req->version, strlen(req->version));
  res_len += strlen(req->version);

  char *path = (char *)malloc(strlen("static") + strlen(req->path));
  memcpy(path, "static", strlen("static"));
  memcpy(path + strlen("static"), req->path, strlen(req->path));

  //4. open the file with that path
  FILE *fp = fopen(path, "r");
  //5. if file ptr is null -> 404
  if (fp == NULL) {
    memcpy(response + res_len, " 404 Not Found\r\n", strlen(" 404 Not Found\r\n"));
    fclose(fp);
  }
  else {
    //6. not null -> 200
    //response code
    memcpy(response + res_len, " 200 OK\r\n", strlen(" 200 OK\r\n"));
    res_len += strlen(" 200 OK\r\n");
    //content-type
    //.png?
    if (strstr(req->path, ".png") != NULL) {
      memcpy(response + res_len, "Content-Type: image/png\r\n\r\n", strlen("Content-Type: image/png\r\n\r\n"));
      res_len += strlen("Content-Type: image/png\r\n\r\n");
    }
    //.html?
    else if (strstr(req->path, ".html") != NULL)  {
      memcpy(response + res_len, "Content-Type: text/html\r\n\r\n", strlen("Content-Type: text/html\r\n\r\n"));
      res_len += strlen("Content-Type: text/html\r\n\r\n");
    }
    //payload
    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    rewind(fp);

    char* payload_buf = (char *)malloc(size);
    int payload_size = fread(payload_buf, size, 1, fp);

    memcpy(response + res_len, payload_buf, size);
    res_len += payload_size;

    free(payload_buf); //X matter 5th
    fclose(fp);

  }
  
  //send the response
  write(fd, response, 17+27+size);

  // after responding to the request. 
  close(fd);

  free(path); //X matter 3rd
  free(response); //X matter 4th
  free(req); //X matter 5th
  return NULL;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Usage: %s <port>\n", argv[0]);
    return 1;
  }
  int port = atoi(argv[1]);
  printf("Binding to port %d. Visit http://localhost:%d/ to interact with your server!\n", port, port);

  // socket:
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);

  // bind:
  struct sockaddr_in server_addr, client_address;
  memset(&server_addr, 0x00, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(port);  
  bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr));

  // listen:
  listen(sockfd, 10);

  // accept:
  socklen_t client_addr_len;
  while (1) {
    int *fd = malloc(sizeof(int));
    *fd = accept(sockfd, (struct sockaddr *)&client_address, &client_addr_len);
    printf("Client connected (fd=%d)\n", *fd);

    pthread_t tid;
    pthread_create(&tid, NULL, client_thread, fd);
    pthread_detach(tid);
  }

  return 0;
}