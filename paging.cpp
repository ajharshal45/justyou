#include <iostream>
using namespace std;

int main() {
    int pageSize, n;

    cout << "Enter page size: ";
    cin >> pageSize;

    cout << "Enter number of pages: ";
    cin >> n;

    int pageTable[n];

    cout << "Enter frame number for each page:\n";
    for (int i = 0; i < n; i++) {
        cout << "Page " << i << " -> Frame: ";
        cin >> pageTable[i];
    }

    int page, offset;

    cout << "Enter page number: ";
    cin >> page;

    cout << "Enter offset: ";
    cin >> offset;

    if (page >= n || offset >= pageSize) {
        cout << "Invalid logical address" << endl;
    } else {
        int frame = pageTable[page];
        int physicalAddress = frame * pageSize + offset;
        cout << "Physical Address = " << physicalAddress << endl;
    }

    return 0;
}
