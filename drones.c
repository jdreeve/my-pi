#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include "drones.h"
#include "darts.h"

void deploy_drones(int num_drones, int darts_per_drone, int shm_id){/*{{{*/
	for(int i=0; i < num_drones; i++){
		deploy_drone(darts_per_drone, shm_id);
	}
	return;
}/*}}}*/

void deploy_drone(int darts_per_drone, int shm_id){/*{{{*/
	fflush(stdout);
	pid_t pid = -1;
	if ((pid=fork())==0){
		srand((unsigned) time(0) ^ (getpid()<<16));
		unsigned long long int* shm_p;
		shm_p = (unsigned long long int*)shmat(shm_id, NULL, 0);
		fflush(stdout);
		unsigned long long int hits = throw_darts(darts_per_drone, shm_p);
		/* TODO: semaphore */
		*shm_p += hits;
		/* TODO: release semaphore */

		shmdt(shm_p);
		exit(0);
	}
}/*}}}*/

void reap_drones(int num_drones){/*{{{*/
	for(int i=0; i < num_drones; i++){
		wait(NULL);
	}
}/*}}}*/
