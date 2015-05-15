#include <1337-617.c>

int main (
  int argc,
  char *argv[]
)
{
  int returnValue = 1;
  char *action;
  if( argc < 2 )
  {
    printf("Go fuck urself\n");
    return 1;
  }
  action = (char *) malloc(sizeof(argv[1]));
  strcpy(action, argv[1]);

  if(strcmp(action, "1n17") == 0)
  {
    if(argv[2])
    {
      _1n17(argv[2]);
      returnValue = 0;
    }
    else
    {
      printf("Failed to init please pass a repo name.");
      returnValue = 1;
    }
  } else
  {
    printf("No such action.");
  }
  free(action);
  return returnValue;
}