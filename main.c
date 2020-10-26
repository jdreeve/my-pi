#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <errno.h>
#include "drones.h"
#include "darts.h"
void validate_input(int argc, char* argv[]);
int get_shared_memory(int shm_key, int shm_size, int shm_flag);
void initialize_shared_memory(int shm_id);
void generate_output_string(char* output_buffer, unsigned long long int total_darts, unsigned long long int darts_in_circle);
unsigned long long int read_shared_memory(int shm_id);

int main(int argc, char *argv[]){/*{{{*/

	validate_input(argc, argv);

	int num_drones = atoi(argv[1]);
	unsigned long long int total_darts = strtoull(argv[2], NULL, 10);
	int darts_per_drone = total_darts / num_drones;
	unsigned long long int darts_in_circle = 0;
	int shm_id;
	int shm_flag;
	int shm_key;
	int shm_size;

	char output_buffer[100];

	printf("Deploying %d drones launching %d darts each.\n", num_drones, darts_per_drone);
	
	shm_key = 17;
	shm_size = sizeof(unsigned long long int);
	shm_flag = IPC_CREAT;//flag to create shared memory

	shm_id = get_shared_memory(shm_key, shm_size, shm_flag);
	
	initialize_shared_memory(shm_id);
	
	deploy_drones(num_drones, darts_per_drone, shm_id);
	
	reap_drones(num_drones);
	
	fflush(stdout);

	darts_in_circle = read_shared_memory(shm_id);

	if(shmctl(shm_id, IPC_RMID, NULL) < 0){
		perror("shmctl");
		printf("shmctl error: errno %d\n", errno);
	}

	
	
	generate_output_string(output_buffer, total_darts, darts_in_circle);

	printf("%s\n", output_buffer);

	return 0;
}/*}}}*/

int get_shared_memory(int shm_key, int shm_size, int shm_flag){/*{{{*/
	int shm_id;
	if((shm_id = shmget(shm_key, shm_size, 0666 | shm_flag)) == -1){
		perror("shmget");
		printf("shmget error\n");
		exit(1);
	}
	return shm_id;
}/*}}}*/

unsigned long long int read_shared_memory(int shm_id){
	unsigned long long int* shm_addr;
	unsigned long long int shm_value;
	shm_addr = (unsigned long long int*) shmat(shm_id, NULL, 0);
	shm_value = *shm_addr;
	shmdt(shm_addr);
	return shm_value;
}

void initialize_shared_memory(int shm_id){
    unsigned long long int* shm_addr;
    shm_addr = (unsigned long long int*) shmat(shm_id, NULL, 0);
    *shm_addr = 0;
    shmdt(shm_addr);
}

void validate_input(int argc, char* argv[]){/*{{{*/
	if(argc != 3){
		printf("Usage: my-pi (num-drones) (total-darts)\n");
		exit(1);
	}
	
	int num_drones = atoi(argv[1]);
	unsigned long long int total_darts = strtoull(argv[2], NULL, 10);

	if(0 == num_drones){
		printf("No drones allocated.\n");
		exit(1);
	}

	int remainder = total_darts % num_drones;
	if(remainder != 0){
		printf("Warning: %d does not evenly divide %lld! A remainder of %d darts will not be thrown.\n", num_drones, total_darts, remainder);
	}

	return;
}/*}}}*/

void generate_output_string(char* output_buffer, unsigned long long int total_darts, unsigned long long int darts_in_circle){/*{{{*/
	 double pi_estimate;

	pi_estimate = 4.0 * darts_in_circle / total_darts;

	sprintf(output_buffer, "Estimated value of pi: %f", pi_estimate);
}/*}}}*/



void add_to_shared_memory(){
}

