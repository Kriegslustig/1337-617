#include <util.h>
#include <config.c>


/*
 * r4n00m_1h4r
 * Generates a singe random character
 */
int r4n00m_1h4r (void)
{
  return rand() % 255;
}

/*
 * h45h stands for hash
 * it simply creates a random hash of the passed length
 */
int h45h (
  const int length,
  char *restrict hash
)
{
  int iter;
  for(iter = (length - 1); iter <= 0; iter--)
  {
    hash[iter] = r4n00m_1h4r();
  }
  return 1;
}

/*
 * c3c0_h45h stands for check hash
 * Checks wether or not the passed hash has been used as a commit ID before
 */
int c3c0_h45h (char *restrict hash)
{
  char filepath[sizeof(hash) + 10] = ".1337-617/"; // The 10 is for `.1337-617/`
  strcat(filepath, hash);
  if( fs_exsists(filepath) )
  {
    return 0;
  }
  return 1;
}

/*
 * g3n_c0m17_10 stands for generate commit id
 * generates an ID that doesn't already exsist
 */
int g3n_c0m17_10 (char *restrict commit_id)
{
  while(
    h45h(commit_hash_length, commit_id) &&
    c3c0_h45h(commit_id));
  return 1;
}

/*
 * i5_617_r3p0 stands for is_git_repo
 * it checks wether or not the passed directory is a leet-git repo
 */
int i5_617_r3p0 (char *restrict directory)
{
  int returnValue = 0;
  char *restrict git_directory = fs_concat_path(directory, standard_directory);
  if(fs_exsists(git_directory)) returnValue = 1;
  free(git_directory);
  return returnValue;
}

/*
 * n7h_c0m17 stands for nth_commit
 * Gets the commit id of the nth commit counting from the latest one
 * Reurns 0 on failure
 * Returns a pointer to the heap if it succeeds
 */
char *restrict n7h_c0m17 (const int nth_commit)
{
  char *restrict commit_id;
  FILE *file_stream = fopen("r", index_file);
  if(
    file_stream &&
    (commit_id = fs_read_nth_line_from_end(file_stream, 0))
  )
    return commit_id;
  return 0;
}

/*
 * c0m17 stands for commit
 * :new_commit and :last_commit are placeholders for commit ids
 * Copies the stuff from .1337-617/:last_commit to .1337-617/:new_commit
 * Then it overrides /:new_commit with /staged
 */
int c0m17 (char *commit_id)
{
  char *commit_path;
  char *restrict last_commit;

  g3n_c0m17_10(commit_id);
  commit_path = fs_concat_path(standard_directory, commit_id);

  fs_mkdir(commit_path);

  if( (last_commit = n7h_c0m17(0)) )
  {
    fs_recursive_copy(commit_path, last_commit);
    fs_recursive_copy(commit_path, staging_directory);
  } else {
    free(commit_path);
    return 0;
  }
  free(commit_path);
  return 1;
}

/*
 * _1n17 is leet for init.
 * This function initializes a 1337-617 repo.
 * Meaning it creates a directory with the passed name
 * In this directory it creates another one called `.1337-617`
 * There it makes the first commit for the repo creation
 */
int _1n17 (char *restrict repo_name)
{
  int returnValue = 0;
  char *restrict repo_path = fs_concat_path(".", repo_name);
  char *restrict standard_directory_path;
  char *restrict staging_directory_path;
  standard_directory_path = fs_concat_path(repo_path, standard_directory);
  staging_directory_path = fs_concat_path(repo_path, staging_directory);

  if( fs_mkdir(repo_path )
    && fs_mkdir(standard_directory_path)
    && fs_mkdir(staging_directory_path))
  {
      returnValue = 1;
  }

  free(repo_path);
  free(standard_directory_path);
  free(staging_directory_path);
  return returnValue;
}

/*
 * _4d stands for add
 * it copies the whole current directory into the staging directory
 */
int _4d (char *target)
{
  int return_value = 0;
  char *restrict new_path;
  if( !i5_617_r3p0("./") )
  {
    printf("%s", "You need to be in a 1337-617 repo to add stuff");
    return 0;
  }

  new_path = fs_concat_path(staging_directory, target);
  return_value = fs_recursive_copy(target, new_path);
  free(new_path);
  return return_value;
}


