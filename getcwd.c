#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <dirent.h>
#include <string.h>
#include <ftw.h>

void creatEnv();

/**
 * 파일을 복사하여 사본을 생성한다. 
 * @param buf   경로명을 저장할 버퍼 변수.
 * @param size  버퍼 변수의 크기
 * @return 버퍼 변수의 시작 주소, 즉 buf
 */
char *mygetcwd(char *buf, size_t size) {
  DIR *dir;
  DIR *dir2;
  struct dirent *dp;
  struct dirent *dp2;
  long cur_ino;
  
  while(((long)(readdir(opendir("."))->d_ino))!=((long)(readdir(opendir(".."))->d_ino))){
    dir2 = opendir(".");
    while( (dp2=readdir(dir2)) != NULL ){ 
      if(strcmp(dp2->d_name,".")==0){
        cur_ino = (long)dp2->d_ino;
      }
    }
  
    dir = opendir("..");
    while( (dp=readdir(dir)) != NULL ){ 
      if(dp->d_ino == cur_ino){
        char newbuf[255] = "/";
        strcat(newbuf, dp->d_name);
        strcat(newbuf, buf);
        strcpy(buf, newbuf);
        break;
      }    
    }
    closedir(dir);
    closedir(dir2);
    chdir("..");
  }
  return buf;
}

int main(void) {
  pid_t pid;
  int status;
  char buf[255];

  creatEnv();
  chdir("dir/sub");

  printf("original func: %s\n", getcwd(NULL, 0));
  printf("mygetcwd func: %s\n", mygetcwd(buf, 255));

  return 0;
}

void creatEnv(){
  mkdir("dir", 0755);
  mkdir("dir/sub", 0755);
  mkdir("dir/sub2", 0);
  
  creat("dir/a", 0755);
  creat("dir/b", 0755);
  creat("dir/sub/x", 0755);
  symlink("dir/a", "dir/sl");
  symlink("dir/x", "dir/dsl");
}