#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>



#define SIZE 15 //define a size of 15
char input[300];
int time_quantum;
int num_process;
int q_burst[SIZE];



/**
 *ready queue structure
 *
 */
    struct readyQueue{
    int priority;
    int name;
    int burstTime[SIZE];
    int waitTime[SIZE];
    int processes[SIZE];
    int timeQuantum;
    char algorithm[30];
};


/**
 * Create the number of ready queues as stated in the given input file
 * @param priority
 * @return a constructed ready queue
 */
struct readyQueue initialize(int *priority){


    int i = 0;
    struct readyQueue inputQueue;


    for(i = 0; i < SIZE; i++){
        inputQueue.processes[i] = i+1; //wait time array
        inputQueue.waitTime[i] = 0;
    }




    int burstTimes[] = {30, 10, 24, 20,17,4,7,11,8,9,5,6,3,2,1}; //set up the array

    for (i = 0; i < SIZE; i++){
        inputQueue.burstTime[i] = burstTimes[i];
    }


    inputQueue.priority = *priority;



    char *algo[] = {"sjf", "fcfs", "rr"};

    if(*priority == 1){
        strcpy(inputQueue.algorithm, algo[0]);
        inputQueue.timeQuantum = 0;
    }

    else if(*priority == 2){
        strcpy(inputQueue.algorithm, algo[1]);
        inputQueue.timeQuantum = 0;
    }

    else{
        strcpy(inputQueue.algorithm, algo[2]);
        inputQueue.timeQuantum = 4;
    }


    return inputQueue; //return ready queue
}



/**
 * first come first serve
 * @param queue
 * @param totalTime
 * @param fp
 */
void firstComeFirstServe(struct readyQueue *queue, int *totalTime, FILE *fp){



    int i;
    int j;
    int totalWaitTime = 0;


    printf("\nReady Queue 1 Applying FCFS Scheduling:\n\n");
    fprintf(fp, "\nReady Queue 1 Applying FCFS Scheduling:\n\n");


    for(i = 0; i < SIZE; i++){ //calculate the wait time for each progress
        for(j = 0; j < i; j++){
            queue->waitTime[i] = queue->waitTime[i]+queue->burstTime[j];
        }
        totalWaitTime = totalWaitTime + queue->waitTime[i];
        *totalTime = *totalTime+ queue->burstTime[i];

    }




    float avgWait = totalWaitTime/(float)SIZE; //calculate the average wait time


    printf("Order of selection by CPU: \n");
    fprintf(fp, "Order of selection by CPU: \n");

    for(i = 0; i < SIZE; i++){
        printf("p%d\t", queue->processes[i]);
        fprintf(fp, "p%d\t", queue->processes[i]);
    }



    printf("\n\nIndividual waiting times for each process: \n");
    fprintf(fp, "\n\nIndividual waiting times for each process: \n");

    for(i = 0; i < SIZE; i++){
        printf("p%d\t =  %d\n", queue->processes[i], queue->waitTime[i]);
        fprintf(fp, "p%d\t =  %d\n", queue->processes[i],  queue->waitTime[i]);
    }




    printf("\nAverage Waiting Time: %g ms\n", avgWait);
    fprintf(fp, "\nAverage Waiting Time: %g ms\n", avgWait);
}



/**
 * shortestJobFirst method
 * @param queue
 * @param totalTime
 * @param fp
 */
void shortestJobFirst(struct readyQueue *queue, int *totalTime, FILE *fp){

    // Initialize variables
    int i = 0;
    int j = i + 1;
    int totalWaitTime = 0;
    int tempBurst = 0;
    int tempProc = 0;
    int position = 0;



    printf("\nReady Queue 1 Applying SJF Scheduling: \n\n");
    fprintf(fp, "\nReady Queue 1 Applying SJF Scheduling: \n\n");


    for(i = 0; i < SIZE; i++){ //make process sorted via burst time
        position = i;


        for(j = i + 1; j < SIZE; j++)
        {
            if(queue->burstTime[j] < queue->burstTime[position]){
                position = j;
            }
        }


        tempBurst = queue->burstTime[i];
        queue->burstTime[i] = queue->burstTime[position];
        queue->burstTime[position] = tempBurst;


        tempProc = queue->processes[i];
        queue->processes[i] = queue->processes[position];
        queue->processes[position] = tempProc;
    }



    for(i = 0; i < SIZE; i++){ //calculate wait time for every process
        for(j = 0; j < i; j++){
            queue->waitTime[i] += queue->burstTime[j];
        }
        totalWaitTime += queue->waitTime[i];
        *totalTime += queue->burstTime[i];
    }




    printf("Order of selection by CPU: \n");
    fprintf(fp, "Order of selection by CPU: \n");

    for(i = 0; i < SIZE; i++){
        printf("p%d\t", queue->processes[i]);
        fprintf(fp, "p%d\t", queue->processes[i]);
    }

    float avgWait = totalWaitTime/(float)SIZE; //calculate the average wait time





    printf("\n\nIndividual waiting times for each process: \n");
    fprintf(fp, "\n\nIndividual waiting times for each process: \n");

    for(i = 0; i < SIZE; i++){
        printf("p%d\t =  %d\n", queue->processes[i], queue->waitTime[i]);
        fprintf(fp, "p%d\t =  %d\n", queue->processes[i],  queue->waitTime[i]);
    }


    printf("\nAverage Waiting Time: %g ms\n", avgWait);
    fprintf(fp, "\nAverage Waiting Time: %g ms\n", avgWait);
}

/**
 * round robin method
 * @param queue
 * @param totalTime
 * @param fp
 */
void roundRobin(struct readyQueue *queue, int total_time, FILE *fp){

    // Initialize variables
    int i = 0;
    int j = 0;
    int turnaroundTime=0;
    int tTime[SIZE];
    int proc = 0;
    int bursts = 0;
    int flag = SIZE;
    int z;
    int q;

    int fin[15];

    int startTime[SIZE];
    startTime[0] = 0;

    for(q = 0 ; q<SIZE; q++){ //save the start time for later use

        if(queue->burstTime[q] > queue->timeQuantum || queue->burstTime[q] == queue->timeQuantum) {
            startTime[q + 1] = startTime[q] + queue->timeQuantum ;
        }
        if(queue->burstTime[q] < queue->timeQuantum){
            startTime[q + 1] = startTime[q] + queue->burstTime[q];
        }


    }


    for(z = 0; z<SIZE; z++){
        tTime[z] = 0;
    }



    /* Print to output and to a file */


    printf("\nReady Queue 1 Applying RR Scheduling:\n\n");
    fprintf(fp, "\nReady Queue 1 Applying RR Scheduling:\n\n");

    printf("\nOrder of selection by CPU: \n");
    fprintf(fp, "\nOrder of selection by CPU:\n");





    /* While loop that performs the round robin */
    /* While i is not equal to SIZE */
    while(i != flag){


        if(queue->burstTime[i] == 0){ //if burst time become 0, then goes to the next
            i++;
        }



        else{


            queue->burstTime[i] = queue->burstTime[i] - queue->timeQuantum; //minus time quantum for every pass by process

            turnaroundTime = turnaroundTime + queue->timeQuantum;




//            tTime[i] = tTime[i] + queue->timeQuantum ;


            if(queue->burstTime[i] < 0 || queue->burstTime[i] == 0){ //if burst time less than 0 or equal 0

                turnaroundTime = turnaroundTime + queue->burstTime[i];

                tTime[i] = turnaroundTime;




                queue->burstTime[i] = 0; //set burst time 0, and finish process


                printf("p%d\t", queue->processes[i]);
                fprintf(fp, "p%d\t", queue->processes[i]);

                fin[i] = queue->processes[i];


            }


            else{ //print the unfinished process

                printf("p%d\t", queue->processes[i]);
                fprintf(fp, "p%d\t", queue->processes[i]);

            }



            if(queue->burstTime[i] > 0){ //dequeue the burst time from ready queue

                /* Save process and burst time at i into a variable */
                proc = queue->processes[i];
                bursts = queue->burstTime[i];


                for(j = 0; j < SIZE-1; j++){ //shift to next element
                    queue->processes[j] = queue->processes[j+1];
                    queue->burstTime[j] = queue->burstTime[j+1];
                }

                /* Enqueue/place the original element i at the end of the queue */
                queue->processes[j] = proc;
                queue->burstTime[j] = bursts;


            }




        }
    }

    printf("\n\nTurnaround times for each process: ");
    fprintf(fp,"\n\nTurnaround times for each process: ");

    //calculate the real turnaround time

    for(i = 0; i<SIZE; i++) {

        tTime[i] = tTime[i] - startTime[fin[i]]+ queue->timeQuantum;

    }

    //print out the result

    for(i = 0; i<SIZE; i++) {
        printf("\np%d = ", fin[i]);
        printf("%d",tTime[i]);
    }



}

/**
 * main method
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char* argv[]){

    // Initialize variables
    int time1 = 0;
    int time2 = 0;
    int time3 = 0;
    int i = 0;
    FILE *fp;

    int q_num[SIZE];

    FILE* inf = fopen("cpu_scheduling_input_file.txt", "w+");
    if(inf == NULL){
        printf("Error");
        exit(1);
    }

    char buf[300];
    char* c;
    while(fgets(buf, sizeof(buf),inf)){
        c = strtok(buf, " ");
        c = strtok(NULL, " ");
        int queue_index = atoi(c);

        c = strtok(NULL," ");
        c = strtok(NULL," ");
        time_quantum = atoi(c);

        c = strtok(NULL, " ");
        int y = 0;
        while((c!=NULL&&y<SIZE)){
            q_num[y] = c;
            c = strtok(NULL, " ");
            q_burst[y] = c;
            c = strtok(NULL, " ");
            y++;


        }
    }




    fp = fopen("cpu_scheduling_output_file.txt", "w+");

    int priority = 2;
    struct readyQueue queue1 = initialize(&priority);


    priority = 3;
    struct readyQueue queue2 = initialize(&priority);


    priority = 1;
    struct readyQueue queue3 = initialize(&priority);


    struct readyQueue order[3]; //store into an array
    order[0] = queue1;
    order[1] = queue3;
    order[2] = queue2;


    for(i = 0; i < 3; i++){ //run the algorithm by steps


        if(strcmp(order[i].algorithm, "rr") == 0){ //if is rr, run round robin scheduling
            roundRobin(&order[i], &time1, fp);
        }


        if(strcmp(order[i].algorithm, "fcfs") == 0){ //run fcfs algorithm on this queue
            firstComeFirstServe(&order[i], &time2, fp);
        }


        if(strcmp(order[i].algorithm, "sjf") == 0){ // run sjf algorithm on this queue
            shortestJobFirst(&order[i], &time3, fp);
        }
    }


    return 0;

}
