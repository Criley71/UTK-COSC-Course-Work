/* prsize8.c
   James S. Plank
   Fall, 1996
   CS360 
   Latest tweaks to keep it up to date: February, 2021. */
  
/* The final version of prsize -- pulling the recusion out of the code between
   opendir() and closedir(), by using a dllist. */

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include "jrb.h"
#include "dllist.h"
#include "jval.h"
#include "fields.h"

int compare(Jval v1, Jval v2)
{
  if (v1.l < v2.l) return -1;
  if (v1.l > v2.l) return 1;
  return 0;
}

long get_size(const char *fn, JRB inodes)
{
  DIR *d;                   /* Return value of opendir(). */
  struct dirent *de;        /* Return value of each readdir() call. */
  struct stat buf;          /* The information about each file returned by stat() */
  int exists;               /* Return value of stat on each file. */
  long total_size;          /* The total size of all files. */

  int fn_size;              /* This is the length of fn -- so we can build the filename. */
  char *dir_fn;             /* This will be the filename including the directory. */
  int dir_fn_size;          /* This is the bytes in dir_fn is, in case we need to make it bigger. */
  int sz;

  Dllist directories, tmp;  /* Dllist of directory names, for doing recusion after closing. */

  /* Initialize */

  d = opendir(fn);
  if (d == NULL) {
    perror(fn);
    exit(1);
  }
  total_size = 0;
  directories = new_dllist();

  /* Start building the directory + files.   We'll start by setting dir_fn_size to fn_size+10,
     and we'll make it bigger as we need to.  It will be more efficient to use a number bigger
     than 10 for this, but 10 will let us debug the code if there's a problem. 

     I'm also setting up dir_fn to hold the directory name and a slash. */

  fn_size = strlen(fn);
  dir_fn_size = fn_size + 10;
  dir_fn = (char *) malloc(sizeof(char) * dir_fn_size);
  if (dir_fn == NULL) { perror("malloc dir_fn"); exit(1); }
  strcpy(dir_fn, fn);
  strcat(dir_fn + fn_size, "/");

  /* Run through the directory and run stat() on each file, 
     keeping track of the total size of all of the files.  */

  for (de = readdir(d); de != NULL; de = readdir(d)) {

    /* First, we need to build dir_fn.  First check to see if it's big enough, and if not,
       we'll call realloc() to reallocate space.  Then we put the filename after the
       slash. */

    sz = strlen(de->d_name);
    if (dir_fn_size < fn_size + sz + 2) {    /* The +2 is for the slash and null character. */
      dir_fn_size = fn_size + sz + 10;
      dir_fn = realloc(dir_fn, dir_fn_size);
    }
    strcpy(dir_fn + fn_size + 1, de->d_name);
   
    exists = lstat(dir_fn, &buf);
    if (exists < 0) {
      fprintf(stderr, "Couldn't stat %s\n", dir_fn);
      exit(1);
    } 

    /* Check the inodes tree to check if we've seen this file before.  
       If so, ignore.  If not, then add in its size. */

    if (jrb_find_gen(inodes, new_jval_l(buf.st_ino), compare) == NULL) {
      jrb_insert_gen(inodes, new_jval_l(buf.st_ino), new_jval_i(0), compare);
      total_size += buf.st_size;
    }

    /* Don't make the recursive call, but instead put the directory into the dllist. */

    if (S_ISDIR(buf.st_mode) && strcmp(de->d_name, ".") != 0 && strcmp(de->d_name, "..") != 0) {
      dll_append(directories, new_jval_s(strdup(dir_fn)));
    }
  }

  /* Make the recursive calls after you've closed the directory.  */

  closedir(d);

  dll_traverse(tmp, directories) {
    total_size += get_size(tmp->val.s, inodes);
  }

  /* Clean up.  You need to free the strings inside the dllist, because you
     allocated them with strdup(), and they'll be a memory leak otherwise. */

  dll_traverse(tmp, directories) free(tmp->val.s);
  free_dllist(directories);
  free(dir_fn);

  return total_size;
}

int main()
{
  long total_size;
  JRB inodes;

  inodes = make_jrb();
  total_size = get_size(".", inodes);
  printf("%ld\n", total_size);
  return 0;
}

/*
#include "dllist.h"
#include "fields.h"
#include "jrb.h"
#include "jval.h"
#include <dirent.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

/*
approach,
implement prsize essentially,
once able to traverse all directories (make sure by printing all file names and if we found a directory)
then print info about each file visited inode, mode, mtime, wtf is the hash value
then using fwrite we can get each required thing about the tar file depending on if we have visited it before
*/

/*
int compare(Jval v1, Jval v2)
{
  if (v1.l < v2.l) return -1;
  if (v1.l > v2.l) return 1;
  return 0;
}

long get_size(const char *fn, JRB inodes)
{
  DIR *d;                   /* Return value of opendir().
/*struct dirent *de;        /* Return value of each readdir() call.
/*struct stat buf;          /* The information about each file returned by stat()
/*int exists;               /* Return value of stat on each file.
/*long total_size;          /* The total size of all files.

/*int fn_size;              /* This is the length of fn -- so we can build the filename.
/*char *dir_fn;             /* This will be the filename including the directory.
/* int dir_fn_size;          /* This is the bytes in dir_fn is, in case we need to make it bigger.
/* int sz;

/*  Dllist directories, tmp;  /* Dllist of directory names, for doing recusion after closing.

/* Initialize */

/*  d = opendir(fn);
  if (d == NULL) {
    perror(fn);
    exit(1);
  }
  total_size = 0;
  directories = new_dllist();

  /* Start building the directory + files.   We'll start by setting dir_fn_size to fn_size+10,
     and we'll make it bigger as we need to.  It will be more efficient to use a number bigger
     than 10 for this, but 10 will let us debug the code if there's a problem.

     I'm also setting up dir_fn to hold the directory name and a slash. */

/* fn_size = strlen(fn);
 dir_fn_size = fn_size + 10;
 dir_fn = (char *) malloc(sizeof(char) * dir_fn_size);
 if (dir_fn == NULL) { perror("malloc dir_fn"); exit(1); }
 strcpy(dir_fn, fn);
 strcat(dir_fn + fn_size, "/");

 /* Run through the directory and run stat() on each file,
    keeping track of the total size of all of the files.  */

/* for (de = readdir(d); de != NULL; de = readdir(d)) {

   /* First, we need to build dir_fn.  First check to see if it's big enough, and if not,
      we'll call realloc() to reallocate space.  Then we put the filename after the
      slash. */

/* sz = strlen(de->d_name);
 if (dir_fn_size < fn_size + sz + 2) {    /* The +2 is for the slash and null character. */
/*  dir_fn_size = fn_size + sz + 10;
 /* dir_fn = realloc(dir_fn, dir_fn_size);
}
/*   strcpy(dir_fn + fn_size + 1, de->d_name);

exists = lstat(dir_fn, &buf);
if (exists < 0) {
  fprintf(stderr, "Couldn't stat %s\n", dir_fn);
  exit(1);
}

/* Check the inodes tree to check if we've seen this file before.
   If so, ignore.  If not, then add in its size. */

/*   if (jrb_find_gen(inodes, new_jval_l(buf.st_ino), compare) == NULL) {
     jrb_insert_gen(inodes, new_jval_l(buf.st_ino), new_jval_i(0), compare);
     total_size += buf.st_size;
   }

   /* Don't make the recursive call, but instead put the directory into the dllist. */

/*  if (S_ISDIR(buf.st_mode) && strcmp(de->d_name, ".") != 0 && strcmp(de->d_name, "..") != 0) {
    dll_append(directories, new_jval_s(strdup(dir_fn)));
  }
}

/* Make the recursive calls after you've closed the directory.  */

/*closedir(d);

dll_traverse(tmp, directories) {
  total_size += get_size(tmp->val.s, inodes);
}

/* Clean up.  You need to free the strings inside the dllist, because you
   allocated them with strdup(), and they'll be a memory leak otherwise. */

/*  dll_traverse(tmp, directories) free(tmp->val.s);
  free_dllist(directories);
  free(dir_fn);

  return total_size;
}

int main()
{
  long total_size;
  JRB inodes;

  inodes = make_jrb();
  total_size = get_size(".", inodes);
  printf("%ld\n", total_size);
  return 0;
}

*/

// fp arg is the file path argument, inodes is jrb containing visited inodes will be used to print, actual path jrb containes the actual path check if we have visited the file
/*
void tar_it_up(const char *fp_arg, JRB inode_jrb, JRB actual_path_jrb) {
  struct stat s, s2;
  int s_value; // return value of s
  Dllist tmp;
  JRB path_nodes;
  JRB inode_nodes;
  DIR *d;            // opendir return value
  struct dirent *de; // readdir() return value

  int file_name_size;     // length of file name
  char *dir_file_name;    // directory + file name
  int dir_file_name_size; // size of dir + file name
  int resize_val;         // if we need to resize

  int inode;

  s_value = lstat(fp_arg, &s);
  if (s_value < 0) {
    fprintf(stderr, "Path name didnt open: %s\n", fp_arg);
    exit(1);
  } else {
   // printf("cool\n");
  }

  // S_ISREG(stat.st_mode) if 0 then its a directory, other  wise its a file
  if (S_ISREG(s.st_mode) != 0) { // we found a file
//    printf("coolx2\n");
    char path[PATH_MAX + 1];          // path to file
    char *p = realpath(fp_arg, path); // gives the absolute path not relative
    printf("%s\n", p);
    path_nodes = jrb_find_str(actual_path_jrb, p); // check if we have visited this yet
    printf("%s\n", actual_path_jrb);

    if (path_nodes == NULL) {                      // we havent visited yet, insert it and then check inode
      // I really only use a jrb over a dllist because we have the find function
      // therefore the value isnt really needed so its just a void NULL.
      jrb_insert_str(actual_path_jrb, strdup(p), new_jval_i(s.st_mode));
      if (stat(fp_arg, &s2) == 0) { // make sure path name is able to open
        inode = s2.st_ino;
        printf("%ld\n", inode);
        inode_nodes = jrb_find_int(inode_jrb, inode);
        if (inode_nodes == NULL) {
          jrb_insert_int(inode_jrb, inode, new_jval_s(strdup(fp_arg)));

          stat("tarf", &s2);
          inode = s2.st_ino;
          jrb_insert_int(inode_jrb, inode, new_jval_s(strdup("tarf")));
          jrb_insert_str(actual_path_jrb, "tarf", new_jval_i(s2.st_mode));

          stat("tarc", &s2);
          inode = s2.st_ino;
          jrb_insert_int(inode_jrb, inode, new_jval_s(strdup("tarc")));
          jrb_insert_str(actual_path_jrb, "tarc", new_jval_i(s2.st_mode));

          stat("mrd", &s2);
          inode = s2.st_ino;
          jrb_insert_int(inode_jrb, inode, new_jval_s(strdup("mrd")));
          jrb_insert_str(actual_path_jrb, "mrd", new_jval_i(s2.st_mode));

        }

    // tar_it_up("tarf", inode_jrb, actual_path_jrb);
    // tar_it_up("mrd", inode_jrb, actual_path_jrb);
    //  tar_it_up("tarc", inode_jrb, actual_path_jrb);


      }
    }

  } else if (S_ISREG(s.st_mode) == 0) { // we found a directory
    d = opendir(fp_arg);                // make sure it opens
    if (d == NULL) {
      perror(fp_arg);
      exit(1);
    } else {
    //  printf("coolx3\n");
    }
    // get the file name length and append / onto end if needed
    file_name_size = strlen(fp_arg);
    dir_file_name_size = file_name_size + 10;
    dir_file_name = (char *)malloc(sizeof(char) * dir_file_name_size);
    if (dir_file_name == NULL) {
      perror("malloc dir_file_name\n");
      exit(1);
    }
    strcpy(dir_file_name, fp_arg);

    if (*(dir_file_name + (file_name_size - 1)) != '/') {
      strcat(dir_file_name + file_name_size, "/");
    }
    printf("%s\n", dir_file_name);

  } else {
    printf("why %ld\n", S_ISREG(s.st_mode));
  }
}
int main(int argc, char *argv[]) {
  JRB inodes = make_jrb();
  JRB path = make_jrb();
  JRB bruh = make_jrb();
  tar_it_up(argv[1], inodes, path);
  jrb_traverse(bruh, inodes){
    printf("%s : %d\n",  bruh->val.s, bruh->key.i);
  }
  jrb_traverse(bruh, path){
    printf("%s : %d\n", bruh->key.s, bruh->val.i);
  }

}
*/

void process_file(char *file_path_arg, JRB inodes_jrb, JRB actual_paths_jrb)
{
}

void process_dir(char *file_path_arg, JRB inodes_jrb, JRB actual_paths_jrb)
{
}

int compare(Jval v1, Jval v2)
{
  if (v1.l < v2.l)
    return -1;
  if (v1.l > v2.l)
    return 1;
  return 0;
}

long get_info(char *file_name, JRB inodes)
{
  DIR *d;            // opendir return val
  struct dirent *de; // return val of each readdir
  struct stat s, s2; // info about each file
  int exists;        // stat return value
  long total_size;   // total size of all files, not needed

  int fn_size;     // length of the name of the file
  char *dir_fn;    // file name with the directory too
  int dir_fn_size; // size of dir_fn name
  int sz;          // resize value

  Dllist directories, tmp; // list of directory names, used fo recursion
  d = opendir(file_name);
  if (d == NULL)
  {
    perror(file_name);
    exit(1);
  }
  total_size = 0;
  directories = new_dllist();

  // begin to build directory + file.
  fn_size = strlen(file_name);
  dir_fn_size = fn_size + 10;
  dir_fn = (char *)malloc(sizeof(char) * dir_fn_size);
  if (dir_fn == NULL)
  {
    perror("malloc dir_fn");
    exit(1);
  }
  strcpy(dir_fn, file_name);
  strcat(dir_fn + fn_size, "/");

  for (de = readdir(d); de != NULL; de = readdir(d))
  {
   // printf("\n");
    sz = strlen(de->d_name);
 //   printf("de->d_name : %s\n", de->d_name);
    stat(dir_fn, &s2);

    if (dir_fn_size < fn_size + sz)
    {
      dir_fn_size = fn_size + sz + 10;
      dir_fn = realloc(dir_fn, dir_fn_size);
    }
    strcpy(dir_fn + fn_size + 1, de->d_name);

    exists = lstat(dir_fn, &s);
    if (exists < 0)
    {
      fprintf(stderr, "Couldn't stat %s\n", dir_fn);
      exit(1);
    }
  //  printf("--------------------------------------\n we are on this file this loop: %s\n", dir_fn);
    if (jrb_find_gen(inodes, new_jval_l(s.st_ino), compare) == NULL || strcmp(de->d_name, ".") != 0 && strcmp(de->d_name, "..") != 0)
    {
    //  printf("NEW current : %s inode : %ld de->d_name : %s\n ", dir_fn, s.st_ino, de->d_name);
    printf("%s | %ld | %lo | %ld\n", dir_fn, s.st_ino, s.st_mode, s.st_mtime );
    }
    else
    {
     // printf("OLD current : %s inode : %ld\n\n", dir_fn, s.st_ino);
    }
    /* Check the inodes tree to check if we've seen this file before.
       If so, ignore.  If not, then add in its size. */

    if (jrb_find_gen(inodes, new_jval_l(s.st_ino), compare) == NULL || (strcmp(de->d_name, ".") != 0 && strcmp(de->d_name, "..")))
    {
     // printf(" adding inode to tree : %ld file name : %s\n\n", s.st_ino, dir_fn);
      printf("adding %s\n", de->d_name);
      jrb_insert_gen(inodes, new_jval_l(s.st_ino), new_jval_s(strdup(dir_fn)), compare);
      total_size += s.st_size;
    }

    /* Don't make the recursive call, but instead put the directory into the dllist. */

    if (S_ISDIR(s.st_mode) && strcmp(de->d_name, ".") != 0 && strcmp(de->d_name, "..") != 0)
    {
      //printf("  apending %s to directories list\n", dir_fn);
      dll_append(directories, new_jval_s(strdup(dir_fn)));
    }
  }

  /* Make the recursive calls after you've closed the directory.  */
//  printf("file_name = %s : file inode = %ld : file mode = %ld\n", dir_fn, s.st_ino, s.st_mode);
  closedir(d);

  dll_traverse(tmp, directories)
  {
    //printf("in the directory traverse at file : %s\n", tmp->val.s);
    total_size += get_info(tmp->val.s, inodes);
  }
  dll_traverse(tmp, directories) free(tmp->val.s);
  free_dllist(directories);
  free(dir_fn);

  return total_size;
}

void list_files(const char* dir_name){
  DIR* dir = opendir(dir_name);
  if(dir == NULL){
    //perror(dir);
    return;
  }
  struct stat s;

  printf("reading files in: %s\n", dir_name);

  struct dirent* entity;
  entity = readdir(dir);
  while(entity != NULL){
    stat(entity->d_name, &s);
    printf("%s || /%s | %d \n", entity->d_name,dir_name, s.st_ino);
    if(entity->d_type  && strcmp(entity->d_name, ".") != 0 && strcmp(entity->d_name, "..") != 0){
      char path[100] = { 0 };
      strcat(path, dir_name);
      strcat(path, "/");
      strcat(path, entity->d_name);
      
      list_files(path);
    }
    entity = readdir(dir);
  }
}
int main(int argc, char **argv)
{
  long total_size;
  JRB inodes, tmp;

  inodes = make_jrb();
  total_size = get_info("./Gradescript-Examples/002", inodes);
  printf("%ld\n", total_size);
  printf("----------------------------------------\n");
  jrb_traverse(tmp, inodes){
    
    printf("%s | %ld\n",tmp->val.s,  tmp->key.v);
  }
  printf("-------------------------------------------------\n");
  list_files("./Gradescript-Examples/002");
  return 0;
  
}
*/