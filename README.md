# Discrete Event Packet Queue Simulator

## Build
Just run `make` in the top-level directory

## Usage
Usage: `./sim [-t nsecs] [-c bps] [-l plength] [-k qsize] [-r rho[,end]]`

- `-t` (Number of seconds to run the simulation)
- `-c` (The simulated link rate of the output link in bits per second)
- `-l` (The average length of a packet in bits)
- `-k` (The size of the simulated queue in number of packets)
- `-r` (The utilization of the queue. The simulator is run for each value from `rho` through to `end`)

## Code
- **src/es.c**        - Array (heap) based priority queue implementation
- **src/rv.c**        - Uniform and exponential distribution random variable generators
- **src/sim.c**       - Simulator event procedures and event generation functions
- **src/main.c**      - Program entry
- **src/test.c**      - Tests
- **scripts/plot.py** - Python script to generate plots of data generated from simulator
- **scripts/dot.sh**  - Script to generate graphical representation (using Graphviz) of the priority queue's tree structure (for testing)

