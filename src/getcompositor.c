#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wayland-client.h>
#include <sys/socket.h>
#include <errno.h>
#include "getcompositor.h"

#define PROCESS_NAME_MAX_LENGTH 128
#define SLASH_PROC_MAX_PATH     128

static pid_t pid_from_fd(int fd) {
    struct ucred ucred;
    socklen_t len = sizeof(struct ucred);

    if (getsockopt(fd, SOL_SOCKET, SO_PEERCRED, &ucred, &len) == -1) {
        perror("getsockopt failed");
        return -1;
    }

    return ucred.pid;
}


static char *process_name_from_pid(const pid_t pid) {
    char *name = malloc(PROCESS_NAME_MAX_LENGTH);
    if (name == NULL) {
        perror("malloc failed");
        return NULL;
    }

    char proc_buf[SLASH_PROC_MAX_PATH];
    snprintf(proc_buf, SLASH_PROC_MAX_PATH, "/proc/%d/comm", pid);
    proc_buf[SLASH_PROC_MAX_PATH - 1] = '\0';

    FILE *fp;
    if ((fp = fopen(proc_buf, "r")) == NULL) {
        fprintf(stderr, "opening '%s' failed: %s\n", proc_buf, strerror(errno));
        return NULL;
    }

    if (fgets(name, PROCESS_NAME_MAX_LENGTH, fp) == NULL) {
        fprintf(stderr, "reading '%s' failed\n", proc_buf);
        return NULL;
    }
    fclose(fp);

    name[strcspn(name, "\n")] = 0;

    return name; // note that `name' has to be freed by caller
}


char *get_compositor(void) {

    struct wl_display *display = wl_display_connect(NULL);
    if (display == NULL) {
        fprintf(stderr, "can't connect to display\n");
        return NULL;
    }

    int fd = wl_display_get_fd(display);
    pid_t pid = pid_from_fd(fd);
    if (pid < 0) {
        return NULL;
    }

    wl_display_disconnect(display);

    return process_name_from_pid(pid);
}
