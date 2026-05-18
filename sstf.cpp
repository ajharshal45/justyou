#include <iostream>
#include <cmath>
using namespace std;

int main() {
    int n, head, seek = 0;

    cout << "Enter number of requests: ";
    cin >> n;

    int req[n], visited[n];

    cout << "Enter request sequence:\n";
    for (int i = 0; i < n; i++) {
        cin >> req[i];
        visited[i] = 0;
    }

    cout << "Enter initial head position: ";
    cin >> head;

    for (int i = 0; i < n; i++) {
        int min = 1e9, index = -1;

        for (int j = 0; j < n; j++) {
            if (!visited[j]) {
                int dist = abs(req[j] - head);
                if (dist < min) {
                    min = dist;
                    index = j;
                }
            }
        }

        visited[index] = 1;
        seek += min;
        head = req[index];
    }

    cout << "Total Seek Time (SSTF): " << seek << endl;

    return 0;
}
