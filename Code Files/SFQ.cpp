#include <iostream>
#include <unordered_map>
#include <queue>
#include <vector>
 
using namespace std;

class SFQ
{
private:
    struct Packet 
    {
        int flowId;
        int packetId;

        Packet(int fId, int pId) : flowId(fId), packetId(pId) {}
    };

    unordered_map<int, queue<Packet>> queues; // Hash table mapping flow IDs to queues
    vector<int> hashKeys; // Vector to store hash keys (flow IDs)
    int currentIndex;

    // Function to compute the hash key (using modulo)
    int hashFunction(int flowId, int tableSize)
    {
        return flowId % tableSize;
    }

public:
    SFQ() : currentIndex(0) {}

    void enqueue(int flowId, int packetId) {
        if (queues.find(flowId) == queues.end()) {
            // Flow ID not found in hash table, create a new queue
            queues[flowId] = queue<Packet>();
            hashKeys.push_back(flowId);
        }
        queues[flowId].push(Packet(flowId, packetId));
    }

    int dequeue() {
        if (hashKeys.empty()) {
            cout << "No packets in any queue." << endl;
            return -1;
        }

        int initialIndex = currentIndex;
        while (true) {
            int key = hashKeys[currentIndex];
            if (!queues[key].empty()) {
                Packet packet = queues[key].front();
                queues[key].pop();
                cout << "Dequeued packet " << packet.packetId << " from flow " << packet.flowId << endl;
                return packet.packetId;
            }
            currentIndex = (currentIndex + 1) % hashKeys.size(); // Move to next key in a round-robin fashion
            if (currentIndex == initialIndex) // Checked all keys without finding any non-empty queue
                break;
        }
        cout << "No packets in any queue." << endl;
        return -1;
    }
};

int main() {
    SFQ sfq;

    sfq.enqueue(0, 1);
    sfq.enqueue(1, 2);
    sfq.enqueue(2, 3);

    sfq.dequeue(); // Should dequeue packet from flow 0
    sfq.dequeue(); // Should dequeue packet from flow 1
    sfq.dequeue(); // Should dequeue packet from flow 2
    sfq.dequeue(); // Should output "No packets in any queue."

    return 0;
}
/*
  Time complexity
    enqueue O(1)
    dequeue O(n) n : no of flows

  Space complexity : O(n+m)
         n is the number of flows and m is the total number of packets.
*/