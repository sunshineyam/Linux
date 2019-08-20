#ifndef COMM_H
#define COMM_H
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <string.h>
#define PATHNAME "."
#define PROJ_ID 0X6666

int createShm(int size);
int destroyShm(int shmid);
int getShm(int size);
#endif
