#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <sys/inotify.h>

#define EVENT_SIZE  ( sizeof (struct inotify_event) )
#define BUF_LEN     ( 1024 * ( EVENT_SIZE + 16 ) )

void list_directory(char* path) {
    DIR* dir = opendir(path);
    struct dirent* dp;
    while ((dp = readdir(dir)) != NULL) {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0) {
            printf("%s\n", dp->d_name);
        }
    }
    closedir(dir);
}

void sort_files_by_type(char* path) {
    char* music_files[] = {".mp3", ".wav", ".ogg", ".flac"};
    char* video_files[] = {".mp4", ".avi", ".mkv", ".wmv"};
    DIR* dir = opendir(path);
    struct dirent* dp;
    while ((dp = readdir(dir)) != NULL) {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0) {
            char* file_extension = strrchr(dp->d_name, '.');
            if (file_extension != NULL) {
                if (strstr(music_files, file_extension) != NULL) {
                    printf("Music file: %s\n", dp->d_name);
                } else if (strstr(video_files, file_extension) != NULL) {
                    printf("Video file: %s\n", dp->d_name);
                } else {
                    printf("Other file: %s\n", dp->d_name);
                }
            }
        }
    }
    closedir(dir);
}

int main(int argc, char** argv) {
    int fd, wd, length;
    char buffer[BUF_LEN];

    fd = inotify_init();
    if (fd < 0) {
        perror("inotify_init");
    }

    wd = inotify_add_watch(fd, argv[1], IN_CREATE | IN_MODIFY);
    if (wd < 0) {
        perror("inotify_add_watch");
    }

    while (1) {
        length = read(fd, buffer, BUF_LEN);
        if (length < 0) {
            perror("read");
        }
        struct inotify_event* event;
        for (char* ptr = buffer; ptr < buffer + length; ptr += EVENT_SIZE + event->len) {
            event = (struct inotify_event*)ptr;
            if (event->mask & IN_CREATE) {
                printf("New file: %s\n", event->name);
            }
        }
    }

    inotify_rm_watch(fd, wd);
    close(fd);

    return 0;
}
