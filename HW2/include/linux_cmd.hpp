#include <dlfcn.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#include <cstdio>
#include <cstdlib>

int linux_chmod(const char* path_name, mode_t mode);
int linux_chown(const char* pathname, uid_t owner, gid_t group);
int linux_close(int fd);
int linux_creat(const char* pathname, mode_t mode);
int linux_fclose(FILE* stream);
FILE* linux_fopen(const char* pathname, const char* mode);
size_t linux_fread(void* ptr, size_t size, size_t nmemb, FILE* stream);
size_t linux_fwrite(const void* ptr, size_t size, size_t nmemb, FILE* stream);
int linux_open(const char* pathname, int flags, mode_t mode);
ssize_t linux_read(int fd, void* buf, size_t count);
int linux_remove(const char* pathname);
int linux_rename(const char* oldpath, const char* newpath);
FILE* linux_tmpfile(void);
ssize_t linux_write(int fd, const void* buf, size_t count);