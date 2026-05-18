#include<iostream>
#include<queue>
using namespace std;

int main(){
    int n, quantum;
    cout<<"Enter number of processes: ";
    cin>>n;
    cout<<"Enter Time Quantum: ";
    cin>>quantum;

    int at[n], bt[n], rt[n], ct[n], tat[n], wt[n];

    for(int i=0; i<n; i++){
        cout<<"Enter Arrival Time and Burst Time for P"<<i+1<<": ";
        cin>>at[i]>>bt[i];
        rt[i]=bt[i];
    }

    queue<int> q;
    bool inQueue[n];
    for(int i=0;i<n;i++) inQueue[i]=false;

    int time=0, completed=0;
    q.push(0); inQueue[0]=true;

    while(completed<n){
        if(q.empty()){
            time++;
            for(int i=0;i<n;i++)
                if(!inQueue[i] && rt[i]>0 && at[i]<=time){ q.push(i); inQueue[i]=true; }
            continue;
        }

        int idx=q.front(); q.pop();
        int exec=min(quantum, rt[idx]);
        time+=exec;
        rt[idx]-=exec;

        for(int i=0;i<n;i++)
            if(!inQueue[i] && rt[i]>0 && at[i]<=time){ q.push(i); inQueue[i]=true; }

        if(rt[idx]==0){
            ct[idx]=time;
            tat[idx]=ct[idx]-at[idx];
            wt[idx]=tat[idx]-bt[idx];
            completed++;
        } else {
            q.push(idx);
        }
    }

    cout<<"\nP\tAT\tBT\tCT\tTAT\tWT\n";
    float avgWT=0, avgTAT=0;
    for(int i=0; i<n; i++){
        cout<<"P"<<i+1<<"\t"<<at[i]<<"\t"<<bt[i]<<"\t"<<ct[i]<<"\t"<<tat[i]<<"\t"<<wt[i]<<"\n";
        avgWT+=wt[i]; avgTAT+=tat[i];
    }
    cout<<"\nAverage Waiting Time: "<<avgWT/n;
    cout<<"\nAverage Turnaround Time: "<<avgTAT/n<<"\n";
    return 0;
}
