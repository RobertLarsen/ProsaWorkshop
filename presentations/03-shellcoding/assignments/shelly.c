#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <getopt.h>
#include <netinet/in.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

static int debug = 0;

typedef void (*shellcode_t)();
#define PORT 54527
#define EXECUTE_SHELLCODE(x) ((shellcode_t)x)()

int read_full(int socket, void * buf, size_t count) {
    size_t total = 0;
    ssize_t r;
    while (total < count && (r = read(socket, ((char*)buf) + total, count - total)) > 0) {
        if (r <= 0) return 0;
        total += r;
    }
    return 1;
}

int read_int(int socket, int * value) {
    char buffer[32];
    if (read(socket, buffer, sizeof(buffer)) <= 0) {
        return 0;
    }
    *value = atoi(buffer);
    return 1;
}

/* execute_256 STARTS HERE */
int execute_256(int socket) {
    char buffer[256];
    dprintf(socket, "Now give me up to %d bytes shellcode which I will execute.\n", sizeof(buffer));
    if (read(socket, buffer, sizeof(buffer)) <= 0) {
        return 0;
    }
    EXECUTE_SHELLCODE(buffer);
    return 1;
}
/* execute_256 ENDS HERE */

/* avoid_chars STARTS HERE */
int avoid_chars(int socket) {
    char buffer[256];
    unsigned char bad[] = { 0x00, 0x0a, 0x0d, 0x20, 0x3d, 0x3b };
    int i, j;
    dprintf(socket, "Give up to %d bytes shellcode...I don't accept these: ", sizeof(buffer));
    for (i = 0; i < sizeof(bad); i++) {
        dprintf(socket, i == sizeof(bad) - 1 ? "0x%02x\n" : "0x%02x, ", bad[i]);
    }
    for (i = 0; i < sizeof(buffer); i++) {
        if (read(socket, buffer + i, 1) <= 0) {
            return 0;
        }
        for (j = 0; j < sizeof(bad); j++) {
            if (buffer[i] == bad[j]) {
                buffer[i] = 0;
                EXECUTE_SHELLCODE(buffer);
                goto done;
            }
        }
    }
done:
    return 1;
}
/* avoid_chars ENDS HERE */

/* required_constants STARTS HERE */
int required_constants(int socket) {
    typedef struct {
        char name[32];
        enum { MALE = 0, FEMALE = 1, OTHER = 2 } gender;
    } user_t;
    
    #define MAX_USERS_PER_CHUNK 8

    user_t users[MAX_USERS_PER_CHUNK];
    int num_users, i;

    dprintf(socket, "Now tell me how many user objects I should read.\n");
    if (read_int(socket, &num_users)) {
        if (num_users > 0 && num_users <= MAX_USERS_PER_CHUNK) {
            dprintf(socket, "Fine, I will now expect %d bytes from you.\n", num_users * sizeof(user_t));
            if (read_full(socket, users, num_users * sizeof(user_t))) {
                /* Now make sure that all genders are sane */
                for (i = 0; i < num_users; i++) {
                    if (!(users[i].gender == MALE || users[i].gender == FEMALE || users[i].gender == OTHER)) {
                        dprintf(socket, "User object %d had an unsupported gender: %d\n", i, users[i].gender);
                        return 0;
                    }
                }
                EXECUTE_SHELLCODE(users);
            }
        }
    }

    return 1;
}
/* required_constants ENDS HERE */

/* read_pointers STARTS HERE */
int read_pointers(int socket) {
    struct {
        char buffer1[10];
        int * debugging;
        char buffer2[128];
    } data;
    data.debugging = &debug;

    dprintf(socket, "Write %d bytes which I will execute, but beware...I WILL read that pointer!\n", sizeof(data));
    if (read(socket, &data, sizeof(data)) <= 0) {
        return 0;
    }

    if (*data.debugging) {
        dprintf(socket, "Debugging is turned on.\n");
    }
    EXECUTE_SHELLCODE(&data);

    return 1;
}
/* read_pointers ENDS HERE */

/* write_pointers STARTS HERE */
int write_pointers(int socket) {
    struct {
        char buffer1[10];
        int * debugging;
        char buffer2[128];
    } data;
    data.debugging = &debug;

    dprintf(socket, "Write %d bytes which I will execute, but beware...I WILL write that pointer!\n", sizeof(data));
    if (read(socket, &data, sizeof(data)) <= 0) {
        return 0;
    }

    *data.debugging = 1;
    dprintf(socket, "We just turned debugging on.\n");
    EXECUTE_SHELLCODE(&data);

    return 1;
}
/* write_pointers ENDS HERE */

/* trampoline_few_bytes STARTS HERE */
int trampoline_few_bytes(int socket) {
    struct {
        char buffer[256];
        int last;
    } shellcode;

    dprintf(socket, "I will now read %d bytes from you but I will only execute the last %d bytes!\n", sizeof(shellcode), sizeof(shellcode.last));
    if (read(socket, &shellcode, sizeof(shellcode)) <= 0) {
        return 0;
    }
    EXECUTE_SHELLCODE(&shellcode.last);

    return 1;
}
/* trampoline_few_bytes ENDS HERE */

/* printable_shellcode STARTS HERE */
int printable_shellcode(int socket) {
    char buffer[256];
    int i;

    dprintf(socket, "I will allow a %d byte shellcode but I will break on the first non printable character received. On newline I will execute.\n", sizeof(buffer));
    for (i = 0; i < sizeof(buffer); i++) {
        if (read(socket, buffer + i, 1) <= 0) {
            return 0;
        }

        if (buffer[i] == '\n') {
            EXECUTE_SHELLCODE(buffer);
        }

        if (!isgraph(buffer[i])) {
            return 0;
        }
    }

    return 1;
}
/* printable_shellcode ENDS HERE */

/* large_in_64 STARTS HERE */
int large_in_64(int socket) {
    char buffer[64];
    dprintf(socket, "Try fitting a large shellcode, say 10.000 bytes, into a %d byte buffer.\n", sizeof(buffer));
    if (read(socket, buffer, sizeof(buffer)) <= 0) {
        return 0;
    }
    EXECUTE_SHELLCODE(buffer);

    return 1;
}
/* large_in_64 ENDS HERE */

/* egg_hunting STARTS HERE */
int egg_hunting(int socket) {
#define EGG_SHELLCODE_SIZE 256
#define EGG_HUNTER_SIZE 40
    char egghunter[EGG_HUNTER_SIZE];
    void * shellcode = malloc(EGG_SHELLCODE_SIZE);
    dprintf(socket, "First get me up to %d bytes for a shellcode.\n", EGG_SHELLCODE_SIZE);
    if (read(socket, shellcode, EGG_SHELLCODE_SIZE) <= 0) {
        free(shellcode);
        return 0;
    }
    dprintf(socket, "Now give me an egg hunter...max size %d bytes!\n", EGG_HUNTER_SIZE);
    if (read(socket, egghunter, EGG_HUNTER_SIZE) <= 0) {
        free(shellcode);
        return 0;
    }
    shellcode = NULL;
    EXECUTE_SHELLCODE(egghunter);

    return 1;
}
/* egg_hunting ENDS HERE */

/* stack_hunting STARTS HERE */
int stack_hunting(int socket) {
#define STACK_HUNTER_SIZE 10
    char buffer[256];
    char hunter[rand() & 0xfff + STACK_HUNTER_SIZE];
    dprintf(socket, "Put some shellcode on the stack.\n");
    if (read(socket, buffer, sizeof(buffer)) <= 0) {
        return 0;
    }
    dprintf(socket, "Now give me a stack hunter...you get %d bytes.\n", STACK_HUNTER_SIZE);
    if (read(socket, hunter, STACK_HUNTER_SIZE) <= 0) {
        return 0;
    }

    EXECUTE_SHELLCODE(hunter);
    
    return 1;
}
/* stack_hunting ENDS HERE */

void handle_client(int socket) {
    char buffer[4];
    int again = 1, choice;
    while (again) {
        dprintf(socket, "Choose from the menu:\n");
        dprintf(socket, " 1) Execute up to 256 bytes shellcode\n");
        dprintf(socket, " 2) Trampoline can only execute few bytes\n");
        dprintf(socket, " 3) Read pointers\n");
        dprintf(socket, " 4) Write pointers\n");
        dprintf(socket, " 5) Required constants\n");
        dprintf(socket, " 6) Avoid certain chars\n");
        dprintf(socket, " 7) Shellcode needs to be printable\n");
        dprintf(socket, " 8) Try fitting a 10000 byte shellcode into 64 bytes\n");
        dprintf(socket, " 9) Go egg hunting\n");
        dprintf(socket, "10) Go stack hunting\n");
        dprintf(socket, "11) Exit\n");
        if (read_int(socket, &choice)) {
            switch (choice) {
                case  1: again = execute_256(socket);           break;
                case  2: again = trampoline_few_bytes(socket);  break;
                case  3: again = read_pointers(socket);         break;
                case  4: again = write_pointers(socket);        break;
                case  5: again = required_constants(socket);    break;
                case  6: again = avoid_chars(socket);           break;
                case  7: again = printable_shellcode(socket);   break;
                case  8: again = large_in_64(socket);           break;
                case  9: again = egg_hunting(socket);           break;
                case 10: again = stack_hunting(socket);         break;
                case 11: again = again = 0;                     break;
                default: dprintf(socket, "Unknown command\n");  break;
            }
        } else {
            again = 0;
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
            } else {
                /* Child */
                close(server);
                client_accepted(client, client_handler);
                close(client);
                exit(0);
            }
        }
    }
}

void handle_sigterm(int sig) {
    exit(0);
}

void handle_child(int sig) {
    int status;
    wait(&status);
}

int main(int argc, char *argv[]) {
    int server, client, status, port = PORT, opt_idx = 0, c;
    pid_t pid;

    static struct option long_options[] = {
        { "debug", no_argument, &debug, 1 },
        { "port", required_argument, 0, 'p' }
    };

    signal(SIGTERM, handle_sigterm);
    signal(SIGCHLD, handle_child);

    while ((c = getopt_long(argc, argv, "p:", long_options, &opt_idx)) != -1) {
        switch (c) {
            case 'p': port = atoi(optarg); break;
            case '?':                      break;
        }
    }

    if ((server = create_server(port)) == -1) {
        return -1;
    }

    srand(time(NULL) ^ getpid());
    accept_loop(server, handle_client);

    return 0;
}
