#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <getopt.h>
#include <fstream>
#include <thread>
#include <zconf.h>
#include <iostream>
#include <ares_build.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include "../libtehs/forkProcess.h"
#include "listenConn.h"

using namespace std;

static int isKillFlag = 0;
static int typeID = 0;

//项目家目录
static char homeSrc[50] = "/home/zyx/workspace/tehs";

//监听的端口
#define PORT 8888
#define QUEUE 20
int fd;
int conn;

struct option longopts[] = {
    {"fork", no_argument, NULL, 'f'},
    {"stop", required_argument, NULL, 's'},
    {"help", no_argument, NULL, 'h'},
    {0, 0, 0, 0},
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
int charToInt(char flagChar) {
  char flagString[10];
  flagString[0] = flagChar;
  return (int) flagString[0];
}

//保存Client信息
void saveClient(int conn, char *buffer, int len) {
  char connAndLenConfig[50];
  char buffConfig[50];

  //设置文件位置
  sprintf(connAndLenConfig, "%s/config/conn", homeSrc);
  sprintf(buffConfig, "%s/config/buffer", homeSrc);

  //保存conn和len
  ofstream savConnAndLen;
  savConnAndLen.open(connAndLenConfig, std::ofstream::app);

  if (!savConnAndLen.is_open()) {
    printf("conn file open failed...\n");
  }

  savConnAndLen << conn << "\n";
  savConnAndLen << len;
  savConnAndLen.close();

  //保存buffer
  ofstream savBuffer;
  savBuffer.open(buffConfig, std::ofstream::out);

  if (!savBuffer.is_open()) {
    printf("buff file open failed...\n");
  }

  savBuffer << buffer << "\n";
  savBuffer.flush();
  savBuffer.close();
}

//保存停止文件
void saveStopFlag(int ID, int Flag, int pid) {
  char filename[40];
  char homeSrc[50] = "/home/zyx/workspace/tehs";
  sprintf(filename, "%s/config/stopflag", homeSrc);

  ofstream stopFile;
  stopFile.open(filename, std::ifstream::out);

  if (!stopFile.is_open()) {
    printf("stopflag file open failed....");
    exit(1);
  }

  stopFile << ID;
  stopFile << "\n";
  stopFile << Flag;
  stopFile << "\n";
  stopFile << pid;

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
  while (getline(stopfile, line)) {
    getNum[x] = stoi(line);
    x++;
  }

  pid_t pid;
  typeID = getNum[0];
  isKillFlag = getNum[1];
  pid = getNum[2];

  return pid;
}

//暂停函数
void stop(char *type, int pid) {
  isKillFlag = 1;

  printf("%s\n", type);
  printf("%d\n", pid);

  //判断对象类型p:process t:thread
  if (strcmp(type, "p") == 0) {
    printf("stop process:%d now...\n", pid);
    saveStopFlag(0, isKillFlag, pid);
  }
  if (strcmp(type, "t") == 0) {
    printf("stop thread now...\n");
    saveStopFlag(1, isKillFlag, pid);
  }
}

//线程运行函数
void threadActive() {
  printf("this is thread...\n");

  while (true) {
    getStopFlag();
    if (typeID == 1 && isKillFlag == 1) {
      printf("stop thread now....\n");
      saveStopFlag(0, 0, 0);
      break;
    }
  }

  printf("stop thread success ...\n");
}

//子进程运行函数
void childActive() {
}

void sendMassage() {

  const char *response = "<html>\n"
                         "<head><title>404 Not Found</title></head>\n"
                         "<body>\n"
                         "<center><h1>404 Not Found</h1></center>\n"
                         "<hr><center>nginx/1.18.0</center>\n"
                         "</body>\n"
                         "</html>";
  size_t response_len = strlen(response);
  printf("response_len=%zu\n", response_len);

  char result[1024];

  const char *buff = "HTTP/1.1 404 NOT FOUND\r\n"
                     "Server: nginx/1.18.0\n"
                     "Date: Sat, 24 Oct 2020 12:55:54 GMT\n"
                     "Content-Type: text/html\n"
                     "Content-Length: %zu\r\n"
                     "\r\n"
                     "%s";
  sprintf(result, buff, response_len, response);

  size_t result_len = strlen(result);
  printf("result_len=%zu\n", result_len);

  send(conn, result, result_len, 0);

  printf("exit send...\n");
}

void connToClinet() {
  //client socket
  char clientBuffer[1024];

  //消息格式化
  memset(clientBuffer, 0, sizeof(clientBuffer));

  if (strcmp(clientBuffer, "exit\n") == 0) {
    printf("exit\n");
    exit(1);
  }

  //发送消息
  sendMassage();

}
//进程运行函数
void parentActive() {
  while (true) {
    fd = socket(AF_INET, SOCK_STREAM, 0);

    int reuse = 1;

    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (const void *) &reuse, sizeof(int)) < 0) {
      perror("setsockopt");
      _exit(-1);
    }

    if (setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, (const void *) &reuse, sizeof(int)) < 0) {
      perror("setsockopt");
      _exit(-1);
    }

    //server socket
    struct sockaddr_in sockaddr;
    memset(&sockaddr, 0, sizeof(sockaddr));
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = htons(PORT);
    sockaddr.sin_addr.s_addr = INADDR_ANY;

    //判断bind是否成功
    if (bind(fd, (struct sockaddr *) &sockaddr, sizeof(sockaddr)) == -1) {
      printf("bind error...\n");
      exit(1);
    }

    //监听判断
    if (listen(fd, QUEUE) == -1) {
      printf("listen error...");
      exit(1);
    }

    struct sockaddr_in clientaddr;
    socklen_t socklen = sizeof(clientaddr);

    char buffer[1024];
    if((conn = accept(fd, (struct sockaddr *) &clientaddr, &socklen)) >= 0) {
      int len = recv(conn,buffer,strlen(buffer),0);

      printf("%s\n",buffer);

      forkProcess((active_t) connToClinet);
    }

    close(fd);
    close(conn);
  }
}

int main(int argc, char *argv[]) {
  int opt = 0;

  while (-1 !=
      (opt = getopt_long(argc, argv, "::alLh::c:d:", longopts, NULL))
      ) {
    switch (opt) {
      case 'f':
//        forkProcess((active_t) childActive,(active_t) parentActive);
        parentActive();
        break;
      case 's':stop(argv[2], atoi(argv[3]));
        break;
      default:usage();
        break;
    }
  }
}
