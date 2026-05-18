#include<iostream>
using namespace std;

int main(){
    int n;
    cout<<"Enter number of processes: ";
    cin>>n;

    int at[n], bt[n], pr[n], rt[n], ct[n], tat[n], wt[n];

    for(int i=0; i<n; i++){
        cout<<"Enter Arrival Time, Burst Time and Priority for P"<<i+1<<": ";
        cin>>at[i]>>bt[i]>>pr[i];
        rt[i]=bt[i];
    }

    int time=0, completed=0;
    while(completed<n){
        int idx=-1, minPR=1e9;
        for(int i=0; i<n; i++){
            if(at[i]<=time && rt[i]>0 && pr[i]<minPR){
                minPR=pr[i];
                idx=i;
            }
        }
        if(idx==-1){ time++; continue; }

        rt[idx]--;
        time++;
        if(rt[idx]==0){
            ct[idx]=time;
            tat[idx]=ct[idx]-at[idx];
            wt[idx]=tat[idx]-bt[idx];
            completed++;
        }
    }

    cout<<"\nP\tAT\tBT\tPR\tCT\tTAT\tWT\n";
    float avgWT=0, avgTAT=0;
    for(int i=0; i<n; i++){
        cout<<"P"<<i+1<<"\t"<<at[i]<<"\t"<<bt[i]<<"\t"<<pr[i]<<"\t"<<ct[i]<<"\t"<<tat[i]<<"\t"<<wt[i]<<"\n";
        avgWT+=wt[i]; avgTAT+=tat[i];
    }
    cout<<"\nAverage Waiting Time: "<<avgWT/n;
    cout<<"\nAverage Turnaround Time: "<<avgTAT/n<<"\n";
    return 0;
}
