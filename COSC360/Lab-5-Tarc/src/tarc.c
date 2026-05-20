/*Connor Riley
tarc cosc360 plank 3/17/24
program that will tar a given a directory and print it to console
*/
#include "dllist.h"
#include "fields.h"
#include "jrb.h"
#include "jval.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <utime.h>

// every file and directory we hit will have to have the size of name of it in
// little endian and 4 bytes, do this with bit arithmetic, most of the time it wont be readable
// and bug fixing this drove me insane as its te only part that isnt done via fwrite(buf)
void file_name_size_to_hex(char *file) {
  int size = strlen(file);
  char first = (size & 0xFF);
  char second = (size >> 8) & 0xFF;
  char third = (size >> 16) & 0xFF;
  char fourth = (size >> 24) & 0xFF;
  printf("%c%c%c%c", first, second, third, fourth);
}

// If it is a file and it is our first time seeing the file then we will print out the contents of the
// file byte by byte. make sure the file can open and iterate through it.
void print_file_bytes(const char *file_path) {
  FILE *file;
  unsigned char byte;
  file = fopen(file_path, "rb");
  if (file == NULL) {
    printf("no file to read\n");
    return;
  }
  while (fread(&byte, 1, 1, file) == 1) {
    printf("%c", byte);
  }
  fclose(file);
  return;
}
// tarc(current file, initial directory name, inodes, paths, directories)
// 1st 2 arguments are the same just needed for recursive call, inodes, paths and directories prevent dupes
long tarc(const char *filename, const char *initial_filename, JRB inodes, JRB paths, Dllist directories) {
  DIR *d;
  struct dirent *de;
  struct stat buf;
  int exists;
  int file_name_size;                 // length of GRADESCRIPT-EXAMPLES/XXX/FILENAME
  int size_of_initial_directory_name; // this is length of GRADESCRIPT-EXAMPLES/XXX
  int size_of_initial_parent_name;    // this is length of GRADESCRIPT-EXAMPLES/
  int j;                              // iterator for finding the suffix size
  int suffix_size;                    // size of relattive path
  long i;
  char *str;                          // full file name
  char *relative_path_with_null_char; // relative path + null char
  Dllist dll_ptr;                     // dllist iterator

  exists = lstat(filename, &buf); // make sure that the file exists
  if (exists == -1) {             // if not then return error
    perror(filename);
    return -1;
  }
  // this calculates the size of the suffix by finding the difference of the total length
  // and the prefix length via the initial file name arguemnt
  file_name_size = strlen(filename);
  size_of_initial_directory_name = strlen(initial_filename);
  j = size_of_initial_directory_name - 1;

  while (initial_filename[j] != '/') {
    j--;
  }

  j += 1;
  int x = 0;
  suffix_size = file_name_size - j;
  relative_path_with_null_char = (char *)malloc(sizeof(char) * suffix_size + 1);
  for (int k = j; k < file_name_size; k++) {
    relative_path_with_null_char[x] = filename[k];
    x++;
  }
  relative_path_with_null_char[x] = '\0'; // in gradescripts this is 010/filename. used
  // when finding the size of the file name as you need the relative path from the initial directory

  // check if its a directory
  if (S_ISDIR(buf.st_mode)) {
    /*                                                                                     // if its a directory then we print: */
    file_name_size_to_hex(relative_path_with_null_char);                                   // size of filename little endian int
    fwrite(relative_path_with_null_char, strlen(relative_path_with_null_char), 1, stdout); // the name of the file
    fwrite(&buf.st_ino, 8, 1, stdout);                                                     // 8 byte long inode
    fwrite(&buf.st_mode, 4, 1, stdout);                                                    // 4 byte int mode
    fwrite(&buf.st_mtime, 8, 1, stdout);                                                   // 8 byte modification time

    // set str to the directory as we will concatnate it with files in the directory
    str = (char *)malloc(sizeof(char) * (strlen(filename) + 1000));

    d = opendir(filename); // open the directory to loop through it
    if (d == NULL) {
      fprintf(stderr, "Couldn't open \"%s\"\n", filename);
      return -1;
    }
    // for loop to visit each file in the directory and add it to the directories dllist if
    // it isnt a . or .. directory
    for (de = readdir(d); de != NULL; de = readdir(d)) {
      sprintf(str, "%s/%s", filename, de->d_name); // concat for full file name
      exists = lstat(str, &buf);                   // make sure you can open the concatnated file name
      if (exists == -1) {
        perror(filename);
      } else if (strcmp(de->d_name, ".") != 0 && strcmp(de->d_name, "..") != 0) {
        dll_append(directories, new_jval_s(strdup(str))); // add it to the directories dllist
      }
    }

    closedir(d);
    free(str);
    str = NULL;
    // traverse the directories list
    dll_traverse(dll_ptr, directories) {
      exists = lstat(dll_ptr->val.s, &buf);
      if (exists == -1) {
        perror(filename);
        return -1;
      }
      // make sure that the path does exist
      str = realpath(dll_ptr->val.s, NULL);
      if (str == NULL) {
        fprintf(stderr, "realpath failed %s\n", dll_ptr->val.s);
        return -1;
      }
      // make sure we havent visited the file or directory before
      if (jrb_find_str(paths, str) == NULL) {
        jrb_insert_str(paths, str, new_jval_s(strdup(dll_ptr->val.s)));
      } else {
        continue;
      }
      // recursivly call the function on eachh directory or file
      i = tarc(dll_ptr->val.s, initial_filename, inodes, paths, directories);
      if (i == -1) {
        fprintf(stderr, "Error found\n");
        return i;
      }
    }

    return 0;
  } else if (S_ISREG(buf.st_mode)) {
    // if its a file and not a directory we print out the
    // file name size
    // file name
    // inode
    // mod time
    // size of the file
    // print the files contents
    if (jrb_find_dbl(inodes, buf.st_ino) == NULL) {
      file_name_size_to_hex(relative_path_with_null_char);
      fwrite(relative_path_with_null_char, strlen(relative_path_with_null_char), 1, stdout);
      fwrite(&buf.st_ino, 8, 1, stdout);
      fwrite(&buf.st_mode, 4, 1, stdout);
      fwrite(&buf.st_mtime, 8, 1, stdout);
      fwrite(&buf.st_size, 8, 1, stdout);
      print_file_bytes(filename);
      jrb_insert_dbl(inodes, (double)buf.st_ino, new_jval_s(strdup(filename)));
    } else {
      // if we have seen the file then only print
      // file name size
      // file name
      // inode
      file_name_size_to_hex(relative_path_with_null_char);
      fwrite(relative_path_with_null_char, strlen(relative_path_with_null_char), 1, stdout);
      fwrite(&buf.st_ino, 8, 1, stdout);
    }
    return 0;
  }
}

int main(int argc, char **argv) {
  JRB inodes, paths, tmp_in, tmp_pat;
  inodes = make_jrb();
  paths = make_jrb();
  Dllist direct = new_dllist();
  tarc(argv[1], argv[1], inodes, paths, direct);
}
