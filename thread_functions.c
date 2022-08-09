#include "defs.h"

void* goRunner(void *runner){
    RunnerType thisRunner = *((RunnerType*) runner);
    int randomRow;
    int randomStep;
    int newPosition;
    char *str;
    while(thisRunner.dead == 0 && thisRunner.ent.currPos != 2){
        // Generate random step and store newPosition for later to check if there will be any collisions.
        randomRow = randm(100);
        if(randomRow < 70){
            randomStep = randm(4);
            //newPosition = thisRunner.ent.currPos - randomStep;
            while( thisRunner.ent.currPos - randomStep < 2 ){
                randomStep = randm(4);
            }
            newPosition = thisRunner.ent.currPos - randomStep;
            // Decrement health points accordingly
            if(thisRunner.health - randomStep < 0){
                thisRunner.health = 0;
            }else{
                thisRunner.health = thisRunner.health - randomStep;
            }
        }else{
            randomStep = randm(3);
            //newPosition = thisRunner.ent.currPos - randomStep;
            while( thisRunner.ent.currPos + randomStep > 35){
                randomStep = randm(3);
            }
            newPosition = thisRunner.ent.currPos + randomStep;
            //newPosition = thisRunner.ent.currPos + randomStep;
        }
        // Lock the race's semaphor
        sem_wait(&race->mutex);
        int i;
        for( i = 0; i < race->numDorcs; i++ ){
            if(race->dorcs[i]->currPos == newPosition && race->dorcs[i]->path == thisRunner.ent.path){
                mvprintw(race->statusRow, 30, "STATUS: collision between %s and dorc", thisRunner.name);
                refresh();
                race->statusRow = race->statusRow + 1;
                // decrease the runner's health by 3 points
                if(thisRunner.health - 3 < 0){
                    thisRunner.health = 0;
                }else{
                    thisRunner.health = thisRunner.health - 3;
                }
                // delete this?
                // break;
                //break;
            }
        }
        // Unlock the race's semaphor
        sem_post(&race->mutex);
        // Check if the runner is dead
        if(thisRunner.health <= 0){
            strcpy(thisRunner.ent.avatar, "+");
            thisRunner.dead = 1;
            sem_wait(&race->mutex);
            mvprintw(race->statusRow, 30, "STATUS: %s is dead", thisRunner.name, thisRunner.ent.currPos, thisRunner.ent.path);
            refresh();
            sem_post(&race->mutex);
            race->statusRow = race->statusRow + 1;
        }

        sem_wait(&race->mutex);
        // Clear last position
        mvprintw(thisRunner.ent.currPos, thisRunner.ent.path, " ");
        refresh();
        // Move the runner to new position
        mvprintw(newPosition, thisRunner.ent.path, "%s", thisRunner.ent.avatar);
        refresh();
        // 2,38 2,42
        if(!strcmp(thisRunner.name,"Timmy")){
            mvprintw(2, 38, "%2d", thisRunner.health);
            refresh();
        }else if(!strcmp(thisRunner.name,"Harold")){
            mvprintw(2, 42, "%2d", thisRunner.health);
            refresh();
        }else{
            mvprintw(race->statusRow, 30, "Error:");
            refresh();
        }
        sem_post(&race->mutex);
        // Update current position
        thisRunner.ent.currPos = newPosition;

        usleep(250000);
        //if(thisRunner.dead == 1){
        //    mvprintw(race->statusRow, 30, "STATUS: %s is dead %d %d", thisRunner.name, thisRunner.ent.currPos, thisRunner.ent.path);
        //}
    }
    if(thisRunner.dead == 0 && strcmp(race->winner, "") == 0){
        sem_wait(&race->mutex);
        mvprintw(race->statusRow, 30, "OUTCOME: The winner is %s", thisRunner.name);
        refresh();
        sem_post(&race->mutex);
        race->statusRow = race->statusRow + 1;
        strcpy(race->winner, thisRunner.name);
    }
    sem_wait(&race->mutex);
    mvprintw(thisRunner.ent.currPos, thisRunner.ent.path, "%s", thisRunner.ent.avatar);
    refresh();
    sem_post(&race->mutex);
    //strcpy(race->winner, " ");
    //RunnerType thisRunner = *((RunnerType*) runner);
    //scrPrt(thisRunner->name, 11, 12);
}




void* goDorc(void *dorc){

    //Typecasting the parameter
    EntityType *thisDorc = (EntityType*) dorc;

    //mvprintw(thisDorc.currPos, thisDorc.path, "%s", thisDorc.avatar);
    //refresh();

    // Declare used variables
    int newRow, newColumn, randomRow, randomColumn;
    while(thisDorc->currPos != 35){

        randomRow = randm(5);
        newRow = thisDorc->currPos + randomRow;
        if(newRow > 35){
            newRow = 35;
        }
        // randBetween
        //newColumn = rand()%(((thisDorc->currPos+2)+1)-(thisDorc->currPos-2)) + (thisDorc->currPos+2);
        newColumn = randBetween(thisDorc->currPos-2, thisDorc->currPos+2);
        if(newColumn != 14 && newColumn != 12 && newColumn != 10){
            //diff = abs(newColumn - 12) < abs(newColumn - 14) ? 12 : 14;
            //diff = abs(newColumn - 10) < abs(newColumn - 12) ? 10 : 12;
            newColumn = getDorcColumn(newColumn);
        }

        sem_wait(&race->mutex);
        // Clear last position
        mvprintw(thisDorc->currPos, thisDorc->path, " ");
        refresh();

        // Move the dorc to new position
        mvprintw(newRow, newColumn, "%s", thisDorc->avatar);
        refresh();

        sem_post(&race->mutex);
        thisDorc->currPos = newRow;
        thisDorc->path = newColumn;

        usleep(700000);
    }
    sem_wait(&race->mutex);
    mvprintw(thisDorc->currPos, thisDorc->path, "");
    refresh();
    sem_post(&race->mutex);
}
