#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <ifaddrs.h>

typedef uint64_t usize;

char **get_all_interfaces_names(usize *count) {
    int capacity = 10;
    int index = 0;

    char **names = malloc(capacity * sizeof(char*));
    if (!names) {
        perror("malloc");
        return NULL;
    }

    struct ifaddrs *addrs;
    if (getifaddrs(&addrs) == -1) {
        perror("getifaddrs");
        exit(EXIT_FAILURE);
    }

    for (struct ifaddrs *ifa = addrs; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == NULL) {
            continue;
        }

        int exists = 0;
        for (int i = 0; i < index; i++) {
            if (strcmp(names[i], ifa->ifa_name) == 0) {
                exists = 1;
                break;
            }
        }
        if (exists) continue;

        if (index >= capacity) {
            capacity *= 2;
            char **temp = realloc(names, capacity * sizeof(char*));
            if (!temp) {
                perror("realloc");
                break;
            }
            names = temp;
        }

        names[index] = strdup(ifa->ifa_name);
        if (!names[index]) {
            perror("strdup");
            break;
        }

        index++;
    }

    freeifaddrs(addrs);
    *count = index;
    return names;
}

void free_interfaces_names(char **names, int count) {
    for (int i = 0; i < count; i++) {
        free(names[i]);
    }
    free(names);
}
