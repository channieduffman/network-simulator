# Packet Routing Simulation

This uses a **Discrete Event Simulation** to model a simple network, gathering relevant data as it runs.

---

## Topology

The current network topology is fixed during `Simulator::setup()` for the sake of fleshing out the design of the system. 

### Entities

1. Nodes
2. Links
3. Packets
4. Events

#### Nodes

**Nodes** are the routers in the simulation. They generate, receive, and route packets.

#### Links

**Links** are the means by which packets travel from one node to the next.

#### Packets 

**Packets** are the individual points of data being passed through the network.

#### Events

**Events** are the drivers of the simulation, triggering actions that schedule future events until the simulation end time is reached.

---

## System

1. The topology of the network and initial events are setup during `Simulator::setup()`
2. These events (initially `PACKET_GENERATION` events) trigger handlers
3. These handlers (methods of `Node`s or `Link`s) track the number of packets delivered and dropped, prepare the packets for further routing, and schedule future events
4. When the maximum time has been reached, the simulation ends and prints a report to the console

---
