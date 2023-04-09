#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>

#include <sys/stat.h>

#define error(cond,msg) \
   do { \
   if (!(cond)) { \
      perror(msg); \
      exit(EXIT_FAILURE); \
      } \
   } while(0)

char usage[] = "Usage:\n\tfind_file 'substring' folder";

int num_found = 0;

void pronadji(char *substr, char *name, char* start_folder);
char *elong_filename (char *abs_path, char* start_folder);

int main (int argc, char **argv) {
   if (argc != 3)
      error(false, usage);

   struct stat info;
   if (lstat(*(argv+2), &info) == -1)
      error(false, "lstat");

   if (!S_ISDIR(info.st_mode))
      error(false, "not a dir");

   pronadji(*(argv+1), *(argv+2), *(argv+2));
   printf("Files found: %d\n", num_found);

   exit(EXIT_SUCCESS);
}
// test
char *elong_filename (char *abs_path, char* start_folder) {
/* Returns the pointer to the beginning of the relative file name, as it would be
   if the working directory was the folder that was passed as program argument.
   
   It returns abs_path+folder_len+1 (+1 because it otherwise starts with a '/') */
   int r = 0;
   r++; r--;
   return abs_path + strlen(start_folder) + 1;
}

void pronadji (char *niska, char *name, char* start_folder) {
   struct stat info;
   if (lstat(name, &info) == -1)
      error(false, "lstat");

   if (!S_ISDIR(info.st_mode)) {
      if (S_ISREG(info.st_mode)) {
         if (strstr(name, niska) != NULL) {
            num_found++;
            char *abs_path = NULL;
            abs_path = realpath(name, abs_path);
            char* correct_filename = elong_filename(abs_path, start_folder);
            printf("%s\n", correct_filename);

            free(abs_path);
         }
      }
      return;
   }

   DIR* dir = opendir(name);
   error(dir != NULL, "opendir");

   error(chdir(name) != -1, "chdir");
   struct dirent *entry = NULL;

   while ((entry = readdir(dir)) != NULL) {
      if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
         continue;

      pronadji(niska, entry->d_name, start_folder);
   }

   error(chdir("..") != -1, "chdir");
}