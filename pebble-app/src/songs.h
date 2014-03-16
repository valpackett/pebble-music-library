#ifndef SONGS
#define SONGS

void songs_init();
void songs_show(int8_t parent_type, int8_t parent_play_type, int8_t parent_id);
void songs_deinit();
bool songs_is_active();
void songs_content_start(uint16_t count);
void songs_content_add(uint16_t index, uint32_t id, char* name);

#endif
