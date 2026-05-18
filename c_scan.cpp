#include <iostream>
#include <cmath>
using namespace std;

int main() {
    int n, head, disk_size, seek = 0;

    cout << "Enter number of requests: ";
    cin >> n;

    int req[n], left[n], right[n];
    int l = 0, r = 0;

    cout << "Enter request sequence:\n";
    for (int i = 0; i < n; i++)
        cin >> req[i];

    cout << "Enter initial head position: ";
    cin >> head;

    cout << "Enter disk size: ";
    cin >> disk_size;

    for (int i = 0; i < n; i++) {
        if (req[i] < head)
            left[l++] = req[i];
        else
            right[r++] = req[i];
    }

    for (int i = 0; i < l - 1; i++) {
        for (int j = i + 1; j < l; j++) {
            if (left[i] > left[j]) {
                int temp = left[i];
                left[i] = left[j];
                left[j] = temp;
            }
        }
    }

    for (int i = 0; i < r - 1; i++) {
        for (int j = i + 1; j < r; j++) {
            if (right[i] > right[j]) {
                int temp = right[i];
                right[i] = right[j];
                right[j] = temp;
            }
        }
    }

    for (int i = 0; i < r; i++) {
        seek += abs(right[i] - head);
        head = right[i];
    }

    seek += abs((disk_size - 1) - head);
    seek += (disk_size - 1);
    head = 0;

    for (int i = 0; i < l; i++) {
        seek += abs(left[i] - head);
        head = left[i];
    }

    cout << "Total Seek Time (C-SCAN): " << seek << endl;

    return 0;
}

