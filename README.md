# Packet Routing Simulation

This uses a **Discrete Event Simulation** to model a simple network, gathering relevant data as it runs.

---

## TODO

- [ ] Tests
- [ ] Refactoring
- [ ] Configurable topology

---

## Topology

The current network topology is fixed during `Simulator::setup()` for the sake of fleshing out the design of the system. 

### Entities

1. Nodes
    - **Nodes** are the routers in the simulation. They generate, receive, and route packets.
2. Links
    - **Links** are the means by which packets travel from one node to the next.
3. Packets
    - **Packets** are the individual points of data being passed through the network.
4. Events
    - **Events** are the drivers of the simulation, triggering actions that schedule future events until the simulation end time is reached.
5. Simulator
    - The **Simulator** is the conductor of the application, setting up the topology, scheduling and processing events.

---

## System

1. The topology of the network and initial events are setup during `Simulator::setup()`
2. These events (initially `PACKET_GENERATION` events) trigger handlers
3. These handlers (methods of `Node` or `Link`) track the number of packets delivered and dropped, prepare the packets for further routing, and schedule future events
4. When the maximum time has been reached, the simulation ends and prints a report to the console

---
