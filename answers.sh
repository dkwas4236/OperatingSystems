# Darion Kwasnitza
# ID#: 3122890
# Assignment: CMPT 360 - Lab 1

# Answer 1
# a) This will simulate 3 CPU operations, then 5 CPU operations, and then 4 CPU operations. The -c flag will 
# display which processes will be scheduled and when, so I expect there to be a list of 12 time units in total;
# 3 time units will be executing process ID 0, 5 time units will be executing process ID 1, and 4 time units will 
# be executing process ID 2, all of them executing on the CPU because the second parameter of each process pair
# is 100.
./process-run.py -l 3:100,5:100,4:100 -c

# b) The CPU utilization should be 100 percent in each case because the second parameter of each of the process
#  pairs is 100, meaning the CPU will be used 100 percent of the time to run the processes. To justify this, 
#  you can see the execution below with the -p flag shows the CPU is running the processes 100 percent of 
#  the time.
./process-run.py -l 3:100,5:100,4:100 -p

# c) Changing the order of the jobs should not change the overall execution time for this workload, this is 
# because the processes are all running 100 percent by the CPU in any order, and there is no time waiting
# for I/O to complete. As expected, the execution below with a different execution order still takes 12 units
# of time in total just like the original execution.  
./process-run.py -l 4:100,3:100,5:100 -c

# Answer 2
# a) Below is the modified 3-step job only issuing I/O instructions.
./process-run.py -l 3:0,5:0,4:0 -c

# b) Changing the order of the jobs should change the overall execution time for this workload. This is
# because the processes are all running 100 percent on I/O, meaning that depending on the order, processes
# will be blocked for shorter or longer times until I/O is done. As evidence of this, the execution below
# has a different order of jobs and takes 2 more time units to complete compared to the original execution.
./process-run.py -l 4:0,3:0,5:0 -c

# c) The CPU utilization for this workload is 0 as shown by the execution below. This is different from 
# question 1 because in question 1, 100 percent of processes were executed by the CPU. This change is 
# generally not great as you can see by the past executions that show I/O processes take longer to execute
# than CPU processes. It would be great if the CPU could be doing useful work all of the time, however I/O
# processes can cause the CPU to wait until the I/O process is done.
./process-run.py -l 3:0,5:0,4:0 -p

# Answer 3
# a) The default switch policy is -S SWITCH_ON_IO because the system is switching to another ready process
# when an I/O operation begins. Evidence of this is shown by processes being READY and then being executed
# by the I/O when another I/O operation begins. The execution below has the SWITCH_ON_IO flag and is exactly
# the same as the original execution. 
./process-run.py -l 3:0,5:0,4:0 -S SWITCH_ON_IO -c

# b) The difference between SWITCH_ON_IO and SWITCH_ON_END is that when the SWITCH_ON_END flag is set, the 
# system will not switch to another process when an I/O operation begins. As evidence of this, the execution
# below with the SWITCH_ON_END flag set takes many more time units than the execution with SWITCH_ON_IO 
# execution
./process-run.py -l 3:0,5:0,4:0 -S SWITCH_ON_END -c

# c) The executions below show the main difference between the SWITCH_ON_IO policy and the SWITCH_ON_END
# policy as being how many time units it takes to complete all processes. Even though the difference in 
# time units is not as durastic in my made up example when compared to the difference in part A and B, 
# there is still time a difference especially in processes using more I/O and less CPU.  
./process-run.py -l 3:20,5:80,4:60 -S SWITCH_ON_IO -c
./process-run.py -l 3:20,5:80,4:60 -S SWITCH_ON_END -c

# Answer 4
# a) The default policy is the -I IO_RUN_LATER policy because there are times in the execution when the 
# I/O shows that it is done but then another process is not issued right away. There is a waiting time 
# before the next I/O process is completed again meaning that the default is IO_RUN_LATER. As evidence 
# of this, both executions below act in the same way. 
./process-run.py -l 3:0,5:100,5:100,5:100 -c 
./process-run.py -l 3:0,5:100,5:100,5:100 -c -I IO_RUN_LATER

# b) The policy running below is the IO_RUN_IMMEDIATE policy and it runs quicker than the alternative
# IO_RUN_LATER policy. As you can see from the execution below, there is no waiting for a process to
# be ran after an I/O process is done, instead a process is switches back to the process that issues it
# right away. 
./process-run.py -l 3:0,5:100,5:100,5:100 -c -I IO_RUN_IMMEDIATE
 
# Answer 5
# a) The example workload I created has a difference of 16.9 percent. The reason the difference is so
# noticeable is because CPU is executing the majority of the processes in the executable example below.
# The executable with the IO_RUN_IMMEDIATE is allowing the CPU to execute processes after I/O is done,
# whereas the IO_RUN_LATER might not immediately allow the CPU to execute process after I/O is done. 
# The difference in these policies creates a time difference of 59 units to 71 units respectively.
./process-run.py -l 10:70,15:80,20:90 -c -I IO_RUN_IMMEDIATE
./process-run.py -l 10:70,15:80,20:90 -c -I IO_RUN_LATER

# Answer 6
# a) The example that I made below actually has the exact same time units to execute both policies. I
# knew moving closer to I/O executing processes more would allow the difference in CPU running to be 
# less noticeable like in question 5. However, if too much of the execution is reliant on I/O then 
# there will be seperation once again. Interestingly enough, 60 percent CPU was my first try and 
# ended up being exactly 81 time units to 81 time units respectively spent completing all processes.
# The difference is exactly 0 for the example I made below. 
./process-run.py -l 10:60,15:60,20:60 -c -I IO_RUN_IMMEDIATE
./process-run.py -l 10:60,15:60,20:60 -c -I IO_RUN_LATER

# all the work I have done above is my own - Darion
