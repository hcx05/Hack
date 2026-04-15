
---
Share : Code, data(static/dynamic/static vars), OS resources
Not share (thread control block) : ID, PC, register set, stack(local vars)

Benefits :
	Multithread can be running in parallel on diff processors, Share the memory and resources of the process they belong, Economy, etc.
	
Concurrency (scheduling, focus on one core) ; Parallelism (simultaneous, multicore execution)

User threads :
	POSIX Pthreads(UNIX, Linux, iOS, macOS), Java threads(JVM)
	Fast to create and manage
	Managed without kernel support
	
Kernel threads :
	Win32, POSIX Pthreads, Mac OS X, iOS, Android
	
Multithreading Models :
	Many-to-One :
		Benefits : Thread management is done by **thread library** in user space, efficient
		Drawbacks : One’s blocked, all blocked, Not really Parallelism
	One-to-One :
	Benefits : More concurrency
	Drawbacks :
		Slower creation (create a user thread → create a kernel thread)
		May crashed when there are to many kernel thread
	Many-to-Many :
		User threads num ≥ Kernel threads num
		The num of kernel threads depends on applications or machines
		Benefits : Flexibility, Parallelism
		Drawbacks : hard to implement
		→ Variation : two-level model : Enable user to let particular thread working independently

Threads Libraries :
	Pthreads(Implementation under the POSIX specification) :
		Pthread Creation :
		main
		`…`
		`pthread_create(&thread1, NULL, func1, &arg);`
		`pthread_join(thread1, *status);`
		thread1
		`func(&arg){`
		`…`
		`return(*status)`
		`}`
		`&thread1 : pthread_t, specify its address`
		`NULL : attribute, control thread’s behavior (including stack size and scheduling information)`
		`func1 : function pointer`
		`&arg : parameter of the function, constrain the number at 1, pack if needed`
	Pthread Joining & Detaching :
		`pthread_create()` returns a thread token, `pthread_join()` blocks the main thread and waits for the thread and retrieves its return value. If loading data isn’t needed, call `pthread_detach()`instead of `pthread_join()`, tells the system to release resources once it finishes, otherwise, those resources taken from child threads will become a **zombie thread**
	Linux Threads :
		Threads in Linux are represented as tasks that share the same address space; each thread corresponds to a `task_struct` in the kernel
		Use the fork system call to create a new process and a copy of the associated data of the parent process

Implicit Threading (run-time libraries and compilers handle the creation and management of threading) :
	Threads pool :
	A set of threads sit and wait for work
	Work well when the tasks can be executed **asynchronously**
	Benefits : Task can be scheduled, faster, etc.
	Java thread pool (three factory methods):
		Single thread executor `newSingleThreadExecutor()`
		Fixed thread executor `newFixedThreadPool(int size)`
		Cache thread executor `newCachedThreadPool()`
	Fork-Join Parallelism :
		Fork-Join parallelism follows an asynchronous execution model with explicit synchronization at the join phase
		Normally serve as explicit thread creation, but also a good choice for implicit threading
		Threads are not created directly during `fork()` , instead, existing threads from a pool execute the forked tasks.
		A library manages the number of threads that are created and assign tasks to threads
		General algorithm :
			`Task(problem)`
			`if problem is small enough`
			`solve the problem directly`
			`else`
			`subtask1 = fork(new Task(subset of problem))`
			`subtask2 = fork(new Task(subset of problem))`
			`result1 = join(subtask1)`
			`result2 = join(subtask2)`
			`return combined results`
	OpenMP(Open Multi-Processing) :
		A set of compiler directives and an API for C, C++, FORTRAN
		Provide support for parallel programming in shared memory environments
		Identifies parallel regions as blocks of code that may rum in parallel
	Grand Central Dispatch :
		Task parallelism
		A technology developed by Apple
		Combination of a run-time library, an API, and language extensions
		Allow developers to identify sections of code(tasks) to run in parallel
		placing tasks on a **dispatch queue**(serial or concurrent)
	Threading Issue :
		`exec()` works in the same way as processes
	Thread Cancellation :
		Asynchronous cancellation(immediately), Deferred cancellation(periodically)
		Use an API to sets a thread’s cancellation
	In Java :
		`interrupt()` ← Deferred cancellation
		`pthread_testcancel();` ← cancellation point
	Thread-Local Storage :
		Local variables are visible only during a single function invocation
		TLS data are visible across function invocations