<p class="has-line-data" data-line-start="0" data-line-end="1">Definition</p>
<p class="has-line-data" data-line-start="2" data-line-end="3">This project uses multiple process communication methods to locate a particular keyword within the provided files and outputs the results into a file. In this project, process communication techniques such as pipeline temporary output and memory sharing were implemented.</p>
<p class="has-line-data" data-line-start="4" data-line-end="10">psearch1a: A program that uses fork() to generate child processes, each of which scans an input file.<br>
psearch1b: A program that uses fork() to generate child processes, each of which scans an input file. Children communicate in this version using shared anonymous memory (mmap()).<br>
psearch1c: A program that uses fork() to generate child processes, a program that uses anonymous pipes(pipe()) to collect data from child processes, each of which scans an input file.<br>
Keyword: A word that input files include and that’s what program needs to find.<br>
Input files: Files that have some sentences in.<br>
Output file: Includes some sentences from input files which are includes keyword, line index of matched lines and input file names where keyword is found.</p>
<p class="has-line-data" data-line-start="11" data-line-end="12">Installation</p>
<p class="has-line-data" data-line-start="13" data-line-end="18">Makefile of project is ready in folder. You need to just run make command and follow the instructions.<br>
·      Open Terminal<br>
·      Go to project folder using cd command(Home folder is recommended)<br>
·      cd psearch<br>
·      make</p>
<p class="has-line-data" data-line-start="19" data-line-end="20">Usage</p>
<p class="has-line-data" data-line-start="21" data-line-end="33">After installation, programs are ready to run. Usages are explained below:<br>
Psearch1a: ./psearch1a keyword n input1 input2 … output<br>
Psearch1b: ./psearch1b keyword n input1 input2 … output<br>
Psearch1c: ./psearch1c keyword n input1 input2 … output<br>
·      keyword: Given word to search among input files.<br>
·      n: number of input files<br>
·      input1, input2, …, inputn: names of input files<br>
·      output: output file name<br>
Example using:<br>
./psearch1a Los 3 input1.txt input2.txt input3.txt output.txt<br>
./psearch1b Denizli 2 input1.txt input2,txt output.txt<br>
./psearch1c Hollywood 1 input3.txt output.txt</p>
<p class="has-line-data" data-line-start="38" data-line-end="39">Structure and Operation</p>
<p class="has-line-data" data-line-start="40" data-line-end="44">psearch1a:<br>
·      The program generates child processes, each of which examines an input file, using fork().<br>
·      Every child process reads lines from the matching input file and saves them to a temporary file.<br>
·      After the child processes have finished, the parent process merges the temporary files and writes them to the output file.</p>
<p class="has-line-data" data-line-start="45" data-line-end="49">psearch1b:<br>
·      The program generates child processes, each of which examines an input file, using fork().<br>
·      Using shared anonymous memory (mmap()), each child process reads the matching input file and sends matching lines to the parent process.<br>
·      After the child processes have finished, the parent process combines the data in the memory region and writes it to the output file.</p>
<p class="has-line-data" data-line-start="50" data-line-end="54">psearch1c:<br>
·      The program generates child processes, each of which scans an input file, using fork().<br>
·      Using an anonymous pipe (pipe()), each child process scans the associated input file and sends any lines that match to the parent process.<br>
·      After the child processes have finished, the parent process aggregates the data coming through the pipe and writes it to the output file.</p>
<p class="has-line-data" data-line-start="55" data-line-end="59">Results<br>
·      psearch1a uses a simple file processing technique for collecting the results.<br>
·      psearch1b uses shared memory, psearch1b collects data from child processes.<br>
·      psearch1c uses anonymous pipe, psearch1c collects data from child processes.</p>