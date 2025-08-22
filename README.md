# File Reverse and Verify  

This repo has 2 programs:  
- Q1: Reverse file content in different ways  
- Q2: Verify reversed files  

---

## Q1: Reverse File  

### Compile  
```bash
g++ 2025202032_A1_Q1.cpp
```

### Run

```bash
# flag 0
./a.out <filepath> 0 <block_size>

# flag 1
./a.out <filepath> 1

# flag 2
./a.out <filepath> 2 <start_index> <end_index>
```

### Common for all flags

* Print **"no flag found"** if args invalid
* Check errors after every system call, exit if error
* Change `input.txt` file mode to **0644** (default 0436) using `chmod`
* Create/check directory with `mkdir`
* Open input file, create new `flag_input` file
* Convert block size / indices to numbers, check valid
* Read in chunks using `malloc` buffer
* Get file size using `stat`

### Flag 0

* Read block wise → reverse → write to new file

### Flag 1

* Pointer at `tot_size - block_size`
* Read block (`min(64kb, rem_size)`) → reverse → write → move pointer back

### Flag 2

* **3 loops:**

  1. From `start_index - block_size`, read → reverse → write → move back
  2. From `start_index`, read → write (no reverse) → move forward
  3. From `end_index - block_size`, read → reverse → write → move back

---

## Q2: Verify Files

### Compile

```bash
g++ 2025202032_A2_Q2.cpp
```

### Run

```bash
# flag 0
./a.out <newfilepath> <oldfilepath> <directory> 0 <block_size>

# flag 1
./a.out <newfilepath> <oldfilepath> <directory> 1

# flag 2
./a.out <newfilepath> <oldfilepath> <directory> 2 <start_index> <end_index>
```

### Common for all flags

* Print **"no flag found"** if args invalid
* Check errors after every system call, exit if error
* Check directory exists
* Ensure files are `.txt`
* Compare file sizes, proceed only if same
* Open newfile and oldfile
* Convert block size / indices to numbers, check valid
* Read in chunks using `malloc` buffers
* Get file size using `stat`

### Flag 0

* Read block wise, check if one block is reverse of other

### Flag 1

* Pointer1 at `tot_size - block_size`, pointer2 at 0
* Read blocks → verify reverse → move pointers (p1 back, p2 forward)

### Flag 2

* **3 loops:**

  1. Pointer1 at `start_index - block_size`, pointer2 at 0 → check reverse
  2. Pointer1 at `start_index`, pointer2 at `start_index` → check same
  3. Pointer1 at `end_index - block_size`, pointer2 at `end_index` → check reverse

### Permissions Check

* Used `stat` to get modes of newfile, oldfile, directory
* Printed permissions for **user, group, others**:

  * `S_IRUSR` (read), `S_IWUSR` (write), `S_IXUSR` (execute)
  * `S_IRGRP`, `S_IWGRP`, `S_IXGRP`
  * `S_IROTH`, `S_IWOTH`, `S_IXOTH`

---

## Notes

* Max block read = **64KB**
* Done using system calls only
* Handles errors, invalid args, and file permissions
