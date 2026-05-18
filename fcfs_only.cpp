#include<iostream>
using namespace std;

int main(){
    int n;
    cout<<"Enter number of processes: ";
    cin>>n;

    int at[n], bt[n], ct[n], tat[n], wt[n];

    for(int i=0; i<n; i++){
        cout<<"Enter Arrival Time and Burst Time for P"<<i+1<<": ";
        cin>>at[i]>>bt[i];
    }

    // Sort by arrival time
    for(int i=0; i<n-1; i++)
        for(int j=0; j<n-i-1; j++)
            if(at[j]>at[j+1]){
                swap(at[j],at[j+1]);
                swap(bt[j],bt[j+1]);
            }

    int time=0;
    for(int i=0; i<n; i++){
        if(time<at[i]) time=at[i];
        time+=bt[i];
        ct[i]=time;
        tat[i]=ct[i]-at[i];
        wt[i]=tat[i]-bt[i];
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
