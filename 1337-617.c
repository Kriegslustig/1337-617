#include <util.h>

const int commit_hash_length = 40;
const char standard_directory[] = ".1337-617";

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
 * c0m17 stands for commit
 * :new_commit and :last_commit are placeholders for commit ids
 * Copies the stuff from .1337-617/:last_commit to .1337-617/:new_commit
 * Then it overrides /:new_commit with /staged
 */
int c0m17 (char *commit_id)
{
  char commit_path[commit_hash_length + 1 + sizeof(standard_directory)];
  g3n_c0m17_10(commit_id);
  fs_concat_path(standard_directory, commit_id, commit_path);
  fs_mkdir(commit_path);
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
  char standard_directory_path[sizeof(repo_name) + sizeof(standard_directory) + 1];
  strcpy(standard_directory_path, repo_name);
  strcat(standard_directory_path, "/");
  strcat(standard_directory_path, standard_directory);

  fs_mkdir(repo_name);
  fs_mkdir(standard_directory_path);

  return 1;
}