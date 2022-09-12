#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _emoji_t {
    //struct _emoji_t *head;
    const unsigned char *source;  // souce, emoji
    const unsigned char *trans; //  translation, emoji meaning
    struct _emoji_t *next;
} emoji_t;

// typedef struct emoji_resource_ {
// } emoji_resource;

void emoji_init(emoji_t *emoji);
void emoji_add_translation(emoji_t *emoji, const unsigned char *source, const unsigned char *translation);
const unsigned char *emoji_translate_file_alloc(emoji_t *emoji, const char *fileName);
void emoji_destroy(emoji_t *emoji);

#ifdef __cplusplus
}
#endif
