#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include <sys/poll.h>
#include <iostream>
#include <stdlib.h>

#define MAX_BUF 1024

using namespace std;

int main(int argc, char** argv)
{
  int mcu_id = 0;
  if (argc == 2) {
    mcu_id = argv[1][0] - '0';
  }
  printf("Trying to open terminal...\n");
#ifdef __linux__
      string tempdir = "/tmp/";
#else
      string tempdir = getenv("TMPDIR");
#endif
  string outpath = tempdir + "SILOUTTERMINAL";
  string inpath = tempdir + "SILINTERMINAL";
  outpath.push_back(mcu_id + '0');
  inpath.push_back(mcu_id + '0');
  int outfd = open(outpath.c_str(), O_RDONLY);
  int infd = open(inpath.c_str(), O_WRONLY);
  printf("Terminal Opened\n");

  char buf[MAX_BUF];
  while (1) {
    struct pollfd fds;
    fds.events = POLLIN;
    fds.fd = outfd; /* this is STDIN */
    if (poll(&fds, 1, 0) == 1) {
      int len = read(outfd, buf, MAX_BUF);
      if (len > 0) {
        buf[len] = 0;
        printf("Received: %s\n", buf);
      }
    }
    fds.fd = 0; /* this is STDIN */
    if (poll(&fds, 1, 0) == 1) {
      string input;
      cin >> input;
      write(infd, input.c_str(), input.size() + 1);
    }
    usleep(10000);
  }

  return 0;
}
