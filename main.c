#include "defs.h"

RaceInfoType *race;

void printBorders();
void freeDorcs();
void freeRunners();
void freeAllRunnerEntity(EntityType*, EntityType*);

void freeAllRunnerEntity(EntityType *firstPlayer, EntityType *secondPlayer){
    free(firstPlayer);
    free(secondPlayer);
}

void printBorders(){
    scrPrt("------------------------", 1, 0);
    scrPrt("------------------------", 36, 0);
}

void freeDorcs(){
    int i;
    for(i = 0; i<race->numDorcs; i++){
        free(race->dorcs[i]);
    }
}

void freeRunners(){
    int i;
    for(i = 0; i<race->numRunners; i++){
        free(race->runners[i]);
    }
}


int main(){
    // Initializing the race

    // Dynamically allocate the "race" global variable
    race = (RaceInfoType*) malloc(sizeof(RaceInfoType));

    // The tortoise
    RunnerType *timmy = (RunnerType*) malloc(sizeof(RunnerType));
    strcpy(timmy->name, "Timmy");
    timmy->health = 50;
    timmy->dead = 0;
    EntityType *timmyEntity = (EntityType*) malloc(sizeof(EntityType));
    strcpy(timmyEntity->avatar, "T");
    timmyEntity->currPos = 35;
    timmyEntity->path = 10;
    timmy->ent = *timmyEntity;

    // The hare
    RunnerType *harold = (RunnerType*) malloc(sizeof(RunnerType));
    strcpy(harold->name, "Harold");
    harold->health = 50;
    harold->dead = 0;
    EntityType *haroldEntity = (EntityType*) malloc(sizeof(EntityType));
    strcpy(haroldEntity->avatar, "H");
    haroldEntity->currPos = 35;
    haroldEntity->path = 14;
    harold->ent = *haroldEntity;

    // Add the runners to race's array of runners.
    race->numRunners = 2;
    race->numDorcs = 0;
    race->runners[0] = timmy;
    race->runners[1] = harold;

    // Initialize race's semaphore
    if(sem_init(&race->mutex, 0, 1) < 0){
        printf("Error: on semaphore init.\n");
    }

    // Initialize race's winner to empty string
    strcpy(race->winner, "");

    // Initialize race's status row
    race->statusRow = 4;

    // Seed the random number generator
    srand( (unsigned)time(NULL) );

    // Initialize the use of ncurses library
    initNcurses();
    scrPrt("Health:  T   H", 0, 30);
    scrPrt("50", 2, 38);
    scrPrt("50", 2, 42);

    // Create runner threads
    pthread_t timmyThread, haroldThread;
    pthread_create(&timmyEntity->thr, NULL, goRunner, timmy);
    pthread_create(&haroldEntity->thr, NULL, goRunner, harold);

    // Save thread pointers in the runners entity structure
    //timmyEntity->thr = timmyThread;
    //haroldEntity->thr = haroldThread;

    //scrPrt("------------------------", 1, 0);
    //scrPrt("------------------------", 36, 0);
    printBorders();

    // race loop
    int dorcPercentage = 30;
    int columnPercentage = 33;
    int randomForDorcs;
    int randomForDorcColumn;
    EntityType *dorcEntity;
    while(strcmp("", race->winner) == 0){
        randomForDorcs = randm(100);
        // Check if generated random number include in dorcPercentage
        if(randomForDorcs < dorcPercentage){
            // Create a dorc entity
            dorcEntity = (EntityType*) malloc(sizeof(EntityType));
            strcpy(dorcEntity->avatar,"d");
            // Set the start row of the dorc
            dorcEntity->currPos = 2;
            // Position the dorc in one of the three columns
            randomForDorcColumn = randm(100);
            if( randomForDorcColumn < columnPercentage ){
                dorcEntity->path = 10;
            }else if( randomForDorcColumn < columnPercentage * 2){
                dorcEntity->path = 12;
            }else{
                dorcEntity->path = 14;
            }
            // Store the entity in race's dorcs array
            race->dorcs[race->numDorcs] = dorcEntity;
            // Create a thread for this t dorc
            pthread_t dorcThread;
            pthread_create(&dorcThread, NULL, goDorc, dorcEntity);
            // Store the thread in the entity structure-Created for the dorc.
            dorcEntity->thr = dorcThread;
            // Add number of dorcs
            //race->dorcs[race->numDorcs] = dorcEntity;
            race->numDorcs = race->numDorcs + 1;
            // Sleep 250,000 micro-seconds
            usleep(250000);
        }
    }




    pthread_join(timmyEntity->thr, NULL);
    pthread_join(haroldEntity->thr, NULL);

    int i;
    for(i=0; i<race->numDorcs; i++){
        pthread_cancel(race->dorcs[i]->thr);
    }

    cleanupNcurses(37);
    freeAllRunnerEntity(timmyEntity, haroldEntity);
    freeDorcs();
    freeRunners();
    free(race);
}


