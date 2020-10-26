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
#include <sstream>
#include "../libtehs/forkProcess.h"

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
const char *Content_Type;
const char *sendFile;

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


void sendMassage() {

  const char *response;

  const char *indexHome;
  indexHome = "/home/zyx/workspace/tehs/html/django-blog-tutorial-templates%s";

  char sendFiles[100];

  sprintf(sendFiles, indexHome, sendFile);

  ifstream hltmlfile(sendFiles);
  ostringstream tmp;
  tmp << hltmlfile.rdbuf();
  std::string str = tmp.str();
  response = str.c_str();

  size_t response_len = strlen(response);

  printf("response_len=%zu\n", response_len);

  char result[1000000];


  const char *buff = "HTTP/1.1 200 OK\r\n"
                     "Server: nginx/1.18.0\n"
                     "Date: Sat, 24 Oct 2020 12:55:54 GMT\n"
                     "Content-Type: %s\n"
                     "Content-Length: %zu\r\n"
                     "\r\n"
                     "%s";
  sprintf(result, buff, Content_Type,response_len, response);

//  printf("Content_Type=%s\n", Content_Type);

//  printf("%s\n", result);
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
void PortListener() {
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
}
//进程运行函数
[[noreturn]] void parentActive() {
  while (true) {
    PortListener();

    struct sockaddr_in clientaddr;
    socklen_t socklen = sizeof(clientaddr);

    char buffer[1024];
    string bufferstr;
    if((conn = accept(fd, (struct sockaddr *) &clientaddr, &socklen)) >= 0) {
      recv(conn,buffer,sizeof(buffer),0);

      bufferstr = buffer;


      //测试代码
      size_t acceptInBuffer = bufferstr.find("Accept: ");
      size_t acceptInBuffer1 = bufferstr.find("\r\n",acceptInBuffer+1);

      int size = acceptInBuffer1 - acceptInBuffer;
      int size1 = acceptInBuffer + sizeof("Accept: ") -1;

      string acceptString = bufferstr.substr(size1,size);

//      string flag = "text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9";

      if (acceptString.size() > 50){
        Content_Type = "text/html";
      }else{
        Content_Type =acceptString.c_str();
      }

      printf("%s\n",acceptString.c_str());


      //
      size_t getInBuffer = bufferstr.find("GET ");
      size_t getInBuffer1 = bufferstr.find("HTTP/1.1",getInBuffer+1);

      int size3 = getInBuffer1 - getInBuffer - 5;
      int size4 = getInBuffer + sizeof("GET ")-1;

      string getString = bufferstr.substr(size4,size3);

      printf("%s\n",getString.c_str());


      if (getString=="/"){
        sendFile="/index.html";
      }else{
        sendFile = getString.c_str();
      }




      forkProcess((active_t) connToClinet);
//      connToClinet();
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
        parentActive();
      case 's':stop(argv[2], atoi(argv[3]));
        break;
      default:usage();
        break;
    }
  }
}
