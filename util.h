/*
 * Some utilities to easily import
 */

#define MAX_FILEPATH_LENGTH 4096

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>

#include <sys/stat.h>
#include <sys/types.h>

char *restrict substring (
  const char *restrict string,
  const int from,
  const int length
)
{
  int current_position = 0;
  char *restrict new_string = malloc(length);
  do {
    new_string[current_position] = string[from + current_position];
  } while( current_position++ < (length - 1) );
  return new_string;
}

char *restrict substr (
  const char *restrict string,
  const int from,
  const int to
)
{
  int length = to - from;
  char *restrict new_string = substring(string, from, length);
  return new_string;
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

char *restrict fs_basepath (const char *restrict filepath)
{
  int basepath_length = 0;
  char *restrict last_char;
  char *restrict basepath;
  last_char = strrchr(filepath, '/');
  if(last_char != 0)
  {
    basepath_length = last_char - filepath;
    basepath = substring(filepath, 0, basepath_length);
  } else
  {
    basepath = malloc(1);
    *basepath = '.';
  }
  return basepath;
}

mode_t fs_mode (const char *restrict filepath) {
  struct stat file_status = {0};
  if( !fs_exsists(filepath) )
  {
    printf("No such file: %s\n", filepath);
    return 0;
  }
  stat(filepath, &file_status);
  return file_status.st_mode;
}

int fs_parent_mode (const char *restrict filename)
{
  int returnValue = 0;
  char *restrict basepath = fs_basepath(filename);

  returnValue = fs_mode(basepath);
  free(basepath);
  return returnValue;
}

int fs_mkdir (const char *restrict dirname)
{
  int parent_mode;
  if( !fs_exsists(dirname) )
  {
    if((parent_mode = fs_parent_mode(dirname)))
    {
      mkdir(dirname, parent_mode);
      return 1;
    } else
    {
      return 0;
    }
  } else
  {
    printf("A something with this name already exsists.: %s\n", dirname);
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
  char buffer[4096];
  int size;
  FILE *src_file;
  FILE *dst_file;
  if( fs_exsists(src) )
  {
    src_file = fopen(src, "r");
    dst_file = fopen(dst, "w");
    while((size = fread(buffer, 1, 4096, src_file)) > 0)
      fwrite(buffer, 1, size, dst_file);
    fclose(src_file);
    fclose(dst_file);
    return 1;
  }
  return 0;
}

int fs_is_dir (const char *restrict filepath)
{
  if( fs_exsists(filepath) && S_ISDIR(fs_mode(filepath)) )
  {
    return 1;
  }
  return 0;
}

char *restrict fs_concat_path (
  const char *restrict basepath,
  const char *restrict extend_with
)
{
  char *restrict new_path = malloc(snprintf(NULL, 0, "%s/%s", basepath, extend_with) + 1);
  sprintf(new_path, "%s/%s", basepath, extend_with);
  return new_path;
}

int fs_recursive_copy (
  const char *restrict source,
  const char *restrict destination
)
{
  DIR *source_directory;
  struct dirent *source_directory_list_item;
  char *restrict new_source;
  char *restrict new_destination;
  if( fs_exsists(source) )
  {
    fs_mkdir(destination);
    source_directory = opendir(source);
    while( (source_directory_list_item = readdir(source_directory)) )
    {
      if( (source_directory_list_item->d_name)[0] != '.' )
      {
        new_source = fs_concat_path(source, source_directory_list_item->d_name);
        new_destination = fs_concat_path(destination, source_directory_list_item->d_name);
        if( fs_is_dir(new_source) )
        {
          fs_recursive_copy(new_source, new_destination);
        } else
        {
          fs_cp(new_source, new_destination);
        }
        free(new_source);
        free(new_destination);
      }
    }
    return 1;
    closedir(source_directory);
  }
  return 0;
}

int write_string_to_file (
  const char *restrict filename,
  const char *restrict string
)
{
  FILE *file_stream;
  if( (file_stream = fopen(filename, "a")) )
  {
    fprintf(file_stream, "%s", string);
    fclose(file_stream);
    return 1;
  }
  return 0;
}

int fs_filesize (const char *restrict filename) {
  struct stat file_status = {0};
  stat(filename, &file_status);
  return file_status.st_size;
}

char *restrict read_string_from_file (const char *restrict filename)
{
  FILE *file_stream = NULL;
  char *restrict file_content;
  off_t file_size = 0;
  if( fs_exsists(filename) )
  {
    file_size = fs_filesize(filename);
    file_content = malloc(file_size);
    file_stream = fopen(filename, "r");
    fread(file_content, 1, file_size, file_stream);
    fclose(file_stream);
    return file_content;
  }
  return 0;
}

char *restrict fs_first_line (
  FILE *file_stream
)
{
  const long int origin = ftell(file_stream);
  char char_buffer[1];
  char *line_buffer = malloc(1);
  int current_position = 0;

  while(
    fread(char_buffer, 1, 1, file_stream) > 0 &&
    *char_buffer != '\n'
  )
  {
    line_buffer = realloc(line_buffer, ++current_position);
    line_buffer[current_position - 1] = *char_buffer;
  }
  fseek(file_stream, origin, 0);
  return line_buffer;
}

int find_next_in_filestream (
  FILE *file_stream,
  char some_char
)
{
  const long int origin = ftell(file_stream);
  int current_position = 0;
  char file_buffer[1];

  while(
    fread(file_buffer, 1, 1, file_stream) != 0 &&
    *file_buffer != some_char
  )
    current_position++;
  fseek(file_stream, origin, 1);
  return current_position;
}

/*
 * Finds the last occurence of char, looking from SEEK_CUR
 * and returns a position relative to SEEK_SET
 */
int find_last_in_filestream (
  FILE *file_stream,
  const char find_this
)
{
  const long int origin = ftell(file_stream);
  char current_character[1];
  int offset = 0;

  do
  {
    offset--;
    fseek(file_stream, -1, 1);
    fread(current_character, 1, 1, file_stream);
    fseek(file_stream, -1, 1);
  } while(
    *current_character != find_this &&
    ftell(file_stream) > 0
  );
  fseek(file_stream, origin, 0);
  return offset;
}

char *restrict fs_read_nth_line (
  FILE *file_stream,
  const int nth
)
{
  char *string;
  int current_line = 0;
  int char_offset = 0;

  while(
    current_line++ < nth &&
    (char_offset = find_next_in_filestream(file_stream, '\n') + 1) &&
    (fseek(file_stream, char_offset, 1) != -1)
  );
  string = fs_first_line(file_stream);
  return string;
}

char *restrict fs_read_nth_line_from_end (
  FILE *file_stream,
  int nth
)
{
  const long int origin = ftell(file_stream);
  char *restrict return_string;
  int current_index = 0;

  fseek(file_stream, 0, 2);
  do
  {
    fseek(file_stream, find_last_in_filestream(file_stream, '\n'), 1);
  } while(current_index++ < nth);
  fseek(file_stream, 1, 1);
  return_string = fs_first_line(file_stream);
  fseek(file_stream, origin, 0);
  return return_string;
}
