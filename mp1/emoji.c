#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
 
// Return your favorite emoji.  Do not allocate new memory.
// (This should **really** be your favorite emoji, we plan to use this later in the semester. :))
char *emoji_favorite() {
return "🥲";
}
// Count the number of emoji in the UTF-8 string `utf8str`, returning the count.  You should
// consider everything in the ranges starting from (and including) U+1F000 up to (and including) U+1FAFF.
int emoji_count(const unsigned char *utf8str) {
int count = 0;
for (size_t i = 0; i < strlen(utf8str)-1; i++) {
  if (utf8str[i] == 0xf0 && utf8str[i+1] == 0x9f) {
    count++;
  }
}
return count;
}
// Return a random emoji stored in new heap memory you have allocated.  Make sure what
// you return is a valid C-string that contains only one random emoji.
char *emoji_random_alloc() {
//U+1f000 = 126976(in decimal), 1faff = 129791
//new heap memory
char *s = malloc(5); // emoji always 4-bytes and last one should be 0x00 to tell it end.
 //generate random emoji into decimal
int ran_dec = rand() % (4036996031 - 4036984960) + 4036984960;
//printf("%d\n", ran_dec);
/* memcpy() results depends on machine architecture and endian-ness */
s[0] = (ran_dec >> 24) & 0xFF;
s[1] = (ran_dec >> 16) & 0xFF;
s[2] = (ran_dec >> 8) & 0xFF;
s[3] = ran_dec & 0xFF;
s[4] = '\0';
return s;
free(s);
}
 
 
 
// Modify the UTF-8 string `utf8str` to invert the FIRST character (which may be up to 4 bytes)
// in the string if it the first character is an emoji.  At a minimum:
// - Invert "😊" U+1F60A ("\xF0\x9F\x98\x8A") into ANY non-smiling face.
// - Choose at least five more emoji to invert.
void emoji_invertChar(unsigned char *utf8str) {
 //int c = 1; // first character
 
 if (utf8str[0] == 0xf0 && utf8str[1] == 0x9f) {
   if (utf8str[2] == 0x98 && utf8str[3] == 0x89) {
     //whinking to cold sweat
     utf8str[3] = utf8str[3] + 10; // 93
   }
   else if (utf8str[2] == 0x98 && utf8str[3] == 0x8a) {
     // smiling to non-smiling
     utf8str[3] = utf8str[3] + 10; // 94
   }
   else if (utf8str[2] == 0x98 && utf8str[3] == 0x8b) {
     // savouring declious to unamused
     utf8str[3] = utf8str[3] + 7; // 92
   }
   else if (utf8str[2] == 0x98 && utf8str[3] == 0x8c) {
     // relieved to confounded
     utf8str[3] = utf8str[3] + 10; // 96
   }
   else if (utf8str[2] == 0x98 && utf8str[3] == 0x8d) {
     // heart-shaped eyes to pensive
     utf8str[3] = utf8str[3] + 8; // 95
   }
   else if (utf8str[2] == 0x98 && utf8str[3] == 0x8f) {
     // smirking to kissing
     utf8str[3] = utf8str[3] + 9; // 98
   }
 }
   /*
   if (c == 1) {
     utf8str[3] = utf8str[3] + 10;
     c++;
   }
   else {
     utf8str[3]++;
     c++;
   }
   */
}
 
 
// Modify the UTF-8 string `utf8str` to invert ALL of the character by calling your
// `emoji_invertChar` function on each character.
void emoji_invertAll(unsigned char *utf8str) {
 for (int i = 0; i < (int)strlen(utf8str); i = i + 5) {
   utf8str = utf8str + i;
   emoji_invertChar(utf8str);
 }
}
 
 
// Reads the full contents of the file `fileName, inverts all emojis, and
// returns a newly allocated string with the inverted file's content.
unsigned char *emoji_invertFile_alloc(const char *fileName) {
 FILE *fp;
 
 fp = fopen(fileName, "r+"); // reading + writing
 if (!fp) {
   return NULL;
 }
 
 // find the length of the file
 fseek(fp, 0L, SEEK_END); // seek to the EOF
 long size = ftell(fp); // get the current position
 fseek(fp, 0L, SEEK_SET);
 //rewind(fp); // rewind to the beginning of the file
 
 // make new variable "invfile" by mallocing witht he length of the file
 char* invfile = calloc(1, size+1);
 //if (!invfile) fclose(fp);
 
 // fread the file
 fread(invfile, size, 1, fp);
//  if (1 != fread(invfile, size, 1, fp)) {
//   fclose(fp);
//   free(invfile);
//  }

 // emoji_invertAll
 emoji_invertAll(invfile);
 //invfile[size] = "\0";

 fclose(fp);
 return invfile;
 free(invfile);
}
 

