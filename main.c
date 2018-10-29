
/* Source: K&R */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <time.h>

#define MAX_PATH 1024

void dirwalk(char *, void (*fcn)(char *));
void fsize(char *);
char get_file_type(struct stat stbuf);
char get_file_modification_date(struct stat stbuf, char *file_modification_date);
/*
déclarer fonctions !
*/

/* get_file_type returns a character representing a file's type ('d' for
  directory, 'l' for link, '-' for regular file)
  It returns -1 if it the file type is unknown.
*/
char get_file_type(struct stat stbuf) {


  char file_type_char;

  switch (stbuf.st_mode & S_IFMT) {
   // case S_IFBLK:  printf("block device\n");            break;
   // case S_IFCHR:  printf("character device\n");        break;
   case S_IFDIR:  file_type_char = 'd';                 break; // directory
   // case S_IFIFO:  printf("FIFO/pipe\n");               break;
   case S_IFLNK:  file_type_char = 'l';                 break; // link
   case S_IFREG:  file_type_char = '-';                 break; // regular file
   // case S_IFSOCK: printf("socket\n");                  break;
   default:       file_type_char = -1;                  break; // ??
  }
  return file_type_char;
}




// char get_file_modification_date(struct stat stbuf, char *file_modification_date) {

/* Le code marche tel quel, mais plus lorsque j'en fais une fonction à part.
  Je ne vois pas d'où vient le problème..
*/

//   struct tm *tmp;
//   char format[] = "%a %b %d %T %Y"; // eg. Mon Oct 29 12:11:06 2018
//
//   tmp = localtime(&stbuf.st_mtime);
//   if (tmp == NULL) {
//       perror("localtime");
//       exit(EXIT_FAILURE);
//   }
//
//   if (strftime(file_modification_date, sizeof(file_modification_date), format, tmp) == 0) {
//       fprintf(stderr, "strftime returned 0\n");
//       exit(EXIT_FAILURE);
//   }
//
//   return 0;
// }

void fsize(char *name)
{
  struct stat stbuf;
  char file_type_char;
  char file_modification_date[200];

  if (stat(name, &stbuf) == -1) {
    //fprintf(stdout, "yo %s\n",strerror( errno )); // hem hem
    fprintf(stderr, "fsize: can't access %s\n", name);
    return;
  }

  if (S_ISDIR(stbuf.st_mode)) //((stbuf.st_mode & S_IFMT) == S_IFDIR)
    dirwalk(name, fsize);

  file_type_char = get_file_type(stbuf);

  // get_file_modification_date(stbuf, file_modification_date);

  // Je laisse le code ici, car je n'arrive pas à en faire une fonction à part.
  // C'est peut-être dû au pointeur utilisé.. ?
  struct tm *tmp;
  char format[] = "%a %b %d %T %Y"; // eg. Mon Oct 29 12:11:06 2018
  tmp = localtime(&stbuf.st_mtime);
  if (tmp == NULL) {
      perror("localtime");
      exit(EXIT_FAILURE);
  }
  if (strftime(file_modification_date, sizeof(file_modification_date), format, tmp) == 0) {
      fprintf(stderr, "strftime returned 0\n");
      exit(EXIT_FAILURE);
  }



  printf("%c\t%8ld %s %s\n", file_type_char, stbuf.st_size, file_modification_date, name);
  // print file type, permissions, size, date, pathname

}


void dirwalk(char *dir, void (*fcn)(char *))
{
  char name[MAX_PATH];
  struct dirent *dp;
  DIR *dfd;

  if ((dfd = opendir(dir)) == NULL) {
    fprintf(stderr, "dirwalk: can't open %s\n", dir);
    return;
  }
  while ((dp = readdir(dfd)) != NULL) {
    if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0)
      continue;
    if (strlen(dir)+strlen(dp->d_name)+2 > sizeof(name))
      fprintf(stderr, "dirwalk: name %s/%s too long\n", dir, dp->d_name);
    else {
      sprintf(name, "%s/%s", dir, dp->d_name);
      (*fcn)(name);
    }
  }
  closedir(dfd);
}

/*
int suiton_destination_listing_no_jutsu(sources, pointer-to-func)
{
  process_files(sources, pointer-to-list-func);
}

int katon_copy_no_jutsu(sources, pointer-func)
{
  process_files(sources, pointer-to-copy-func);
}

int process_files(??, pointer-to-processing function) //eg. copy_file, print_file_info
{
  while (--count > 0)// for each file
  {
    if is_dir
      dirwalk
    else
      (*processing_function)()
  }
}

int print_file_info()
{
  blah;
}

int copy_file()
{
  bleh;
}

*/

int main(int argc, char **argv)
{
  /*
  1. Récupère paramètres (sources et dest)
  2. Copie fichiers (chaque source dans dest)
    1. si fichier: copie
    2. si dossier: dirwalk avec fonction copie
  3. Liste fichiers de dest
    Fonction liste avec dirwalk

  process_args(&sources, &dest);
  katon_copy_no_jutsu(sources, dest);
  suiton_destination_listing_no_jutsu(dest);

  */

  if (argc == 1)
    fsize(".");
  else
    while (--argc > 0)
      fsize(*++argv);
  return 0;
}
