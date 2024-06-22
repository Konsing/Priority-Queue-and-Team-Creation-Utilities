#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define CMDLINE_MAX 512
#define MAX_ARGS 16
#define MAX_CHARS 32

typedef struct struct_t {
  char *running_command;
  char *full_command;
  char args[MAX_ARGS][MAX_CHARS];
  struct struct_t *next;
} node_cmd;

char *remove_extra_spaces(char *str) {
  int i = 0, j = 0;
  while (str[i]) {
    if (str[i] != ' ' || (str[i] == ' ' && str[i + 1] != ' ')) {
      str[j++] = str[i];
    }
    i++;
  }
  str[j] = '\0';
  return str;
}

void parse_and_split(char *cmd, node_cmd **root) {
  char *token = strtok(cmd, "|");
  node_cmd *current = *root;

  while (token != NULL) {
    current->full_command = token;
    current->next = malloc(sizeof(node_cmd));
    current = current->next;
    token = strtok(NULL, "|");
  }

  current = *root;
  while (current->next != NULL) {
    char cmd_copy[CMDLINE_MAX];
    strcpy(cmd_copy, current->full_command);
    remove_extra_spaces(cmd_copy);
    token = strtok(cmd_copy, " ");
    current->running_command = token;
    int k = 0;
    token = strtok(NULL, " ");
    while (token != NULL) {
      if (k == MAX_ARGS) {
        fprintf(stderr, "Error: too many process arguments\n");
        exit(0);
      }
      strcpy(current->args[k], token);
      token = strtok(NULL, " ");
      k++;
    }
    current = current->next;
  }
  current->next = NULL;
}

void print_list(node_cmd **root) {
  node_cmd *current = *root;
  while (current->next != NULL) {
    printf("Running command: %s\n", current->running_command);
    printf("Full command: %s\n", current->full_command);
    printf("Args\n");
    for (int i = 0; i < MAX_ARGS; i++) {
      printf("%d. ", i);
      for (int j = 0; j < MAX_CHARS; j++) {
        printf("%c", current->args[i][j]);
      }
      printf("\n");
      // puts("\t");
    }
    current = current->next;
  }
}

// void whitespace(char *str) {
//   int l = strlen(str);
//   char *new_str = malloc(sizeof(char) * 1024);
//   char temp;
//   int x = 0;

//   for (int i = 0; i < l; i++) {
//     if (str[i] == ' ' && temp == '\0')
//       continue;

//     else {
//       if (str[i] != '>') {
//         if (str[i] != ' ') {
//           new_str[x] = str[i];
//           temp = new_str[x];
//           x++;
//           continue;
//         }
//         for (int z = i + 1; z < l; z++) {
//           if (str[z] != ' ') {
//             if (str[z] != '|') { // any char other than a space or |
//               while (i < z) {
//                 new_str[x] = str[i];
//                 temp = str[i];
//                 x++;
//                 i++;
//               }
//               i--;
//               break;
//             }
//             // str[z] = '|'
//             i = z - 1;
//             break;
//           }
//         }
//       } else {
//         // add a space at the start and at least 1 '>'
//         new_str[x] = ' ';
//         x++;
//         new_str[x] = '>';
//         x++;
//         if (str[i + 1] != '>') { // if only 1 > print
//           new_str[x] = ' ';
//           temp = new_str[x];
//           x++;
//           continue;
//         }
//         new_str[x] = '>';
//         x++;
//         new_str[x] = ' ';
//         temp = new_str[x];
//         x++;
//         i += 1;
//       }
//     }
//   }

//   // printf("'%s'\n", new_str);

//   char *better = malloc(sizeof(char) * 1024);
//   l = strlen(new_str);
//   x = 0;

//   for (int i = 0; i < l; i++) {
//     if (new_str[i] != ' ') {
//       better[x] = new_str[i];
//       x++;
//       continue;
//     }
//     if (new_str[i + 1] != ' ') { // check next char for space
//       better[x] = ' ';
//       x++;
//       continue;
//     }

//     // if we have more than 1 space in a row
//     for (int z = i + 2; z < l; z++) {
//       if (new_str[z] != ' ') {
//         if (new_str[z] == '>') {
//           // printf("z: %d\n", z);
//           better[x] = ' ';
//           x++;
//           better[x] = new_str[z];
//           i = z - 1;
//           break;
//         }
//         // keep spaces
//         while (i < z) {
//           better[x] = new_str[i];
//           x++;
//           i++;
//         }
//         i--;
//         break;
//       }
//     }
//   }
// }

int main(void) {
  char cmd[CMDLINE_MAX];
  char copy_cmd[CMDLINE_MAX];
  char *argv[MAX_ARGS]; // probably dont need this
  node_cmd *root;

  while (1) {
    char *nl;
    root = malloc(sizeof(node_cmd));

    /* Print prompt */
    printf("sshell@ucd$ ");
    fflush(stdout);

    /* Get command line */
    fgets(cmd, CMDLINE_MAX, stdin);

    /* Parse pipelines */
    parse_and_split(cmd, &root);
    // print_list(&root); for testing

    /* Print command line if stdin is not provided by terminal */
    if (!isatty(STDIN_FILENO)) {
      printf("%s", cmd);
      fflush(stdout);
    }

    /* Remove trailing newline from command line */
    nl = strchr(cmd, '\n');
    if (nl)
      *nl = '\0';

    strcpy(copy_cmd, cmd);

    char *ptr = strtok(copy_cmd, " ");
    int i = 0;
    int argc = 0;
    while (ptr != NULL) {
      argv[i] = ptr;
      ptr = strtok(NULL, " ");
      i++;
      argc++;
    }
    argv[i] = NULL;

    for (int k = 0; k < argc; k++) {
      printf("argv[%d] = %s\n", k, argv[k]);
    }

    /* Builtin command */
    if (!strcmp(cmd, "exit")) {
      fprintf(stderr, "Bye...\n");
      fprintf(stderr, "+ completed 'exit' [0]\n");
      break;
    }

    if (!strcmp(argv[0], "cd")) {
      if (argv[1]) {
        if (chdir(argv[1]) == -1) {
          perror("chdir\n");
          fprintf(stderr, "+ completed 'cd' [1]\n");
        }
      } else {
        printf("Error: No directory specified\n");
      }
      fprintf(stderr, "+ completed 'cd' [0]\n");
      continue;
    }

    if (!strcmp(cmd, "pwd")) {
      char cwd[256];
      if (getcwd(cwd, sizeof(cwd)) == NULL)
        perror("getcwd() error");
      else {
        printf("%s\n", cwd);
        fprintf(stderr, "+ completed 'pwd' [0]\n");
      }
      continue;
    }

    /* Regular command */
    pid_t pid = fork();

    if (pid == 0) {
      // in child
      execvp(argv[0], argv);
      perror("execvp");
      exit(1);
    } else if (pid > 0) {
      // in parent
      int status;
      waitpid(pid, &status, 0);
      printf("+ completed '%s' [%d]\n", root->full_command,
             WEXITSTATUS(status));
    } else {
      perror("fork");
      exit(1);
    }

    print_list(&root);
  }
  return EXIT_SUCCESS;
}