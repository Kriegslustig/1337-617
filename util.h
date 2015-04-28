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

void print_array (const char *restrict string) {
  int itrator = 0;
  do {
    printf("%c", string[itrator]);
  } while( string[itrator++] != 0 );
}

void fprint_array (FILE *some_file, const char *restrict string) {
  do {
    fprintf(some_file, "%c", string[0]);
  } while( *(string++) != 0 );
}

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
  char *restrict basepath = fs_basepath(filename);
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
    printf("A something with this name already exsists.: ");
    print_array(dirname);
    printf("\n");
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

int fs_concat_path (
  const char *restrict basepath,
  const char *restrict extend_with,
  char *restrict new_path
)
{
  strcat(new_path, basepath);
  strcat(new_path, "/");
  strcat(new_path, extend_with);
  return sizeof(new_path);
}

int fs_recursive_copy (
  const char *restrict source,
  const char *restrict destination
)
{
  DIR *source_directory;
  struct dirent *source_directory_list_item;
  char *restrict new_source = (char *restrict) malloc(sizeof(source) + 255);
  char *restrict new_destination = (char *restrict) malloc(sizeof(source) + 255);
  if( fs_exsists(source) )
  {
    fs_mkdir(destination);
    source_directory = opendir(source);
    while( (source_directory_list_item = readdir(source_directory)) )
    {
      if( (source_directory_list_item->d_name)[0] != '.' )
      {
        fs_concat_path(source, source_directory_list_item->d_name, new_source);
        fs_concat_path(destination, source_directory_list_item->d_name, new_destination);
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
  FILE *file_stream_clone = file_stream;
  char char_buffer[1];
  char *line_buffer = malloc(1);
  int current_position = 1;

  while(
    fread(char_buffer, 1, 1, file_stream_clone) &&
    *char_buffer != '\n'
  )
  {

    line_buffer[current_position - 1] = *char_buffer;
    fseek(file_stream_clone, 1, 1);
    line_buffer = realloc(line_buffer, ++current_position);
  }
  return line_buffer;
}

int find_next_in_filestream (
  FILE *file_stream,
  char some_char
)
{
  int current_position = 0;
  char file_buffer[1];

  while(
    fread(file_buffer, 1, 1, file_stream) != 0 &&
    *file_buffer != some_char
  )
    current_position++;
  fseek(file_stream, ((current_position + 1) * -1), 1);
  return current_position;
}

char *restrict fs_read_nth_line (
  const int nth,
  const char *filename
)
{
  FILE *file_stream;
  char *string;
  int current_line = 0;
  int char_offset = 0;

  if( fs_exsists(filename) )
  {
    file_stream = fopen(filename, "r");
    while(
      current_line++ < nth &&
      (char_offset = find_next_in_filestream(file_stream, '\n') + 1) &&
      (fseek(file_stream, char_offset, 1))
    )
    {
      printf("%d\n", char_offset);
    }
    string = fs_first_line(file_stream);
    return string;
  }
  return 0;
}

char *restrict fs_read_nth_line_from_end (
  int nth,
  const char *filename
)
{
  char *restrict file_content;
  char *tmp_string;
  char *tmp_pointer;
  char *string;

  if( fs_exsists(filename) )
  {
    file_content = read_string_from_file(filename);
    tmp_string = file_content;
    while(
      --nth >= 0 &&
      (tmp_pointer = strrchr(tmp_string, '\n'))
    )
    {
      *tmp_pointer = '\0';
      printf("%lo\n", strlen(tmp_string));
    }
    print_array(tmp_string);
    printf("\n");
    tmp_pointer = strrchr(tmp_string, '\n');
    if( tmp_pointer )
      string = substring(tmp_pointer + 1, 0, tmp_pointer - tmp_string);
    else
    {
      string = malloc(strlen(tmp_string));
      string = tmp_string;
    }
    return string;
    free(file_content);
  }
  return 0;
}
