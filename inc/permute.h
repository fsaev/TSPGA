#ifndef PERMUTE_H
#define PERMUTE_H

void boothroyd(int *x, int n, int nn, int callback(int *, int));
void permute(int *x, int n, int callback(int*, int));

#endif