// OS Project 4
// Bruce C. Miller
// Round robin scheduling simulation, quantums set to 3 and 10 ticks.

import java.util.*;
import java.io.*;
import java.lang.String;

class scheduling {
    // public variables
    public static int clock = 0;
    public static int count = 0;
    public static int currentProcess = 0;
    public static int finishedProcesses = 0;
    public static int i, j, k;
    public static int c1 = 0, c2 = 0;
    public static double avgRunTime = 0;
    public static double avgResponseTime = 0;
    public static String procName;
    public static Random ranNumGen;
    // array of processes to act as a queue
    public static process[] pQ = new process[2000];

    // main
    public static void main(String[] args) {
        ranNumGen = new Random();       // random number

        // init queue with empty proc
        for (i = 0; i < 2000; i++) {
            pQ[i] = new process();
        }

        while (clock < 10000) {
            // check if there are processes to run
            if (availableProcesses()) {
                // if current process is not finished, increment clock
                if (!pQ[currentProcess].finished()) {
                    pQ[currentProcess].tick();
                    // if this is the first tick for a process, set the
                    // start time to the clock
                    if (pQ[currentProcess].runTime == 1) {
                        pQ[currentProcess].startTime = clock;
                    }
                } else if (pQ[currentProcess].finished()) {
                    // the current process is finished, set the finish time
                    // to clock
                    pQ[currentProcess].finishTime = clock;
                    // for priority (not working)
                    //      pQ[currentProcess].priority = 11;
                }
                // randomly spawn new processes at a ratio of 17
                // processes per 100 ticks
                i = ranNumGen.nextInt() % 100 + 1;
                if (i <= 17) {
                    spawnProcess();
                    count++;
                }
                //      Arrays.sort(pQ);        // for priority (not working)
                //      currentProcess = 0;     // for priority (not working)
                if ((clock % 10) == 0) {        // for round robin
                    // time slicing algorithm, set to slice every
                    // 10 ticks
                    if (currentProcess < count) {
                        currentProcess++;
                    } else if (currentProcess == count) {
                        currentProcess = 0;
                    }
                }
                clock++; // increment the clock
            }
        }

        // calculate the average and finished processes
        for (k = 0; k < 2000; k++) {
            avgRunTime = avgRunTime + pQ[k].runTime;
            if (pQ[k].responseTime() >= 0 && pQ[k].finishTime != 0) {
                avgResponseTime = avgResponseTime + pQ[k].responseTime();
            }
            if (pQ[k].finishTime > 0) {
                finishedProcesses++;
            }
            // display running information
            System.out.println(pQ[k].id + " " + pQ[k].duration + " " +
                               pQ[k].priority + " " + pQ[k].arrivalTime + " " +
                               pQ[k].startTime + " " + pQ[k].finishTime);
        }

        avgRunTime = (avgRunTime / count);
        avgResponseTime = (avgResponseTime / finishedProcesses);

        // display compiled data
        System.out.println("avgRunTime: " + avgRunTime);
        System.out.println("avgResponseTime: " + avgResponseTime);
        System.out.println("finishedProcesses: " + finishedProcesses);
    }

    // algorithm to spawn a virtual process with random priority and run time
    public static void spawnProcess() {
        Integer procID = new Integer(count);
        procName = "proc" + procID.toString();

        i = ranNumGen.nextInt() % 10 + 1;
        j = ranNumGen.nextInt() % 10 + 1;

        if (count < 2000) {
            // insert process into the queue
            pQ[count] = new process(procName, clock, i, j);
        }
    }

    // boolean method to determine if there are available processes to run
    public static boolean availableProcesses() {
        if (count > 0) {
            for (c2 = 0; c2 <= c1; c2++) {
                if ((pQ[c2].duration - pQ[c2].runTime) > 0) {
                    c1++;
                }
                if (c1 > 0) {
                    return true;
                } else {
                    return false;
                }
            }
        }
        return true;
    }
}

// process class
class process implements Comparable {
    // public variables
    public String id;
    public int arrivalTime;                     // time process arrives into simulation
    public int startTime;                       // time process starts running
    public int finishTime;                      // time process finishes
    public int duration;                        // time process needs to finish
    public int priority;                        // priority of process
    public int runTime;                         // processor time devoted to process

    // default constructor
    process() {
        id = "default";
        arrivalTime = 0;
        startTime = 0;
        finishTime = 0;
        duration = 0;
        priority = 0;
        runTime = 0;
    }

    // constructor
    process(String pId, int pArrivalTime, int pDuration, int pPriority) {
        id = pId;
        arrivalTime = pArrivalTime;
        duration = pDuration;
        priority = pPriority;
        runTime = 0;
    }

    // increment the runtime
    public void tick() {
        runTime++;
    }

    // returns response time
    public double responseTime() {
        if ((startTime - arrivalTime) > 0) {
            return (startTime - arrivalTime);
        } else {
            return -1;
        }
    }

    // boolean method to determine if process is finished
    public boolean finished() {
        if (runTime >= duration) {
            return true;
        } else {
            return false;
        }
    }

    // compare method based first on priority, then on arrivalTime
    public int compareTo(Object thisProcess) {
        process p = (process) thisProcess;
        if ( this.priority > p.priority) {
            return 1;
        } else if (this.priority < p.priority) {
            return 0;
        } else {
            return (this.arrivalTime - p.arrivalTime);
        }
    }
}

