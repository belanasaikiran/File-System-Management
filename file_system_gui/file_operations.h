#ifndef FILE_OPERATIONS_H
#define FILE_OPERATIONS_H

void create_directory(const char *name);
void delete_directory(const char *name);
void create_file(const char *name);
void write_to_file(const char *name, const char *content);
void read_file(const char *name);
void change_directory(const char *path);
void list_directory_contents();

#endif // FILE_OPERATIONS_H
