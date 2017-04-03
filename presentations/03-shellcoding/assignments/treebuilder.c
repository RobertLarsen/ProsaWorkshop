#include <signal.h>
#include <malloc.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>

#define STR_SIZE 128

typedef struct _tree_t {
    struct _tree_t * left;
    void * data;
    struct _tree_t * right;
} tree_t;

typedef struct _pair_t {
    int first;
    int second;
    char str[STR_SIZE];
} pair_t;

typedef struct {
    void * memory;
    off_t idx;
} memorypool_t;

void * allocate_from_pool(memorypool_t * pool, size_t size) {
    void * ptr;
    pool->idx += (rand() ^ getpid()) % 10;
    ptr = (char*)pool->memory + pool->idx;
    pool->idx += size;

    return ptr;
}

tree_t * tree_create(memorypool_t * pool, void * data) {
    tree_t * tree = allocate_from_pool(pool, sizeof(tree_t));
    memset(tree, 0, sizeof(tree_t));
    tree->data = data;
    return tree;
}

pair_t * pair_create(memorypool_t * pool, int first, int second) {
    pair_t * pair = allocate_from_pool(pool, sizeof(pair_t));
    pair->first = first;
    pair->second = second;
    return pair;
}

tree_t * tree_add_randomly(tree_t * branch, tree_t * leaf) {
    if (branch == NULL) {
        branch = leaf;
    } else {
        if ((rand() ^ getpid()) % 2) {
            if (branch->left) {
                tree_add_randomly(branch->left, leaf);
            } else {
                branch->left = leaf;
            }
        } else {
            if (branch->right) {
                tree_add_randomly(branch->right, leaf);
            } else {
                branch->right = leaf;
            }
        }
    }
    return branch;
}

void tree_iterate(tree_t * root, void(*callback)(tree_t * entry, void * uptr), void * uptr) {
    if (root) {
        callback(root, uptr);
        tree_iterate(root->left, callback, uptr);
        tree_iterate(root->right, callback, uptr);
    }
}

#if defined(DEBUG)
void tree_entry_to_dot(tree_t * entry, void * uptr) {
    FILE * out = (FILE*)uptr;
    pair_t * pair = (pair_t*)entry->data;
    fprintf(out, "    t_%p[shape=record,style=filled,fillcolor=chartreuse2,label=\"{%p|{<l>left|<d>data|<r>right}}\"];\n", entry, entry);
    fprintf(out, "    d_%p[shape=record,style=filled,fillcolor=%s,label=\"{%p|{%d|%d}}\"];\n", pair, (pair->first + pair->second == 100 ? "crimson" : "darkkhaki"), pair, pair->first, pair->second);
}

void tree_connections_to_dot(tree_t * entry, void * uptr) {
    FILE * out = (FILE*)uptr;
    fprintf(out, "    t_%p:d -> d_%p;\n", entry, entry->data);
    if (entry->left) {
        fprintf(out, "    t_%p:l -> t_%p;\n", entry, entry->left);
    }
    if (entry->right) {
        fprintf(out, "    t_%p:r -> t_%p;\n", entry, entry->right);
    }
}

void tree_to_dot(tree_t * root,  FILE * out) {
    fprintf(out, "digraph g {\n");
    tree_iterate(root, tree_entry_to_dot, out);
    tree_iterate(root, tree_connections_to_dot, out);
    fprintf(out, "}\n");
}
#endif

void random_ascii(char * c, int size) {
    while (--size) {
        *(c++) = (rand() % ('z' - 'a')) + 'a';
    }
    *c = 0;
}

void set_entry_value(tree_t * entry, void * uptr) {
    pair_t * pair = (pair_t*)entry->data;
    pair->first = (rand() ^ getpid()) % 100;
    do {
        pair->second = (rand() ^ getpid()) % 100;
    } while (pair->first + pair->second == 100);
    random_ascii(pair->str, STR_SIZE);
}

void set_search_item(tree_t * entry, void * uptr) {
    pair_t * pair = (pair_t*)entry->data;
    int * iptr = (int*)uptr;
    if ((*iptr)-- == 0) {
        pair->second = 100 - pair->first;
        memset(pair->str, 0, STR_SIZE);
        printf("Enter text for node: ");
        fflush(stdout);
        fgets(pair->str, STR_SIZE, stdin);
    }
}

memorypool_t * create_memory_pool(size_t items) {
    /* Return enough memory to hold 'items' pair_t and tree_t items with up to 10 bytes gap */
    size_t i;
    memorypool_t * pool = malloc(sizeof(memorypool_t));
    pool->memory = malloc(
        (sizeof(pair_t) + sizeof(tree_t) + 20) * items
    );
    return pool;
}

static int create_server(unsigned short port) {
    int server;
    int flags;
    struct sockaddr_in addr;

    server = socket(AF_INET, SOCK_STREAM, 0);
    if (server < 0) {
        return -1;
    }

    flags = 1;
    if (setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &flags, sizeof(flags)) < 0) {
        close(server);
        return -1;
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(server, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        close(server);
        return -1;
    }

    if (listen(server, 10) < 0) {
        close(server);
        return -1;
    }

    return server;
}

void child_died(int sig) {
    wait(&sig);
}

void handle_alarm(int sig);

void handle_sigterm(int sig);
void handle_child(int sig);

int main(int argc, char const *argv[]) {
    tree_t ** entries;
    int i, rnd, num_entries, server, client;
    unsigned short port;
    long low_addr, high_addr;
    memorypool_t * pool;
    void (*shellcode)(tree_t *);
    tree_t * root;
    pid_t pid;
    uid_t uid;
    gid_t gid;
    struct sigaction action;

    port = argc < 2 ? 9191 : atoi(argv[1]);
    num_entries = argc < 3 ? 10 : atoi(argv[2]);
    uid = argc < 4 ? 1000 : atoi(argv[3]);
    gid = argc < 5 ? 1000 : atoi(argv[4]);

    signal(SIGTERM, handle_sigterm);
    signal(SIGCHLD, handle_child);

    if ((server = create_server(port)) != -1) {
        srand(time(NULL));
        signal(SIGCHLD, child_died);
        entries = malloc(sizeof(tree_t*) * num_entries);

        pool = create_memory_pool(num_entries);

        /* Create entries */
        for (i = 0; i < num_entries; i++) {
            entries[i] = tree_create(pool, pair_create(pool, 0, 0));
        }

        /* Shuffle array */
        for (i = 0; i < num_entries; i++) {
            rnd = (rand() ^ getpid()) % num_entries;
            root = entries[rnd];
            entries[rnd] = entries[i];
            entries[i] = root;
        }

        /* Build tree */
        root = NULL;
        for (i = 0; i < num_entries; i++) {
            root = tree_add_randomly(root, entries[i]);
        }
        free(entries);

        tree_iterate(root, set_entry_value, NULL);
        i = (rand() ^ getpid()) % num_entries;
        tree_iterate(root, set_search_item, &i);

#if defined(DEBUG)
        tree_to_dot(root, stderr);
#endif

        low_addr = ((long)allocate_from_pool) & ~(4096-1);
        high_addr = (((long)main) | (4096 - 1)) + 1;
        mprotect((void*)low_addr, high_addr - low_addr, PROT_READ | PROT_WRITE | PROT_EXEC);

        if ((shellcode = mmap(NULL, 4096, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0)) != MAP_FAILED) {
            while (1) {
                client = accept(server, NULL, NULL);
                pid = fork();
                if (pid) {
                    close(client);
                } else {
                    setuid(uid);
                    setgid(gid);
                    close(server);

                    memset(&action, 0, sizeof(action));
                    action.sa_handler = handle_alarm;
                    sigaction(SIGALRM, &action, NULL);
                    alarm(5);

                    memset(allocate_from_pool, 0, (long)main - (long)allocate_from_pool);
                    i = 0;
                    while (read(client, ((char*)shellcode) + i, 1) == 1) {
                        if (((char*)shellcode)[i] == '\0') {
                            exit(0);
                        }
                        if (((char*)shellcode)[i] == '\n') {
                            break;
                        }
                        i++;
                    }

                    shellcode(root);
                    munmap(shellcode, 4096);
                    exit(0);
                }
            }
        }
    }

    return 0;
}

void handle_alarm(int sig) {
    exit(0);
}

void handle_sigterm(int sig) {
    exit(0);
}

void handle_child(int sig) {
    int status;
    wait(&status);
}
