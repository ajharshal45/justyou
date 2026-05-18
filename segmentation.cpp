#include <iostream>
using namespace std;

int main() {
    int n;

    cout << "Enter number of segments: ";
    cin >> n;

    int base[n], limit[n];

    cout << "Enter base address and limit for each segment:\n";

    for (int i = 0; i < n; i++) {
        cout << "Segment " << i << " Base Address: ";
        cin >> base[i];

        cout << "Segment " << i << " Limit: ";
        cin >> limit[i];
    }

    int segment, offset;

    cout << "Enter segment number: ";
    cin >> segment;

    cout << "Enter offset: ";
    cin >> offset;

    if (segment >= n) {
        cout << "Invalid Segment Number!" << endl;
    }
    else if (offset >= limit[segment]) {
        cout << "Offset exceeds segment limit!" << endl;
    }
    else {

        int physicalAddress = base[segment] + offset;

        cout << "Physical Address = "
             << physicalAddress << endl;
    }

    return 0;
}