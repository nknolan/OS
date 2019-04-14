## Process manager simulation
03/2019

### To Run

`make`, and then run with ./idispatcher

***

Program reads input from stdin and interprets it as a
list of process instructions. End of input is signified by
an empty line. Once finished, prints the amount of time spent
idle followed by a list of processes IDs and the amount of time
they spend running, ready, and blocked in the simulated CPU.

To use with file input:

./idispatcher<./path_to_test_file

