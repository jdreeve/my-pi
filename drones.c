#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>
#include <limits.h>
#include "drones.h"
#include "darts.h"

void deploy_drones(int num_drones, int darts_per_drone, int shm_id){/*{{{*/

	int sem_id = 0;
	semaphore_data drone_semaphore_data;
	int drone_counter=0;

	sem_id = initialize_drone_semaphore(drone_semaphore_data);

	for(int i=0; i < num_drones; i++){
		deploy_drone(darts_per_drone, shm_id, drone_semaphore_data, drone_counter);
		drone_counter++;
	}

	if(semctl(sem_id, 0, IPC_RMID)<0){
		perror("semctl");
		printf("semctl error: errno %d\n", errno);
	}

	return;
}/*}}}*/

int initialize_drone_semaphore(semaphore_data drone_semaphore_data){
	union semun {
		int val;
		struct semid_ds *buf;
		unsigned short *array;
	} argument;

	argument.val = 1;
	
	int id = semget(SEMAPHORE_KEY, 1, 0666 | IPC_CREAT);
	
	semctl(id, 0, SETVAL, argument);

	drone_semaphore_data.id = id;
	
	drone_semaphore_data.lock[0].sem_num = 0;
	drone_semaphore_data.lock[0].sem_op = -1;
	drone_semaphore_data.lock[0].sem_flg = 0;

	drone_semaphore_data.unlock[0].sem_num = 0;
	drone_semaphore_data.unlock[0].sem_op = 1;
	drone_semaphore_data.unlock[0].sem_flg = 0;

	return id;
}

void deploy_drone(int darts_per_drone, int shm_id, semaphore_data drone_semaphore_data, int drone_counter){/*{{{*/
	fflush(stdout);
	pid_t pid = -1;
	if((pid=fork())<0){
		perror("fork");
		printf("Fork error, errno %d\n", errno);
		printf("Drones deployed: %d\n", drone_counter);
		exit(1);
	}
	else if(pid==0){
		srand((unsigned) time(0) ^ (getpid()<<16));
		unsigned long long int hits = 0;
		unsigned long long int* shm_hits;

		hits = throw_darts(darts_per_drone);

		semop(drone_semaphore_data.id, drone_semaphore_data.lock, 1);
		shm_hits = (unsigned long long int*) shmat(shm_id, NULL, 0);
		*shm_hits += hits;
		semop(drone_semaphore_data.id, drone_semaphore_data.unlock, 1); 

		exit(0);
	}
	else if(pid>0){
	}
}/*}}}*/

void reap_drones(int num_drones){/*{{{*/
	for(int i=0; i < num_drones; i++){
		wait(NULL);
	}
}/*}}}*/
