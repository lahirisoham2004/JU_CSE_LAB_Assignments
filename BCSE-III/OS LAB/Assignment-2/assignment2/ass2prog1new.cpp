#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <queue>
#include <climits>

using namespace std;

class Job {
    int jid;
    int arrival;
    int priority;
    vector<int> bursts;
    int wt;
    int tat;

public:
    Job(int jid, int arrival, vector<int> bursts, int priority)
        : jid(jid), arrival(arrival), bursts(bursts), priority(priority), wt(0), tat(0) {}

    int getArrival() const { return arrival; }
    int getPriority() const { return priority; }
    int getBurst(int index = 0) const { return bursts[index]; }
    bool hasRemainingBursts() const { return !bursts.empty(); }
    void reduceCurrentBurst(int amount) { bursts[0] -= amount; if (bursts[0] <= 0) bursts.erase(bursts.begin()); }
    void setWT(int wt) { this->wt = wt; }
    void setTAT(int tat) { this->tat = tat; }
    
    void display() const {
        cout << "Job ID: " << jid << ", Arrival: " << arrival 
             << ", Priority: " << priority << ", WT: " << wt 
             << ", TAT: " << tat << endl;
    }
};

class Jobs {
    vector<Job> jobs;

public:
    Jobs(string filename, int njobs = 20) {
        ifstream in(filename);
        if (!in) {
            cout << "Cannot open file" << endl;
            exit(1);
        }
        
        int jid, arrival, priority;
        while (in >> jid >> arrival) {
            vector<int> bursts;
            int burst;
            while (in >> burst && burst != -1) {
                bursts.push_back(burst);
            }
            priority = bursts.back();
            bursts.pop_back();
            jobs.emplace_back(jid, arrival, bursts, priority);
        }
    }

    void display() const {
        for (const auto& job : jobs) job.display();
    }

    void fcfs() {
        vector<int> tat, wt;
        int time = jobs[0].getArrival();
        
        sort(jobs.begin(), jobs.end(), [](const Job& a, const Job& b) {
            return a.getArrival() < b.getArrival();
        });
        
        for (auto& job : jobs) {
            time = max(time, job.getArrival()) + job.getBurst();
            int turnaround = time - job.getArrival();
            int waiting = turnaround - job.getBurst();

            tat.push_back(turnaround);
            wt.push_back(waiting);
        }
        
        setMetrics(tat, wt);
    }

    void srtn() {
        vector<int> tat, wt;
        vector<int> remainingBursts(jobs.size());
        int time = 0, completed = 0, n = jobs.size();
        
        for (int i = 0; i < n; ++i) remainingBursts[i] = jobs[i].getBurst();

        while (completed < n) {
            int shortest = -1;
            int minBurst = INT_MAX;
            
            for (int i = 0; i < n; ++i) {
                if (jobs[i].getArrival() <= time && remainingBursts[i] > 0 && remainingBursts[i] < minBurst) {
                    minBurst = remainingBursts[i];
                    shortest = i;
                }
            }
            
            if (shortest == -1) {
                time++;
                continue;
            }
            
            remainingBursts[shortest]--;
            time++;

            if (remainingBursts[shortest] == 0) {
                completed++;
                int turnaround = time - jobs[shortest].getArrival();
                int waiting = turnaround - jobs[shortest].getBurst();

                tat.push_back(turnaround);
                wt.push_back(waiting);
            }
        }
        
        setMetrics(tat, wt);
    }

    void rr(int timeSlice = 25) {
        vector<int> tat, wt;
        queue<int> q;
        vector<int> remainingBursts(jobs.size());
        int time = 0;

        for (int i = 0; i < jobs.size(); ++i) remainingBursts[i] = jobs[i].getBurst();
        
        q.push(0);
        int index = 1;

        while (!q.empty()) {
            int current = q.front();
            q.pop();

            if (remainingBursts[current] <= timeSlice) {
                time += remainingBursts[current];
                remainingBursts[current] = 0;

                int turnaround = time - jobs[current].getArrival();
                int waiting = turnaround - jobs[current].getBurst();

                tat.push_back(turnaround);
                wt.push_back(waiting);
            } else {
                time += timeSlice;
                remainingBursts[current] -= timeSlice;

                q.push(current);
            }

            while (index < jobs.size() && jobs[index].getArrival() <= time) {
                q.push(index);
                index++;
            }
        }

        setMetrics(tat, wt);
    }

    void setMetrics(vector<int>& tat, vector<int>& wt) {
        int n = tat.size();
        double avgTAT = 0.0, avgWT = 0.0;

        for (int i = 0; i < n; ++i) {
            jobs[i].setTAT(tat[i]);
            jobs[i].setWT(wt[i]);
            avgTAT += tat[i];
            avgWT += wt[i];
        }

        avgTAT /= n;
        avgWT /= n;

        cout << "Average Turnaround Time: " << avgTAT << endl;
        cout << "Average Waiting Time: " << avgWT << endl;
        display();
    }
};

int main() {
    string filename = "jobs.txt";
    Jobs jobs(filename);

    cout << "First-Come, First-Serve Scheduling:" << endl;
    jobs.fcfs();

    cout << "\nShortest Remaining Time Next Scheduling:" << endl;
    jobs.srtn();

    cout << "\nRound Robin Scheduling (Time Slice = 25):" << endl;
    jobs.rr(25);

    return 0;
}
