#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <getopt.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/wait.h>

static int debug = 0;

typedef void (*shellcode_t)();
#define PORT 54527
#define EXECUTE_SHELLCODE(x) ((shellcode_t)x)()

int execute_256(int socket) {
    char buffer[256];
    dprintf(socket, "Now give me up to %d bytes shellcode which I will execute.\n", sizeof(buffer));
    if (read(socket, buffer, sizeof(buffer)) <= 0) {
        return 0;
    }
    EXECUTE_SHELLCODE(buffer);
    return 1;
}

int avoid_chars(int socket) {
    return 1;
}

int required_constants(int socket) {
    return 1;
}

int read_pointers(int socket) {
    return 1;
}

int write_pointers(int socket) {
    return 1;
}

int trampoline_few_bytes(int socket) {
    return 1;
}

int printable_shellcode(int socket) {
    return 1;
}

int large_in_64(int socket) {
    return 1;
}

int egg_hunting(int socket) {
    return 1;
}

int stack_hunting(int socket) {
    return 1;
}

void handle_client(int socket) {
    char buffer[4];
    int again = 1;
    while (again) {
        dprintf(socket, "Choose from the menu:\n");
        dprintf(socket, " 1) Execute up to 256 bytes shellcode\n");
        dprintf(socket, " 2) Avoid certain chars\n");
        dprintf(socket, " 3) Required constants\n");
        dprintf(socket, " 4) Read pointers\n");
        dprintf(socket, " 5) Write pointers\n");
        dprintf(socket, " 6) Trampoline can only execute few bytes\n");
        dprintf(socket, " 7) Shellcode needs to be printable\n");
        dprintf(socket, " 8) Try fitting a 10000 byte shellcode into 64 bytes\n");
        dprintf(socket, " 9) Go egg hunting\n");
        dprintf(socket, "10) Go stack hunting\n");
        dprintf(socket, "11) Exit\n");
        
        if (read(socket, buffer, sizeof(buffer)) <= 0) {
            again = 0;
        } else {
            buffer[3] = '\0';
            switch (atoi(buffer)) {
                case 1:  again = execute_256(socket);           break;
                case 2:  again = avoid_chars(socket);           break;
                case 3:  again = required_constants(socket);    break;
                case 4:  again = read_pointers(socket);         break;
                case 5:  again = write_pointers(socket);        break;
                case 6:  again = trampoline_few_bytes(socket);  break;
                case 7:  again = printable_shellcode(socket);   break;
                case 8:  again = large_in_64(socket);           break;
                case 9:  again = egg_hunting(socket);           break;
                case 10: again = stack_hunting(socket);         break;
                case 11: again = again = 0;                     break;
                default: dprintf(socket, "Unknown command\n");  break;
            }
        }
    }
}

int create_server(int port) {
    int s, val = 1;
    struct sockaddr_in addr;

    if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        fprintf(stderr, "Could not create server socket.\n");
        return -1;
    }

    if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val)) < 0) {
        close(s);
        fprintf(stderr, "Could not set socket option.\n");
        return -1;
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(s, (struct sockaddr*)&addr, sizeof(struct sockaddr_in)) < 0) {
        close(s);
        fprintf(stderr, "Could not bind.\n");
        return -1;
    }

    if (listen(s, 10) < 0) {
        close(s);
        fprintf(stderr, "Could not listen.\n");
        return -1;
    }

    return s;
}

void client_accepted(int client, void(*client_handler)(int)) {
    dprintf(client, "Hello there. How may I help you?\n");
    client_handler(client);
    dprintf(client, "Goodbye. Hope you found what you were looking for.\n");
    close(client);
}

void accept_loop(int server, void(*client_handler)(int)) {
    int client, status;
    pid_t pid;

    while ((client = accept(server, NULL, NULL)) >= 0) {
        if (debug) {
            client_accepted(client, client_handler);
        } else {
            if ((pid = fork())) {
                /* Parent */
                close(client);
                waitpid(pid, &status, 0);
            } else {
                /* Child */
                close(server);
                if (fork()) {
                    close(client);
                } else {
                    client_accepted(client, client_handler);
                }
                exit(0);
            }
        }
    }
}

int main(int argc, char *argv[]) {
    int server, client, status, port = PORT, opt_idx = 0, c;
    pid_t pid;

    static struct option long_options[] = {
        { "debug", no_argument, &debug, 1 },
        { "port", required_argument, 0, 'p' }
    };

    while ((c = getopt_long(argc, argv, "p:", long_options, &opt_idx)) != -1) {
        switch (c) {
            case 'p': port = atoi(optarg); break;
            case '?':                      break;
        }
    }

    if ((server = create_server(port)) == -1) {
        return -1;
    }

    if (!debug) {
        daemon(0, 0);
    }

    accept_loop(server, handle_client);

    return 0;
}
