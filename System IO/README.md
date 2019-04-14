## System I/O
02/2019
***

### To Run

Run `make` to compile. Runs as `./unixio filename bytes type` where filename is the name of the file you want to read, bytes is the number of bytes to read per call, and value is which system call type to use. 1 = Unix IO, 0 = C stdio.h functions.

To measure relative performance, I used `truncate -s filename 1M` to generate a fairly large file, then used `t.sh` to test the program on it.

#### Observations

Using the Unix open() and read() functions is much slower than the C fopen() and fread() or fgetc() functions at low read sizes but only slightly less at higher read sizes. Overall the speed of the execution varies wildly, which seems to be common among computers and not specific to the server I did most of the testing on. Larger read sizes completed faster than smaller read sizes.

Sample output in sample_output.txt