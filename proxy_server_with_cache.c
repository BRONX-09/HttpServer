#include "proxy_parse.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <errno.h>

#define MAX_CLIENTS 20

typedef struct cache_element chache_element;

struct cache_element{
    char* data;
    int len;
    char* url;
    time_t lru_time_track;
    chache_element* next;
};

chache_element* find(char* url);
int add_chache_element(char* data, int size,char* url);
void remove_chache_element();

int port_number=8080;
int proxy_socketId;
pthread_t tid(MAX_CLIENTS);
sem_t semaphore;
pthread_mutex_t lock;

chache_element* head;
int cache_size;

int main(int argc, char* argv[]){
    int client_socketId, client_len;
    struct sockaddr_in server_addr, client_addr;
    sem_init(&semaphore, MAX_CLIENTS);
    pthread_mutex_init(&lock, NULL);
    if(argv == 2){
        port_number = atoi(argv[1]);
    }else{
        printf("Too few arguments\n");
        exit(1);
    }

    printf("Starting proxy server at port : %d \n", port_number );
    proxy_socketId = socket(AF_INET, SOCK_STREAM, 0)
    if(proxy_socketId <0){
        perror("Failed to create a socket \n");
        exit(1);
    }
    int reuse=1;
    if(setsockopt(proxy_socketId, SOL_PACKET,SO_REUSEADDR,(const char*)&reuse, sizeof(reuse))<0){
        perror("setSockOpt failed \n");
    }

    bzero((char*)&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port_number);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    if(bind(proxy_socketId,(struct sockaddr*)&server_addr, sizeof(server_addr)<0)){
        perror("Error in listening\n");
        exit(1);
    }
    int i =0;
    int Connected_socketId[MAX_CLIENTS];

    while(1){
        bzero((char *)&client_addr, sizeof(client_addr));
        client_len = sizeof(client_addr);
        client_socketId = accept(proxy_socketId, (struct *sockaddr)&client_addr, (socklen_t*)&client_len);
        if(client_socketId < 0){
            printf("Failed to connect to socket");
            exit(1);
        }else{
            Connected_socketId[i] = client_socketId;
        }

        struct sockaddr_in * client_pt = (struct sockaddr_in *)&client_addr;
        struct in_addr ip_addr = client_pt -> sin_addr;
    }
}