# Compile and Run:
```sh
$ gcc q1.c -o q1
$ ./q1
```

# Algorithm
## Student:
```
initialise Starting time of that student.

Approve Parent to Create Next Thread. ( Just before starting wait for maschine. -> to Ensure FCFS)

start waiting for machine.

if patience Exceeded:
    Leaves the washing area.

    Updates the wasted time.

    bump up unhappy student count.


else (if he got an empty washing machine before his patience ran out):
    aquire the machine.

    use for washing ( block machine for W_i time ).

    release the machine for others use.

    bump up the wasted time ( due ti waiting ).

Leave the Washing Area. (In Both Cases)
```

## main:
( Equivalent to someone standing and coordinating Students )
```
Wait For Students to Come.

confirm if the previous student stabilised in the washing area.

send student to washing area.

wait for all students to return.

notedown total wasted time and unhappy Percentage, Report the observation to student body.
```

# Implementation:
## Student:
* Threads are used to simulate students.
* Waits for Machine until His patience tolerance using `sem_timedwait()`
* depict that he is using machine using `sleep()`
* leave the maschine using `sem_post()`

## Machines:
* Semaphores `sem_t Machines`used to keep track of the machines.
* initialised using `sem_init()` to M

## Unhappy and WastedTime:
* While Updating`Unhappy`,`WastedTime` , to avoid clashing of critical section (in Students ), Mutex locks `UnhappyMtx` , `WastedMtx` were Used.

## Other:
* additionally Semaphor `Creation` is used to avoid abiguity in situation where >2 students come at the same time (So that lower indexed Students are awarded machine first).

