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
#include <unistd.h>

struct Directory {
  char *dir_name;
  long dir_inode;
  int dir_mode;
  long dir_mod_time;
} direct;

// alwasys have it return the new position of the tar file pointer to keep offset
int size_of_file_name(char *filename, int offset) {
  char buf[4];
  char hex_buf[9];
  long file_name_size;
  FILE *fp;
  fp = fopen(filename, "rb");
  if (fp == NULL) {
    fprintf(stderr, "couldnt open file\n");
    return 0;
  }
  fseek(fp, offset, SEEK_SET);
  fread(buf, sizeof(char), 4, fp);
  sprintf(hex_buf + 2 * 0, "%02x", buf[3] & 0xff);
  sprintf(hex_buf + 2 * 1, "%02x", buf[2] & 0xff);
  sprintf(hex_buf + 2 * 2, "%02x", buf[1] & 0xff);
  sprintf(hex_buf + 2 * 3, "%02x", buf[0] & 0xff);
  hex_buf[8] = '\0';
  file_name_size = strtol(hex_buf, NULL, 16);
  printf("file name size: %ld\n", file_name_size);
  fclose(fp);
  return file_name_size;
}
// offset          4 + n(file_name) + 8 +     4 +    8 +        8 +   x(bytes)
//       name_length + n +            inode + mode + mod time + size + bytes
//  returns the length of file to keep offset position
int file_name(char *tar_file, int size, int offset) {
  char buf[size + 1];
  char *str = (char *)malloc((sizeof(char) * size));
  FILE *fp;
  fp = fopen(tar_file, "rb");
  if (fp == NULL) {
    printf("Couldnt open file %s\n", tar_file);
    return -1;
  }
  int pos = offset;
  fseek(fp, pos, SEEK_SET);
  fread(buf, sizeof(char), sizeof(buf), fp);
  buf[size] = '\0';
  printf("file name : %s\n", buf);
  // mkdir(str, 0777);
  // printf("offset : %d\n", pos+sizeof(str));
  pos += 4;
  return pos;
  // mknod(str, buf.st_mode, buf.st_dev) == 0)
}

unsigned long file_inode(char *tar_file, int offset) {
  char buf[8];
  char hex_buf[17];
  unsigned long inode;
  FILE *fp;
  fp = fopen(tar_file, "rb");
  if (fp == NULL) {
    fprintf(stderr, "couldnt open file\n");
    return -1;
  }
  printf("inode offset %d\n", offset);
  fseek(fp, offset, SEEK_SET);
  fread(buf, sizeof(char), sizeof(buf), fp);
  printf("inode buf: %s\n", buf);
  sprintf(hex_buf + 2 * 0, "%02x", buf[7] & 0xff);
  sprintf(hex_buf + 2 * 1, "%02x", buf[6] & 0xff);
  sprintf(hex_buf + 2 * 2, "%02x", buf[5] & 0xff);
  sprintf(hex_buf + 2 * 3, "%02x", buf[4] & 0xff);
  sprintf(hex_buf + 2 * 4, "%02x", buf[3] & 0xff);
  sprintf(hex_buf + 2 * 5, "%02x", buf[2] & 0xff);
  sprintf(hex_buf + 2 * 6, "%02x", buf[1] & 0xff);
  sprintf(hex_buf + 2 * 7, "%02x", buf[0] & 0xff);
  hex_buf[16] = '\0';
  for (int i = 0; i < sizeof(buf); i++) {
    printf("%02x ", buf[i] & 0xff);
  }
  printf("\n");
  inode = strtoul(hex_buf, NULL, 16);
  printf("%lu\n", inode);
  fclose(fp);
  return inode;
}

int file_mode(char *tar_file, int file_name_size) {
  char buf[4];
  char hex_buf[9];
  long mode;
  FILE *fp;
  fp = fopen(tar_file, "rb");
  if (fp == NULL) {
    fprintf(stderr, "couldnt open file\n");
    return -1;
  }
  int offset = 4 + file_name_size + 8;
  printf("mode offset %d\n", file_name_size);
  fseek(fp, file_name_size, SEEK_SET);
  fread(buf, sizeof(char), sizeof(buf), fp);
  printf("mode buf: %s\n", buf);
  sprintf(hex_buf + 2 * 0, "%02x", buf[3] & 0xff);
  sprintf(hex_buf + 2 * 1, "%02x", buf[2] & 0xff);
  sprintf(hex_buf + 2 * 2, "%02x", buf[1] & 0xff);
  sprintf(hex_buf + 2 * 3, "%02x", buf[0] & 0xff);
  hex_buf[8] = '\0';
  printf("mode hex: ");
  for (int i = 0; i < sizeof(buf); i++) {
    printf("%02x ", buf[i] & 0xff);
  }
  printf("\n");
  mode = strtol(hex_buf, NULL, 16);
  printf("mode oct: %o\n", mode); // 40755 (0x000041ed) (%d = 16877)is dir
  fclose(fp);
  return mode;
}

long file_last_mod_time(char *tar_file, int file_name_size) {
  char buf[8];
  char hex_buf[17];
  long mod_time;
  FILE *fp;
  fp = fopen(tar_file, "rb");
  if (fp == NULL) {
    fprintf(stderr, "couldnt open file\n");
    return -1;
  }
  int offset = file_name_size;
  fseek(fp, offset, SEEK_SET);
  printf("mod offset %d\n", offset);
  fread(buf, sizeof(char), sizeof(buf), fp);
  printf("mod time buf %s\n", buf);
  sprintf(hex_buf + 2 * 0, "%02x", buf[7] & 0xff);
  sprintf(hex_buf + 2 * 1, "%02x", buf[6] & 0xff);
  sprintf(hex_buf + 2 * 2, "%02x", buf[5] & 0xff);
  sprintf(hex_buf + 2 * 3, "%02x", buf[4] & 0xff);
  sprintf(hex_buf + 2 * 4, "%02x", buf[3] & 0xff);
  sprintf(hex_buf + 2 * 5, "%02x", buf[2] & 0xff);
  sprintf(hex_buf + 2 * 6, "%02x", buf[1] & 0xff);
  sprintf(hex_buf + 2 * 7, "%02x", buf[0] & 0xff);
  hex_buf[16] = '\0';
  for (int i = 0; i < sizeof(buf); i++) {
    printf("%02x ", buf[i] & 0xff);
  }
  printf("\n");
  mod_time = strtol(hex_buf, NULL, 16);
  printf("mod time: %ld\n", mod_time);
  return mod_time;
}

void print_dir(char *path_directory) {
  char buf[4];
  char hex_buf[2 * sizeof(buf) + 1];
  int offset = 0;
  FILE *fp;
  fp = fopen(path_directory, "rb");
  if (fp == NULL) {
    printf("Couldnt open file %s\n", path_directory);
    return;
  }
  int file_name_size = size_of_file_name(path_directory, 0);
  offset += 4;
  int filename_length = file_name(path_directory, file_name_size, offset);
  offset += file_name_size;
  unsigned long inode = file_inode(path_directory, offset);
  offset += 8;
  int filemode = file_mode(path_directory, offset);
  offset += 4;
  long file_mod_time = file_last_mod_time(path_directory, offset);
  offset += 8;
  if (filemode == 16877) {
    printf("was a dir, total offset is: ");
    printf("%d\n", offset);
  }
  printf("-----------------------------------\n");
  int file_name_size2 = size_of_file_name(path_directory, offset);
  offset += 4;
  printf("offset after 2nd file name size: %d\n", offset);
  int filename_length2 = file_name(path_directory, file_name_size2, offset);
  offset += file_name_size2;
  printf("offset after 2nd file name: %d\n", offset);
  unsigned long inode2 = file_inode(path_directory, offset);
  offset += 8;
  int filemode2 = file_mode(path_directory, offset);
  offset += 4;
  long file_mod_time2 = file_last_mod_time(path_directory, offset);
  offset += 8;
  if (filemode2 == 16877) {
    printf("was a dir, total offset is: ");
    printf("%d\n", offset);
  }
}

void freadtest(char *path) {
  struct stat buf;
  if (stat(path, &buf)) {
    printf("no open\n");
    return;
  }
  fread(&buf, sizeof(struct stat), 1, stdin);
  printf("%ld\n", buf.st_ino);
}
// file first time 4(n size) + n(name) + 8(inode) + 4(mode) + 8(mod time) + 8(f size) + n(bytes) = 32 + n + b;
// file seen before 4(n size) + n(name) + 8(inode) = 12 + n;
// dir  4(n size) + n(name) + 8(inode) + 4(mode) + 8(mod time) = 24 + n;
int file_name_size(char *file) {
  char filename_size[5];
  fread(filename_size, sizeof(int), 1, stdin);
  filename_size[4] = '\0';

  char hex_buf[9];
  sprintf(hex_buf + 2 * 0, "%02x", filename_size[3] & 0xff);
  sprintf(hex_buf + 2 * 1, "%02x", filename_size[2] & 0xff);
  sprintf(hex_buf + 2 * 2, "%02x", filename_size[1] & 0xff);
  sprintf(hex_buf + 2 * 3, "%02x", filename_size[0] & 0xff);
  hex_buf[8] = '\0';
  printf("hex name size: %s\n", hex_buf);
  long dec_val = strtol(hex_buf, NULL, 16);
  printf("%d\n", dec_val);
  return dec_val;
}

void tarx() {
  struct stat buf;
  char *str = (char *)malloc(sizeof(char) * 258);
  JRB inodes = make_jrb();
  JRB paths = make_jrb();

  // get file name size
  char filename_size[5];
  fread(filename_size, sizeof(int), 1, stdin);
  filename_size[4] = '\0';

  // convert to hex then into base 10 long
  char hex_buf[9];
  sprintf(hex_buf + 2 * 0, "%02x", filename_size[3] & 0xff);
  sprintf(hex_buf + 2 * 1, "%02x", filename_size[2] & 0xff);
  sprintf(hex_buf + 2 * 2, "%02x", filename_size[1] & 0xff);
  sprintf(hex_buf + 2 * 3, "%02x", filename_size[0] & 0xff);
  hex_buf[8] = '\0';
  printf("hex name size: %s\n", hex_buf);
  int dec_val = strtol(hex_buf, NULL, 16);
  printf("%d\n", dec_val);

  // get file name
  char file_name[dec_val + 1];
  file_name[dec_val] = '\0';
  fread(file_name, dec_val, 1, stdin);
  printf("%s\n", file_name);

  // now get inode

  struct utimbuf time;
  fread(&buf, sizeof(struct stat), 1, stdin);
  time.modtime = buf.st_mtime;
  printf("%ld\n", time.modtime);

  /*
   long inode;
  char inode_str[9];
  char inode_hex[17];
  fread(inode_str, sizeof(long), 1, stdin);
  inode_str[8] = '\0';
  sprintf(inode_hex + 2 * 0, "%02x", inode_str[7] & 0xff);
  sprintf(inode_hex + 2 * 1, "%02x", inode_str[6] & 0xff);
  sprintf(inode_hex + 2 * 2, "%02x", inode_str[5] & 0xff);
  sprintf(inode_hex + 2 * 3, "%02x", inode_str[4] & 0xff);
  sprintf(inode_hex + 2 * 4, "%02x", inode_str[3] & 0xff);
  sprintf(inode_hex + 2 * 5, "%02x", inode_str[2] & 0xff);
  sprintf(inode_hex + 2 * 6, "%02x", inode_str[1] & 0xff);
  sprintf(inode_hex + 2 * 7, "%02x", inode_str[0] & 0xff);
  inode_hex[16] = '\0';
  printf("%s\n", inode_hex);
  inode = strtol(inode_hex, NULL, 16);
  printf("%ld\n", inode);

  //mode
  int mode;
  char mode_str[5];
  char mode_hex[9];
  fread(mode_str, sizeof(int), 1, stdin);
  mode_str[8] = '\0';
  */
}

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
  //printf("buf little endian %s\n", hex);
 // printf("buf big endian %s\n", buf);
  buf[16] = '\0';
  long val = strtol(buf, NULL, 16);
  //printf("buf final: %ld\n", val);
  return val;
}

void tarx(JRB files, JRB directories, Dllist dir_names) {
  int i = 0;
  while (!feof(stdin)) {
    if (feof(stdin)) {
      break;
    }
    char filename_size[5];
    fread(filename_size, sizeof(int), 1, stdin);
    filename_size[4] = '\0';
    long filename_num = four_byte_hex(filename_size);
    if (feof(stdin)) {
      break;
    }

    char filename[filename_num + 1];
    fread(filename, filename_num, 1, stdin);
    filename[filename_num] = '\0';
    if (feof(stdin)) {
      break;
    }

    struct stat buf;
    char inode[9];
    fread(inode, sizeof(long), 1, stdin);
    inode[8] = '\0';
    long inode_num = eight_byte_hex(inode);
    if (feof(stdin)) {
      break;
    }

    JRB tmp;
    long file_size;
    tmp = jrb_find_dbl(files, inode_num);
    struct stat s;
    if (tmp != NULL) {
     // printf("file is duped inode\n"); //<- add it here, if inode exist then stat to get st_size and read in other file, else do what it is now, move to after inode read in
      if (stat(tmp->val.s, &s)) {
        fprintf(stderr, "file couldnt open with shared inode\n");
        exit(-1);
      }
      if(link(tmp->val.s, filename) == 0){
      //  printf("%s linked to %s\n", filename, tmp->val.s);
      }else{
        perror("error");
      }
      chmod(filename, 0777);
      long file_size = s.st_size;
      //printf("hard linked file size %ld\n", file_size);
      char file_bytes_str[file_size];
      FILE *fptr;
      fptr = fopen(tmp->val.s, "r");
      if (fptr == NULL) {
        printf("error opening file %s\n", tmp->val.s);
        perror("error");
        exit(-1);
      }
      fgets(file_bytes_str, file_size, fptr);
     // printf("HERE: %s\n", file_bytes_str);
      fclose(fptr);
      fptr = fopen(filename, "w");
      if (fptr == NULL) {
        printf("error opening file %s\n", filename);
        perror("error");
        exit(-1);
      }
      fwrite(file_bytes_str, file_size+1, 1, fptr);
      fclose(fptr);
      chmod(filename, s.st_mode);
      struct utimbuf time;
      time.modtime = s.st_mtime;
      time.actime = s.st_atime;
      utime(filename, &time);
      
      //printf("file name: %s\nother file tmp->val: %s\n\n",filename, tmp->val.s);

      
      continue;
    }

    char mode[5];
    fread(mode, sizeof(int), 1, stdin);
    mode[4] = '\0';
    long mode_num = four_byte_hex(mode);
    if (feof(stdin)) {
      break;
    }

    char mod_time[9];
    fread(mod_time, sizeof(long), 1, stdin);
    mod_time[8] = '\0';
    unsigned long mod_time_num = eight_byte_hex(mod_time);

    if (mode_num == 16877) {
      mkdir(filename, 0777);
      struct Directory *d = malloc(sizeof(struct Directory));
      d->dir_name = strdup(filename);
      d->dir_inode = inode_num;
      d->dir_mode = mode_num;
      d->dir_mod_time = mod_time_num;
      i += 1;
      /*
      printf("i = %d\n", i);
      printf("file name size: %d\n", filename_num);
      printf("file name: %s \n", filename);
      printf("inode: %ld\n", inode_num);
      printf("mode: %o\n", mode_num);
      printf("mode hex: %0x\n", mode_num);
      printf("mode dec: %d\n", mode_num);
      printf("mod time: %lu\n", mod_time_num);
      printf("\n");
      printf("\n");
      */
      if (jrb_find_dbl(directories, inode_num) == NULL) {
        jrb_insert_dbl(directories, inode_num, new_jval_v(d));
        dll_append(dir_names, new_jval_s(strdup(filename)));
      }
      if (feof(stdin)) {
        break;
      }
      continue;
    }

    if (mode_num == 16877) {

      i += 1;
      /*
      printf("i = %d\n", i);
      printf("file name size: %d\n", filename_num);
      printf("file name: %s \n", filename);
      printf("inode: %ld\n", inode_num);
      printf("mode: %o\n", mode_num);
      printf("mode hex: %0x\n", mode_num);
      printf("mode dec: %d\n", mode_num);
      printf("mod time: %lu\n", mod_time_num);
      printf("\n");
      printf("\n");
      */
      continue;
    }

    if (feof(stdin)) {
      break;
    }
    //printf("\n\n\n");

    char file_size_str[9];
    fread(file_size_str, sizeof(long), 1, stdin);
    file_size_str[8] = '\0';
    file_size = eight_byte_hex(file_size_str);
    if (feof(stdin)) {
      break;
    }
    /*
    printf("\n\n\n");
    printf("i = %d\n", i);
    printf("file name size: %d\n", filename_num);
    printf("file name: %s \n", filename);
    printf("inode: %ld\n", inode_num);
    printf("mode: %o\n", mode_num);
    printf("mode hex: %0x\n", mode_num);
    printf("mode dec: %d\n", mode_num);
    printf("mod time: %lu\n", mod_time_num);
    printf("file size: %d\n", file_size);
    printf("file size str: %s\n", file_size_str);
    */
    char file_bytes_str[file_size];
    fread(file_bytes_str, file_size, 1, stdin);

    FILE *fptr;
    fptr = fopen(filename, "w");
    if (fptr == NULL) {
      printf("error opening file %s\n", filename);
      perror("error");
      exit(-1);
    }
    fwrite(file_bytes_str, sizeof(char) * file_size, 1, fptr);
    fclose(fptr);

    chmod(filename, mode_num);
    struct utimbuf time;
    time.modtime = mod_time_num;
    time.actime = 0;
    utime(filename, &time);
    if (jrb_find_dbl(files, inode_num) == NULL) {
      jrb_insert_dbl(files, inode_num, new_jval_s(strdup(filename)));
    }
    /*
        i += 1;
        printf("i = %d\n", i);
        printf("file name size: %d\n", filename_num);
        printf("file name: %s \n", filename);
        printf("inode: %ld\n", inode_num);
        printf("mode: %o\n", mode_num);
        printf("mode hex: %0x\n", mode_num);
        printf("mode dec: %d\n", mode_num);
        printf("mod time: %lu\n", mod_time_num);
        printf("file_size: %ld\n", file_size);
        printf("%s\n", file_bytes_str);
        */
    // for (int i = 0; i < file_size; i++) {
    // printf("%02x", file_bytes_str[i] & 0xff);
    //  }
    //printf("\n");
    //printf("\n");

    if (feof(stdin)) {
      break;
    }
  }
}
// for directories, set permission to 777, make a jrb with inode key and a value of direectory
// struct. once all files have been made go back through the jrb and set permissions and times

int main(int argc, char *argv[]) {
  // int size = size_of_file_name(argv[1], 0);
  // int file_name_offset_result = file_name(argv[1], size, 0);
  // long inode = file_inode(argv[1], file_name_offset_result);
  // print_dir(argv[1]);
  struct stat s;
  JRB files = make_jrb();
  JRB directories = make_jrb();
  JRB ptr;
  Dllist dir_names = new_dllist();
  Dllist tmp;
  tarx(files, directories, dir_names);
  struct Directory *d;
  jrb_traverse(ptr, directories) {
    // printf("key %ld\n", ptr->key);
    d = (struct Directory *)ptr->val.v;
    if (stat(d->dir_name, &s)) {
      fprintf(stderr, "couldnt stat dir %s\n", d->dir_name);
    }
    chmod(d->dir_name, d->dir_mode);
    struct utimbuf time;
    time.modtime = d->dir_mod_time;
    time.actime = s.st_atime;
    utime(d->dir_name, &time);
    // printf("jrb name: %s\njrb inode: %ld\njrb mode: %o\njrb mod time %ld\n\n", d->dir_name, d->dir_inode, d->dir_mode, d->dir_mod_time);
  }
  /*
  FILE *fp;
  fp = fopen("../Gradescript-Examples/001/J7EzvHU5RqnVLvhV", "rb");
  if (fp == NULL) {
    printf("bruh\n");
  }
  char test[19];
  fread(test, 19, 1, fp);
  printf("%s\n", test);
  for (int i = 0; i < 19; i++) {
    printf("%02x", test[i] & 0xff);
  }
  printf("\n");
  // printf("%s\n", argv[1]);
  */
}