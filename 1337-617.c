#include <util.h>

const int commit_hash_length = 40;

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
  strcat(hash, filepath);
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
int g3n_c0_1m17 (char *restrict commit_id)
{
  h45h(commit_hash_length, commit_id);
  while(c3c0_h45h(commit_id))
    h45h(commit_hash_length, commit_id);
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
  return 1;
}