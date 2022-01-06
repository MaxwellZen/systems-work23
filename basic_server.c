#include "pipe_networking.h"

char computer[] = {0xf0, 0x9f, 0xa7, 0x91, 0xf0, 0x9f, 0x8f, 0xbb, 0xe2, 0x80, 0x8d, 0xf0, 0x9f, 0x92, 0xbb, '\0'};
int f = 0;

static void sighandler(int signo) {
    if (signo==SIGINT) {
        if (f) printf("\n%s Closing down server %s\n", computer, computer);
        exit(0);
    }
}

int main() {

    signal(SIGINT, sighandler);

    int client;
    int listener;

    printf("%s Starting up server %s\n", computer, computer);

    listener = server_setup();

    while (1) {
        client = server_connect(listener);
        if (client==-1) continue;
        printf("Connection: %d\n", client);
        f = fork();
        if (f) {
            close(client);
        } else {
            char buf[1000];
            for (int i = 0; i < 1000; i++) buf[i]=0;

            while (read(client, buf, 1000) != 0) {
                for (int i = 0; buf[i]; i++) {
                    if (buf[i] >= 'a' && buf[i] <= 'z') buf[i] = buf[i] - 'a' + 'A';
                }
                write(client, buf, 1000);
                for (int i = 0; buf[i]; i++) buf[i] = 0;
            }
            exit(0);
        }
    }
}
