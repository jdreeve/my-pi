#ifndef DRONES_H
#define DRONES_H

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define SEMAPHORE_KEY (1066)

typedef struct {
	int id;
	struct sembuf lock[1];
	struct sembuf unlock[1];
} semaphore_data;

void deploy_drones(int num_drones, int darts_per_drone, int shm_id);
int initialize_drone_semaphore(semaphore_data drone_semaphore_data);
void deploy_drone(int darts_per_drone, int shm_id, semaphore_data drone_semaphore_data, int drone_counter);
void reap_drones(int num_drones);



#endif
