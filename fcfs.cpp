#include <iostream>
#include <cmath>
using namespace std;

int main() {
    int n, head, seek = 0;

    cout << "Enter number of requests: ";
    cin >> n;

    int req[n];

    cout << "Enter request sequence:\n";
    for (int i = 0; i < n; i++)
        cin >> req[i];

    cout << "Enter initial head position: ";
    cin >> head;

    for (int i = 0; i < n; i++) {
        seek += abs(req[i] - head);
        head = req[i];
    }

    cout << "Total Seek Time (FCFS): " << seek << endl;

    return 0;
}

