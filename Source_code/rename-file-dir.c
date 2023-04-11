#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>

#include <sys/stat.h>
#include <sys/errno.h>

#define error(condition,message) \
   do { \
      if (!(condition)) { \
         perror(message); \
         exit(EXIT_FAILURE); \
         } \
   } while (0)

char usage[] = "Usage:\n  rename-file <replace> <with> folder/file";

void rename_file (char replace, char with, char *target);
void rename_dir (char replace, char with, char *target);
void find_rename (char replace, char with, char *target);

/** DODAJ: DA RENAME-UJE I FOLDERE (koristi rename()) */

int main (int argc, char **argv) {
   error(argc == 4, usage);
   
   find_rename(argv[1][0], argv[2][0], *(argv+3));
   
   printf("Done!\n");
   
   exit(EXIT_SUCCESS);
}

void rename_file (char replace, char with, char *target) {
   struct stat info;
   error(stat(target, &info) != -1, "stat2");
   
   ssize_t file_size = info.st_size;
   
   int fd = open(target, O_RDONLY);
   error(fd != -1, "open");
   
   char *buffer = malloc((file_size) * sizeof(char));
   
   ssize_t bytes_read = read(fd, buffer, file_size);
   error(bytes_read != -1, "read");

   int name_len = strlen(target) + 1;
   char *new_name = malloc(name_len * sizeof(char));
   
   //  printf("replace: %c with: %c\n", replace, with);
   
   int i = 0;
   while (target[i] != '\0') {
      if (target[i] == replace)
         new_name[i] = with;
      else
         new_name[i] = target[i];
      i++;
   } new_name[i] = '\0';
   
   // printf("new name: %s\n", new_name);
   
   error(unlink(target) != -1, "unlink");
   error(close(fd) != -1, "close");
   
   fd = open(new_name, O_RDWR|O_CREAT, 0644);
   error(fd != -1, "open");

   ssize_t bytes_written = write(fd, buffer, bytes_read);
   error(bytes_written != -1, "write");
   
   if (bytes_written != bytes_read)
      fprintf(stderr, "Bytes written and bytes read do not match!\n");
   
   free(buffer);
   free(new_name);
   close(fd);
   
   return;
}

void rename_dir (char replace, char with, char *target) {
   int name_len = strlen(target) + 1;
   
   char *new_name = malloc(name_len * sizeof(char));
   
   int i = 0;
   while (target[i] != '\0') {
      if (target[i] == replace)
         new_name[i] = with;
      else
         new_name[i] = target[i];
      i++;
   } new_name[i] = '\0';
   
   error(rename(target, new_name) != -1, "rename");
   
   return;
}

void find_rename (char replace, char with, char *target) {
   struct stat info;
   error(stat(target, &info) != -1, "stat1");
   
   if (!S_ISDIR(info.st_mode)) {
      // printf("%s\n", target);
      rename_file(replace, with, target);
      return;
   } 
   /* else {
      rename_dir(replace, with, target);
   } */

   DIR* dir = opendir(target);
   error(dir != NULL, "opendir");

   error(chdir(target) != -1, "chdir");
   
   struct dirent *entry = NULL;
   while ((entry = readdir(dir)) != NULL) {
      if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
         continue;
      
      find_rename(replace, with, entry->d_name);
   }
   
   error(chdir("..") != -1, "chdir");
}