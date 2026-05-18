#include <iostream>
using namespace std;

int main() {
    int n, m;

    cout << "enter number of processes: ";
    cin >> n;

    cout << "enter number of resources: ";
    cin >> m;

    int alloc[10][10], maxm[10][10], need[10][10];
    int avail[10];
    int finish[10];
    int safeSeq[10];

    cout << "enter allocation matrix:\n";
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            cin >> alloc[i][j];

    cout << "enter max matrix:\n";
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            cin >> maxm[i][j];

    cout << "enter available resources:\n";
    for (int i = 0; i < m; i++)
        cin >> avail[i];

    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            need[i][j] = maxm[i][j] - alloc[i][j];

    for (int i = 0; i < n; i++)
        finish[i] = 0;

    int count = 0;

    while (count < n) {
        int found = 0;

        for (int i = 0; i < n; i++) {
            if (finish[i] == 0) {
                int possible = 1;

                for (int j = 0; j < m; j++) {
                    if (need[i][j] > avail[j]) {
                        possible = 0;
                        break;
                    }
                }

                if (possible) {
                    for (int j = 0; j < m; j++)
                        avail[j] += alloc[i][j];

                    safeSeq[count] = i;
                    count++;
                    finish[i] = 1;
                    found = 1;

                    cout << "process " << i << " done" << endl;
                }
            }
        }

        if (found == 0) {
            cout << "deadlock possible, no safe sequence" << endl;
            return 0;
        }
    }

    cout << "safe sequence is: ";
    for (int i = 0; i < n; i++)
        cout << "P" << safeSeq[i] << " ";

    cout << endl;

    return 0;
}
