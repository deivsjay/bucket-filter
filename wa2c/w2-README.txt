Documentation for Warmup Assignment 2
=====================================

+-------+
| BUILD |
+-------+

Comments: Type "make" and the files will all compile. 

+---------+
| GRADING |
+---------+

Basic running of the code : 100 out of 100 pts

Missing required section(s) in README file : -0
Cannot compile : -0
Compiler warnings : -0
"make clean" : -0
Segmentation faults : -0
-2 (will document below as well)    In a future test, the combination: -r 1000 -B 1000000 -P 1000000 results in a Bus Error
Separate compilation : -0
Delay trace printing : -0   (added by me, because it was included in the grading guidelines but not the README file) 
Using busy-wait : -0
Handling of commandline arguments:
    1) -n : -0
    2) -lambda : -0
    3) -mu : -0
    4) -r : -0 (takes a really long time to run 1000 case)
    5) -B : -0
    6) -P : -2  combination: -r 1000 -B 1000000 -P 1000000 has problems, when I ran it the first time, it returned a Bus Error, when I ran it kept going and didn't stop
Trace output :
    1) regular packets: -0
    2) dropped packets: -0
    3) removed packets: -0
    4) token arrival (dropped or not dropped): -0
    5) timestamps: -0   (added by me, because it was included in the grading guidelines but not the README file)
Statistics output :
    1) inter-arrival time : -0
    2) service time : -0
    3) number of customers in Q1 : -0
    4) number of customers in Q2 : -0
    5) number of customers at a server : -0
    6) time in system : -0
    7) standard deviation for time in system : -0
    8) drop probability : -0
Output bad format : -0
Output wrong precision for statistics (should be 6-8 significant digits) : -0
Large service time test : -0
Large inter-arrival time test : -0
Tiny inter-arrival time test : -0
Tiny service time test : -0
Large total number of customers test : -0
Large total number of customers with high arrival rate test : -0
Large inter-packet-arrival time <Cntrl+C> test : -0 (added by me, because it was included in the grading guidelines but not the README file)
Large inter-token-arrival time <Cntrl+C> test : -0  (added by me, because it was included in the grading guidelines but not the README file)
Dropped tokens test : -0
Cannot handle <Cntrl+C> at all (ignored or no statistics) : -0
Can handle <Cntrl+C> but statistics way off : -0
Cannot stop token depositing thread when required : -0  (added by me, because it was included in the grading guidelines but not the README file)
Not using condition variables and do some kind of busy-wait : -0
Synchronization check : -0
Deadlocks : -0
Bad commandline or command : -0 (added by me, because it was included in the grading guidelines but not the README file)

+----------------------+
| BUGS / TESTS TO SKIP |
+----------------------+

Is there are any tests in the standard test suite that you know that it's not
working and you don't want the grader to run it at all so you won't get extra
deductions, please list them here.  (Of course, if the grader won't run these
tests, you will not get plus points for them.)

Comments: 
1) In the minus section, one of the commands that must be is ./warmup2 -r 1000000 -P 1000000 -B 1000000 [-n 3]. 
This takes an extremely, extremely long time to run, even when n is set to 3. When it stops running, it returns
at Bus Error. It was noted twice in the comments above.
2) If you press Ctrl+C before anything has entered the bucket filter system, you will get a Segmentation Fault.

+------------------+
| OTHER (Optional) |
+------------------+

Comments on design decisions: All design decisions followed the spec and mimicked the sample output.
Comments on deviation from spec: When Ctrl+C is pressed, all the elements that were in both lists will be printed
and it will state that they've been removed from the lists. The elements aren't actually removed from the lists. 
But the program performs as though they are. 
