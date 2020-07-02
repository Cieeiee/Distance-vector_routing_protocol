# Distance-vector_routing_protocol
A C++ implement for Distance Vector and Poisoned Reverse algorithm.
## Usage
```bash
make
./DistanceVector configFileName changedConfigName
./PoisonedReverse configFileName changedConfigName
```
## Input: Configuration File Format
The program will be run with two filenames as command line arguments.
The first file describes the topology and consists of 4 sections:
1. The first line is a number, **x**, that indicates the number of routers/nodes in the topology.
2. The following **x** lines contain the names of each router/node in the topology.
3. The next line is a number, **y**, that indicates the number of links/edges in the topology.
4. The following **y** lines contain the details of each link/edge in the topology.
    - Written as the names of two routers/nodes followed by the weight of that link/edge, all separated by spaces.

The second file describes the changes to the link/edge weights and consists of 2 sections:
1. The first next line is a number, **y**, that indicates the number changed links/edges in the topology.
2. The following **y** lines contain the new weight of each changed link/edge in the topology.
   - Written the same as in the first file.
   - Only contains changed links/edges. Where a link is omitted, there is no change to its weight.
   - No new links will be added in this section.

`configL` and `changeConfigL` are the sample config files.

## Output Format
As this is Distance Vector, a node will only be able to communicate with its neighbors. 
Thus, node U can only tell if it is sending data to V, W or X. 
You should indicate which interface the packets will be sent through, as shown below.
The output consist of 4 sections:
1.	The first section
     1.	Begins with the line `#START`
     2.	Is followed by lines of the format `t=n distance from A to B via C is d`, describing the routing updates at each router at during each step towards convergence, where
        - `n` is the number the current step.
          *	`A` value of `0` is the initial state as specified in the input file.
          *	`A` value of `1` is the updated state after all routers have sent their first update to their neighbors.
          *	etc.
        -	`A` is the name of the source router/node
        -	`B` is the name of the destination router/node
        -	`C` is name of an immediate neighbor of `A`
          *	For directly connected nodes, `C` will have the same value as `B`
        -	`d` is the current total distance from `A` to `B` routing via `C`

2.	The 2nd sections
     1.	Begins with the line `#INITIAL`
     2.	Is followed by lines of the format `router A: B is d routing through C`, describing the current routing table of each router/node in the system, where
        -	`A` is the name of the source router/node
        -	`B` is the name of the destination router/node
        -	`C` is name of an immediate neighbor of `A`
        -	`d` is the current total distance from `A` to `B` routing via `C`
        -	Only reachable/known routers should be shown here.

3.	The 3rd sections
     1.	Begins with the line `#UPDATE`
     2.	Is followed by lines of the same format as described in section 1 as the changed weights are applied.
4.	The 4th sections
     1.	Begins with the line `#FINAL`
     2.	Is followed by lines of the same format as described in section 2, but after the system has reached convergence with the changed weights.