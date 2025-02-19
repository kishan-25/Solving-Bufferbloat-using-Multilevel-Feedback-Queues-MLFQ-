# Solving Bufferbloat using Multilevel Feedback Queues (MLFQ)

## Installation
The research paper is not published yet, so the code is not available.

## Project Overview
This project implements a **Multilevel Feedback Queue (MLFQ)** algorithm to mitigate the **bufferbloat** problem in networking. The MLFQ prioritizes packet scheduling based on real-time requirements, ensuring low latency for critical applications like video conferencing, online gaming, and streaming services.

## Overview
Bufferbloat occurs due to excessive packet queuing in oversized network buffers, leading to increased **latency, jitter, and packet loss**. This project presents an MLFQ-based scheduling mechanism that prioritizes packets dynamically to enhance **Quality of Service (QoS)** in networks.

## Features
- **Priority-based scheduling**: Categorizes packets into different priority queues.
- **Real-time adaptation**: Uses a feedback mechanism to promote long-waiting packets.
- **Latency Reduction**: Minimizes delay for real-time applications.
- **Efficient Bandwidth Utilization**: Ensures fair distribution of bandwidth.
- **Simulation of Network Traffic**: Models packet scheduling in a simulated environment.

## Usage
Modify the `config.py` file to customize simulation parameters like packet arrival rates, queue size, and priority levels. Then, execute `main.py` to analyze MLFQ performance.

## Algorithm Details
### 1. **Packet Classification**
   - Video conferencing packets (UDP, ports 16384-32767) → **Highest Priority**
   - Online gaming packets (UDP, ports 3478-3480) → **High Priority**
   - Streaming packets (TCP, keyword 'video') → **Medium Priority**
   - File downloads (Other TCP/UDP traffic) → **Lowest Priority**

### 2. **MLFQ Structure**
   - **Level 0:** Real-time critical packets (video calls, gaming)
   - **Level 1:** Streaming packets
   - **Level N:** Non-time-sensitive traffic (downloads, background processes)

### 3. **Feedback Mechanism**
   - If a packet waits too long in a lower-priority queue, it gets promoted to a higher-priority queue to prevent starvation.

## Results
The simulation evaluates MLFQ efficiency using the following metrics:
- **Packet processing time**
- **Queue wait times**
- **Dropped packets due to congestion**
- **Improved latency for real-time applications**

## Future Work
- Implement **machine learning** for dynamic traffic classification.
- Optimize **aging mechanisms** to balance fairness and efficiency.
- Extend support for **Software-Defined Networking (SDN)** environments.

## References
Relevant research papers and sources used in this project:
- J. Gettys and K. Nichols, "Bufferbloat: Dark buffers in the Internet," Communications of the ACM, 2012.
- K. Nichols and V. Jacobson, "Controlling queue delay," ACM, 2012.
- Various studies on SFQ, CoDel, and FQ-CoDel techniques.

---
### Author
Developed by **Balkishan**
