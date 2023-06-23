#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <termios.h>
#include <unistd.h>
#include <sys/stat.h>

#include "tokenizer.h"

/* Convenience macro to silence compiler warnings about unused function parameters. */
#define unused __attribute__((unused))
#define NORMAL 1
#define LEFT_REDIRECT 2
#define RIGHT_REDIRECT 3
#define PIPE 4
#define R_END 0
#define W_END 1
/* Whether the shell is connected to an actual terminal or not. */
bool shell_is_interactive;

/* File descriptor for the shell input */
int shell_terminal;

/* Terminal mode settings for the shell */
struct termios shell_tmodes;

/* Process group id for the shell */
pid_t shell_pgid;

int cmd_exit(struct tokens* tokens);
int cmd_help(struct tokens* tokens);
int cmd_pwd(struct tokens* tokens);
int cmd_cd(struct tokens* tokens);

/* Built-in command functions take token array (see parse.h) and return int */
typedef int cmd_fun_t(struct tokens* tokens);

/* Built-in command struct and lookup table */
typedef struct fun_desc {
  cmd_fun_t* fun;
  char* cmd;
  char* doc;
} fun_desc_t;

fun_desc_t cmd_table[] = {
    {cmd_help, "?", "show this help menu"},
    {cmd_exit, "exit", "exit the command shell"},
    {cmd_pwd, "pwd", "print current working directory"},
    {cmd_cd, "cd", "change current working directory to specified path"}
};

/* Prints a helpful description for the given command */
int cmd_help(unused struct tokens* tokens) {
  for (unsigned int i = 0; i < sizeof(cmd_table) / sizeof(fun_desc_t); i++)
    printf("%s - %s\n", cmd_table[i].cmd, cmd_table[i].doc);
  return 1;
}

/* Exits this shell */
int cmd_exit(unused struct tokens* tokens) { exit(0); }

int cmd_pwd(unused struct tokens* tokens) {
  char cwd[256];
  if (getcwd(cwd, sizeof(cwd)) == NULL) {
    return 0;
  }
  printf("%s\n", cwd);
  return 1;
}

int cmd_cd(unused struct tokens* tokens) {
  if (tokens_get_length(tokens) != 2) {
    return 0;
  }
  chdir(tokens_get_token(tokens, 1));
  return 1;
}

/* Looks up the built-in command, if it exists. */
int lookup(char cmd[]) {
  for (unsigned int i = 0; i < sizeof(cmd_table) / sizeof(fun_desc_t); i++)
    if (cmd && (strcmp(cmd_table[i].cmd, cmd) == 0))
      return i;
  return -1;
}

/* Intialization procedures for this shell */
void init_shell() {
  /* Our shell is connected to standard input. */
  shell_terminal = STDIN_FILENO;

  /* Check if we are running interactively */
  shell_is_interactive = isatty(shell_terminal);

  if (shell_is_interactive) {
    /* If the shell is not currently in the foreground, we must pause the shell until it becomes a
     * foreground process. We use SIGTTIN to pause the shell. When the shell gets moved to the
     * foreground, we'll receive a SIGCONT. */
    while (tcgetpgrp(shell_terminal) != (shell_pgid = getpgrp()))
      kill(-shell_pgid, SIGTTIN);

    /* Saves the shell's process id */
    shell_pgid = getpid();

    /* Take control of the terminal */
    tcsetpgrp(shell_terminal, shell_pgid);

    /* Save the current termios to a variable, so it can be restored later. */
    tcgetattr(shell_terminal, &shell_tmodes);
  }
}

void decide_mode(int* mode, struct tokens* tokens) {
  size_t tokens_length = tokens_get_length(tokens);
  for (int i = 0; i < tokens_length; i++) {
    if (strcmp(tokens_get_token(tokens, i), "<") == 0) {
      *mode = LEFT_REDIRECT;
      return;
    } else if(strcmp(tokens_get_token(tokens, i), ">") == 0) {
      *mode = RIGHT_REDIRECT;
      return;
    } else if(strcmp(tokens_get_token(tokens, i), "|") == 0) {
      *mode = PIPE;
      return;
    }
  }
  *mode = NORMAL;
  return;
}

int execute_path_resolution(char* command, char** command_array) {
  if (execv(command, command_array) == -1) {
    // Fail to execute program, try path resolution
    char *path_variable = getenv("PATH");
    char* token;
    char* rest = NULL;
    struct stat stat_buffer;
    for (token = strtok_r(path_variable, ":", &rest); token != NULL; token = strtok_r(NULL, ":", &rest)) {
        char temp_str[64];
        strcpy(temp_str, token);
        strcat(temp_str, "/");
        strcat(temp_str, command);
        if (stat(temp_str, &stat_buffer) == 0) {
          strcpy(command, temp_str);
          break;
        }
    }
    if (execv(command, command_array) == -1) {
      return -1;
    }
  }
  return 0;
}

void execute_normal(struct tokens* tokens) {
  pid_t pid;
  pid = fork();
  if (pid) {
    // Parent process
    if (wait(NULL) == -1) {
      printf("wait fail\n");
    }
  } else {
    // Children process
    size_t tokens_length = tokens_get_length(tokens);
    char* prog_argv[tokens_length + 1];
    for (int i = 0; i < tokens_length; i++) {
      char* token = tokens_get_token(tokens, i);
      prog_argv[i] = token;
    }
    prog_argv[tokens_length] = NULL;
    if (execute_path_resolution(prog_argv[0], prog_argv) == -1) {
      printf("execute fail\n");
    }
    exit(0);
  }

}

void execute_redirect(struct tokens* tokens, int direction) {
  char* redirect_file;
  size_t tokens_length = tokens_get_length(tokens);
  int fd;
  
  // Valid token should remove direction flag and redirect file
  pid_t pid = fork();
  if (pid) {
    // Parent process
    if (wait(NULL) == -1) {
      printf("wait fail\n");
    }
  } else {
    // Children process
    int save_stdin = dup(0);
    int save_stdout = dup(1);
    char* prog_argv[tokens_length + 1 - 2];
    for (int i = 0; i < tokens_length; i++) {
      char* token = tokens_get_token(tokens, i);
      if (strcmp(token, "<") == 0 || strcmp(token, ">") == 0) {
        redirect_file = tokens_get_token(tokens, i + 1);
        break;
      }
      prog_argv[i] = token;
    }
    prog_argv[tokens_length - 2] = NULL;
    if (direction == LEFT_REDIRECT) {
      if ((fd = open(redirect_file, O_RDONLY)) < 0) {
        printf("can't open input file\n");
        exit(0);
      } 
      dup2(fd, 0);
      close(fd);
    } else if (direction == RIGHT_REDIRECT) {
      if ((fd = open(redirect_file, O_CREAT|O_TRUNC|O_RDWR, 0644)) < 0) {
        printf("can't open output file\n");
        exit(0);
      }
      dup2(fd, 1);
      close(fd);
    }
    if (execute_path_resolution(prog_argv[0], prog_argv) == -1) {
      printf("execute fail\n");
    }
    dup2(save_stdin, 0);
    dup2(save_stdout, 1);
    close(save_stdin);
    close(save_stdout);
    exit(0);
  }
}

int spawn_proc (int in, int out, char** argv)
{
  pid_t pid;

  if ((pid = fork ()) == 0)
    {
      if (in != 0)
        {
          dup2 (in, 0);
          close (in);
        }

      if (out != 1)
        {
          dup2 (out, 1);
          close (out);
        }

      return execute_path_resolution(argv[0], argv);
    }

  return pid;
}

void execute_pipe(struct tokens* tokens) {
  size_t tokens_length = tokens_get_length(tokens);
  // number of pipes
  int pipe_num = 0;
  /*
  number of args for each command
  we only need an array has capacity larger than pipe_num
  since we can only know pipe_num after parsing, I choose tokens_length which is larger than pipe_num
  */ 
  int argv_num[tokens_length];
  /*
  Parse tokens to count pipes and prepare container for arguments of each process
  For cat shell.c | grep "tokens" | wc 
  argc_group will end up like the following
  {{"cat", "shell.c", NULL}, {"grep", "tokens", NULL}, {"wc", NULL}}
  */
  argv_num[pipe_num] = 0;
  for (int i = 0; i < tokens_length; i++) {
    if (strcmp(tokens_get_token(tokens, i), "|") == 0) {
      pipe_num += 1;
      argv_num[pipe_num] = 0;
    } else {
      argv_num[pipe_num] += 1;
    }
  }
  int process_num = pipe_num + 1;
  char*** argv_group = malloc(sizeof(char**) * process_num);
  for (int i = 0; i < process_num; i++) {
    argv_group[i] = malloc(sizeof(char*) * argv_num[i]);
  }

  // Initialize argv_group
  int process_index = 0;
  int arg_index = 0;
  for (int i = 0; i < tokens_length; i++) {
    char* token = tokens_get_token(tokens, i);
    if (strcmp(token, "|") == 0) {
      argv_group[process_index][arg_index] = NULL;
      arg_index = 0;
      process_index += 1;
    } else {
      argv_group[process_index][arg_index] = token;
      arg_index += 1;
    }
  }

  int p[2];
  int in = 0;

  for (int i = 0; i < pipe_num; i++) {
    pipe(p);
    if (spawn_proc(in, p[1], argv_group[i]) == -1) {
      printf("fail to execute\n");
      exit(-1);
    }
    close(p[1]);
    in = p[0];
  }

  if (in != 0)
    dup2 (in, 0);
  /* Main process */
  
  if (execute_path_resolution(argv_group[pipe_num][0], argv_group[pipe_num]) == -1) {
    printf("fail to execute\n");
    exit(-1);
  }
  
}
int main(unused int argc, unused char* argv[]) {
  init_shell();

  static char line[4096];
  int line_num = 0;

  /* Please only print shell prompts when standard input is not a tty */
  if (shell_is_interactive)
    fprintf(stdout, "%d: ", line_num);

  while (fgets(line, 4096, stdin)) {
    /* Split our line into words. */
    struct tokens* tokens = tokenize(line);

    /* Find which built-in function to run. */
    int fundex = lookup(tokens_get_token(tokens, 0));

    if (fundex >= 0) {
      cmd_table[fundex].fun(tokens);
    } else {
      int mode = 0;
      decide_mode(&mode, tokens);
      if (mode == NORMAL) {
        execute_normal(tokens);
      } else if (mode == LEFT_REDIRECT) {
        execute_redirect(tokens, mode);
      } else if (mode == RIGHT_REDIRECT) {
        execute_redirect(tokens, mode);
      } else if (mode == PIPE) {
        execute_pipe(tokens);
      }
    }

    if (shell_is_interactive)
      /* Please only print shell prompts when standard input is not a tty */
      fprintf(stdout, "%d: ", ++line_num);

    /* Clean up memory */
    tokens_destroy(tokens);
  }

  return 0;
}
