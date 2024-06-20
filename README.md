- ## Name - Hardik Advani
- ## Enrollment No - 22113055
- ## Branch - Production and Industrial Engineering
- ## Project - CPU Scheduler
  
# CPU-Scheduling-Algorithms
This C++ program implements several CPU scheduling algorithms. Included algorithms are First Come First Serve (FCFS), Round Robin (RR), Shortest Process Next (SPN), Shortest Remaining Time (SRT), Highest Response Ratio Next (HRRN), Feedback (FB), and Aging.

## Table of Contents
- [CPU-Scheduling-Algorithms]
  - [Algorithms]
    - [Feedback (FB)]
    - [Round Robin with varying time quantum (RR)]
    - [Highest Response Ratio Next (HRRN)]
    - [Shortest Remaining Time (SRT)]
    - [Shortest Process Next (SPN)]
    - [Feedback with varying time quantum (FBV)]
    - [Aging]
    - [First Come First Serve (FCFS)]
  - [Installation]
  - [Input Format]
  - [TestCases Implemented]
  - [Takeaways]

## Algorithms Used in the code

### First Come First Serve (FCFS)
- FCFS, or First Come First Served, is a scheduling algorithm where the first arriving process is the first to be executed. It's straightforward but can result in inefficiency when long-burst processes delay shorter ones. FCFS lacks prioritization, making it a non-preemptive method. Regardless of burst time or priority, the earliest arriving process takes precedence. While common in batch systems where process order matters, FCFS may not optimize overall performance.

### Round Robin with varying time quantum (RR)
- Round Robin (RR) with a variable quantum is a CPU scheduling algorithm employing a time-sharing method to allocate CPU time among processes. Unlike traditional RR, where the quantum (time slice) is fixed, this version permits adjusting the quantum dynamically to accommodate process needs. Consequently, shorter-burst processes receive smaller quanta, optimizing resource utilization.

### Shortest Process Next (SPN)
- Shortest Process Next (SPN) is a scheduling algorithm that prioritizes process execution based on their burst time, representing the time required for task completion. It operates non-preemptively, meaning a process continues until it finishes or enters a waiting state.

- The algorithm manages a process queue, assigning each arrival a burst time. Execution begins with the process boasting the shortest burst time, while subsequent arrivals join the queue, sorted by burst time. Consequently, the process with the shortest burst time remains at the queue's forefront, ensuring its immediate execution.

### Shortest Remaining Time (SRT)
- Shortest Remaining Time Next (SRT) is akin to the Shortest Process Next (SPN) algorithm, but it operates preemptively. This implies that once a process commences execution, it can be interrupted by a newcomer boasting a shorter remaining time.

- The algorithm manages a process queue, assigning burst times upon arrival. Execution begins with the process possessing the shortest remaining time, while subsequent arrivals are incorporated into the queue, organized by their remaining time. Consequently, the process with the shortest remaining time consistently occupies the queue's forefront, ensuring its immediate execution.

### Highest Response Ratio Next (HRRN)

- Highest Response Ratio Next (HRRN) is a scheduling algorithm prioritizing process execution according to their response ratio. Operating non-preemptively, once a process begins execution, it continues until completion or a waiting state ensues.

- The response ratio assesses the waiting time of a process relative to its burst time. Processes are then ranked by their response ratio, with the highest-ranked process executed first. New arrivals are appended to the queue and sorted based on their response ratio. Consequently, the process boasting the highest response ratio perpetually occupies the queue's forefront, ensuring immediate execution.

### Feedback (FB)

 - Feedback is a multi-level priority scheduling algorithm distributing CPU time among processes based on their priority levels. It utilizes several priority queues, each designated for a distinct priority level.

 - Processes with higher priorities are granted CPU time foremost. Upon arrival, new processes are placed in the suitable priority queue corresponding to their priority level. Upon completion of execution, a process transitions to the subsequent lower priority queue.

### Feedback with varying time quantum (FBV)

 - Feedback with varying time quantum employs multiple priority queues, each with a unique time quantum assigned to its priority level. This design enhances algorithm efficiency by allocating more time to higher-priority processes and less time to lower-priority ones.

### Aging

 - To mitigate starvation, an aging scheduler is implemented. During each rescheduling operation, such as a timeout, the scheduler elevates the priority of all ready processes by a fixed increment. This strategy prevents starvation by ensuring that each ready process is overlooked by the scheduler only a limited number of times before attaining the highest priority.

## Installation
1. Duplicate the repository to your local machine.
   
2. Install the g++ compiler and make utility by executing the following command:
   ```bash
   sudo apt-get install g++ make
   ```
3. Compile the code by running the `make` command.

4. Execute the generated executable file to run the program.

## Input Format
- Line 1: "trace" or "stats"
- Trace: When the scheduler operates in "trace" mode, it provides detailed information or logs about the scheduling decisions made during the execution of the program. This information often includes timestamps, process IDs, CPU bursts, waiting times, and other relevant data points. Trace mode is useful for debugging, performance analysis, or understanding the scheduling behavior of the system.

- Stats: In "stats" mode, the scheduler generates summary statistics or metrics related to the scheduling performance. These statistics may include average waiting time, turnaround time, CPU utilization, throughput, and other performance indicators. Stats mode provides a high-level overview of how well the scheduler is performing in terms of efficiency, fairness, and responsiveness.

- Line 2: The input format specifies a comma-separated list indicating which CPU scheduling policies to analyze or visualize, along with their parameters if applicable. Each algorithm is represented by a number corresponding to the introduction section or the attached test cases.

- For Round Robin and Aging algorithms, a parameter specifies the quantum \( q \) to be utilized. Thus, a policy entered as "2-4" indicates Round Robin with \( q = 4 \), while "8-1" denotes Aging with \( q = 1 \).
 1. FCFS (First Come First Serve)
 2. RR (Round Robin)
 3. SPN (Shortest Process Next)
 4. SRT (Shortest Remaining Time)
 5. HRRN (Highest Response Ratio Next)
 6. FB-1, (Feedback where all queues have q=1)
 7. FB-2i, (Feedback where q= 2i)
 8. Aging
- Line 3: The input requires an integer indicating the final instant to be utilized in the simulation and displayed on the timeline. This value determines the duration of the simulation and sets the upper limit for time visualization.
- Line 4: The input expects an integer indicating the number of processes to simulate. This value determines the quantity of processes to be included in the simulation, influencing the complexity and duration of the analysis.
- Line 5: The description of processes begins, with each process detailed on a separate line. For algorithms 1 through 7, each process is delineated by a comma-separated list containing:

1. A string denoting the process name.
2. The arrival time of the process.
3. The service time required by the process.
   
### Testcases

![Screenshot 2024-06-20 204126](https://github.com/hargit19/CPU-Scheduler-ACM-Open/assets/127594594/e01f4e1d-de42-4e2f-be3f-0d90135e0979)

![Screenshot 2024-06-20 204427](https://github.com/hargit19/CPU-Scheduler-ACM-Open/assets/127594594/a7f7625b-5ec2-4acd-9126-ac37a422dbac)

![Screenshot 2024-06-20 204404](https://github.com/hargit19/CPU-Scheduler-ACM-Open/assets/127594594/8d0ad691-a9bf-4810-aa3f-ee9aa55cef4d)

![Screenshot 2024-06-20 204250](https://github.com/hargit19/CPU-Scheduler-ACM-Open/assets/127594594/97f43fe3-61a5-405b-8130-91ee6bd9f78d)

### Takeaways
- Gain in-depth knowledge of various CPU scheduling algorithms, including their advantages, disadvantages, and use cases.
- Learn how different algorithms handle processes and their impact on system performance.
- Gain insights into operating system concepts and how CPU scheduling is managed at the OS level.




