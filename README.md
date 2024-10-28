To run the simulation, use `run.py`. Two options are available:

1. **Printing Options:**
   - Choose between `--printing` and `--no-printing`.
   - `--printing` is the default and will print the game history.

2. **Executive File:**
   - The file `main` will be created.

3. **Providing Arguments:**
   - Two options are available:

   - **Option 1: List of Filenames**
     - Provide a list of filenames. Each simulation will run once.
     - Example: `./main.exe game1.txt game2.txt game3.txt`
     - Note: Statistics are not available for this regime.

   - **Option 2: Filename and Integer**
     - Provide one filename and an integer.
     - Example: `./main game1.txt 5`
     - This will run the simulation using `game1.txt` five times and print the statistics.


**Additional Information**:

Three sample boards are provided (game1.txt, game2.txt, game3.txt).
Adjust endline characters in the provided boards for cross-platform compatibility.

**Project Documentation**:

Explore the UML folder for the UML diagram detailing the project structure.




** Simple Performance Test**
(run on @students account)
1. With Printing Option Enabled (1,000 Iterations):
The total time taken for processing "game3.txt" 100 times, with the printing option enabled, was 1,013,980 microseconds.
(8,891,681 lines of text)

2. Without Printing Option (100,000 Iterations):
Disabling the printing option and processing "game3.txt" 100,000 times took 3,677,534 microseconds.


