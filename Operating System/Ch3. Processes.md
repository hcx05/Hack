
---
Layout of a process in memory :
	Text(compiled code) → Data(global vars) → Heap(dynamic vars) → Stack(functions parameters, return addresses, local vars)
		low memory → high memory
	Stack and Heap start at opposite ends and grow towards each other
	Size of Text and data section are fixed, stack and heap are not

Program counter (called Instruction Pointer in x86 architecture) :
	Represents the status of the current activity of a process
	Point to the address of the next instruction

Process Control Block(task control block) :
	Exist only in kernel memory
	Process state, Program counter, CPU registers, CPU scheduling information, Memory-management information, Accounting information, I/O status information
	Linux’s `tast_struct` :
		In Linux kernel, all active processes use double linked list
		The kernel maintains a current pointer points to the process currently executing

Thread,

Process Scheduling :
	Degree of multiprogramming : The number of processes currently in memory
	Time sharing : switch CPU frequently
	Scheduling Queues :
		Job queue(New state) : All processes in the system
		Ready queue(Ready State) : Processes in main memory, wait until selected, or dispatched
		Wait queue(Wait State) : Processes waiting for an I/O device
	Schedulers :
		Long-term scheduler(job scheduler) :
			Control degree of multiprogramming(both too high or too low will cause CPU being idle)
			Selects a good mix of CPU-bound & I/O-bound
			Selects which process should be loaded into memory and brought into the ready queue
			New state → Ready state
		Short-term scheduler(CPU scheduler) :
			Selects which process should be executed and allocated CPU
			Ready state → Run state
		Medium-term scheduler :
			Sometimes needs to reduce the degree of multiprogramming
			Selects which processes should be swapping in/out memory
			Ready state → Wait state

Context Switch :
	Kernel saves the state of the old process
	Context-switch time is purely overhead
		→memory speed,
		→number of register,
		→special instructions : a single instruction to save/load all registers
		→hardware support : multiple sets of registers

Operations on Processes :
	Process Creation :
		The `systemd` process serves as the root parent process(PID = 1)
		Resource sharing(depends on the design of the OS) :
			Share all, share some, no sharing
		Two possibilities of execution :
			Concurrently, parent wait until children terminate
		Two possibilities of address space
			Duplicate, PC stays the same, different address, communication via sharing vars
			New program loaded into it, reset PC, different address, communication via message passing
		Unix/Linux Pross Creation :
			`fork()` system call :
				Duplicates, execute concurrently
			`execlp()` system call :
			Load a new binary file into memory
		Memory space of `fork()` :
			Currently we use copy-on-write technique to store differences, not copy all the content(child will grow slowly at run-time)

Process Termination :
	`exit()`:
		All resources, including physical & virtual memory, open files, I/O buffers, are deallocated by the OS
		The process may return a status value(an integer) to its waiting parent
	After calling `exit()` , its entry in the process table must remain until the parent process calls `wait()`, ‘cuz the process table contains the process’s exit status
	Zombie process :
		A process that has terminated, but its parent hasn’t called `wait()`
	Orphan process :
		One’s parent terminated without calling `wait()`
		Solution : `init` process(`systemd` process) periodically invokes `wait()` to collect the orphan child
		Linux also allows other process to be their parents
	Parent can abort children via specifying its PID
	Cascading termination :
		Do not allow the existence of orphan process(initialized by the OS)

Inter-process Communication :
	Purposes for process cooperation : Information sharing, Computation speedup, Modularity
	Shared memory :
		Require more careful user synchronization
		Require system calls only to establish shared-memory regions
	Message passing :
		No conflict : More efficient for small data generally(no need for synchronize)
		Use `send`/`recv` (memory buffer + message) message(synchronization)
	IPC in Shared memory :
		Normally, OS try to prevent one process from accessing another process’s memory
		Consumer & Producer :
			bounded buffer(circular) :
				next free : in(pointer, global var)
				first available : out(pointer, global var)
				empty : in = out
				full : (in + 1)% B = out
	IPC in Message-Passing :
		Needs Message system to implement Send/ Receive
		Slower, due to memory copy
		Implementation of communication link :
			→physical(shared memory, HW bus, or network)
			→logical(Direct or indirect communication, Symmetric or asymmetric communication, Blocking or non-blocking, Automatic or explicit buffering, etc.)
				Naming :
					Direct communication :
						Name each other explicitly(Send P, Receive Q)
						Links are established automatically
						One-to-One between links and processes(Drawback : hard coded)
						The link usually be bi-directional
					Indirect communication :
						Messages are directed and received from mailboxes(ports)
						Each mailbox has an unique ID
						Link established only if processes share a common mailbox
						Many-to-Many between links and processes
						The link may be unidirectional or bi-directional
						Mailbox owns by either OS or processes
	Synchronization :
		Blocking(synchronous), non-blocking(asynchronous)

Communication in Client-Server Systems
	Remote Procedure Call :
		Remote function code(blocking code, won’t return until terminate)
		Client stubs(little daemon runs on client) :
			Pack parameters into a message(i.e. parameters marshaling)
	Server stub :
		receives the message, unpacks the parameters, and performs the procedure
	Use External Data Representation (XDR) format
	At most once : Every message has a timestamp, letting server to detect repeated message
	Exactly once : Implement At most once procedure and return ACK to client
	Fixed port address :
		RPC call has a fixed port number at compile time
	Rendezvous mechanism :
		OS provide a rendezvous(matchmaker) daemon on a fixed RPC port