#ifndef READER_H
#define READER_H

int open_csv(char *path);
char** line_csv(int *entries_read);
#endif
