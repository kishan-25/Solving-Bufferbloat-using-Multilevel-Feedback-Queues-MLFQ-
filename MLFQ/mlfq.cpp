#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <fstream>

using namespace std;

// Packet structure representing network packets
struct Packet 
{
    int id;
    int arrivalTime;
    int size; // Size of the packet
};

// Function to perform Round Robin scheduling for network packets
void roundRobin(vector<Packet>& packets, int bandwidth, ofstream& logFile, int level) 
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
            logFile << currentPacket.id << " " << time << " " << level << endl; // Log packet id, time, and level
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
    ofstream logFile("mlfq_log.txt"); // Open file for logging
    if (!logFile.is_open()) 
    {
        cerr << "Unable to open log file" << endl;
        return;
    }

    // Perform Round Robin at each bandwidth level
    for (int i = 0; i < numOfLevels; ++i) 
    {
        cout << "Bandwidth Level " << i << ":" << endl;
        roundRobin(packets, bandwidthLevels[i], logFile, i);
    }

    logFile.close(); // Close the log file
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

