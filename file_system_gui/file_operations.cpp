#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <dirent.h>
#include <iostream>

void create_directory(const char *name) {
    if (mkdir(name, 0777) == -1) {
        perror("mkdir failed");
    } else {
        std::cout << "Directory created: " << name << std::endl;
    }
}

void delete_directory(const char *name) {
    if (rmdir(name) == -1) {
        perror("rmdir failed");
    } else {
        std::cout << "Directory deleted: " << name << std::endl;
    }
}

void create_file(const char *name) {
    int fd = open(name, O_CREAT | O_WRONLY, 0666);
    if (fd == -1) {
        perror("File creation failed");
    } else {
        std::cout << "File created: " << name << std::endl;
        close(fd);
    }
}

void write_to_file(const char *name, const char *content) {
    int fd = open(name, O_WRONLY | O_APPEND);
    if (fd == -1) {
        perror("File opening for writing failed");
        return;
    }
    if (write(fd, content, strlen(content)) == -1) {
        perror("Write failed");
    } else {
        std::cout << "Data written to file: " << name << std::endl;
    }
    close(fd);
}

void read_file(const char *name) {
    int fd = open(name, O_RDONLY);
    if (fd == -1) {
        perror("File opening for reading failed");
        return;
    }

    char buffer[1024];
    ssize_t bytesRead;
    while ((bytesRead = read(fd, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytesRead] = '\0';
        std::cout << buffer;
    }

    if (bytesRead == -1) {
        perror("Read failed");
    }
    close(fd);
}

void change_directory(const char *path) {
    if (chdir(path) == -1) {
        perror("chdir failed");
    } else {
        std::cout << "Changed directory to: " << path << std::endl;
    }
}

void list_directory_contents() {
    DIR *dir;
    struct dirent *entry;

    dir = opendir(".");
    if (dir == NULL) {
        perror("opendir failed");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        std::cout << entry->d_name << std::endl;
    }

    closedir(dir);
}
