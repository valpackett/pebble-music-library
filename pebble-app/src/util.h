#ifndef _UTIL
#define _UTIL

// http://blog.liw.fi/posts/strncpy/
#define fucking_copy_string(dst, src, len) if (snprintf((dst), (len), "%s", (src)) >= (len)) snprintf((dst), (len), "%s", "Overflow Error")

#endif
