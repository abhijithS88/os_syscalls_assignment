for q1 compilation : g++ 2025202032_A1_Q1.cpp
for q1 execution =>
flag 0 : ./a.out <filepath> 0 <block_size> 
flag 1 : ./a.out <filepath> 1 
flag 2 : ./a.out <filepath> 2 <start_index> <end_index>

for q2 compilation : g++ 2025202032_A2_Q2.cpp
for q2 execution =>
flag 0 : ./a.out <newfilepath> <oldfilepath> <directory> 0 <block_size>
flag 1 : ./a.out <newfilepath> <oldfilepath> <directory> 1 
flag 2 : ./a.out <newfilepath> <oldfilepath> <directory> 2 <start_index> <end_index>


q1 explanation:

common for all flags :
printed "no flag found" when arguments are invalid.
where ever calling a sys call checking for error and returning(exiting the program) after error.
changed the input.txt file mode to 0644 (defualt created was 0436) using chmod.
created/checked directory using mkdir.
open input file and create new flag_input file.
if there are block_size or start_ind and end_ind, convert them to numerical value are verify they are correct.
to read the file in chuncks, create a buffer using malloc.
got the size of input files using stat sys call.

flag 0:
read block wise and reverse blockwise and write on new file.

flag 1:
having a pointer pointing at tot_size - block_size, read block wise, here the block_size is min(64kb,rem_size),
and reverse the block and write on the new file and reduce the pointer by block_size.

flag 2:
total 3 loops:
   1st loop: having a pointer pointing at start_index - block_size, read block wise, here the block_size is min(64kb,rem_size),
   and reverse the block and write on the new file and reduce the pointer by block_size.
   2nd loop: having a pointer pointing at start_index, reaf block wise, here the block_size is min(64kb,rem_size),
   and write the block on the new file and increase the pointer by block_size.
   3rd loop: having a pointer pointing at end_index - block_size, read block wise, here the block_size is min(64kb,rem_size),
   and reverse the block and write on the new file and reduce the pointer by block_size.


q2 explanation:

common for all flags :
printed "no flag found" when arguments are invalid.
where ever calling a sys call checking for error and returning(exiting the program) after error.
checked the given directory is present or not.
checked the given files are in .txt format
checked the sizes of files, proceeded for manual testing iff sizes are same.
opened newfile and oldfile.
if there are block_size or start_ind and end_ind, convert them to numerical value are verify they are correct.
to read the newfile,oldfile in chuncks, create a buffer1,buffer2 using malloc.
got the size of input files using stat sys call.

flag 0: 
read block wise and verified one block is reverse of other.

flag 1:
having a pointer1 pointing at tot_size - block_size, another pointer2 at 0, read block wise, here the block_size is min(64kb,rem_size),
and verified one block is reverse of other and reduce the pointer1 by block_size, increase pointer2 by block_size.

flag 2:
total 3 loops:
   1st loop: having a pointer1 pointing at start_index - block_size, another pointer2 at 0, read block wise, here the block_size is min(64kb,rem_size),
   and verified one block is reverse of other and reduce the pointer1 by block_size, increase pointer2 by block_size.
   2nd loop: having a pointer1 pointing at start_index, another pointer2 at start_ind, reaf block wise, here the block_size is min(64kb,rem_size),
   and verified both blocks are same and increase both pointer by block_size.
   3rd loop: having a pointer1 pointing at end_index - block_size, another pointer2 at end_ind, read block wise, here the block_size is min(64kb,rem_size), and verified one block is reverse of other and reduce the pointer1 by block_size, increase pointer2 by block_size.

used stat sys call found modes of newfile, oldfile, directory and using below command printed the relavent info for all user, others, group on newfile, oldfile and directory.
S_IRUSR  // User read
S_IWUSR  // User write
S_IXUSR  // User execute
S_IRGRP  // Group read
S_IWGRP  // Group write
S_IXGRP  // Group execute
S_IROTH  // Others read
S_IWOTH  // Others write
S_IXOTH  // Others execute









