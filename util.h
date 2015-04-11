/*
 * Some utilities to easily import
 */

#define MAX_FILEPATH_LENGTH 4096

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/stat.h>
#include <sys/types.h>

void print_array (char *string) {
  do {
    printf("%c", string[0]);
  } while( *(string++) != 0 );
}

void substring (
  const char *restrict string,
  const int from,
  const int length,
  char *restrict new_string
)
{
  int current_position = 0;
  do {
    new_string[current_position] = string[from + current_position];
  } while( current_position++ < (length - 1) );
}

void substr (
  const char *restrict string,
  const int from,
  const int to,
  char *restrict new_array
)
{
  int length = to - from;
  substring(string, from, length, new_array);
}

int fs_basepath (
  const char *restrict filepath,
  char *restrict basepath
)
{
  int basepath_length = 0;
  char *restrict last_char;
  last_char = strrchr(filepath, '/');
  if(last_char != 0)
  {
    basepath_length = last_char - filepath;
    substring(filepath, 0, basepath_length, basepath);
  } else
  {
    *basepath = '.';
  }
  return basepath_length;
}

int fs_exsists (const char *restrict filepath)
{
  struct stat file_status = {0};
  if( stat(filepath, &file_status) == -1 )
  {
    return 0;
  }
  return 1;
}

mode_t fs_mode (char *restrict filepath) {
  struct stat file_status = {0};
  if( !fs_exsists(filepath) )
  {
    printf("No suchfile:");
    print_array(filepath);
    printf("\n");
    return 0;
  }
  stat(filepath, &file_status);
  return file_status.st_mode;
}

int fs_mkdir (const char *restrict dirname)
{
  char *basepath = malloc(sizeof(dirname));
  mode_t mode;

  if( !fs_exsists(dirname) )
  {
    realloc(basepath, fs_basepath(dirname, basepath));
    mode = fs_mode(basepath);
    mkdir(dirname, mode);
    return 1;
  } else
  {
    printf("A something with this name already exsists.");
  }
  return 0;
}