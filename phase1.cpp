#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>

using namespace std;

char M[100][4];
char IR[4];
char R[4];

int IC;
bool C;

vector<string> dataLines;
int dataIndex = 0;

ifstream fin;
ofstream fout;

void wait() {
    cout << "\nPress ENTER for next step...";
    cin.get();
}

void INIT() {

    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 4; j++) {
            M[i][j] = ' ';
        }
    }

    IC = 0;
    C = false;

    memset(IR, ' ', 4);
    memset(R, ' ', 4);

    dataLines.clear();
    dataIndex = 0;
}

void printMemory() {

    cout << "\n========== MEMORY ==========\n";

    for (int i = 0; i < 100; i++) {

        cout << i << " : ";

        for (int j = 0; j < 4; j++) {
            cout << M[i][j];
        }

        cout << endl;
    }

    cout << "============================\n";
}

void printRegister() {

    cout << "\nRegister : ";

    for (int i = 0; i < 4; i++) {
        cout << R[i];
    }

    cout << endl;
}

void READ() {

    if (dataIndex >= dataLines.size()) {
        cout << "No data available\n";
        return;
    }

    string line = dataLines[dataIndex++];

    int addr = (IR[2] - '0') * 10 + (IR[3] - '0');

    cout << "\nGD Instruction";
    cout << "\nReading data into memory from " << addr << endl;

    wait();

    int k = 0;

    for (int i = addr; i < addr + 10 && i < 100; i++) {

        for (int j = 0; j < 4; j++) {

            if (k < line.size())
                M[i][j] = line[k++];
            else
                M[i][j] = ' ';
        }
    }

    printMemory();
}

void WRITE() {

    int addr = (IR[2] - '0') * 10 + (IR[3] - '0');

    cout << "\nPD Instruction";
    cout << "\nWriting from memory location " << addr << endl;

    wait();

    cout << "\nOUTPUT : ";

    for (int i = addr; i < addr + 10 && i < 100; i++) {

        for (int j = 0; j < 4; j++) {

            cout << M[i][j];
            fout << M[i][j];
        }
    }

    cout << endl;
    fout << endl;
}

bool EXECUTE() {

    cout << "\n================================";
    cout << "\nIC = " << IC << endl;

    wait();

    for (int i = 0; i < 4; i++) {
        IR[i] = M[IC][i];
    }

    cout << "\nFetched Instruction : ";

    for (int i = 0; i < 4; i++) {
        cout << IR[i];
    }

    cout << endl;

    wait();

    IC++;

    string op = "";
    op += IR[0];
    op += IR[1];

    int addr = (IR[2] - '0') * 10 + (IR[3] - '0');

    if (op == "LR") {

        cout << "\nExecuting LR";
        cout << "\nLoading M[" << addr << "] into Register\n";

        wait();

        for (int i = 0; i < 4; i++) {
            R[i] = M[addr][i];
        }

        printRegister();
    }

    else if (op == "SR") {

        cout << "\nExecuting SR";
        cout << "\nStoring Register into M[" << addr << "]\n";

        wait();

        for (int i = 0; i < 4; i++) {
            M[addr][i] = R[i];
        }

        printMemory();
    }

    else if (op == "CR") {

        cout << "\nExecuting CR";
        cout << "\nComparing Register with M[" << addr << "]\n";

        wait();

        C = true;

        for (int i = 0; i < 4; i++) {

            if (R[i] != M[addr][i]) {
                C = false;
                break;
            }
        }

        cout << "\nCondition Flag = ";

        if (C)
            cout << "TRUE\n";
        else
            cout << "FALSE\n";
    }

    else if (op == "BT") {

        cout << "\nExecuting BT";

        wait();

        if (C) {

            IC = addr;

            cout << "\nBranch Taken";
            cout << "\nIC = " << IC << endl;
        }
        else {

            cout << "\nCondition False";
            cout << "\nNo Branch\n";
        }
    }

    else if (op == "GD") {

        READ();
    }

    else if (op == "PD") {

        WRITE();
    }

    else if (IR[0] == 'H') {

        cout << "\nProgram Halted\n";
        return false;
    }

    else {

        cout << "\nInvalid Instruction\n";
        return false;
    }

    return true;
}

bool LOAD(string filename) {

    fin.open(filename);

    if (!fin.is_open()) {

        cout << "Cannot open input file\n";
        return false;
    }

    string line;

    int m = 0;

    bool program = false;
    bool data = false;

    while (getline(fin, line)) {

        if (line.substr(0, 4) == "$AMJ") {

            INIT();

            m = 0;
            program = true;
            data = false;

            cout << "Loading Program...\n";
        }

        else if (line.substr(0, 4) == "$DTA") {

            IC = 0;

            program = false;
            data = true;

            cout << "\nProgram Loaded Into Memory\n";

            printMemory();

            wait();
        }

        else if (line.substr(0, 4) == "$END") {

            cout << "\nEnd of Program\n";
        }

        else if (program) {

            int k = 0;

            for (int i = 0; i < line.size(); i++) {

                if (k > 0 && k % 4 == 0) {
                    m++;
                }

                M[m][k % 4] = line[i];

                k++;
            }

            m++;
        }

        else if (data) {

            dataLines.push_back(line);
        }
    }

    fin.close();

    return true;
}

int main() {

    fout.open("output.txt");

    if (!LOAD("input.txt"))
        return 0;

    while (EXECUTE()) {

        wait();
    }

    fout.close();

    return 0;
}