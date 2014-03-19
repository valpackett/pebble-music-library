#ifndef _COMM
#define _COMM

#include <pebble.h>

#define MSG_TYPE 0
enum mtype {
  REQ,
  RSP_START,
  RSP_DATA,
  RSP_END, // ununsed
  PLAY
};

#define MSG_CTX 1
#define MSG_PARENT_CTX 2
enum mcontext {
  ARTISTS,
  ALBUMS,
  PLAYLISTS,
  SONGS
};

#define COUNT 100
#define INDEX 101
#define ID 102
#define NAME 103

void request_data(int8_t context, int8_t parent_context, int8_t id);
void request_play(int8_t parent_context, int32_t parent_id, int8_t index);

#endif
