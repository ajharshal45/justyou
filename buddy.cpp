#include <bits/stdc++.h>
using namespace std;

class BuddySystem {
    int total_size;
    map<int, vector<int>> free_list;

public:
    BuddySystem(int size) {
        total_size = 1;
        while (total_size < size) total_size *= 2;
        free_list[total_size].push_back(0);
        cout << "Buddy System initialized with " << total_size << " KB of memory.\n";
    }

    void allocate(int size) {
        int n = 1;
        while (n < size) n *= 2;

        cout << "\nRequest for " << size << " KB (Adjusted to " << n << " KB)...\n";

        int current_size = n;
        while (current_size <= total_size && free_list[current_size].empty()) {
            current_size *= 2;
        }

        if (current_size > total_size) {
            cout << "Error: Not enough memory available for " << size << " KB.\n";
            return;
        }

        while (current_size > n) {
            int addr = free_list[current_size].front();
            free_list[current_size].erase(free_list[current_size].begin());

            current_size /= 2;
            free_list[current_size].push_back(addr);
            free_list[current_size].push_back(addr + current_size);

            cout << "Splitting " << current_size * 2 << " KB block at " << addr 
                 << " into buddies of " << current_size << " KB.\n";
        }

        int allocated_addr = free_list[n].front();
        free_list[n].erase(free_list[n].begin());
        cout << "Allocated " << n << " KB at address " << allocated_addr << ".\n";
        cout << "Internal Fragmentation: " << n - size << " KB.\n";
    }

    void deallocate(int addr, int size) {
        int n = 1;
        while (n < size) n *= 2;

        cout << "\nFreeing " << n << " KB at address " << addr << "...\n";

        int current_size = n;
        int current_addr = addr;

        while (current_size < total_size) {
            int buddy_addr = current_addr ^ current_size;
            auto& list = free_list[current_size];
            auto it = find(list.begin(), list.end(), buddy_addr);

            if (it != list.end()) {
                cout << "Buddy at " << buddy_addr << " is free. Merging " << current_size 
                     << " KB blocks into " << current_size * 2 << " KB block.\n";
                list.erase(it);
                current_addr = min(current_addr, buddy_addr);
                current_size *= 2;
            } else {
                break;
            }
        }
        free_list[current_size].push_back(current_addr);
        cout << "Memory block of " << current_size << " KB released at " << current_addr << ".\n";
    }

    void display() {
        cout << "\n---------- Free List State ----------\n";
        bool empty = true;
        for (auto it = free_list.begin(); it != free_list.end(); ++it) {
            int size = it->first;
            vector<int>& addrs = it->second;
            if (!addrs.empty()) {
                empty = false;
                cout << size << " KB blocks: ";
                for (size_t i = 0; i < addrs.size(); ++i) {
                    cout << "[" << addrs[i] << "] ";
                }
                cout << endl;
            }
        }
        if (empty) cout << "No free memory!\n";
        cout << "-------------------------------------\n";
    }
};

int main() {
    int total;
    cout << "Enter total memory size (in KB): ";
    cin >> total;

    BuddySystem buddy(total);
    buddy.display();

    int choice, size, addr;
    while (true) {
        cout << "\n--- Buddy System Menu ---\n";
        cout << "1. Allocate Memory\n";
        cout << "2. Deallocate Memory\n";
        cout << "3. Display Free List\n";
        cout << "4. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1) {
            cout << "Enter size to allocate (KB): ";
            cin >> size;
            buddy.allocate(size);
            buddy.display();
        } else if (choice == 2) {
            cout << "Enter start address: ";
            cin >> addr;
            cout << "Enter original requested size (KB): ";
            cin >> size;
            buddy.deallocate(addr, size);
            buddy.display();
        } else if (choice == 3) {
            buddy.display();
        } else if (choice == 4) {
            break;
        } else {
            cout << "Invalid choice!\n";
        }
    }

    return 0;
}
