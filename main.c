/* Source: K&R */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "dirent.h"


#define NAME_MAX 14
#define MAX_PATH 1024

typedef struct {
  long ino;
  char name[NAME_MAX+1];
} Dirent;

typedef struct {
  int fd;
  Dirent d;
} DIR;

DIR *opendir(char *dirname);
Dirent *readdir(DIR *dfd);
void closedir(DIR *dfd);

char *name;
struct stat stbuf;
int stat(char *, struct stat *);
void dirwalk(char *, void (*fcn)(char *));

stat(name, &stbuf);

struct stat
{
  dev_t   st_dev;
  ino_t   st_ino;
  short   st_mode;
  short   st_nlink;
  short   st_uid;
  short   st_gid;
  dev_t   st_rdev;
  off_t   st_size;
  time_t  st_atime;
  time_t  st_mtime;
  time_t  st_ctime;
};

void fsize(char *);

int main(int argc, char **argv)
{
  if (argc == 1)
    fsize(".");
  else
    while (--argc > 0)
      fsize(*++argv);
  return 0;
}

void fsize(char *name)
{
  struct stat stbuf;

  if (stat(name, &stbuf) == -1) {
    fprintf(stderr, "fsize: can't access %s\n", name);
    return;
  }
  if ((stbuf.st_mode & S_IFMT) == S_IFDIR)
    dirwalk(name, fsize);
  printf("%8ld %s\n", stbuf.st_size, name);
}

void dirwalk(char *dir, void (*fcn)(char *))
{
  char name[MAX_PATH];
  Dirent *dp;
  DIR *dfd;

  if ((dfd = opendir(dir)) == NULL) {
    fprintf(stderr, "dirwalk: can't open %s\n", dir);
    return;
  }
  while ((dp = readdir(dfd)) != NULL) {
    if (strcmp(dp->name, ".") == 0 || strcmp(dp->name, "..") == 0)
      continue;
    if (strlen(dir)+strlen(dp->name)+2 > sizeof(name))
      fprintf(stderr, "dirwalk: name %s/%s too long\n", dir, dp->name);
    else {
      sprintf(name, "%s/%s", dir, dp->name);
      (*fcn)(name);
    }
  }
  closedir(dfd);
}
