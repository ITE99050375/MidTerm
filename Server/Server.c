#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int atgc, char *argv[])
{
    int socket_desc, new_socket, c;
    struct sockaddr_in server, client;
    char *reply_message;
    char recv_message2[4];
    int guess_num[4] = {0, 0, 0, 0};
    int answer[4] = {0, 0, 0, 0};
    int recv_message[4] = {0, 0, 0, 0};
    int result[2] = {0, 0};
    int i, j;
    int flag = 0;

    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1)
    {
        printf("Connot create socket");
    }
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8888);
    if (bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        puts("bind failed");
        return 1;
    }
    puts("bind done");
    listen(socket_desc, 3);
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
    while ((new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t *)&c)))
    {
        if (new_socket < 0)
        {
            perror("accept failed");
            return 1;
        }
        puts("Connection accepted");

        srand(time(NULL));
        for (i = 0; i < 4;)
        {
            guess_num[i] = rand() % 10;
            for (j = 0; j < i; j++)
            {
                if (guess_num[j] == guess_num[i])
                {
                    flag = 1;
                    break;
                }
                flag = 0;
            }
            if (flag == 0)
            {
                i++;
            }
        }

        puts("number:");
        for (int i = 0; i < 4; i++)
        {
            printf("%d", guess_num[i]);
        }
        printf("\n");
        while (1)
        {
            if (recv(new_socket, recv_message2, 4, 0) < 0)
            {
                perror("recv error");
                break;
            }
            else
            {
                recv_message[0] = recv_message2[0];
                recv_message[1] = recv_message2[1];
                recv_message[2] = recv_message2[2];
                recv_message[3] = recv_message2[3];
                puts("Data Received:");
                answer[0] = (char)recv_message[0] - '0';
                answer[1] = (char)recv_message[1] - '0';
                answer[2] = (char)recv_message[2] - '0';
                answer[3] = (char)recv_message[3] - '0';
                result[0] = 0;
                result[1] = 0;
                for (int i = 0; i < 4; i++)
                {
                    if (answer[i] == guess_num[i])
                    {
                        result[0]++;
                        continue;
                    }
                    for (int j = 0; j < 4; j++)
                    {
                        if (answer[i] == guess_num[j])
                        {
                            result[1]++;
                            break;
                        }
                    }
                }
                if (result[0] == 4)
                {
                    reply_message = "success";
                    write(new_socket, reply_message, strlen(reply_message));
                    puts("success");
                    break;
                }
                else
                {
                    reply_message = (char *)malloc(sizeof(char) * 4);
                    sprintf(reply_message, "%dA%dB", result[0], result[1]);
                    write(new_socket, reply_message, 4);
                    puts("theResult:");
                    puts(reply_message);
                    free(reply_message);
                    result[0] = 0;
                    result[1] = 0;
                }
            }
        }
    }
    return 0;
}
