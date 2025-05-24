#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>

using namespace std;

// Packet structure representing network packets
struct Packet 
{
    int id;
    int arrivalTime;
    int size; // Size of the packet
};

// Function to perform Round Robin scheduling for network packets
void roundRobin(vector<Packet>& packets, int bandwidth) 
{
    queue<Packet> q;
    int time = 0;
    int n = packets.size();
    int index = 0;
    while (!q.empty() || index < n) 
    {
        if (!q.empty()) 
        {
            Packet currentPacket = q.front();
            q.pop();
            int transmitTime = min(currentPacket.size, bandwidth);
            currentPacket.size -= transmitTime;
            time += transmitTime;
            cout << "Transmitting packet " << currentPacket.id << " at time " << time << endl;
            while (index < n && packets[index].arrivalTime <= time) 
            {
                q.push(packets[index]);
                index++;
            }
            if (currentPacket.size > 0) 
            {
                q.push(currentPacket);
            }
        } 
        else 
        {
            q.push(packets[index]);
            time = packets[index].arrivalTime;
            index++;
        }
    }
}

// Function to implement MLFQ for network packets
void MLFQ(vector<Packet>& packets, vector<int>& bandwidthLevels, int numOfLevels) 
{
    // Perform Round Robin at each bandwidth level
    for (int i = 0; i < numOfLevels; ++i) 
    {
        cout << "Bandwidth Level " << i << ":" << endl;
        roundRobin(packets, bandwidthLevels[i]);
    }
}

int main() 
{
    // Input packets
    vector<Packet> packets = 
    {
        {1, 0, 100}, // Packet ID, Arrival Time, Size
        {2, 2, 50},
        {3, 4, 200},
        {4, 5, 150},
        {5, 7, 80}
    };

    // Bandwidth for each level
    vector<int> bandwidthLevels = {50, 100, 200}; // For example

    int numOfLevels = bandwidthLevels.size();

    // Execute MLFQ
    MLFQ(packets, bandwidthLevels, numOfLevels);

    return 0;
}


/*
Time Complexity Analysis:

Enqueue Operation (roundRobin):
In each iteration of the while loop, we perform enqueue operations for packets arriving at or before the current time. Since the packets are sorted by arrival time, this operation takes 
𝑂(1)
O(1) time for each arriving packet.
The worst-case scenario is when all packets arrive at the same time, leading to 
𝑂(𝑛)
O(n) enqueue operations, where 
n is the total number of packets.
Hence, the time complexity of the enqueue operation is 
𝑂(𝑛)
O(n).
Dequeue Operation (roundRobin):
The dequeue operation involves transmitting packets based on the available bandwidth. In the worst case, a packet might need to be transmitted multiple times until its size becomes zero.
Since the size of each packet decreases during transmission, the total number of transmit operations across all packets is linear in the sum of packet sizes.
Let 
S be the sum of packet sizes. The time complexity of the dequeue operation is 
𝑂(𝑆)
MLFQ Function (MLFQ):
The MLFQ function iterates over each bandwidth level and calls the roundRobin function for each level.
Since the number of bandwidth levels is constant and independent of the number of packets, the time complexity of the MLFQ function is proportional to the time complexity of the roundRobin function at each level.
Therefore, the overall time complexity of the MLFQ function is 
𝑂(𝑆).

Space Complexity Analysis:

Queue (roundRobin):
The space complexity of the queue used in the roundRobin function depends on the maximum number of packets that can be enqueued simultaneously.
In the worst case, all packets arrive at the same time and need to be enqueued, leading to a space complexity of 
O(n), where 
𝑛
n is the total number of packets.
MLFQ Function (MLFQ):
The MLFQ function does not incur additional space complexity beyond the space required for the packets themselves and any auxiliary data structures used within the roundRobin function.
Therefore, the space complexity of the MLFQ function is 
O(n), where 
𝑛
n is the total number of packets.
In summary, the time complexity of the provided network simulator is 
𝑂(𝑆)
O(S), where 
S is the sum of packet sizes, and the space complexity is 
𝑂(𝑛)
O(n), where 
𝑛
n is the total number of packets.

*/