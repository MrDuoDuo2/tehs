#include "../libtehs/fork_process.h"
#include <ares_build.h>
#include <fcntl.h>
#include <getopt.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zconf.h>

static int isKillFlag = 0;
static int typeID = 0;

//项目家目录
static char *home_src;

//监听的端口
#define PORT 9000
#define QUEUE 20

int fd;
int conn;
char *content_type;
char *send_file;
static proc_t proc_s[1024];

void parentActive();
void list();
void stop();
char * substr(char *str,int pos,int len);

int get_option(int argc, char *argv[]);


struct option longopts[] = {
    {"run", no_argument, NULL, 'r'},
    {"stop", required_argument, NULL, 's'},
    {"list", no_argument, NULL, 'l'},
    {"help", no_argument, NULL, 'h'},
    {NULL, 0, NULL, 0},
};


void usage() {
  printf("\n"
          "Usage: tehs [options]\n"
          "\n"
          "Options:\n"
          "    -r, --run       start program\n"
          "    -s, --stop     "
          "stop the process or thread\n"
          "    -l, --list       list all proc\n"
          "    -h, --help      help\n");

  exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]) {
  get_option(argc,argv);
//  while (-1 !=
//         (opt = getopt_long(argc, argv, "::alLh::c:d:", longopts, NULL))
//  ) {
//    switch (opt) {
//
//  }
}

int get_option(int argc, char *argv[]){
  char *p;

  for (int i = 1; i < argc; ++i) {
    p = argv[i];

    if (*p++ != '-') {
      printf("invalid option: \"%s\"", argv[i]);
      return 0;
    }

    while (*p) {
      switch (*p++) {
        case 'r':
          parentActive();
          break;
        case 's':stop(argv[2], atoi(argv[3]));
          break;
          //    case 'l':list();
          //      break;
        default:usage();
          break;
      }
    }
  }
}

//void list() {
//  char *filename;
//  filename = strcat(home_src,"/config/proc/fatherProcess");
//
//  int input_fd;
//
//  input_fd = open(filename,O_RDONLY);
//
//  char *line;
//
//  while (getline(list_file, line)) {
//    cout << line;
//  }
//
//  list_file.close();
//}

//暂停函数
void stop(char *type, int pid) {
  isKillFlag = 1;

  printf("%s\n", type);
  printf("%d\n", pid);

  //判断对象类型p:process t:thread
  if (strcmp(type, "p") == 0) {
    printf("stop process:%d now...\n", pid);
    kill(pid, SIGINT);
  }
}

void sendMassage() {
  char *index_home = "/html";
  char *send_files;

  send_files = strcat(home_src,strcat(index_home,send_file));

  int input_fd;
  char buf[1024];
  input_fd = open(send_files,O_RDONLY);

  read(input_fd,buf,1024);

  char *buff = strcat("HTTP/1.1 200 OK\r\n"
                "Server: nginx/1.18.0\n"
                "Date: Sat, 24 Oct 2020 12:55:54 GMT\n"
                "Content-Type: " ,strcat(content_type,strcat("\n""Content-Length:",strcat((char *)strlen(buf), "\r\n\r\n"))));
  char *result = strcat(buff,buf);
  send(conn, result, strlen(result), 0);

  printf("exit send...\n");
}

void conn_to_clinet() {
  //client socket
  char client_buffer[1024];

  //消息格式化
  memset(client_buffer, 0, sizeof(client_buffer));

  if (strcmp(client_buffer, "exit\n") == 0) {
    printf("exit\n");
    exit(1);
  }

//  save_child_id(home_src,getpid(),"send");
  //发送消息
  sendMassage();

}

void sig_handler(int i) {
  for(;;) {
    char *cmd = "ls /home/zyx/workspace/tehs/tmp";
    FILE *stream = popen(cmd, "r");

    char *buffer;
    if (stream != NULL) {
      // 每次从 stream 中读取指定大小的内容
      while (fgets(buffer, 100, stream))
        printf(buffer);
      if (buffer == NULL)
        exit(1);

      pclose(stream);
    }
    exit(i);
  }
}

int listen_and_accept(int port) {
  signal(SIGINT, sig_handler);
  fd = socket(AF_INET, SOCK_STREAM, 0);

  int reuse = 1;

  //设置地址复用
  if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (const void *) &reuse, sizeof(int)) < 0) {
    perror("setsockopt");
    _exit(-1);
  }

  //设置端口复用
  if (setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, (const void *) &reuse, sizeof(int)) < 0) {
    perror("setsockopt");
    _exit(-1);
  }

  //server socket
  struct sockaddr_in sockaddr;
  memset(&sockaddr, 0, sizeof(sockaddr));
  sockaddr.sin_family = AF_INET;
  sockaddr.sin_port = htons(port);
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

  struct sockaddr_in client_addr;
  socklen_t socket_len = sizeof(client_addr);

  conn = accept(fd, (struct sockaddr *) &client_addr, &socket_len);

  return conn;
}

void set_home_src() {
  char *bufferstr;
  char *buffer;
  //也可以将buffer作为输出参数
  if((buffer = getcwd(NULL, 0)) == NULL)
  {
    perror("getcwd error");
  }
  else
  {
    bufferstr = buffer;
    home_src = bufferstr;
    free(buffer);
  }
}

void parentActive() {
  last_proc_list = 0;
  int s;
  int pid;

  set_home_src();

  printf("%d\n", getpid());

  last_proc_list++;

  pid = fork();
  proc_s[0].pid = getpid();

  for (s = 0; s < last_proc_list; s++) {
    if (proc_s[s].pid==-1){
      break;
    }
  }

  proc_s[s].pid = pid;

  for (int i = 0; i < last_proc_list; i++) {
    printf("%d",proc_s[i].pid);
  }

  for (;;){

  }

  //  for (;;) {
//    int conn = listen_and_accept(PORT);
//
//    char buffer[1024];
//    char *buffer_str;
//    if (conn >= 0) {
//      recv(conn, buffer, sizeof(buffer), 0);
//
//      buffer_str = buffer;

//      //获取accept
//      size_t accept_start_point = buffer_str.find("Accept: ");
//      size_t accept_end_point = buffer_str.find("\r\n", accept_start_point + 1);
//
//      int len_of_accept = accept_end_point - accept_start_point;
//      int accept_value_point = accept_start_point + sizeof("Accept: ") - 1;
//
//      char *accept_value = substr(buffer_str,accept_value_point, len_of_accept);
//
//      //accept判断
//      if (strlen(accept_value) > 50) {
//        content_type = "text/html";
//      } else {
//        content_type = accept_value;
//      }
//
//      //获取路由路径
//      size_t fist_line_start_point = buffer_str.find("GET ");
//      size_t fist_line_end_point = buffer_str.find("HTTP/1.1", fist_line_start_point + 1);
//
//      int len_of_fist_line = fist_line_end_point - fist_line_start_point - 5;
//      int fist_line_value_point = fist_line_start_point + sizeof("GET ") - 1;
//
//      char *get_value = substr(buffer_str,fist_line_value_point, len_of_fist_line);

//      //路径判断
//      if (get_value[0] == "/") {
//        send_file = "/index.html";
//      } else if (get_value.find(".jpg") != string::npos) {
//        content_type = "image/jpeg";
//        send_file = get_value;
//      } else if (get_value.find(".png") != string::npos) {
//        content_type = "image/png";
//        send_file = get_value;
//      } else {
//        send_file = get_value;
//      }

//      fork_new_proc((active_t) conn_to_clinet);
//
//    }
//    close(fd);
//    close(conn);
//  }
}

static void
tehs_master_process_cycle(){

}

char * substr(char *str,int pos,int len){
  int t = 0;
  char *new_str;
  for (int i = pos; i < strlen(str); ++i) {
    new_str += str[pos];
    if (t == len){
      break;
    }
    if (str[i] == 0){
      return new_str;
    }
    t++;
  }
}


