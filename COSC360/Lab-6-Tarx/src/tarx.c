/*
Connor Riley
3/26/2023
tarx, reads in a tarfile from stdin and recreates the directory
that emulates calling tarx on it.
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
#include <unistd.h>
#include <utime.h>

/*since files will be added after the directory is made, so the modtime will have
changed and possibly have a different permissions, have directories stored in jrb to then
at the end go through the directories and change mtime and permissions  */
struct Directory {
  char *dir_name;    // name
  long dir_inode;    // inode
  int dir_mode;      // mode
  long dir_mod_time; // mtime
} direct;

/*tar files store data in 4 bytes and 8 byte sets in little endian, this converts the little endian hex
and returns as an int or long decimal number.*/
long four_byte_hex(char *hex) {
  char buf[9];
  sprintf(buf + 2 * 0, "%02x", hex[3] & 0xff);
  sprintf(buf + 2 * 1, "%02x", hex[2] & 0xff);
  sprintf(buf + 2 * 2, "%02x", hex[1] & 0xff);
  sprintf(buf + 2 * 3, "%02x", hex[0] & 0xff);
  buf[8] = '\0';
  long val = strtol(buf, NULL, 16);
  return val;
}

long eight_byte_hex(char *hex) {
  char buf[17];
  sprintf(buf + 2 * 0, "%02x", hex[7] & 0xff);
  sprintf(buf + 2 * 1, "%02x", hex[6] & 0xff);
  sprintf(buf + 2 * 2, "%02x", hex[5] & 0xff);
  sprintf(buf + 2 * 3, "%02x", hex[4] & 0xff);
  sprintf(buf + 2 * 4, "%02x", hex[3] & 0xff);
  sprintf(buf + 2 * 5, "%02x", hex[2] & 0xff);
  sprintf(buf + 2 * 6, "%02x", hex[1] & 0xff);
  sprintf(buf + 2 * 7, "%02x", hex[0] & 0xff);
  buf[16] = '\0';
  long val = strtol(buf, NULL, 16);
  return val;
}
/*this reads in the tarfile as standard input and begins to translate the data to create the files
it reads in the size of the filesname, filename, inode, mode, size of file, and contents of the file
It does checks to see if the file is hard linked or a directory and will perform different tasks */
void tarx(JRB files, JRB directories, Dllist dir_names) {
  while (!feof(stdin)) {
    if (feof(stdin)) {
      break;
    }
    // 4 bytes of filename size
    char filename_size[5];
    fread(filename_size, sizeof(int), 1, stdin);
    filename_size[4] = '\0';
    long filename_num = four_byte_hex(filename_size);
    if (feof(stdin)) {
      break;
    }
    // set filename to filename_size + 1 for null char
    char filename[filename_num + 1];
    fread(filename, filename_num, 1, stdin);
    filename[filename_num] = '\0';
    if (feof(stdin)) {
      fprintf(stderr, "it no worky\n");
      break;
    }
    // get inode, 8 byte long little endian
    char inode[9];
    fread(inode, sizeof(long), 1, stdin);
    inode[8] = '\0';
    long inode_num = eight_byte_hex(inode);
    if (feof(stdin)) {
      fprintf(stderr, "it no worky\n");
      break;
    }
    // check to see if we have seen inode, if so then its a hard link and we need to link them
    JRB tmp;
    long file_size;
    tmp = jrb_find_dbl(files, inode_num);
    struct stat s;
    if (tmp != NULL) {

      if (stat(tmp->val.s, &s)) {
        fprintf(stderr, "file couldnt open with shared inode\n");
        exit(-1);
      }
      // link original inode to the new file.
      if (link(tmp->val.s, filename) == 0) {

      } else {
        perror("error");
      }
      chmod(filename, 0777); // give all permissions to edit the file
      long file_size = s.st_size;
      // since its hardlinked we have to find the file contents and inode from the file it is linked to.
      // read in the other file and write it to the new file
      char file_bytes_str[file_size];
      FILE *fptr;
      fptr = fopen(tmp->val.s, "r");
      if (fptr == NULL) {
        fprintf(stderr, "it no worky\n");
        exit(-1);
      }
      fread(file_bytes_str, sizeof(char) * file_size, 1, fptr);
      fclose(fptr);
      fptr = fopen(filename, "w");
      if (fptr == NULL) {
        fprintf(stderr, "it no worky\n");
        exit(-1);
      }
      // change mod time of both and the mode of the new file
      fwrite(file_bytes_str, sizeof(char) * file_size, 1, fptr);
      fclose(fptr);
      chmod(filename, s.st_mode);
      struct utimbuf time;
      time.modtime = s.st_mtime;
      time.actime = s.st_atime;
      utime(filename, &time);

      continue;
    }
    // get the mode, 4 byte int little endian
    char mode[5];
    fread(mode, sizeof(int), 1, stdin);
    mode[4] = '\0';
    long mode_num = four_byte_hex(mode);
    if (feof(stdin)) {
      fprintf(stderr, "it no worky\n");
      break;
    }
    // mod time 8 byte long little endian
    char mod_time[9];
    fread(mod_time, sizeof(long), 1, stdin);
    mod_time[8] = '\0';
    unsigned long mod_time_num = eight_byte_hex(mod_time);
    // check if the mode is a directory, i brute forced it lmao 💪
    if (mode_num == 16877 || mode_num == 16749 || mode_num == 16725 || mode_num == 16709 || mode_num == 16837 || mode_num == 16757 ||
        mode_num == 16733 || mode_num == 16869 || mode_num == 16717 || mode_num == 16885 || mode_num == 16845 || mode_num == 16741 ||
        mode_num == 16893 || mode_num == 16853 || mode_num == 16765 || mode_num == 16861) {
      // make the new directory with all permissions and make a Directory object and store it in jrb
      mkdir(filename, 0777);
      struct Directory *d = malloc(sizeof(struct Directory));
      d->dir_name = strdup(filename);
      d->dir_inode = inode_num;
      d->dir_mode = mode_num;
      d->dir_mod_time = mod_time_num;

      if (jrb_find_dbl(directories, inode_num) == NULL) {
        jrb_insert_dbl(directories, inode_num, new_jval_v(d));
        dll_append(dir_names, new_jval_s(strdup(filename)));
      }
      if (feof(stdin)) {
        break;
      }
      continue;
    }

    if (feof(stdin)) {
      fprintf(stderr, "it no worky\n");
      break;
    }
    // get the size of the file
    char file_size_str[9];
    if (!fread(file_size_str, sizeof(long), 1, stdin)) {
      fprintf(stderr, "it no worky");
      exit(-1);
    }
    file_size_str[8] = '\0';
    file_size = eight_byte_hex(file_size_str);
    if (feof(stdin)) {
      break;
    }
    // read in the contents of the file and write it to the file
    char file_bytes_str[file_size];
    if (!fread(file_bytes_str, file_size, 1, stdin)) {
      fprintf(stderr, "it no worky");
      exit(-1);
    }
    FILE *fptr;
    fptr = fopen(filename, "w");
    if (fptr == NULL) {
      printf("error opening file %s\n", filename);
      perror("error");
      exit(-1);
    }
    fwrite(file_bytes_str, sizeof(char) * file_size, 1, fptr);
    fclose(fptr);
    //set the permissions and the mod time
    chmod(filename, mode_num);
    struct utimbuf time;
    time.modtime = mod_time_num;
    time.actime = 0;
    utime(filename, &time);
    if (jrb_find_dbl(files, inode_num) == NULL) {
      jrb_insert_dbl(files, inode_num, new_jval_s(strdup(filename)));
    }

    if (feof(stdin)) {
      break;
    }
  }
}
// for directories, set permission to 777, make a jrb with inode key and a value of direectory
// struct. once all files have been made go back through the jrb and set permissions and times

int main(int argc, char *argv[]) {
  struct stat s;
  JRB files = make_jrb();
  JRB directories = make_jrb();
  JRB ptr;
  Dllist dir_names = new_dllist();
  Dllist tmp;
  tarx(files, directories, dir_names);
  struct Directory *d;
  jrb_traverse(ptr, directories) {
    //iterate through the directories 
    d = (struct Directory *)ptr->val.v;
    if (stat(d->dir_name, &s)) {
      fprintf(stderr, "couldnt stat dir %s\n", d->dir_name);
    }
    //change the permissions and mod times 
    chmod(d->dir_name, d->dir_mode);
    struct utimbuf time;
    time.modtime = d->dir_mod_time;
    time.actime = s.st_atime;
    utime(d->dir_name, &time);
  }
}