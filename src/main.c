#include <getopt.h>
#include <zconf.h>
#include <ares_build.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include "../libtehs/tehs_process_cycle.h"



int fd;
int conn;
int tehs_spawn_process = 0;

struct option longopts[] = {
    {"run", no_argument, NULL, 'r'},
    {"stop", required_argument, NULL, 's'},
    {"list", no_argument, NULL, 'l'},
    {"help", no_argument, NULL, 'h'},
    {NULL, 0, NULL, 0},
};


void usage() {
  fprintf(stderr,
          "\n"
          "Usage: tehs [options]\n"
          "\n"
          "Options:\n"
          "    -r, --run       start program\n"
          "    -s, --stop     "
          "stop the process or thread\n"
          "    -l, --list       list all proc\n"
          "    -h, --help      help\n");

  exit(0);
}

static int tehs_proccess_options();


static int tehs_get_options(int argc, char *const *argv);

int main(int argc, char *const *argv) {
    if(tehs_spawn_process == 1){
        tehs_master_process_cycle();
    }


}


int tehs_get_options(int argc, char *const *argv){
    int opt = 0;

    while (-1 !=
           (opt = getopt_long(argc, argv, "::alLh::c:d:", longopts, NULL))
            ) {
        switch (opt) {
            case 'r':
                tehs_spawn_process=1;
            case 's':

                break;
            case 'l':

                break;
            case '?':
            case 'h':
                usage();
                break;
            default:usage();
                break;
        }
    }
    return 0;
}
//
//void list() {
//    string filename;
//    filename = home_src + "/config/proc/fatherProcess";
//
//    ifstream list_file;
//    list_file.open(filename, std::ifstream::in);
//
//    string line;
//    while (getline(list_file, line)) {
//        cout << line;
//    }
//
//    list_file.close();
//}
//
////暂停函数
//void stop(char *type, int pid) {
//    isKillFlag = 1;
//
//    printf("%s\n", type);
//    printf("%d\n", pid);
//
//    //判断对象类型p:process t:thread
//    if (strcmp(type, "p") == 0) {
//        printf("stop process:%d now...\n", pid);
//        kill(pid, SIGINT);
//    }
//}
//
//void sendMassage() {
//    string index_home = "/html";
//    string send_files;
//
//    send_files = home_src + index_home + send_file;
//
//    ifstream html_file(send_files, std::fstream::binary);
//
//    stringstream ss;
//    ss << html_file.rdbuf();
//
//    string buff = "HTTP/1.1 200 OK\r\n"
//                  "Server: nginx/1.18.0\n"
//                  "Date: Sat, 24 Oct 2020 12:55:54 GMT\n"
//                  "Content-Type: " + content_type + "\n""Content-Length:" + to_string(ss.str().size()) + "\r\n\r\n";
//    string result = buff + ss.str();
//
//    send(conn, result.c_str(), result.size(), 0);
//
//    printf("exit send...\n");
//}
//
//void conn_to_clinet() {
//    //client socket
//    char client_buffer[1024];
//
//    //消息格式化
//    memset(client_buffer, 0, sizeof(client_buffer));
//
//    if (strcmp(client_buffer, "exit\n") == 0) {
//        printf("exit\n");
//        exit(1);
//    }
//
////  save_child_id(home_src,getpid(),"send");
//    //发送消息
//    sendMassage();
//
//}
//
//void sig_handler(int i) {
//    while (true) {
//        string cmd = "ls /home/zyx/workspace/tehs/tmp";
//        FILE *stream = popen(cmd.c_str(), "r");
//
//        char *buffer;
//        if (stream != NULL) {
//            // 每次从 stream 中读取指定大小的内容
//            while (fgets(buffer, 100, stream))
//                printf(buffer);
//            if (buffer == NULL)
//                exit(1);
//
//            pclose(stream);
//        }
//        exit(i);
//    }
//}
//
//int listen_and_accept(int port) {
//    signal(SIGINT, sig_handler);
//    fd = socket(AF_INET, SOCK_STREAM, 0);
//
//    int reuse = 1;
//
//    //设置地址复用
//    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (const void *) &reuse, sizeof(int)) < 0) {
//        perror("setsockopt");
//        _exit(-1);
//    }
//
//    //设置端口复用
//    if (setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, (const void *) &reuse, sizeof(int)) < 0) {
//        perror("setsockopt");
//        _exit(-1);
//    }
//
//    //server socket
//    struct sockaddr_in sockaddr;
//    memset(&sockaddr, 0, sizeof(sockaddr));
//    sockaddr.sin_family = AF_INET;
//    sockaddr.sin_port = htons(port);
//    sockaddr.sin_addr.s_addr = INADDR_ANY;
//
//    //判断bind是否成功
//    if (bind(fd, (struct sockaddr *) &sockaddr, sizeof(sockaddr)) == -1) {
//        printf("bind error...\n");
//        exit(1);
//    }
//
//    //监听判断
//    if (listen(fd, QUEUE) == -1) {
//        printf("listen error...");
//        exit(1);
//    }
//
//    struct sockaddr_in client_addr;
//    socklen_t socket_len = sizeof(client_addr);
//
//    conn = accept(fd, (struct sockaddr *) &client_addr, &socket_len);
//
//    return conn;
//}
//
//void set_home_src() {
//    string bufferstr;
//    char *buffer;
//    //也可以将buffer作为输出参数
//    if((buffer = getcwd(NULL, 0)) == NULL)
//    {
//        perror("getcwd error");
//    }
//    else
//    {
//        bufferstr = buffer;
//        home_src = bufferstr;
//        free(buffer);
//    }
//}
//
//void parentActive() {
//    set_home_src();
//
//    printf("%d\n", getpid());
//    while (true) {
//        int conn = listen_and_accept(PORT);
//
//        char buffer[1024];
//        string buffer_str;
//        if (conn >= 0) {
//            recv(conn, buffer, sizeof(buffer), 0);
//
//            buffer_str = buffer;
//
//            //获取accept
//            size_t accept_start_point = buffer_str.find("Accept: ");
//            size_t accept_end_point = buffer_str.find("\r\n", accept_start_point + 1);
//
//            int len_of_accept = accept_end_point - accept_start_point;
//            int accept_value_point = accept_start_point + sizeof("Accept: ") - 1;
//
//            string accept_value = buffer_str.substr(accept_value_point, len_of_accept);
//
//            //accept判断
//            if (accept_value.size() > 50) {
//                content_type = "text/html";
//            } else {
//                content_type = accept_value;
//            }
//
//            //获取路由路径
//            size_t fist_line_start_point = buffer_str.find("GET ");
//            size_t fist_line_end_point = buffer_str.find("HTTP/1.1", fist_line_start_point + 1);
//
//            int len_of_fist_line = fist_line_end_point - fist_line_start_point - 5;
//            int fist_line_value_point = fist_line_start_point + sizeof("GET ") - 1;
//
//            string get_value = buffer_str.substr(fist_line_value_point, len_of_fist_line);
//
//            //路径判断
//            if (get_value == "/") {
//                send_file = "/index.html";
//            } else if (get_value.find(".jpg") != string::npos) {
//                content_type = "image/jpeg";
//                send_file = get_value;
//            } else if (get_value.find(".png") != string::npos) {
//                content_type = "image/png";
//                send_file = get_value;
//            } else {
//                send_file = get_value;
//            }
//
//            fork_new_proc((active_t) conn_to_clinet);
//
//        }
//        close(fd);
//        close(conn);
//    }
//}
//
//static void
//tehs_master_process_cycle(){
//
//}
