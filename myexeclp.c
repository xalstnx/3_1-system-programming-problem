#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#define MAXARGS 31
#define MAXFILENAME 1024
#define MAXPATHSTR 2048

extern char **environ;
int myexeclp(const char *file, const char *args, ...);

int main(void) {
  char path[MAXPATHSTR];
  sprintf(path, "PATH=%s:%s", getcwd(NULL, 0), getenv("PATH"));
  putenv(path);
  // prepare the executable file named "hello"
  system("cp hello.tmp hello.c");
  system("clang-7 -pthread -lm -o hello hello.c");
  system("rm hello.c");
  //execlp("hello", "hello", "-a", "-b", "-c", (char *) 0);
  myexeclp("hello", "hello", "-a", "-b", "-c", (char *) 0);
  return 0;
}

int myexeclp(const char *file, const char *args, ...){
  char fname[MAXFILENAME] = "./";
  strcat(fname, file);
  
  va_list ap;
  
  char *array[MAXARGS +1];
  int argno = 0;


  va_start(ap, args);
  while (args != 0 && argno < MAXARGS)
  {
    array[argno++] = args;
    args = va_arg(ap, const char *);
  }
  array[argno] = (char *) 0;
  
  va_end(ap);
  
  execve(fname, array, 0);
  return 0;
}