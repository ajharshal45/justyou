#include <iostream>
using namespace std;

int main() {
    int n, m;

    cout << "enter number of processes: ";
    cin >> n;

    cout << "enter number of resources: ";
    cin >> m;

    int alloc[10][10], req[10][10];
    int avail[10];
    int finish[10];

    cout << "enter allocation matrix:\n";
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            cin >> alloc[i][j];

    cout << "enter request matrix:\n";
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            cin >> req[i][j];

    cout << "enter available resources:\n";
    for (int i = 0; i < m; i++)
        cin >> avail[i];

    for (int i = 0; i < n; i++) {
        int flag = 0;
        for (int j = 0; j < m; j++) {
            if (alloc[i][j] != 0) {
                flag = 1;
                break;
            }
        }
        if (flag == 0)
            finish[i] = 1;
        else
            finish[i] = 0;
    }

    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            if (finish[i] == 0) {
                int possible = 1;

                for (int j = 0; j < m; j++) {
                    if (req[i][j] > avail[j]) {
                        possible = 0;
                        break;
                    }
                }

                if (possible) {
                    for (int j = 0; j < m; j++)
                        avail[j] += alloc[i][j];

                    finish[i] = 1;
                }
            }
        }
    }

    int deadlock = 0;

    for (int i = 0; i < n; i++) {
        if (finish[i] == 0) {
            cout << "Process " << i << " is in deadlock" << endl;
            deadlock = 1;
        }
    }

    if (deadlock == 0)
        cout << "No deadlock detected" << endl;

    return 0;
}
