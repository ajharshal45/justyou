#include<iostream>
using namespace std;

int main(){
    int n;
    cout<<"Enter number of processes: ";
    cin>>n;

    int at[n], bt[n], ct[n], tat[n], wt[n];
    bool done[n];

    for(int i=0; i<n; i++){
        cout<<"Enter Arrival Time and Burst Time for P"<<i+1<<": ";
        cin>>at[i]>>bt[i];
        done[i]=false;
    }

    int time=0, completed=0;
    while(completed<n){
        int idx=-1, minBT=1e9;
        for(int i=0; i<n; i++){
            if(!done[i] && at[i]<=time && bt[i]<minBT){
                minBT=bt[i];
                idx=i;
            }
        }
        if(idx==-1){ time++; continue; }

        time+=bt[idx];
        ct[idx]=time;
        tat[idx]=ct[idx]-at[idx];
        wt[idx]=tat[idx]-bt[idx];
        done[idx]=true;
        completed++;
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
