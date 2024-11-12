#ifndef FILE_OPERATIONS_H
#define FILE_OPERATIONS_H

#include <string>

int create_directory(const char *name);
int delete_directory(const char *name);
int create_file(const char *name);
int write_to_file(const char *name, const char *content);
std::string  read_file(const char *name);
int change_directory(const char *path);
std::string list_directory_contents(const char *path);
int delete_file(const char *name);
#endif // FILE_OPERATIONS_H
