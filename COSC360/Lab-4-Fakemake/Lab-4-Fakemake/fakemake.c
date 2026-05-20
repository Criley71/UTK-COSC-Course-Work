/*
Connor Riley
02/23/2024
A Makefile written in c
takes an fm file that specifies c files, executables, flags and libraries
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

#include "dllist.h"
#include "fields.h"
#include "jrb.h"
#include "jval.h"

typedef struct files {
  Dllist c_files;   //.c files
  Dllist h_files;   // header files
  Dllist flags;     // flags
  Dllist libs;      // libraries
  char *executable; // executable
  Dllist o_files;   // object files
} File_Struct;

// takes a Dllist and an input struct to read the fm file and fill
// out the dll lists
void read_line_info(Dllist dlist, IS is) {
  Dllist tmp;
  for (int i = 1; i < is->NF; i++) {
    dll_append(dlist, new_jval_v((void *)strdup(is->fields[i]))); // dups string into list
  }                                                               // credit to planks notes -> dllists src/dllreverse.c
}
//this function reads the fm file and distrubutes them into their respective dllist
void process_desc_file(File_Struct *f, IS is) {
  int exec_count = 0;
  while (get_line(is) >= 0) { // this should prevent blank lines
    if (is->NF > 1) {
      // a file/flag
      // check if first letter on the line is c, h, f, l, e
      // similar to fam tree lab to where we determined type of input
      if (is->NF == 0) {
        continue;
      }
      //checks the first char on each line for type of file
      if (strcmp(is->fields[0], "C") == 0) {
        read_line_info(f->c_files, is);
      } else if (strcmp(is->fields[0], "H") == 0) {
        read_line_info(f->h_files, is);
      } else if (strcmp(is->fields[0], "F") == 0) {
        read_line_info(f->flags, is);
      } else if (strcmp(is->fields[0], "L") == 0) {
        read_line_info(f->libs, is);
      } else if (strcmp(is->fields[0], "E") == 0) {
        exec_count += 1;
        if (exec_count > 1 || is->NF > 2) {
          // printf("Error: More than 1 executable\n");
          fprintf(stderr, "fmakefile (%d) cannot have more than one E line\n", is->line);
          exit(-1);
        }
        f->executable = strdup(is->fields[1]); // should always be one i belive
      } else {
        printf("Error: invalid type, not C, H, F, L or E\n");

        exit(-1);
      }
    }
  }
  if (exec_count == 0) { //checks if there are multiple exe 
    fprintf(stderr, "No executable specified\n");
    exit(1);
  }
}
//processes the c file to an object file via system call
void create_o_files(char *c_file_name, File_Struct *f) {
  // printf("gcc -c %s\n", c_file_name);
  Dllist tmp;
  char *command = (char *)malloc(1024 * sizeof(char));
  strcpy(command, "gcc -c");
  dll_traverse(tmp, f->flags) {
    strcat(command, " ");
    strcat(command, tmp->val.s);
  }
  strcat(command, " ");
  strcat(command, c_file_name);

  printf("%s\n", command);
  if (system(command) != 0) {
    fprintf(stderr, "Command failed.  Exiting\n");
    free(command);
    exit(-1);
  }

  free(command);
}
//used to check the most recent time a  header file has been modified 
long long h_max_check(File_Struct *f) {
  long long max = 0;
  Dllist tmp;
  struct stat s;
  char *header_name;
  dll_traverse(tmp, f->h_files) {
    header_name = strdup(tmp->val.s);
    if (stat(header_name, &s) == -1) {
      // printf("header file '%s' does not exist\n", header_name);
      exit(-1);
    }
    if (max < s.st_mtime) {
      max = s.st_mtime;
    }
  }
  return max;
}
//similar to the h_max_check above but for if an obkect file
//needs to be remade if it is older than the c file
long long o_max_check(File_Struct *f) {
  long long max = 0;
  Dllist tmp;
  struct stat s;
  char *o_file_name;
  dll_traverse(tmp, f->o_files) {
    o_file_name = strdup(tmp->val.s);
    if (stat(o_file_name, &s) == -1) {
      //  printf("header file '%s' does not exist\n", o_file_name);
      exit(-1);
    }
    if (max < s.st_mtime) {
      max = s.st_mtime;
    }
  }
  return max;
}

//checks for an executable existance, if it doesnt exist then 
//we will make one and if it does exist then we see if an object file is
//more recent then the executable
int exec_stat_check(File_Struct *f) {
  Dllist tmp;
  struct stat s;
  char *final_command = malloc(10000 * sizeof(char));
  if (stat(f->executable, &s) == -1) {
    // executable does not exist need to make one;

    
    //frankenstein monster the command to create the exe through a lot of strcat 
    strcat(final_command, "gcc -o");
    strcat(final_command, " ");
    strcat(final_command, f->executable);
    dll_traverse(tmp, f->flags) {
      strcat(final_command, " ");
      strcat(final_command, tmp->val.s);
    }

    dll_traverse(tmp, f->o_files) {
      strcat(final_command, " ");
      strcat(final_command, tmp->val.s);
    }

    dll_traverse(tmp, f->libs) {
      strcat(final_command, " ");
      strcat(final_command, tmp->val.s);
    }
   
    printf("%s\n", final_command);
    if (system(final_command) != 0) {
      fprintf(stderr, "Command failed.  Fakemake exiting\n");
      free(final_command);
      exit(-1);
    }
    free(final_command);
    return 1;
  } else if (s.st_mtime - o_max_check(f) < 0) {
    // executable is older than an o file

    strcat(final_command, "gcc -o");
    strcat(final_command, " ");
    strcat(final_command, f->executable);
    dll_traverse(tmp, f->flags) {
      strcat(final_command, " ");
      strcat(final_command, tmp->val.s);
    }

    dll_traverse(tmp, f->o_files) {
      strcat(final_command, " ");
      strcat(final_command, tmp->val.s);
    }
    dll_traverse(tmp, f->libs) {
      strcat(final_command, " ");
      strcat(final_command, tmp->val.s);
    }
    
    printf("%s\n", final_command);
    if (system(final_command) != 0) {
      fprintf(stderr, "Command failed.  Fakemake exiting\n");
      free(final_command);
      exit(-1);
    }
    free(final_command);
    return 1;
  } else {
    free(final_command);
    //if exe both exist and is more recent than object files then its up to date
    printf("%s up to date\n", f->executable);
  }
}
/*the function that ties the whole program together
it traverses the c file dllist to determine if its corresponding o file
exists or if it is older than the c file
*/
void c_stat_check(File_Struct *f) {
  Dllist tmp;
  struct stat s;
  char *file_name;
  char *file_object_name;
  char *dot_o = ".o";
  long long h_mod_time;
  long long o_mod;
  int o_made;
  int need_to_remake_exe = 0;
  dll_traverse(tmp, f->c_files) {
    o_made = 0;
    file_name = strdup(tmp->val.s);
    file_object_name = strdup(tmp->val.s);
    if (stat(file_name, &s) == -1) {
      fprintf(stderr, "fmakefile: %s: No such file or directory\n", file_name);
      exit(-1);
    }
    size_t length = strlen(file_name);
    char *is_dot_c = strstr(file_object_name, ".c"); // may need to check if it even ends in .c
    if (is_dot_c == NULL) {
      // printf("doesnt end in .c, %s\n", file_name);
      exit(-1);
    }
    strcpy(is_dot_c, ".o");
    // need to compare max header time of files
  
    if (stat(file_object_name, &s) == -1) {
      // o file does not exist we need to create it 
      create_o_files(file_name, f);
      o_made = 1;
      need_to_remake_exe += 1;
    } else {
      //o file does exist, now we need to check if it is more recent then the .c file
      o_mod = s.st_mtime;
      
      stat(file_name, &s);
      long long c_mod = s.st_mtime;


      if (c_mod - o_mod < 0) {
        // c is older, dont need to remake .o file
      } else {
        // o is older, we need to remake .o file
        create_o_files(file_name, f);
        o_made = 1;
        need_to_remake_exe += 1;
      }
      h_mod_time = h_max_check(f);
      
      if (h_mod_time > o_mod && o_made == 0) {
        // header is newer, we need to remake %s file
        create_o_files(file_name, f);
        need_to_remake_exe += 1;
      } else {
        // header is older than c file file
      }
    }
    
    dll_append(f->o_files, new_jval_s(strdup(file_object_name))); // add o file to list
    free(file_name);
    free(file_object_name);
  }

    exec_stat_check(f);
}

void delete_lists(File_Struct* f){
  free_dllist(f->c_files);
  free_dllist(f->flags);
  free_dllist(f->h_files);
  free_dllist(f->libs);
  free_dllist(f->o_files);
  free(f->executable);
}



int main(int argc, char *argv[]) {
  FILE *description_file;
  IS is;
  char *description_file_name;
  struct stat sb;

  File_Struct *ok_computer; // CHANGE THIS CANT THINK OF GOOD NAME RN
  ok_computer = malloc(sizeof(struct files));
  ok_computer->c_files = new_dllist();
  ok_computer->h_files = new_dllist();
  ok_computer->libs = new_dllist();
  ok_computer->flags = new_dllist();
  ok_computer->o_files = new_dllist();

  if (argc == 1) {//if no arg passed then default to fmakefile
    description_file_name = "fmakefile";
    is = new_inputstruct(description_file_name);
    if (is == NULL) {
      perror("Error ");
      exit(1);
    }
  }//else we roll ( •_•)>⌐■-■  (⌐■_■)
  if (argc == 2) {
    is = new_inputstruct(argv[1]);
    if (is == NULL) {
      perror(argv[1]);
      exit(1);
    }
  }

  process_desc_file(ok_computer, is);
  c_stat_check(ok_computer);

  // exec_stat_check(ok_computer);
  delete_lists(ok_computer);
  free(ok_computer);
  jettison_inputstruct(is);
  return 0;

}