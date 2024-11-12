#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <dirent.h>
#include <iostream>
#include <cerrno>

int create_directory(const char *name) {
    if (mkdir(name, 0777) == -1) {
        perror("mkdir failed");
        return errno;
    } else {
        std::cout << "Directory created: " << name << std::endl;
    }
    return 0;
}

int delete_directory(const char *name) {
    DIR *dir = opendir(name);
    if (dir == NULL) {
        perror("opendir failed");
        return errno;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        std::string path = std::string(name) + "/" + entry->d_name;
        struct stat statbuf;
        if (stat(path.c_str(), &statbuf) == -1) {
            perror("stat failed");
            closedir(dir);
            return errno;
        }

        if (S_ISDIR(statbuf.st_mode)) {
            if (delete_directory(path.c_str()) != 0) {
                closedir(dir);
                return errno;
            }
        } else {
            if (unlink(path.c_str()) == -1) {
                perror("unlink failed");
                closedir(dir);
                return errno;
            }
        }
    }

    closedir(dir);

    if (rmdir(name) == -1) {
        perror("rmdir failed");
        return errno;
    } else {
        std::cout << "Directory deleted: " << name << std::endl;
    }
    return 0;
}

int create_file(const char *name) {
    int fd = open(name, O_CREAT | O_WRONLY, 0666);
    if (fd == -1) {
        perror("File creation failed");
        return errno;
    } else {
        std::cout << "File created: " << name << std::endl;
        close(fd);
    }
    return 0;
}

int write_to_file(const char *name, const char *content) {
    int fd = open(name, O_WRONLY | O_APPEND);
    if (fd == -1) {
        perror("File opening for writing failed");
        return errno;
    }
    if (write(fd, content, strlen(content)) == -1) {
        perror("Write failed");
        return errno;
    } else {
        std::cout << "Data written to file: " << name << std::endl;
    }
    close(fd);
    return 0;
}

std::string read_file(const char *name) {
    int fd = open(name, O_RDONLY);
    if (fd == -1) {
        perror("File opening for reading failed");
        return "Error: " + std::string(strerror(errno));
    }

    char buffer[1024];
    ssize_t bytesRead;
    std::string content;
    while ((bytesRead = read(fd, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytesRead] = '\0';
        std::cout << buffer;
        content += buffer;
    }

    if (bytesRead == -1) {
        perror("Read failed");
        content = "Error: " + std::string(strerror(errno));
    }
    close(fd);
    return content;
}

int change_directory(const char *path) {
    if (chdir(path) == -1) {
        perror("chdir failed");
        return errno;
    } else {
        std::cout << "Changed directory to: " << path << std::endl;
    }

    return 0;
}

int delete_file(const char *name) {
    if (unlink(name) == -1) {
        perror("unlink failed");
        return errno;
    } else {
        std::cout << "File deleted: " << name << std::endl;
    }
    return 0;
}

std::string list_directory_contents(const char *path) {
    DIR *dir = opendir(path);
    if (dir == NULL) {
        return "Error: " + std::string(strerror(errno));
    }

    struct dirent *entry;
    std::string contents;
    while ((entry = readdir(dir)) != NULL) {
        contents += entry->d_name;
        contents += "\n";
    }

    closedir(dir);
    return contents;
}
