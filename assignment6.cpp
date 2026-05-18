#include <bits/stdc++.h>
using namespace std;

class Process{
public:
    int id;
    int arrival_time;
    int burst_time;
    int priority;
    int completion_time;
    int tat;
    int wt;
    int remaining_time;

    Process(int pid, int at, int bt, int pr){
        id = pid;
        arrival_time = at;
        burst_time = bt;
        priority = pr;
        remaining_time = bt;
        completion_time = 0;
        tat = 0;
        wt = 0;
    }
};

void printGanttChart(vector<pair<int,int>>& gantt){
    cout << "\nGantt Chart:\n";
    cout << " ";
    for(int i = 0; i < gantt.size(); i++){
        cout << "------";
    }
    cout << "\n|";

    for(int i = 0; i < gantt.size(); i++){
        if(gantt[i].first == 0)
            cout << " IDLE |";
        else
            cout << "  P" << gantt[i].first << "  |";
    }
    cout << "\n ";

    for(int i = 0; i < gantt.size(); i++){
        cout << "------";
    }
    cout << "\n";

    if(!gantt.empty()){
        cout << 0;
        for(int i = 0; i < gantt.size(); i++){
            cout << "\t" << gantt[i].second;
        }
    }
    cout << "\n";
}

void printProcessTable(vector<Process>& processes){
    cout << "\nProcess Table:\n";
    cout << "P_id\tAT\tBT\tCT\tTAT\tWT\n";
    for(int i = 0; i < processes.size(); i++){
        cout << "P" << processes[i].id << "\t" << processes[i].arrival_time << "\t" << processes[i].burst_time << "\t" << processes[i].completion_time << "\t" << processes[i].tat << "\t" << processes[i].wt << "\n";
    }
}

void printAverages(vector<Process>& processes){
    int n = processes.size();
    double total_tat = 0, total_wt = 0;
    for(int i = 0; i < n; i++){
        total_tat += processes[i].tat;
        total_wt += processes[i].wt;
    }
    cout << "\nAverage Waiting Time: " << total_wt / n << "\n";
    cout << "Average Turnaround Time: " << total_tat / n << "\n";
}

void fcfs(vector<Process> processes){
    cout << "\n========== FCFS Scheduling ==========\n";
    int n = processes.size();

    sort(processes.begin(), processes.end(), [](const Process& a, const Process& b){
        if(a.arrival_time != b.arrival_time) return a.arrival_time < b.arrival_time;
        return a.id < b.id;
    });

    vector<pair<int,int>> gantt;
    int current_time = 0;

    for(int i = 0; i < n; i++){
        if(current_time < processes[i].arrival_time){
            gantt.push_back({0, processes[i].arrival_time});
            current_time = processes[i].arrival_time;
        }
        current_time += processes[i].burst_time;
        processes[i].completion_time = current_time;
        processes[i].tat = processes[i].completion_time - processes[i].arrival_time;
        processes[i].wt = processes[i].tat - processes[i].burst_time;
        gantt.push_back({processes[i].id, current_time});
    }

    sort(processes.begin(), processes.end(), [](const Process& a, const Process& b){
        return a.id < b.id;
    });

    printGanttChart(gantt);
    printProcessTable(processes);
    printAverages(processes);
}

void sjfNonPreemptive(vector<Process> processes){
    cout << "\n========== SJF Non-Preemptive Scheduling ==========\n";
    int n = processes.size();
    vector<bool> completed(n, false);
    vector<pair<int,int>> gantt;
    int current_time = 0, done = 0;

    while(done < n){
        int idx = -1;
        int minBurst = INT_MAX;
        for(int i = 0; i < n; i++){
            if(!completed[i] && processes[i].arrival_time <= current_time){
                if(processes[i].burst_time < minBurst){
                    minBurst = processes[i].burst_time;
                    idx = i;
                } else if(processes[i].burst_time == minBurst && processes[i].arrival_time < processes[idx].arrival_time){
                    idx = i;
                }
            }
        }
        if(idx == -1){
            int minAT = INT_MAX;
            for(int i = 0; i < n; i++){
                if(!completed[i] && processes[i].arrival_time < minAT){
                    minAT = processes[i].arrival_time;
                }
            }
            gantt.push_back({0, minAT});
            current_time = minAT;
        } else {
            current_time += processes[idx].burst_time;
            processes[idx].completion_time = current_time;
            processes[idx].tat = processes[idx].completion_time - processes[idx].arrival_time;
            processes[idx].wt = processes[idx].tat - processes[idx].burst_time;
            completed[idx] = true;
            done++;
            gantt.push_back({processes[idx].id, current_time});
        }
    }

    sort(processes.begin(), processes.end(), [](const Process& a, const Process& b){
        return a.id < b.id;
    });

    printGanttChart(gantt);
    printProcessTable(processes);
    printAverages(processes);
}

void sjfPreemptive(vector<Process> processes){
    cout << "\n========== SJF Preemptive (SRTF) Scheduling ==========\n";
    int n = processes.size();
    for(int i = 0; i < n; i++) processes[i].remaining_time = processes[i].burst_time;

    vector<int> timeline;
    int current_time = 0, done = 0;

    while(done < n){
        int idx = -1;
        int minRemaining = INT_MAX;
        for(int i = 0; i < n; i++){
            if(processes[i].arrival_time <= current_time && processes[i].remaining_time > 0){
                if(processes[i].remaining_time < minRemaining){
                    minRemaining = processes[i].remaining_time;
                    idx = i;
                } else if(processes[i].remaining_time == minRemaining && processes[i].arrival_time < processes[idx].arrival_time){
                    idx = i;
                }
            }
        }

        if(idx == -1){
            int minAT = INT_MAX;
            for(int i = 0; i < n; i++){
                if(processes[i].remaining_time > 0 && processes[i].arrival_time < minAT){
                    minAT = processes[i].arrival_time;
                }
            }
            while(current_time < minAT){
                timeline.push_back(0);
                current_time++;
            }
        } else {
            timeline.push_back(processes[idx].id);
            processes[idx].remaining_time--;
            current_time++;

            if(processes[idx].remaining_time == 0){
                processes[idx].completion_time = current_time;
                processes[idx].tat = processes[idx].completion_time - processes[idx].arrival_time;
                processes[idx].wt = processes[idx].tat - processes[idx].burst_time;
                done++;
            }
        }
    }

    vector<pair<int,int>> gantt;
    if(!timeline.empty()){
        int prevId = timeline[0];
        for(int t = 1; t < timeline.size(); t++){
            if(timeline[t] != prevId){
                gantt.push_back({prevId, t});
                prevId = timeline[t];
            }
        }
        gantt.push_back({prevId, (int)timeline.size()});
    }

    sort(processes.begin(), processes.end(), [](const Process& a, const Process& b){
        return a.id < b.id;
    });

    printGanttChart(gantt);
    printProcessTable(processes);
    printAverages(processes);
}

void roundRobin(vector<Process> processes, int quantum){
    cout << "\n========== Round Robin Scheduling (Quantum = " << quantum << ") ==========\n";
    int n = processes.size();
    for(int i = 0; i < n; i++) processes[i].remaining_time = processes[i].burst_time;

    sort(processes.begin(), processes.end(), [](const Process& a, const Process& b){
        if(a.arrival_time != b.arrival_time) return a.arrival_time < b.arrival_time;
        return a.id < b.id;
    });

    vector<pair<int,int>> gantt;
    queue<int> readyQueue;
    int current_time = 0, done = 0;
    vector<bool> inQueue(n, false);

    readyQueue.push(0);
    inQueue[0] = true;

    while(done < n){
        if(readyQueue.empty()){
            int minAT = INT_MAX;
            int minIdx = -1;
            for(int i = 0; i < n; i++){
                if(processes[i].remaining_time > 0 && processes[i].arrival_time < minAT){
                    minAT = processes[i].arrival_time;
                    minIdx = i;
                }
            }
            gantt.push_back({0, minAT});
            current_time = minAT;
            readyQueue.push(minIdx);
            inQueue[minIdx] = true;
        }

        int idx = readyQueue.front();
        readyQueue.pop();

        int execTime = min(quantum, processes[idx].remaining_time);
        current_time += execTime;
        processes[idx].remaining_time -= execTime;

        for(int i = 0; i < n; i++){
            if(!inQueue[i] && processes[i].remaining_time > 0 && processes[i].arrival_time <= current_time){
                readyQueue.push(i);
                inQueue[i] = true;
            }
        }

        if(processes[idx].remaining_time == 0){
            processes[idx].completion_time = current_time;
            processes[idx].tat = processes[idx].completion_time - processes[idx].arrival_time;
            processes[idx].wt = processes[idx].tat - processes[idx].burst_time;
            done++;
        } else {
            readyQueue.push(idx);
        }

        gantt.push_back({processes[idx].id, current_time});
    }

    sort(processes.begin(), processes.end(), [](const Process& a, const Process& b){
        return a.id < b.id;
    });

    printGanttChart(gantt);
    printProcessTable(processes);
    printAverages(processes);
}

void priorityNonPreemptive(vector<Process> processes){
    cout << "\n========== Priority Non-Preemptive Scheduling ==========\n";
    cout << "(Lower number = Higher priority)\n";
    int n = processes.size();
    vector<bool> completed(n, false);
    vector<pair<int,int>> gantt;
    int current_time = 0, done = 0;

    while(done < n){
        int idx = -1;
        int highestPri = INT_MAX;
        for(int i = 0; i < n; i++){
            if(!completed[i] && processes[i].arrival_time <= current_time){
                if(processes[i].priority < highestPri){
                    highestPri = processes[i].priority;
                    idx = i;
                } else if(processes[i].priority == highestPri && processes[i].arrival_time < processes[idx].arrival_time){
                    idx = i;
                }
            }
        }
        if(idx == -1){
            int minAT = INT_MAX;
            for(int i = 0; i < n; i++){
                if(!completed[i] && processes[i].arrival_time < minAT){
                    minAT = processes[i].arrival_time;
                }
            }
            gantt.push_back({0, minAT});
            current_time = minAT;
        } else {
            current_time += processes[idx].burst_time;
            processes[idx].completion_time = current_time;
            processes[idx].tat = processes[idx].completion_time - processes[idx].arrival_time;
            processes[idx].wt = processes[idx].tat - processes[idx].burst_time;
            completed[idx] = true;
            done++;
            gantt.push_back({processes[idx].id, current_time});
        }
    }

    sort(processes.begin(), processes.end(), [](const Process& a, const Process& b){
        return a.id < b.id;
    });

    printGanttChart(gantt);
    printProcessTable(processes);
    printAverages(processes);
}

void priorityPreemptive(vector<Process> processes){
    cout << "\n========== Priority Preemptive Scheduling ==========\n";
    cout << "(Lower number = Higher priority)\n";
    int n = processes.size();
    for(int i = 0; i < n; i++) processes[i].remaining_time = processes[i].burst_time;

    vector<int> timeline;
    int current_time = 0, done = 0;

    while(done < n){
        int idx = -1;
        int highestPri = INT_MAX;
        for(int i = 0; i < n; i++){
            if(processes[i].arrival_time <= current_time && processes[i].remaining_time > 0){
                if(processes[i].priority < highestPri){
                    highestPri = processes[i].priority;
                    idx = i;
                } else if(processes[i].priority == highestPri && processes[i].arrival_time < processes[idx].arrival_time){
                    idx = i;
                }
            }
        }

        if(idx == -1){
            int minAT = INT_MAX;
            for(int i = 0; i < n; i++){
                if(processes[i].remaining_time > 0 && processes[i].arrival_time < minAT){
                    minAT = processes[i].arrival_time;
                }
            }
            while(current_time < minAT){
                timeline.push_back(0);
                current_time++;
            }
        } else {
            timeline.push_back(processes[idx].id);
            processes[idx].remaining_time--;
            current_time++;

            if(processes[idx].remaining_time == 0){
                processes[idx].completion_time = current_time;
                processes[idx].tat = processes[idx].completion_time - processes[idx].arrival_time;
                processes[idx].wt = processes[idx].tat - processes[idx].burst_time;
                done++;
            }
        }
    }

    vector<pair<int,int>> gantt;
    if(!timeline.empty()){
        int prevId = timeline[0];
        for(int t = 1; t < (int)timeline.size(); t++){
            if(timeline[t] != prevId){
                gantt.push_back({prevId, t});
                prevId = timeline[t];
            }
        }
        gantt.push_back({prevId, (int)timeline.size()});
    }

    sort(processes.begin(), processes.end(), [](const Process& a, const Process& b){
        return a.id < b.id;
    });

    printGanttChart(gantt);
    printProcessTable(processes);
    printAverages(processes);
}

int main(){
    int n;
    cout << "Enter the number of processes: ";
    cin >> n;

    vector<Process> processes;
    for(int i = 0; i < n; i++){
        int at, bt, pr;
        cout << "Enter details for Process " << i + 1 << " (Arrival_Time Burst_Time Priority): ";
        cin >> at >> bt >> pr;
        processes.push_back(Process(i + 1, at, bt, pr));
    }

    int quantum;
    cout << "Enter Time Quantum for Round Robin: ";
    cin >> quantum;

    int choice;
    do{
        cout << "\n--- CPU Scheduling Algorithms ---\n";
        cout << "1. FCFS\n";
        cout << "2. SJF (Non-Preemptive)\n";
        cout << "3. SJF (Preemptive/SRTF)\n";
        cout << "4. Round Robin\n";
        cout << "5. Priority (Non-Preemptive)\n";
        cout << "6. Priority (Preemptive)\n";
        cout << "7. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch(choice){
            case 1: fcfs(processes); break;
            case 2: sjfNonPreemptive(processes); break;
            case 3: sjfPreemptive(processes); break;
            case 4: roundRobin(processes, quantum); break;
            case 5: priorityNonPreemptive(processes); break;
            case 6: priorityPreemptive(processes); break;
            case 7: cout << "Exiting...\n"; break;
            default: cout << "Invalid choice! Please try again.\n";
        }
    } while(choice != 7);

    return 0;
}
