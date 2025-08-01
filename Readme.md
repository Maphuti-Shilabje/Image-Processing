# C++ Image Processing & Component Analysis CLI

A command-line tool written in modern C++ to perform connected-component analysis on PGM images. The program identifies distinct objects within an image based on pixel connectivity, filters them by size, and outputs a new, processed image. This project demonstrates strong algorithm design and C++ memory management principles.

![png of the tool running](image.png)
*(Your task: Create a GIF of you compiling with `make` and running the `findcomp` command on a sample image, showing the output file being created. It's a game-changer!)*

---

### Key Concepts & Technologies:
*   **Core Language:** **C++17**
*   **Algorithm:** Implemented a **Breadth-First Search (BFS)** based "flood-fill" algorithm from scratch to detect 4-connected components.
*   **Memory Management:** Strictly adheres to **RAII (Resource Acquisition Is Initialization)** principles, using smart pointers and implementing the "Rule of Six" for proper resource handling and prevention of memory leaks.
*   **Object-Oriented Design:** Encapsulated all logic within a `PGMImageProcessor` class, exposing a clean, reusable public API to build, manipulate, and write components.
*   **Unit Testing:** Developed a comprehensive suite of unit tests using the **Catch2 framework** to ensure the correctness and robustness of component extraction and filtering logic.
*   **Build System:** Utilized a **Makefile** for efficient compilation and testing workflows.

---

### How to Compile and Run:

1.  **Prerequisites:** Requires a C++17 compatible compiler (like g++) and `make`.
2.  **Compile the main program:**
    ```bash
    make
    ```
3.  **Run the unit tests:**
    ```bash
    make tests
    ```
4.  **Example Usage:**
    ```bash
    ./findcomp -t 150 -m 50 -w output.pgm input.pgm
    ```
    *This command processes `input.pgm` using a threshold of 150, discards components smaller than 50 pixels, and writes the result to `output.pgm`.*