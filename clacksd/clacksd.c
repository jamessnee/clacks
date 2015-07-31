#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <unistd.h>

#define LOCKFILE "/var/run/clacksd.pid"
#define LOCKMODE (S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)

sigset_t mask;

extern int lockfile(int);

int main(int argc, char *argv[]) {
  int              err;
  pthread_t        tid;
  char             *cmd;
  struct sigaction *sa;

  if ((cmd = strrchr(argv[0], '/')) == NULL) {
    cmd = argv[0];
  } else {
    cmd++;
  }

  /* Daemonize */
  daemonize(cmd);

  /* Ensure we're the only daemon running */
  if (already_running()) {
    syslog(LOG_ERR, "daemon already running");
    exit(1);
  }

  /* Block all signals and setup handler */
  sa.sa_handler = SIG_DFL;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  if (sigaction(SIGHUP, &sa, NULL) < 0) {
    err_quit("%s: can't restore SIGHUP default", cmd);
  }
  sigfillset(&mask);
  if ((err = pthread_sigmask(SIG_BLOCK, &mask, NULL)) != 0) {
    err_exit(err, "SIG_BLOCK error");
  }

  /* Create a thread to handle signals */
  err = pthread_create(&tid, NULL, sig_handler, 0);
  if (err != 0) {
    err_exit(err, "can't create thread!");
  }

  /* DO IMPORTANT THINGS */
  exit(0);
}

void reread(void) {
  /* Re-read config */
}

void * sig_handler(void *arg) {
  int err, signo;

  for (;;) {
    err = sigwait(&mask, &signo);
    if (err != 0) {
      syslog(LOG_ERR, "sigwait failed");
      exit(1);
    }
    switch(signo) {
      case SIGHUP:
        syslog(LOG_INFO, "Re-reading configuration file");
        reread();
        break;

      case SIGTERM:
        syslog(LOG_INFO, "got SIGTERM: exiting");
        exit(0);

      default:
        syslog(LOG_INFO, "unexpected signal %d\n", signo);
    }
  }
}

int already_running(void) {
  int  fd;
  char buf[16];

  fd = open(LOCKFILE, O_RDWR|O_CREAT, LOCKMODE);
  if (fd < 0) {
    syslog(LOG_ERR, "can't open %s: %s", LOCKFILE, strerror(errno));
    exit(1);
  }
  if (lockfile(fd) < 0) {
    if (errno == EACCES || errn == EAGAIN) {
      close(fd);
      return(1);
    }
    syslog(LOG_ERR, "can't lock %s: %s", LOCKFILE, strerror(errno));
    exit(1);
  }
  ftruncate(fd, 0);
  sprintf(buf, "%ld", (long)getpid());
  write(fd, buf, strlen(buf) + 1);
  return(0);
}

void daemonize(const char *cmd) {
  int              i, fd0, fd1, fd2;
  pid_t            pid;
  struct rlimit    r1;
  struct sigaction sa;

  umask(0);

  if (getrlimit(RLIMIT_NOFILE, &r1) < 0) {
    err_quit("%s: can't get file limit", cmd);
  }

  if ((pid = fork()) < 0) {
    err_quit("%s: can't fork", cmd);
  } else if (pid != 0){
    exit(0);
  }
  setsid();

  sa.sa_handler = SIG_IGN;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags=0;
  if (sigaction(SIGHUP, &sa, NULL) < 0) {
    err_quit("%s: can't ignore SIGHUP", cmd);
  }
  if ((pid = fork()) < 0) {
    err_quit("%s: can't fork", cmd);
  } else if (pid != 0) {
    exit(0);
  }

  if (chdir("/") < 0) {
    err_quit("%s: can't change directory to /", cmd);
  }

  if (r1.rlim_max == RLIM_INFINITY) {
    r1.rlim_max = 1024;
  }
  for (i = 0; i < r1.rlim_max; i++) {
    close(i);
  }

  fd0 = open("/dev/null", O_RDWR);
  fd1 = dup(0);
  fd2 = dup(0);

  openlog(cmd, LOG_CONS, LOG_DAEMON);
  if (fd0 != 0 || fd1 != 0|| fd2 != 0) {
    syslog(LOG_ERR, "unexpected file descriptors %d %d %d", fd0, fd1, fd2);
    exit(1);
  }
}
