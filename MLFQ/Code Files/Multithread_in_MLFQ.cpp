#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <thread> 
#include <mutex>
#include <condition_variable>

using namespace std;

// Process structure
struct Process {
  int id;
  int arrivalTime;
  int burstTime;
  int remainingTime;
  int level; // Priority level for MLFQ
};

// Thread-safe queue class
class ThreadSafeQueue {
public:
  void push(Process* process) {
    unique_lock<mutex> lock(m_mutex);
    m_queue.push(process);
    m_condVar.notify_one();
  }

  Process* pop() {
    unique_lock<mutex> lock(m_mutex);
    while (m_queue.empty()) {
      m_condVar.wait(lock);
    }
    Process* process = m_queue.front();
    m_queue.pop();
    return process;
  }

  bool empty() const { // Corrected empty check
    unique_lock<mutex> lock(m_mutex);
    return m_queue.empty();
  }

private:
  mutable mutex m_mutex;
  condition_variable m_condVar;
  queue<Process*> m_queue;
};

// Function to perform Round Robin scheduling (threaded version)
void roundRobin(ThreadSafeQueue& q, vector<Process>& processes, int quantum) {
  int time = 0;
  int n = processes.size();
  int index = 0;

  vector<thread> threads;
  while (!q.empty() || index < n) {
    if (!q.empty()) {
      Process* currentProcess = q.pop();
      int executeTime = min(quantum, currentProcess->remainingTime);
      currentProcess->remainingTime -= executeTime;
      time += executeTime;
      cout << "Executing process " << currentProcess->id << " at time " << time << endl;

      // New thread for execution (optional, simulates parallel downloads)
      threads.push_back(thread([currentProcess, executeTime]() {
        // Simulate download process (can be replaced with actual work)
        this_thread::sleep_for(chrono::milliseconds(executeTime * 100)); // Adjust sleep time as needed
      }));

      while (index < n && processes[index].arrivalTime <= time) {
        q.push(&processes[index]);
        index++;
      }

      if (currentProcess->remainingTime > 0) {
        q.push(currentProcess);
      }
    } else {
      q.push(&processes[index]);
      time = processes[index].arrivalTime;
      index++;
    }
  }

  // Wait for all download threads to finish (optional)
  for (auto& thread : threads) {
    thread.join();
  }
}

// Function to implement MLFQ with multiple feedback levels
void MLFQ(vector<Process>& processes, vector<int>& timeQuantum, int numOfLevels) {
  for (int i = 0; i < processes.size(); ++i) {
    processes[i].level = 0; // Initially, all processes are at level 0
    processes[i].remainingTime = processes[i].burstTime;
  }

  ThreadSafeQueue queues[numOfLevels]; // Array of thread-safe queues

  // Perform Round Robin at each feedback level using threads
  vector<thread> threads;
  for (int i = 0; i < numOfLevels; ++i) {
    threads.push_back(thread(roundRobin, ref(queues[i]), ref(processes), timeQuantum[i]));
  }

  // Wait for all level threads to finish
  for (auto& thread : threads) {
    thread.join();
  }
}

// Hashing function for Round Robin
int hashRoundRobin(int processId, int numOfLevels) {
  return processId % numOfLevels;
}

int main() {
  // Input processes (complete the time quantum vector)
  vector<Process> processes = {
    {1, 0, 8, 0, 0},
    {2, 1, 4, 0, 0},
    {3, 2, 9, 0, 0},
    {4, 3, 5, 0, 0},
    {5, 4, 2, 0, 0},
  };

  // Time quantum for each feedback level (complete this vector
  // Time quantum for each feedback level (complete this vector)
  vector<int> timeQuantum = {2, 4, 8}; // Example time quantum for 3 levels

  int numOfLevels = timeQuantum.size();

  // Assign processes to levels using hashing
  unordered_map<int, vector<Process>> levelMap;
  for (auto& process : processes) {
    int level = hashRoundRobin(process.id, numOfLevels);
    levelMap[level].push_back(process);
  }

  // Execute MLFQ
  for (int i = 0; i < numOfLevels; ++i) {
    cout << "Processes in level " << i << ":" << endl;
    for (auto& process : levelMap[i]) {
      cout << "Process " << process.id << " with burst time " << process.burstTime << endl;
    }
  }

  MLFQ(processes, timeQuantum, numOfLevels);

return 0;
}