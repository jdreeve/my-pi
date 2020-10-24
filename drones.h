#ifndef DRONES_H
#define DRONES_H
void deploy_drones(int num_drones, int darts_per_drone, int shm_id);
void deploy_drone(int darts_per_drone, int shm_id);
void reap_drones(int num_drones);

#endif
