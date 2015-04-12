/*
 * Some utilities to easily import
 */

#define MAX_FILEPATH_LENGTH 4096

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/stat.h>
#include <sys/types.h>

void print_array (char *string) {
  do {
    printf("%c", string[0]);
  } while( *(string++) != 0 );
}

void fprint_array (FILE *some_file, const char *restrict string) {
  do {
    fprintf(some_file, "%c", string[0]);
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

int fs_exsists (const char *restrict filepath)
{
  struct stat file_status = {0};
  if( stat(filepath, &file_status) == -1 )
  {
    return 0;
  }
  return 1;
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

int fs_parent_mode (const char *restrict filename)
{
  mode_t mode;
  char *restrict basepath;
  basepath = (char *restrict) malloc(sizeof(filename));
  realloc(basepath, fs_basepath(filename, basepath));
  if((mode = fs_mode(basepath)))
  {
    return mode;
  }
  free(basepath);
  return 0;
}

int fs_mkdir (const char *restrict dirname)
{
  if( !fs_exsists(dirname) )
  {
    mkdir(dirname, fs_parent_mode(dirname));
    return 1;
  } else
  {
    printf("A something with this name already exsists.");
  }
  return 0;
}

int fs_rm (const char* filename)
{
  if( fs_exsists(filename) )
  {
    unlink(filename);
    return 1;
  }
  return 0;
}

int fs_cp (const char *restrict src, const char *restrict dst)
{
  if( fs_exsists(src) )
  {
    return 1;
  }
  return 0;
}

int write_string_to_file (
  const char *restrict filename,
  const char *restrict string
)
{
  FILE *file_stream;
  if( fs_parent_mode(filename) )
  {
    if( (file_stream = fopen(filename, "w")) )
    {
      fprint_array(file_stream, string);
      fclose(file_stream);
      return 1;
    }
  }
  return 0;
}
