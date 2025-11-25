#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>  // Добавьте эту строку для inet_addr

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return 1;
    }
    
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(20001);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect");
        close(sockfd);
        return 1;
    }
    
    // Отправляем тестовые данные: begin=1, end=2, mod=9
    uint64_t begin = 1, end = 2, mod = 9;
    char buffer[sizeof(uint64_t) * 3];
    memcpy(buffer, &begin, sizeof(uint64_t));
    memcpy(buffer + sizeof(uint64_t), &end, sizeof(uint64_t));
    memcpy(buffer + 2 * sizeof(uint64_t), &mod, sizeof(uint64_t));
    
    if (send(sockfd, buffer, sizeof(buffer), 0) < 0) {
        perror("send");
        close(sockfd);
        return 1;
    }
    
    // Получаем ответ
    char response[sizeof(uint64_t)];
    if (recv(sockfd, response, sizeof(response), 0) < 0) {
        perror("recv");
        close(sockfd);
        return 1;
    }
    
    uint64_t result;
    memcpy(&result, response, sizeof(uint64_t));
    printf("Received result: %lu\n", (unsigned long)result);
    
    close(sockfd);
    return 0;
}