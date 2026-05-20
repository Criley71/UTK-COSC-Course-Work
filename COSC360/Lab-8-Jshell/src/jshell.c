/**
 * Connor Riley
 * 4/21/2024
 * COSC 360 Plank
 * primative shell that allows piping, and file redirection
 */
#include "dllist.h"
#include "fields.h"
#include "jrb.h"
#include "jval.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/**
 * Command struct used to hold info about each string of commands
 */
typedef struct command {
  char *stdin;             // filename to redirect stdin
  char *stdout;            // filename to redirect stdout
  int append_stdout;       // boolean for appending
  int wait;                // wait bolean
  int n_lines;             // num of lines needed to be executed
  int n_commands;          // num of commands, this does not include <, >, >>, NOWAIT, END;
  int *argcs;              // argcs[i] for ith commands argc amount [i=0 argc, i=1 argc, ...]
  char ***argvs;           // argcv[i] is the argv array for the ith command array[[i=0 args], [i=1 args], ...]
  char ***actual_commands; // commands that arent nowait, >, >>, <, END
  int *actual_command_count;
  Dllist com_list; // read commands incrementally
} Command;

int main(int argc, char *argv[], char **envp) {
  IS is = new_inputstruct(NULL);   // input struct used to read in commands
  int status;                      // wait variable
  const int MAX_PIPES = 128;       // amount of pipes made in the pipe array
  Command cmd;                     // command struct
  cmd.n_lines = 0;                 // amount of lines read
  int wait_bool = 1;               // 0 = nowait found, 1 = need to wait
  cmd.argvs = NULL;                // array of strings for each line
  cmd.argcs = NULL;                // amount of arguments for each command
  cmd.actual_commands = NULL;      // array of string commands that dont include file redirection,
  cmd.actual_command_count = NULL; // amount of arguments for each actual command
  char *infile_name = NULL;        // input file redirect name
  char *outfile_name = NULL;       // output file redirect name if NOT appending
  char *append_name = NULL;        // output file redirect name if appending to file
  int fd1, fd2;                    // file descriptors, 1 = input, 2 = output
  cmd.n_commands = 0;              // amount of commands read

  while (get_line(is) >= 0) {
    cmd.argvs = realloc(cmd.argvs, (cmd.n_lines + 1) * sizeof(char **)); // resize arrays to hold all commands
    cmd.argvs[cmd.n_lines] = malloc((is->NF + 1) * sizeof(char *));
    cmd.argcs = realloc(cmd.argcs, (cmd.n_lines + 1) * sizeof(int));
    cmd.argcs[cmd.n_lines] = is->NF + 1;
    wait_bool = 1; // reset the wait boolean to true
    if (strcmp(is->fields[0], "END") == 0) {
      int commands_completed = 0; // if we read the end we then begin executing the commands
      if (strcmp(cmd.argvs[cmd.n_lines - 1][0], "NOWAIT") == 0) {
        cmd.n_lines -= 1; // if no wait exists then we decrease the amount of commands
        cmd.n_commands -= 1;
        wait_bool = 0; // set we dont need to wait
      }
      // if we have only 1 command then we begin reading the lines
      if (cmd.n_commands == 1) { // the amount of commands not including <, >, >>, nowait
        for (int i = 0; i < cmd.n_lines; i++) {
          if (strcmp(cmd.argvs[i][0], "<") == 0 || strcmp(cmd.argvs[i][0], ">") == 0 || strcmp(cmd.argvs[i][0], ">>") == 0) {
            continue; // skip over <, > and >>
          }

          fflush(stdout);
          fflush(stdin);
          fflush(stderr);

          int id = fork();

          if (id == 0) {
            if (strcmp(cmd.argvs[i][0], "NOWAIT") == 0) {
              continue;
            } // after we fork, check if infile or outfile needs to be redirected
            // set the file descriptors using dup2()
            if (infile_name != NULL) {
              fd1 = open(infile_name, O_RDONLY);
              if (fd1 < 0) {
                perror("fd1");
                exit(1);
              }
              if (dup2(fd1, 0) != 0) {
                perror("dup2 fd1");
                exit(1);
              }
              close(fd1);
            }
            if (outfile_name != NULL) {
              fd2 = open(outfile_name, O_WRONLY | O_TRUNC | O_CREAT, 0644);
              if (fd2 < 0) {
                perror("fd2");
                exit(1);
              }
              if (dup2(fd2, 1) != 1) {
                perror("dup2 fd1");
                exit(1);
              }
              close(fd2);
            }
            if (append_name != NULL) {
              fd2 = open(append_name, O_WRONLY | O_APPEND | O_CREAT, 0644);
              if (fd2 < 0) {
                perror("append fd2");
                exit(1);
              }
              if (dup2(fd2, 1) != 1) {
                perror("append dup2 fd1");
                exit(1);
              }
              close(fd2);
            }
            // call execvp on the command to execute
            execvp(cmd.argvs[i][0], cmd.argvs[i]); // if called successfully then it overwrites remaining code
            perror(cmd.argvs[i][0]);
            exit(1);
          } else {
            if (wait_bool == 1) {
              while (1 == 1) {
                // wait through all commands and if wait = id then we are done
                int pid = wait(&status);
                if (pid == id) {
                  break;
                } else {
                }
              }
            }
          }
        }
        // reset all values
        free(cmd.argcs);
        free(cmd.argvs);
        cmd.argvs = NULL;
        cmd.argcs = NULL;
        cmd.n_lines = 0;
        wait_bool = 1;
        infile_name = NULL;
        outfile_name = NULL;
        append_name = NULL;
        cmd.n_commands = 0;
        free(cmd.actual_commands);
        cmd.actual_commands = NULL;
        free(cmd.actual_command_count);
        cmd.actual_command_count = NULL;
        continue;
      } else {                       // multiple commands
        JRB wait_nodes = make_jrb(); // store pid to wait
        JRB tmp;
        int pipes[MAX_PIPES][2]; // array to store all pipes
        for (int i = 0; i < cmd.n_commands; i++)
          pipe(pipes[i]); // initialize the pipes
        int command_count = 0;
        int id = -1;

        for (int i = 0; i < cmd.n_commands; i++) {
          if ((id = fork()) == 0) {              // fork the commands
            if (i == 0 && infile_name != NULL) { // first command reads from file input
              close(STDIN_FILENO);
              fd1 = open(infile_name, O_RDONLY); // if first command and file redirection then we call this
              if (fd1 < 0) {
                perror("fd1");
                exit(1);
              }
              if (dup2(fd1, STDIN_FILENO) != 0) {
                perror("dup2 fd1");
                exit(1);
              }
            } else if (i > 0) { // reads from pipe of previous program
              close(STDIN_FILENO);
              dup2(pipes[i - 1][0], STDIN_FILENO);
            } else {
            }
            // if last command and file redirection then we write to file from pipe
            if (i == cmd.n_commands - 1 && (outfile_name != NULL || append_name != NULL)) { // last command writes to file
              if (outfile_name != NULL) {
                close(STDOUT_FILENO);
                fd2 = open(outfile_name, O_WRONLY | O_TRUNC | O_CREAT, 0666);
                if (fd2 < 0) {
                  perror("fd2");
                  exit(1);
                }
                if (dup2(fd2, STDOUT_FILENO) != 1) {
                  perror("dup2 fd1");
                  exit(1);
                }
              } else if (append_name != NULL) {
                close(STDOUT_FILENO);
                fd2 = open(append_name, O_WRONLY | O_APPEND | O_CREAT, 0666);
                if (fd2 < 0) {
                  perror("append fd2");
                  exit(1);
                }
                if (dup2(fd2, STDOUT_FILENO) != 1) {
                  perror("append dup2 fd1");
                  exit(1);
                }
              }
            } else if (i < cmd.n_commands - 1) { // if its not the last then we read from previous pipe
              close(STDOUT_FILENO);
              dup2(pipes[i][1], STDOUT_FILENO);
            } else {
            }
            // close da pipes
            for (int i = 0; i < cmd.n_commands; i++) {
              close(pipes[i][0]);
              close(pipes[i][1]);
            }

            // call commands
            execvp(cmd.actual_commands[i][0], cmd.actual_commands[i]); // if called successfully then it overwrites remaining code
            perror(cmd.actual_commands[i][0]);
            exit(1);
          } else {
            // insert the fork id to wait
            jrb_insert_int(wait_nodes, id, new_jval_i(i));
          }
        }

        // close all the pipes
        for (int i = 0; i < cmd.n_commands; i++) {
          close(pipes[i][0]);
          close(pipes[i][1]);
        }
        if (wait_bool == 1) {
          int found_pid = 0; // wait all ids out if needing to wait
          while ((found_pid = wait(NULL)) != id) {
          }
        }
        // reset all the variables to default values
        free(cmd.argcs);
        free(cmd.argvs);
        cmd.argvs = NULL;
        cmd.argcs = NULL;
        cmd.n_lines = 0;
        wait_bool = 1;
        infile_name = NULL;
        outfile_name = NULL;
        append_name = NULL;
        cmd.n_commands = 0;
        free(cmd.actual_commands);
        cmd.actual_commands = NULL;
        free(cmd.actual_command_count);
        cmd.actual_command_count = NULL;
        continue;
      }
    }
    if (is->NF == 0) { // if we read a line that is blank then we skip over it
      continue;
    }
    int actual_command_found = 1; // 1 is true, is set to 0 in instance of <, >, >>
    for (int i = 0; i < is->NF; i++) {
      cmd.argvs[cmd.n_lines][i] = strdup(is->fields[i]);
      if (strcmp(cmd.argvs[cmd.n_lines][i], "<") == 0) {
        infile_name = strdup(is->fields[i + 1]); // set the infile name
        // cmd.n_commands -= 1;
        actual_command_found = 0;
      }
      if (strcmp(cmd.argvs[cmd.n_lines][i], ">") == 0) {
        outfile_name = strdup(is->fields[i + 1]); // set the outfile name if not appending
        // cmd.n_commands -= 1;
        actual_command_found = 0;
      }
      if (strcmp(cmd.argvs[cmd.n_lines][i], ">>") == 0) {
        append_name = strdup(is->fields[i + 1]); // set the outfile name if appending
        // cmd.n_commands -= 1;
        actual_command_found = 0;
      }
    }
    if (actual_command_found == 1) { // if we read an "actual" command meaning not a redirect or wait then we add that to
                                     // the array of commands to be ran when piping
      cmd.actual_command_count = realloc(cmd.actual_command_count, (cmd.n_commands + 1) * sizeof(int));
      cmd.actual_commands = realloc(cmd.actual_commands, (cmd.n_commands + 1) * sizeof(char **));
      cmd.actual_command_count[cmd.n_commands] = is->NF + 1;
      cmd.actual_commands[cmd.n_commands] = malloc((is->NF + 1) * sizeof(char *));
      for (int i = 0; i < cmd.actual_command_count[cmd.n_commands] - 1; i++) {
        cmd.actual_commands[cmd.n_commands][i] = strdup(cmd.argvs[cmd.n_lines][i]);
      }
      cmd.actual_commands[cmd.n_commands][is->NF] = NULL;
    }
    if (actual_command_found == 1) {
      cmd.n_commands++; //increment actual command count
    }
    cmd.argvs[cmd.n_lines][is->NF] = NULL; //add a null to end of each command
    cmd.n_lines += 1;
  }
 
}
