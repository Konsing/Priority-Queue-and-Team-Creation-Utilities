# C++ Priority Queue and Team Creation Utilities

This project includes a C++ implementation of a priority queue and utilities for creating teams based on certain criteria. The project includes scripts for building a heap, creating teams, and managing the priority queue operations.

## Project Files

1. **priority_queue.cpp**
   - Implementation of the priority queue.

2. **priority_queue.h**
   - Header file for the priority queue implementation.

3. **build_heap.cpp**
   - Script to build a heap from a JSON file containing operations.

4. **create_teams.cpp**
   - Script to create teams from a JSON file containing player statistics.

5. **Makefile**
   - Makefile for compiling the C++ source code.

## How to Use

### Prerequisites

- C++ compiler (e.g., g++)
- JSON for Modern C++ library

### Steps

1. **Clone the Repository**

   ```sh
   git clone https://github.com/yourusername/Priority-Queue-Team-Creation.git
   cd Priority-Queue-Team-Creation
   ```

2. **Compile the Code**

   ```sh
   make
   ```

3. **Run the Applications**

   - **Build Heap**
     ```sh
     ./buildheap.exe input.json
     ```

   - **Create Teams**
     ```sh
     ./createteams.exe input.json
     ```

## Directory Structure

```
src/
├── priority_queue.cpp
├── priority_queue.h
├── build_heap.cpp
├── create_teams.cpp
├── Makefile
```

## Example

### Building the Heap

To build the heap from a JSON file containing operations:

```sh
./buildheap.exe input.json
```

### Creating Teams

To create teams based on player statistics from a JSON file:

```sh
./createteams.exe input.json