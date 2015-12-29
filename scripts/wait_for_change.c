#include <sys/inotify.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <malloc.h>

typedef struct _name_watch_t {
    char * file;
    int descriptor;
    struct _name_watch_t * next;
} name_watch_t;

name_watch_t * name_watch_alloc(char * file, int descriptor, name_watch_t * next) {
    name_watch_t * w = malloc(sizeof(name_watch_t));
    w->file = file;
    w->descriptor = descriptor;
    w->next = next;
    return w;
}

char * file_name(name_watch_t * watch, int descriptor) {
    if (watch == NULL) return NULL;
    if (watch->descriptor == descriptor) return watch->file;
    return file_name(watch->next, descriptor);
}

name_watch_t * name_watch_remove(name_watch_t * w, int descriptor) {
    name_watch_t * n;
    if (w == NULL) {
        return NULL;
    } else if (w->descriptor == descriptor) {
        n = w->next;
        free(w);
        return n;
    } else {
        w->next = name_watch_remove(w->next, descriptor);
        return w;
    }
}

int main(int argc, char ** argv) {
    int fd, i, wd;
    struct inotify_event evt;
    char * s;
    struct stat st;
    name_watch_t * head = NULL;
    if ((fd = inotify_init()) >= 0) {
        for (i = 1; i < argc; i++) {
            if ((wd = inotify_add_watch(fd, argv[i], IN_MODIFY|IN_DELETE_SELF)) >= 0) {
                head = name_watch_alloc(argv[i], wd, head);
            } else {
                fprintf(stderr, "Could not add watch on %s\n", argv[i]);
                return -1;
            }
        }
        while (read(fd, &evt, sizeof(struct inotify_event)) == sizeof(struct inotify_event)) {
            s = file_name(head, evt.wd);
            if (evt.mask & IN_DELETE_SELF) {
                head = name_watch_remove(head, evt.wd);
                inotify_rm_watch(fd, evt.wd);
                if (stat(s, &st) == 0 && (wd = inotify_add_watch(fd, s, IN_MODIFY|IN_DELETE_SELF)) >= 0) {
                    head = name_watch_alloc(s, wd, head);
                } else {
                    fprintf(stderr, "%s was removed\n", s);
                    s = NULL;
                }
            }
            if (s) {
                printf("%s\n", s);
                fflush(stdout);
            }
        }
    } else {
        perror("inotify_init");
        return -1;
    }
    return 0;
}
