#include <stdio.h>
#include <stdlib.h>
#include <libc.h>

# define ispipe(x) (strcmp(x, "|") == 0)
# define issem(x) (strcmp(x, ";") == 0)

void func(char **tab, int size, int child, char *delimiter){
  if(size < 0 || tab[size] == NULL)
    return ;
  if(ispipe(tab[size])){
    tab[size] = NULL;
    int fd[2];
    pipe(fd);
    int p = fork();
    if(!p){
      dup2(fd[1], 1);
      close(fd[1]);
      close(fd[0]);
      func(tab, size - 2, 1,tab[size - 1]);
    }
    int p2 = fork();
    if(!p2){
      dup2(fd[0], 0);
      close(fd[0]);
      close(fd[1]);
      func(tab, size -1, 1, NULL);
    }
    close(fd[0]);
    close(fd[1]);
    waitpid(-1, 0, 0);
    waitpid(-1, 0, 0);
  }
  else{
    if(!child)
    {
       if(!fork())
        execve(tab[size], tab+ size, NULL);
       else
          waitpid(-1, 0, 1);
    }
    else
    {
      if(delimiter){
        int sum = size;
        while(tab[sum]){
          if(strcmp(tab[sum], delimiter) == 0)
            break;
          sum++;
        }
        char **ptr = malloc((sum - size + 1) * sizeof(char*));
        int  i = 0, j = size;
        while(i < sum - size){
          ptr[i] = tab[j++];
          printf("%s\n", ptr[i]);
          i++;
        }
        ptr[sum-size] = NULL;
        execve(*ptr, ptr, NULL);
      }
      else
        execve(tab[size], tab+ size, NULL);
    }
  }
}

int main(int argc, char *argv[])
{
  char *stack[1000] = {NULL};
  char *queue[1000] = {NULL};
  int i = 1; 
  int j  = argc - argc;
  while (argv[i]) {
    if(ispipe(argv[i]) && stack[0] == NULL)
      stack[0] = argv[i];
    else
      queue[j++] = argv[i];
    i++;
  }
  if(stack[0])
    queue[j++] = stack[0];
  i = -1;
  while(queue[++i])/*printf("%s\n", queue[i])*/;
  func(queue, --i, 0, NULL);
}
