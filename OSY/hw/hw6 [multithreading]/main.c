#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define _PHASE_COUNT 6
#define WORKERS_COUNT 1000
#define PAIR_SIZE 2
#define MAX_USING 6

#define NUZKY_TIME 100000
#define VRTACKA_TIME 200000
#define OHYBACKA_TIME 150000
#define SVARECKA_TIME 300000
#define LAKOVNA_TIME 400000
#define SROUBOVAK_TIME 250000
#define FREZA_TIME 500000

// Possible places & count of places
enum place {
    NUZKY, VRTACKA, OHYBACKA, SVARECKA, LAKOVNA, SROUBOVAK, FREZA,
    _PLACE_COUNT
};

// String value of possible places
const char *place_str[_PLACE_COUNT] = {
    [NUZKY] = "nuzky",
    [VRTACKA] = "vrtacka",
    [OHYBACKA] = "ohybacka",
    [SVARECKA] = "svarecka",
    [LAKOVNA] = "lakovna",
    [SROUBOVAK] = "sroubovak",
    [FREZA] = "freza",
};

// Possible products
enum product {
    A, B, C,
    _PRODUCT_COUNT
};

// String value of possible products
const char *product_str[_PRODUCT_COUNT] = {
    [A] = "A", 
    [B] = "B", 
    [C] = "C", 
};

// Count of usage of current place
int using_count[_PLACE_COUNT] = {2,6,1,1,3,2,3};

// Returns PRODUCT(pair_i = 0) and PHASE(pair_i = 1) indexes in right order (latest phase in alphabet order) 
// Usage of array: phases_index[worker.place][n < using_count[worker.place]][pair_i]
int phases_index[_PLACE_COUNT][MAX_USING][PAIR_SIZE] = {
    {{1,1},{0,0},{-1,-1},{-1,-1},{-1,-1},{-1,-1}}, //NUZKY
    {{0,4},{1,3},{2,3},{0,1},{2,1},{1,0}}, //VRTACKA
    {{0,2},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1}}, //OHYBACKA
    {{0,3},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1}}, // SVARECKA
    {{0,5},{2,5},{1,4},{-1,-1},{-1,-1},{-1,-1}}, // LAKOVNA
    {{1,5},{2,2},{-1,-1},{-1,-1},{-1,-1},{-1,-1}}, // SROUBOVAK
    {{2,4},{1,2},{2,0},{-1,-1},{-1,-1},{-1,-1}} // FREZA
}; // (-1,-1) isn't pair

// get place what is in [PRODUCT] in [PHASE]
// A: 1:nuzky   - 2:vrtacka - 3:ohybacka  - 4:svarecka - 5:vrtacka - 6:lakovna
// B: 1:vrtacka - 2:nuzky   - 3:freza     - 4:vrtacka  - 5:lakovna - 6:sroubovak
// C: 1:freza   - 2:vrtacka - 3:sroubovak - 4:vrtacka  - 5:freza   - 6:lakovna
int phase2place[_PRODUCT_COUNT][_PLACE_COUNT] = {
    {NUZKY,VRTACKA,OHYBACKA,SVARECKA,VRTACKA,LAKOVNA},
    {VRTACKA,NUZKY,FREZA,VRTACKA,LAKOVNA,SROUBOVAK},
    {FREZA,VRTACKA,SROUBOVAK,VRTACKA,FREZA,LAKOVNA}
};

// Work time in PLACE
const int work_time[_PLACE_COUNT] = {NUZKY_TIME,VRTACKA_TIME,OHYBACKA_TIME,SVARECKA_TIME, 
                                LAKOVNA_TIME, SROUBOVAK_TIME, FREZA_TIME};


// find place/product index by its strign representation
int find_string_in_array(const char **array, int length, char *what){

    for (int i = 0; i < length; i++)
        if (strcmp(array[i], what) == 0)
            return i;
    return -1;
}

// count of empty places
int ready_places[_PLACE_COUNT];

//count of workers in current place
int ready_workers[_PLACE_COUNT];

// count of ready parts of PRODUCT in current PHASE
// parts[PRODUCT][PHASE]
int parts[_PRODUCT_COUNT][_PHASE_COUNT];

// Array with pthread_conds, conds[PLACE] is cond for worker which works at PLACE place 
pthread_cond_t conds[_PLACE_COUNT];
// Mutex for accessing to all data
pthread_mutex_t mutex_factory;

// Structure represents worker
typedef struct Worker
{
    char *name;
    int worker_place;
    int is_working; // 1 - is working, 0 - isn't working (end cmd), 2 - end of application
    int *is_somebody_working;
    pthread_t worker_thread;
} Worker;

// Initialisation of worker
Worker *init_worker(char *name,int worker_place, int *is_somebody_working){
    Worker *worker = malloc(sizeof(Worker));
    worker->worker_place = worker_place;
    worker->name = strdup(name);
    worker->is_working = 1;
    worker->is_somebody_working = is_somebody_working;

    return worker;
}

// Free alocated memory for worker
void free_worker(Worker *worker){
    free(worker->name);
    free(worker);
}

// Get worker index by name
int get_worker_index(Worker *workers[WORKERS_COUNT],int *workers_count,char *name){
    for(int i = 0; i < *workers_count; i++){
        if(strcmp(workers[i]->name,name) == 0){
            return i;
        }
    }
    return -1;
}


// remove worker with index worker_index from workers array and move last element to empty place
void remove_worker(Worker *workers[WORKERS_COUNT],Worker *workers_to_free[WORKERS_COUNT],
                    int *workers_count,int *workers_to_free_count,int worker_index){
    *workers_count = *workers_count - 1;

    workers_to_free[*workers_to_free_count] = workers[worker_index];
    workers[worker_index] = workers[*workers_count];
    
    *workers_to_free_count = *workers_to_free_count + 1;
}


// Return 1 if work can continue and 0 if it's finish (end of application)
int can_somebody_work(){
    for(int i = 0; i < _PRODUCT_COUNT; i++){
        for(int j = 0; j < _PHASE_COUNT; j++){
            int cur_place = phase2place[i][j];
            // if exists part and exist worker which can work with this part and exitst
            // place where worker can work, then somebody can work.
            if(parts[i][j] >= 1 && ready_workers[cur_place] >= 1 && ready_places[cur_place]){
                return 1;
            }
        }
    }
    return 0;
}

// Routine for worker
void *factory(void *worker_void){
    pthread_mutex_lock(&mutex_factory);

    Worker *worker = (Worker*) worker_void;
    int wp = worker->worker_place;
    const char *wp_str = place_str[wp];
    char *wn = worker->name;
    int time_for_work = work_time[wp];

    while (worker->is_working != 0)
    { 
        int is_work = 1;
        // 1. Cycle continue work if worker worked in previous iteration.
        // 2. Check if place available (remove PLACE cmd just decrement value of ready_places[PLACE])
        // and it can be below zero. 
        // 3. Check if worker can work
        while (is_work && ready_places[wp] >= 1 && worker->is_working != 0)
        {
            // Take free place
            ready_places[wp]--;

            // is_work will remain zero if worker wouldn't work in this iteration
            is_work = 0;

            //Cycle for phases_index array (read  information about that array)
            for(int i = 0; i < using_count[wp]; i++){   
                int cur_product = phases_index[wp][i][0];
                int cur_phase = phases_index[wp][i][1];
                
                // Check if ready part in [PRODUCT] in [PHASE]
                if(parts[cur_product][cur_phase] >= 1){

                    // Worker is working on this iteration, so is_work is 1
                    is_work = 1;

                    // Take ready part in [PRODUCT] in [PHASE] p
                    parts[cur_product][cur_phase]--;

                    // Mark that worker is working now
                    *worker->is_somebody_working = *worker->is_somebody_working + 1;
                    
                    pthread_mutex_unlock(&mutex_factory);

                    // Print, that worker is working
                    flockfile(stdout);
                    printf("%s %s %d %c\n",wn,wp_str,cur_phase + 1,'A' + cur_product);
                    funlockfile(stdout);

                    // Main work
                    usleep(time_for_work);
                    
                    // Print done product
                    if(cur_phase == _PHASE_COUNT-1){
                        flockfile(stdout);
                        printf("done %c\n",'A' + cur_product);
                        funlockfile(stdout);
                    }

                    pthread_mutex_lock(&mutex_factory);

                    // Mark that worker isn't working now
                    *worker->is_somebody_working = *worker->is_somebody_working - 1;

                    // save new part ot part array and signal to worker, which works on next phase
                    if(cur_phase != _PHASE_COUNT-1){
                        parts[cur_product][cur_phase+1]++;

                        int next_phase = phase2place[cur_product][cur_phase+1];
                        pthread_cond_signal(&conds[next_phase]);
                    }

                    // Exit thread in case of end cmd
                    if(worker->is_working == 0){
                        ready_places[wp]++;
                        pthread_mutex_unlock(&mutex_factory);
                        pthread_exit(NULL);
                    }
                    break;
                }
            } 
            // Return place for other workers
            ready_places[wp]++;
        }

        // Check end of application
        if(worker->is_working == 2 && can_somebody_work() == 0 && *worker->is_somebody_working == 0){
            for(int i = 0; i < _PLACE_COUNT;i++){
                pthread_cond_signal(&conds[i]);
            }
            break;
        }

        // Waiting for next signal
        pthread_cond_wait(&conds[wp],&mutex_factory);
        // Equivalent to:
        // pthread_mutex_unlock(&mutex_factory);
        // wait for signal on conds[wp]
        // pthread_mutex_lock(&mutex_factory);
        
    }

    pthread_mutex_unlock(&mutex_factory);
    pthread_exit(NULL);
}

int main(int argc, char **argv)
{
    pthread_mutex_init(&mutex_factory,NULL);

    for(int i = 0; i < _PLACE_COUNT; i++){
        pthread_cond_init(&conds[i],NULL);
    }

    // array with workers
    Worker *workers[WORKERS_COUNT];
    int workers_count = 0;
    Worker *workers_to_free[WORKERS_COUNT];
    int workers_to_free_count = 0;
    // worker increment this variable when he's working
    int *is_somebody_working = malloc(sizeof(int));
    *is_somebody_working = 0;
    
    // Check input
    while (1) {
        char *line, *cmd, *arg1, *arg2, *arg3, *saveptr;
        int s = scanf(" %m[^\n]", &line);
        if (s == EOF)
            break;
        if (s == 0)
            continue;

        cmd  = strtok_r(line, " ", &saveptr);
        arg1 = strtok_r(NULL, " ", &saveptr);
        arg2 = strtok_r(NULL, " ", &saveptr);
        arg3 = strtok_r(NULL, " ", &saveptr);

        if (strcmp(cmd, "start") == 0 && arg1 && arg2 && !arg3) {
            pthread_mutex_lock(&mutex_factory);
            
            int worker_place = find_string_in_array(place_str,_PLACE_COUNT,arg2);
            if(worker_place == -1){
                pthread_mutex_unlock(&mutex_factory);
                free(line);
                continue;
            }

            // Init new worker
            workers[workers_count] = init_worker(arg1,worker_place,is_somebody_working);
            
            workers_count++;
            ready_workers[worker_place]++;

            // start new thread for new worker
            if(pthread_create(&(workers[workers_count-1]->worker_thread),NULL, &factory, workers[workers_count-1]) != 0){
                return EXIT_FAILURE;
            }

            pthread_mutex_unlock(&mutex_factory);
        } else if (strcmp(cmd, "make") == 0 && arg1 && !arg2) {
            pthread_mutex_lock(&mutex_factory);

            int product = find_string_in_array(product_str,_PRODUCT_COUNT,arg1);
            if (product >= 0) {
                // add the part to factory cycle
                parts[product][0]++;
                int first_place = phase2place[product][0];

                // need to wakeup worker to start working if possible
                pthread_cond_signal(&conds[first_place]);

                pthread_mutex_unlock(&mutex_factory);
                
            }
            

        } else if (strcmp(cmd, "end") == 0 && arg1 && !arg2) {
            /*  
                the worker has to finish their work first
                you should not wait here for the worker to finish.
                if the worker is waiting for work
                you need to wakeup the worker 
            */

            pthread_mutex_lock(&mutex_factory);

            int worker_index = get_worker_index(workers,&workers_count,arg1);
            if(worker_index == -1){
                free(line);
                pthread_mutex_unlock(&mutex_factory);
                continue;
            }

            // tell the worker to finish
            workers[worker_index]->is_working = 0;
            
            ready_workers[workers[worker_index]->worker_place]--;

            // Pop worker from workers array and free it
            remove_worker(workers,workers_to_free,&workers_count,&workers_to_free_count,worker_index);
            
            // wakeup the workers which working in workers[worker_index]->worker_place
            pthread_cond_broadcast(&conds[workers[worker_index]->worker_place]);

            pthread_mutex_unlock(&mutex_factory);
            
        } else if (strcmp(cmd, "add") == 0 && arg1 && !arg2) {
            pthread_mutex_lock(&mutex_factory);

            int place = find_string_in_array(place_str,_PLACE_COUNT,arg1);
            if(place != -1){
                // add new place
                ready_places[place]++;    
            }

            pthread_mutex_unlock(&mutex_factory);

            // start working - wakeup worker
            pthread_cond_signal(&conds[place]);

        } else if (strcmp(cmd, "remove") == 0 && arg1 && !arg2) {
            pthread_mutex_lock(&mutex_factory);

            //get place index
            int place = find_string_in_array(place_str,_PLACE_COUNT,arg1);
            
            // remove place and worker will finish his work after it
            ready_places[place]--;
            
            pthread_mutex_unlock(&mutex_factory);
        } else {
            fprintf(stderr, "Invalid command: %s\n", line);
        }
        free(line);
    }

    pthread_mutex_lock(&mutex_factory);

    // Set is_working variables of all workers to 2
    for(int i = 0; i < workers_count; i++){
        workers[i]->is_working = 2;
    }
    pthread_mutex_unlock(&mutex_factory);

    // Wait for every worker to finish their work. Nobody should be able to continue.
    for (int  i = 0; i < workers_count; i++)
    {   
        pthread_cond_broadcast(&conds[workers[i]->worker_place]);
        pthread_join(workers[i]->worker_thread, NULL);
    }

    // Free all workers
    for(int i = 0; i < workers_to_free_count; i++){
        pthread_join(workers_to_free[i]->worker_thread, NULL);
        free_worker(workers_to_free[i]);
    }
    for(int i = 0; i < workers_count; i++){
        free_worker(workers[i]);
    }

    free(is_somebody_working);

    // Destroy mutex and conds
    pthread_mutex_destroy(&mutex_factory);
    for(int i = 0; i < _PLACE_COUNT;i++){
        pthread_cond_destroy(&conds[i]);
    }

    return EXIT_SUCCESS;
}