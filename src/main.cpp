#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <getopt.h>
#include <fstream>
#include <thread>
#include <zconf.h>
#include <iostream>
#include "../libtehs/forkProcess.h"

using namespace std;

static int isKillFlag = 0;
static int typeID = 0;//

struct option longopts[] = {
    {"fork",  no_argument, NULL, 'f'},
    {"stop",  required_argument, NULL, 's'},
    {"help", no_argument, NULL, 'h'},
    {0,       0,             0,      0},
};

void usage() {
  fprintf(stderr,
          "\n"
          "Usage: pat [options]\n"
          "\n"
          "Options:\n"
          "    -f, --fork       fork a child process\n"
          "    -s, --stop     "
          "stop the process or thread\n"
          "    -h, --help      help\n");

  exit(0);
}

//类型转换
int charToInt(char flagChar){
  char flagString[10];
  flagString[0] = flagChar;
  return (int)flagString[0];
}

//保存停止文件
void saveStopFlag(int ID,int Flag,int pid) {
  char filename[40];
  char homeSrc[50] = "/home/zyx/workspace/tehs";
  sprintf(filename, "%s/config/stopflag",homeSrc);

  ofstream stopFile;
  stopFile.open(filename, std::ifstream::out);

  if (!stopFile.is_open()) {
    printf("stopflag file open failed....");
    exit(1);
  }

  stopFile<<ID;
  stopFile<<"\n";
  stopFile<<Flag;
  stopFile<<"\n";
  stopFile<<pid;

  stopFile.close();
}

//获取停止文件
pid_t getStopFlag() {
  char filename[40];
  char homeSrc[50] = "/home/zyx/workspace/tehs";

  sprintf(filename, "%s/config/stopflag", homeSrc);

  ifstream stopfile;
  stopfile.open(filename, std::ifstream::in);

  while (!stopfile.is_open()) {
    printf("stopflag file open failed....");
    sleep(1);
  }

  string line;
  int getNum[3];
  int x = 0;//占位符

  //讲数据存入数组
  while (getline(stopfile,line)) {
    getNum[x]=stoi(line);
    x++;
  }

  pid_t pid;
  typeID = getNum[0];
  isKillFlag = getNum[1];
  pid = getNum[2];

  return pid;
}

//暂停函数
void stop(char *type,int pid) {
  isKillFlag = 1;

  printf("%s\n",type);
  printf("%d\n",pid);

  //判断对象类型p:process t:thread
  if (strcmp(type,"p") == 0) {
    printf("stop process:%d now...\n", pid);
    saveStopFlag(0, isKillFlag, pid);
  }
  if (strcmp(type,"t") == 0) {
    printf("stop thread now...\n");
    saveStopFlag(1, isKillFlag, pid);
  }
}

//线程运行函数
void threadActive(){
  printf("this is thread...\n");

  while (true){
    getStopFlag();
    if(typeID == 1 && isKillFlag == 1){
      printf("stop thread now....\n");
      saveStopFlag(0,0, 0);
      break;
    }
  }

  printf("stop thread success ...\n");
}

//子进程运行函数
void childActive(){
  while (true) {
    if (typeID == 0 && getStopFlag() == getpid() && isKillFlag == 1) {
      saveStopFlag(0,0, getpid());
      break;
    }
  }
}

//子进程运行函数
void parentActive(){
  std::thread test_thread (threadActive);
  test_thread.join();
}

int main(int argc, char *argv[] ) {
  int opt = 0;

  while (-1 !=
      (opt = getopt_long(argc, argv, "::alLh::c:d:", longopts, NULL))
      ) {
    switch (opt) {
      case 'f':
        forkProcess((active_t) childActive,(active_t) parentActive);
        break;
      case 's':stop(argv[2],atoi(argv[3]));
        break;
      default:
        usage();
        break;
    }
  }

}