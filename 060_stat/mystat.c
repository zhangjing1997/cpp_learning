#include <ctype.h>
#include <grp.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

char * getFileType(struct stat sb) {
  char * fileType = NULL;
  switch (sb.st_mode & S_IFMT) {
    case S_IFBLK:
      fileType = strdup("block special file");
      break;
    case S_IFCHR:
      fileType = strdup("character special file");
      break;
    case S_IFDIR:
      fileType = strdup("directory");
      break;
    case S_IFIFO:
      fileType = strdup("fifo");
      break;
    case S_IFLNK:
      fileType = strdup("symbolic link");
      break;
    case S_IFREG:
      fileType = strdup("regular file");
      break;
    case S_IFSOCK:
      fileType = strdup("socket");
      break;
  }
  return fileType;
}

char decidePermChr(struct stat sb, int cst, char c) {
  char ans;
  if (sb.st_mode & cst) {
    ans = c;
  }
  else {
    ans = '-';
  }
  return ans;
}

char * getPermStr(struct stat sb) {
  char * permString = malloc(11 * sizeof(*permString));
  switch (sb.st_mode & S_IFMT) {
    case S_IFBLK:
      permString[0] = 'b';
      break;
    case S_IFCHR:
      permString[0] = 'c';
      break;
    case S_IFDIR:
      permString[0] = 'd';
      break;
    case S_IFIFO:
      permString[0] = 'p';
      break;
    case S_IFLNK:
      permString[0] = 'l';
      break;
    case S_IFREG:
      permString[0] = '-';
      break;
    case S_IFSOCK:
      permString[0] = 's';
      break;
  }
  permString[1] = decidePermChr(sb, S_IRUSR, 'r');
  permString[2] = decidePermChr(sb, S_IWUSR, 'w');
  permString[3] = decidePermChr(sb, S_IXUSR, 'x');
  permString[4] = decidePermChr(sb, S_IRGRP, 'r');
  permString[5] = decidePermChr(sb, S_IWGRP, 'w');
  permString[6] = decidePermChr(sb, S_IXGRP, 'x');
  permString[7] = decidePermChr(sb, S_IROTH, 'r');
  permString[8] = decidePermChr(sb, S_IWOTH, 'w');
  permString[9] = decidePermChr(sb, S_IXOTH, 'x');
  permString[10] = '\0';
  return permString;
}

//This function is for Step 4
char * time2str(const time_t * when, long ns) {
  char * ans = malloc(128 * sizeof(*ans));
  char temp1[64];
  char temp2[32];
  const struct tm * t = localtime(when);
  strftime(temp1, 512, "%Y-%m-%d %H:%M:%S", t);
  strftime(temp2, 32, "%z", t);
  snprintf(ans, 128, "%s.%09ld %s", temp1, ns, temp2);
  return ans;
}

//print the first line
void printFirstLine(char * filename, struct stat sb) {
  printf("  File: %s", filename);
  if (S_ISLNK(sb.st_mode)) {
    char linktarget[256];
    ssize_t len = readlink(filename, linktarget, 256);
    linktarget[len] = '\0';
    printf(" -> %s", linktarget);
  }
  printf("\n");
}

//print the second line
void printSecondLine(struct stat sb) {
  char * sb_fileType = getFileType(sb);
  printf("  Size: %-10lu\tBlocks: %-10lu IO Block: %-6lu %s\n",
         (long)sb.st_size,
         (long)sb.st_blocks,
         (long)sb.st_blksize,
         sb_fileType);
  free(sb_fileType);
}

//print the third line
void printThirdLine(struct stat sb) {
  printf("Device: %lxh/%lud\tInode: %-10lu  Links: %lu",
         (long)sb.st_dev,
         (long)sb.st_dev,
         (long)sb.st_ino,
         (long)sb.st_nlink);
  if (S_ISCHR(sb.st_mode) || S_ISBLK(sb.st_mode)) {
    printf("     Device type: %x,%x", (int)major(sb.st_rdev), (int)minor(sb.st_rdev));
  }
  printf("\n");
}

//print the fourth line
void printFourthLine(struct stat sb) {
  char * sb_permStr = getPermStr(sb);
  struct passwd * pwd = getpwuid(sb.st_uid);
  struct group * grp = getgrgid(sb.st_gid);
  printf("Access: (%04o/%s)  Uid: (%5d/%8s)   Gid: (%5d/%8s)\n",
         sb.st_mode & ~S_IFMT,
         sb_permStr,
         sb.st_uid,
         pwd->pw_name,
         sb.st_gid,
         grp->gr_name);
  free(sb_permStr);
}

//print the last four lines
void printLast4Lines(struct stat sb) {
  //5th line
  char * aTimeStr = time2str(&sb.st_atime, sb.st_atim.tv_nsec);
  printf("Access: %s\n", aTimeStr);
  free(aTimeStr);
  //6th line
  char * mTimeStr = time2str(&sb.st_mtime, sb.st_mtim.tv_nsec);
  printf("Modify: %s\n", mTimeStr);
  free(mTimeStr);
  //7th line
  char * cTimeStr = time2str(&sb.st_ctime, sb.st_ctim.tv_nsec);
  printf("Change: %s\n", cTimeStr);
  free(cTimeStr);
  //last line
  printf(" Birth: -\n");
}

int main(int argc, char ** argv) {
  if (argc < 2) {
    fprintf(stderr, "Usage: %s <pathname>\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  for (int i = 1; i < argc; i++) {
    struct stat sb;
    int status;
    if ((status = lstat(argv[i], &sb)) == -1) {
      perror("lstat");
      exit(EXIT_FAILURE);
    }
    printFirstLine(argv[i], sb);
    printSecondLine(sb);
    printThirdLine(sb);
    printFourthLine(sb);
    printLast4Lines(sb);
  }
  return EXIT_SUCCESS;
}
