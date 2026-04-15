
---
Non-preemptive(cooperative) scheduling

Preemptive scheduling :
	Require process synchronization
	Incurs a cost associated with access to shared data

CPU scheduler : Select a process to execute

Dispatcher : Switching the context, switching to user mode, jump to the proper location in the selected program
	Latency : Scheduling time, interrupt re-enable time, context switch time

Scheduling Algorithms :
	Criteria :
		CPU utilization, Throughput, Waiting time,
		Turnaround time : From submission to complete,
		Response time : From submission to the first response
	Approximate SJF :

$$ \tau_{n+1} = \alpha t_n + (1-\alpha)\tau_n $$
	Round-Robin Scheduling :
		Each process get a small unit of CPU time (time quantum)
		Typically, higher average **turnaround time** than SJF, but better **response time**
		TQ should be larger than 80% of CPU bursts
	Priority Scheduling :
		SJF is a priority scheduling where priority is the predicted next CPU burst time
		Use aging to solve starvation(indefinite blocking) problem (as time progresses increase the priority of processes)
	Multilevel Queue Scheduling :
		Having separate queues for each distinct priority, and priority scheduling simply schedules the process in the highest-priority queue.
		Ready queue is partitioned into separate sub-queues :
		Each sub-queue has its own scheduling algorithm
		In no particular order
		Scheduling must be done between queues :
			→Fixed priority scheduling: possibility of starvation
			→Time slice – each queue gets a certain amount of CPU time(e.g. 80%, 20%)
		Also works well when priority scheduling is combined with RR :
			If there are multiple processes in the highest-priority queue, they are executed in round-robin order
	Multilevel Feedback Queue Scheduling :
		In particular order
		A process can move between the various queues; aging can be implemented
		separate processes according to the characteristic of their CPU burst :
			I/O burst first(CPU burst short, SJF)
		Multilevel Feedback Queue :
			Numbers of queue, Scheduling algorithms, Methods used to determine when to upgrade/demote a process
		
Multi-Processor Scheduling :
	Asymmetric multiprocessing :
		Main processor (system) :
			Handles all system activities, can be a performance bottleneck
			Only one processor access kernel data, reduce the need for data sharing
		Other processor (application)
	Symmetric multiprocessing (SMP) :
		Self-scheduling
		Need synchronization mechanism
		Two possible strategies for organizing the threads eligible to be scheduled :
			All threads may be in a common ready queue :
				Possibly race condition; must be made as a critical section
				CPU can easily takeover other’s work
		Each processor may have its own private queue of threads :
			The queue is free from race condition; more common
			Need load balancing
	Load-balancing :
		Only necessary on systems where each processor has its own private queue of eligible processes to execute
		Often implemented in parallel
		Reduce waiting time

Multi-core Processor Scheduling :
	Multi-core Processor:
		Faster and consume less power
		Memory stall : When access memory, it spends a significant amount of time waiting for the data become available. (e.g. cache miss)
	Multi-threaded multi-core systems :
		i.e. Intel Hyper-threading(Chip-multithreading, CMT)
		Takes advantage of memory stall to make progress on another thread while memory retrieve happens
	Two ways to multithread a processor :
		Coarse-grained : Flush the instruction pipeline, the cost is high
		Fine-grained : Thread switching at the boundary of an instruction cycle
	NUMA (non-uniform memory access) :
		Occurs in systems containing combined CPU and memory boards
		If the operating system is NUMA-aware, it will assign memory closes to the CPU the thread is running on.

Little’s Formula :
	n = average queue length
	W = average waiting time in queue
	λ = average arrival rate into queue
	n = W x λ