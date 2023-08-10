#pragma once

#ifdef _WIN32
#include <direct.h>
#include <sys/stat.h>
#include <sys/types.h>

#ifdef __CAKE__
#pragma CAKE diagnostic push
#pragma CAKE diagnostic ignore "-Wstyle"
#endif

// https://docs.microsoft.com/pt-br/cpp/c-runtime-library/reference/mkdir-wmkdir?view=msvc-160
#define mkdir(a, b) _mkdir(a)
#define rmdir _rmdir
#define chdir _chdir

#ifdef __CAKE__
#pragma CAKE diagnostic pop
#endif

/*
 opendir,  readdir closedir for windows.
 include dirent.h on linux
*/

enum {
  DT_UNKNOWN = 0,
  DT_FIFO = 1,
  DT_CHR = 2,
  DT_DIR = 4,
  DT_BLK = 6,
  DT_REG = 8,
  DT_LNK = 10,
  DT_SOCK = 12,
  DT_WHT = 14
};

struct dirent {
  ino_t d_ino;             /* Inode number */
  off_t d_off;             /* Not an offset; see below */
  unsigned short d_reclen; /* Length of this record */
  unsigned char d_type;    /* Type of file; not supported
                                   by all filesystem types*/
  char d_name[256];        /* Null-terminated filename */
};

#ifdef __CAKE__
#pragma CAKE diagnostic push
#pragma CAKE diagnostic ignore "-Wstyle"
#endif
struct TAGDIR;
typedef struct TAGDIR DIR;

#ifdef __CAKE__
#pragma CAKE diagnostic pop
#endif

DIR *opendir(const char *name);
int closedir(DIR *dirp);
struct dirent *readdir(DIR *dirp);

#else

#define MAX_PATH 500

// https://man7.org/linux/man-pages/man2/mkdir.2.html
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#endif

char *realpath(const char *restrict path, char *restrict resolved_path);

int get_self_path(char *buffer, int maxsize);

char *read_file(const char *path);
char *dirname(char *path);

const char *get_posix_error_message(int error);
