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

  if( strcmp(action, "1n17" ) == 0)
  {
    if( (returnValue = has_n_args(argv, 2, "Failed to init please pass a repo name.")) )
    {
      returnValue = _1n17(argv[2]);
    }
  } else if (strcmp(action, "4d") == 0)
  {
    if( (returnValue = has_n_args(argv, 2, "Need a file or directory.")) )
    {
      returnValue = _4d(argv[2]);
    }
  } else
  {
    printf("No such action.");
  }
  free(action);
  return returnValue;
}