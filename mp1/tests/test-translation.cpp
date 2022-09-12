 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>

 #include "../emoji-translate.h"
 #include "lib/catch.hpp"


TEST_CASE("translate simple text - emoji translation works with an empty emoji_t", "[weight=1][part=2]") {
  emoji_t emoji;
  emoji_init(&emoji);

  unsigned char *translation = (unsigned char *) emoji_translate_file_alloc(&emoji, "tests/txt/simple.txt");
  REQUIRE(translation != NULL);

  INFO("translation := " << translation);
  REQUIRE(strcmp((char *) translation, "I 🧡💙 Illinois!") == 0);
  free(translation);

  emoji_destroy(&emoji);
}

// TEST_CASE("translate simple text - emoji translation with one translation", "[weight=1][part=2]") {
//   emoji_t emoji;
//   emoji_init(&emoji);

//   emoji_add_translation(&emoji, (const unsigned char *) "🧡", (const unsigned char *) "heart");

//   unsigned char *translation = (unsigned char *) emoji_translate_file_alloc(&emoji, "tests/txt/simple.txt");
//   REQUIRE(translation != NULL);

//   INFO("translation := " << translation);
//   REQUIRE(strcmp((char *) translation, "I heart💙 Illinois!") == 0);
//   free(translation);

//   emoji_destroy(&emoji);
// }

// TEST_CASE("translate simple text - emoji translation with two translations", "[weight=1][part=2]") {
//   emoji_t emoji;
//   emoji_init(&emoji);

//   emoji_add_translation(&emoji, (const unsigned char *) "🧡", (const unsigned char *) "heart");
//   emoji_add_translation(&emoji, (const unsigned char *) "💙", (const unsigned char *) "&heart");

//   unsigned char *translation = (unsigned char *) emoji_translate_file_alloc(&emoji, "tests/txt/simple.txt");
//   REQUIRE(translation != NULL);

//   INFO("translation := " << translation);
//   REQUIRE(strcmp((char *) translation, "I heart&heart Illinois!") == 0);
//   free(translation);

//   emoji_destroy(&emoji);
// }


// TEST_CASE("translate simple text - emoji translation with tiebreaks in translation", "[weight=1][part=2]") {
//   emoji_t emoji;
//   emoji_init(&emoji);

//   emoji_add_translation(&emoji, (const unsigned char *) "🧡", (const unsigned char *)"heart");
//   emoji_add_translation(&emoji, (const unsigned char *)"💙", (const unsigned char *)"&heart");
//   emoji_add_translation(&emoji, (const unsigned char *)"🧡💙", (const unsigned char *)":illini heart:");

//   unsigned char *translation = (unsigned char *) emoji_translate_file_alloc(&emoji, "tests/txt/simple.txt");
//   REQUIRE(translation != NULL);

//   INFO("translation := " << translation);
//   REQUIRE(strcmp((char *) translation, "I :illini heart: Illinois!") == 0);
//   free(translation);

//   emoji_destroy(&emoji);
// }


// TEST_CASE("translate simple text - emoji translation with tiebreaks (reverse order)", "[weight=1][part=2]") {
//   emoji_t emoji;
//   emoji_init(&emoji);

//   emoji_add_translation(&emoji, (const unsigned char *)"🧡💙", (const unsigned char *)":illini heart:");
//   emoji_add_translation(&emoji, (const unsigned char *)"🧡", (const unsigned char *)"heart");
//   emoji_add_translation(&emoji, (const unsigned char *)"💙", (const unsigned char *)"&heart");
  
//   unsigned char *translation = (unsigned char *) emoji_translate_file_alloc(&emoji, "tests/txt/simple.txt");
//   REQUIRE(translation != NULL);

//   INFO("translation := " << translation);
//   REQUIRE(strcmp((char *) translation, "I :illini heart: Illinois!") == 0);
//   free(translation);

//   emoji_destroy(&emoji);
// }


// TEST_CASE("translate long text - emoji translation with many translations", "[weight=1][part=2]") {
//   emoji_t emoji;
//   emoji_init(&emoji);

//   emoji_add_translation(&emoji, (const unsigned char *) "🙅", (const unsigned char *)"no, ");
//   emoji_add_translation(&emoji, (const unsigned char *) "🖐", (const unsigned char *)"but ");
//   emoji_add_translation(&emoji, (const unsigned char *) (const unsigned char *)"🙅👄💬🧑🔮", (const unsigned char *)"We don't talk about Bruno, ");
//   emoji_add_translation(&emoji, (const unsigned char *) (const unsigned char *)"💒👰💍🤵", (const unsigned char *)"It was my wedding day ");
//   emoji_add_translation(&emoji, (const unsigned char *) "💄💅💇🙅🌤🌄", (const unsigned char *)"We were getting ready, and there wasn't a cloud in the sky ");
//   emoji_add_translation(&emoji, (const unsigned char *) "🧑🔮🚶😈", (const unsigned char *)"Bruno walks in with a mischievous grin ");
//   emoji_add_translation(&emoji, (const unsigned char *) "😠🗯🗨💢🤫", (const unsigned char *)"You telling this story or am I?");

//   const char* expected = "We don't talk about Bruno, no, no, no, \n"
//   "We don't talk about Bruno, but \n"
//   "It was my wedding day \n"
//   "We were getting ready, and there wasn't a cloud in the sky \n"
//   "Bruno walks in with a mischievous grin \n"
//   "You telling this story or am I?";

//   unsigned char *translation = (unsigned char *) emoji_translate_file_alloc(&emoji, "tests/txt/long.txt");
//   REQUIRE(translation != NULL);
  
//   INFO("translation := " << translation);
//   REQUIRE(strcmp((char *) translation, expected) == 0);
//   free(translation);

//   emoji_destroy(&emoji);
// }


