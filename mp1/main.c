#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "emoji.h"
#include "emoji-translate.h"

int main() {
  // Feel free to delete all of this and use this function in any way you want:
  // printf("Currently, `main.c` does nothing but has all of your code included.\n");
  // printf("- Edit `main.c` to test your functions within the context of `main`.\n");
  // printf("- View the `test` directory to view the test cases defiend for this MP.\n");
  // printf("- Run `make test` and `./test` to run the test suite.\n");

  emoji_t emoji;
  emoji_init(&emoji);

  emoji_add_translation(&emoji, (const unsigned char *) "🧡", (const unsigned char *) "heart");
  emoji_add_translation(&emoji, (const unsigned char *) "💙", (const unsigned char *) "&heart");

  unsigned char *translation = (unsigned char *) emoji_translate_file_alloc(&emoji, "tests/txt/simple.txt");

  printf(translation);
  free(translation);

  emoji_destroy(&emoji);
  return 0;
}