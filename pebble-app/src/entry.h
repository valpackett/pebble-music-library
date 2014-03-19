#ifndef _ENTRY
#define _ENTRY
#define ENTRY_NAME_LEN 64

typedef struct {
  uint32_t id;
  char name[ENTRY_NAME_LEN];
} Entry;

#endif
