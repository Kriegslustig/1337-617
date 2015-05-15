#include <1337-617.c>

int main (
  int argc,
  char *argv[]
)
{
  char *action;
  if(argc < 2) {
    printf("Go fuck urself\n");
    return 1;
  }
  action = malloc(strlen(argv[1]));
  action = argv[1];

  if(strcmp(action, "1n17") == 0)
  {
    if(argv[2]) {
      _1n17(argv[2]);
      return 0;
    }
    else {
      printf("Failed to init please pass a repo name.");
      return 1;
    }
  } else
  {
    printf("No such action.");
    return 1;
  }
}