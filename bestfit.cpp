#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Block {
    int id;
    int size;
    int originalSize;
    int allocatedProcessId = -1;
};

struct Process {
    int id;
    int size;
    int allocatedBlockId = -1;
};

void displayResults(const vector<Block>& blocks, const vector<Process>& processes) {
    cout << "\nProcess No.\tProcess Size\tBlock No.\n";
    for (const auto& p : processes) {
        cout << "P" << p.id << "\t\t" << p.size << "\t\t";
        if (p.allocatedBlockId != -1)
            cout << "B" << p.allocatedBlockId << endl;
        else
            cout << "Not Allocated" << endl;
    }
}

void firstFit(vector<Block> blocks, vector<Process> processes) {
    for (auto& p : processes) {
        for (auto& b : blocks) {
            if (b.allocatedProcessId == -1 && b.size >= p.size) {
                b.allocatedProcessId = p.id;
                p.allocatedBlockId = b.id;
                // Note: In some variations, the block size is reduced. 
                // In fixed partitioning, the whole block is used. 
                // We'll assume fixed partitioning for simplicity unless dynamic is requested.
                break;
            }
        }
    }
    displayResults(blocks, processes);
}

void bestFit(vector<Block> blocks, vector<Process> processes) {
    for (auto& p : processes) {
        int bestIdx = -1;
        for (int i = 0; i < blocks.size(); i++) {
            if (blocks[i].allocatedProcessId == -1 && blocks[i].size >= p.size) {
                if (bestIdx == -1 || blocks[i].size < blocks[bestIdx].size) {
                    bestIdx = i;
                }
            }
        }
        if (bestIdx != -1) {
            blocks[bestIdx].allocatedProcessId = p.id;
            p.allocatedBlockId = blocks[bestIdx].id;
        }
    }
    displayResults(blocks, processes);
}

void worstFit(vector<Block> blocks, vector<Process> processes) {
    for (auto& p : processes) {
        int worstIdx = -1;
        for (int i = 0; i < blocks.size(); i++) {
            if (blocks[i].allocatedProcessId == -1 && blocks[i].size >= p.size) {
                if (worstIdx == -1 || blocks[i].size > blocks[worstIdx].size) {
                    worstIdx = i;
                }
            }
        }
        if (worstIdx != -1) {
            blocks[worstIdx].allocatedProcessId = p.id;
            p.allocatedBlockId = blocks[worstIdx].id;
        }
    }
    displayResults(blocks, processes);
}

void nextFit(vector<Block> blocks, vector<Process> processes) {
    int lastIdx = 0;
    for (auto& p : processes) {
        bool allocated = false;
        int count = 0;
        while (count < blocks.size()) {
            if (blocks[lastIdx].allocatedProcessId == -1 && blocks[lastIdx].size >= p.size) {
                blocks[lastIdx].allocatedProcessId = p.id;
                p.allocatedBlockId = blocks[lastIdx].id;
                allocated = true;
                break;
            }
            lastIdx = (lastIdx + 1) % blocks.size();
            count++;
        }
    }
    displayResults(blocks, processes);
}

int main() {
    int nBlocks, nProcesses;
    cout << "Enter number of blocks: ";
    cin >> nBlocks;
    vector<Block> initialBlocks(nBlocks);
    cout << "Enter size of each block:\n";
    for (int i = 0; i < nBlocks; i++) {
        initialBlocks[i].id = i + 1;
        cin >> initialBlocks[i].size;
        initialBlocks[i].originalSize = initialBlocks[i].size;
    }

    cout << "Enter number of processes: ";
    cin >> nProcesses;
    vector<Process> initialProcesses(nProcesses);
    cout << "Enter size of each process:\n";
    for (int i = 0; i < nProcesses; i++) {
        initialProcesses[i].id = i + 1;
        cin >> initialProcesses[i].size;
    }

    int choice;
    while (true) {
        cout << "\n--- Memory Allocation Menu ---\n";
        cout << "1. First Fit\n";
        cout << "2. Best Fit\n";
        cout << "3. Worst Fit\n";
        cout << "4. Next Fit\n";
        cout << "5. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 5) break;

        switch (choice) {
            case 1: firstFit(initialBlocks, initialProcesses); break;
            case 2: bestFit(initialBlocks, initialProcesses); break;
            case 3: worstFit(initialBlocks, initialProcesses); break;
            case 4: nextFit(initialBlocks, initialProcesses); break;
            default: cout << "Invalid choice!\n";
        }
    }

    return 0;
}
