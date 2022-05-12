#include "libmini.h"

long errno;

#define WRAPPER_RETval(type) \
    errno = 0;               \
    if (ret < 0) {           \
        errno = -ret;        \
        return -1;           \
    }                        \
    return ((type)ret);
#define WRAPPER_RETptr(type) \
    errno = 0;               \
    if (ret < 0) {           \
        errno = -ret;        \
        return NULL;         \
    }                        \
    return ((type)ret);

int sigismember(const sigset_t *set, int sig) {
    unsigned long _sig = sig - 1;
    if (_NSIG_WORDS == 1)
        return 1 & (set->sig[0] >> _sig);
    else
        return 1 & (set->sig[_sig / _NSIG_BPW] >> (_sig % _NSIG_BPW));
}

int sigaddset(sigset_t *set, int sig) {
    unsigned long _sig = sig - 1;
    if (_NSIG_WORDS == 1)
        set->sig[0] |= 1UL << _sig;
    else
        set->sig[_sig / _NSIG_BPW] |= 1UL << (_sig % _NSIG_BPW);

    return 0;
}

int sigdelset(sigset_t *set, int sig) {
    unsigned long _sig = sig - 1;
    if (_NSIG_WORDS == 1)
        set->sig[0] &= ~(1UL << _sig);
    else
        set->sig[_sig / _NSIG_BPW] &= ~(1UL << (_sig % _NSIG_BPW));

    return 0;
}

int sigemptyset(sigset_t *set) {
    switch (_NSIG_WORDS) {
        default:
            memset(set, 0, sizeof(sigset_t));
            break;
        case 2:
            set->sig[1] = 0;
        case 1:
            set->sig[0] = 0;
            break;
    }

    return 0;
}

int sigfillset(sigset_t *set) {
    switch (_NSIG_WORDS) {
        default:
            memset(set, -1, sizeof(sigset_t));
            break;
        case 2:
            set->sig[1] = -1;
        case 1:
            set->sig[0] = -1;
            break;
    }

    return 0;
}

int sigpending(sigset_t *set) {
    long ret = sys_rt_sigpending(set, sizeof(sigset_t));
    WRAPPER_RETval(int);
}

int sigprocmask(int how, const sigset_t *set, sigset_t *oldset) {
    long ret = sys_rt_sigprocmask(how, set, oldset, sizeof(sigset_t));
    WRAPPER_RETval(int);
}

unsigned int alarm(unsigned int seconds) {
    long ret = sys_alarm(seconds);
    WRAPPER_RETval(unsigned int);
}

ssize_t write(int fd, const void *buf, size_t count) {
    long ret = sys_write(fd, buf, count);
    WRAPPER_RETval(ssize_t);
}

int pause() {
    long ret = sys_pause();
    WRAPPER_RETval(int);
}

unsigned int sleep(unsigned int seconds) {
    long ret;
    struct timespec req, rem;
    req.tv_sec = seconds;
    req.tv_nsec = 0;
    ret = sys_nanosleep(&req, &rem);
    if (ret >= 0) return ret;
    if (ret == -EINTR) {
        return rem.tv_sec;
    }
    return 0;
}

void exit(int error_code) {
    sys_exit(error_code);
    /* never returns? */
}

size_t strlen(const char *s) {
    size_t count = 0;
    while (*s++) count++;
    return count;
}

void *memset(void *s, int c, size_t count) {
    char *xs = s;

    while (count--) *xs++ = c;
    return s;
}

#define PERRMSG_MIN 0
#define PERRMSG_MAX 34

static const char *errmsg[] = {"Success",
                               "Operation not permitted",
                               "No such file or directory",
                               "No such process",
                               "Interrupted system call",
                               "I/O error",
                               "No such device or address",
                               "Argument list too long",
                               "Exec format error",
                               "Bad file number",
                               "No child processes",
                               "Try again",
                               "Out of memory",
                               "Permission denied",
                               "Bad address",
                               "Block device required",
                               "Device or resource busy",
                               "File exists",
                               "Cross-device link",
                               "No such device",
                               "Not a directory",
                               "Is a directory",
                               "Invalid argument",
                               "File table overflow",
                               "Too many open files",
                               "Not a typewriter",
                               "Text file busy",
                               "File too large",
                               "No space left on device",
                               "Illegal seek",
                               "Read-only file system",
                               "Too many links",
                               "Broken pipe",
                               "Math argument out of domain of func",
                               "Math result not representable"};

void perror(const char *prefix) {
    const char *unknown = "Unknown";
    long backup = errno;
    if (prefix) {
        write(2, prefix, strlen(prefix));
        write(2, ": ", 2);
    }
    if (errno < PERRMSG_MIN || errno > PERRMSG_MAX)
        write(2, unknown, strlen(unknown));
    else
        write(2, errmsg[backup], strlen(errmsg[backup]));
    write(2, "\n", 1);
    return;
}