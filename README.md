# CPU Scheduling Algorithms
Practicing OS fundamentals by re-creating the following CPU scheduling algorithms:
- Round Robin
- Shortest Job First
- Priority Scheduling (No Preemption)
- Priority Scheduling (Preemption)

Code will output the algorithm used, waiting time for all processes, and the overall average waiting time.

## Compile
```bash
g++ src/main.cpp -o bin/main
```

## Run Specific Input (Default)
Ensure lines 432 and 433 are the following:

```cpp
// return !testAlgorithms(16);
return generateOutput();
 ```

Then run the following command after compiling

```bash
bin/main <input_file.txt>
```

Will write the output to an `output.txt` file.
  
## Run All Test Cases
Change lines 432 and 433 to the following:

```cpp
return !testAlgorithms(16);
// return generateOutput();
```
Then run the following command after compiling

```bash
bin/main
```
Will display output to the terminal.
