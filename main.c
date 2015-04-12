#include <1337-617.h>

int main ()
{
  char some_file[] = "testfile";
  char another_file[] = "testfile_cp";
  char some_string[] = "SOmething or other!!";
  write_string_to_file(some_file, some_string);
  fs_cp(some_file, another_file);
  print_array(read_string_from_file(another_file));
  fs_rm(some_file);
  printf("\n");
  print_array(read_string_from_file(another_file));
  fs_rm(another_file);
  printf("\n");
  return 0;
}