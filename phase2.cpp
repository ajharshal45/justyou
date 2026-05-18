#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <cstdlib>
#include <ctime>

using namespace std;

// =====================================================
// MEMORY
// =====================================================

string M[300];

// =====================================================
// FRAME TRACKING
// =====================================================

bool usedFrames[30];

// =====================================================
// REGISTERS
// =====================================================

string IR = "";
string R  = "";

int IC = 0;
bool C = false;

// =====================================================
// INTERRUPTS
// =====================================================

int SI = 0;
int PI = 0;
int TI = 0;

// =====================================================
// COUNTERS
// =====================================================

int TTC = 0;
int LLC = 0;

int TTL = 50;
int TLL = 50;

// =====================================================
// PAGE TABLE
// =====================================================

int PTR;

// =====================================================
// DATA
// =====================================================

vector<string> dataCards;
int dataIndex = 0;

// =====================================================
// FILES
// =====================================================

ifstream fin;
ofstream fout;

// =====================================================
// WAIT
// =====================================================

void wait() {

    cout << "\nPress ENTER for next step...";
    cin.get();
}

// =====================================================
// MEMORY DISPLAY
// =====================================================

void printMemory() {

    cout << "\n================ MEMORY =================\n";

    for (int i = 0; i < 300; i++) {

        if (M[i] != "") {

            cout << setw(3) << i << " : " << M[i] << endl;
        }
    }

    cout << "=========================================\n";
}

// =====================================================
// PAGE TABLE DISPLAY
// =====================================================

void printPageTable() {

    cout << "\n============= PAGE TABLE =============\n";

    for (int i = 0; i < 10; i++) {

        if (M[PTR + i] != "") {

            cout << "Page " << i
                 << " -> Frame "
                 << M[PTR + i]
                 << endl;
        }
    }

    cout << "======================================\n";
}

// =====================================================
// INITIALIZE
// =====================================================

void INIT() {

    for (int i = 0; i < 300; i++) {
        M[i] = "";
    }

    for (int i = 0; i < 30; i++) {
        usedFrames[i] = false;
    }

    IR = "";
    R  = "";

    IC = 0;

    C = false;

    SI = 0;
    PI = 0;
    TI = 0;

    TTC = 0;
    LLC = 0;

    dataCards.clear();
    dataIndex = 0;
}

// =====================================================
// FRAME ALLOCATION
// =====================================================

int allocateFrame() {

    while (true) {

        int frame = rand() % 30;

        if (!usedFrames[frame]) {

            usedFrames[frame] = true;

            return frame;
        }
    }
}

// =====================================================
// TERMINATE
// =====================================================

void TERMINATE(int EM) {

    cout << "\n\n============= TERMINATION =============\n";

    switch (EM) {

        case 0:
            cout << "NO ERROR\n";
            break;

        case 1:
            cout << "OUT OF DATA ERROR\n";
            break;

        case 2:
            cout << "LINE LIMIT EXCEEDED\n";
            break;

        case 3:
            cout << "TIME LIMIT EXCEEDED\n";
            break;

        case 4:
            cout << "OPERATION CODE ERROR\n";
            break;

        case 5:
            cout << "OPERAND ERROR\n";
            break;

        case 6:
            cout << "INVALID PAGE FAULT\n";
            break;
    }

    cout << "IC  = " << IC << endl;
    cout << "IR  = " << IR << endl;
    cout << "TTC = " << TTC << endl;
    cout << "LLC = " << LLC << endl;

    cout << "=======================================\n";

    exit(0);
}

// =====================================================
// ADDRESS MAP
// =====================================================

int addressMap(int VA) {

    cout << "\n=========== ADDRESS MAP ===========\n";

    cout << "VA = " << VA << endl;

    if (VA < 0 || VA >= 100) {

        PI = 2;

        cout << "OPERAND ERROR\n";

        return -1;
    }

    int page = VA / 10;
    int offset = VA % 10;

    cout << "Page Number = " << page << endl;
    cout << "Offset = " << offset << endl;

    wait();

    string entry = M[PTR + page];

    if (entry == "") {

        PI = 3;

        cout << "PAGE FAULT\n";

        return -1;
    }

    int frame = stoi(entry);

    int RA = frame * 10 + offset;

    cout << "Frame Number = " << frame << endl;
    cout << "RA = " << RA << endl;

    cout << "===================================\n";

    return RA;
}

// =====================================================
// READ
// =====================================================

void READ(int RA) {

    cout << "\nREAD OPERATION\n";

    if (dataIndex >= dataCards.size()) {

        TERMINATE(1);
    }

    string line = dataCards[dataIndex++];

    int k = 0;

    for (int i = RA; i < RA + 10; i++) {

        string word = "";

        for (int j = 0; j < 4; j++) {

            if (k < line.size())
                word += line[k++];
            else
                word += ' ';
        }

        M[i] = word;
    }

    printMemory();
}

// =====================================================
// WRITE
// =====================================================

void WRITE(int RA) {

    cout << "\nWRITE OPERATION\n";

    LLC++;

    if (LLC > TLL) {

        TERMINATE(2);
    }

    string output = "";

    for (int i = RA; i < RA + 10; i++) {

        output += M[i];
    }

    cout << "OUTPUT : " << output << endl;

    fout << output << endl;
}

// =====================================================
// MOS
// =====================================================

void MOS(int VA) {

    cout << "\n================ MOS =================\n";

    if (TI == 0 && SI == 1) {

        cout << "TI=0 SI=1 -> READ\n";

        int RA = addressMap(VA);

        if (PI == 3) {

            cout << "\nVALID PAGE FAULT\n";

            int page = VA / 10;

            int frame = allocateFrame();

            M[PTR + page] = to_string(frame);

            cout << "New Frame Allocated = "
                 << frame << endl;

            printPageTable();

            wait();

            PI = 0;

            RA = addressMap(VA);
        }

        READ(RA);
    }

    else if (TI == 0 && SI == 2) {

        cout << "TI=0 SI=2 -> WRITE\n";

        int RA = addressMap(VA);

        WRITE(RA);
    }

    else if (TI == 0 && SI == 3) {

        cout << "TI=0 SI=3 -> TERMINATE\n";

        TERMINATE(0);
    }

    else if (TI == 2 && SI == 1) {

        TERMINATE(3);
    }

    else if (TI == 2 && SI == 2) {

        int RA = addressMap(VA);

        WRITE(RA);

        TERMINATE(3);
    }

    else if (TI == 2 && SI == 3) {

        TERMINATE(0);
    }

    if (TI == 0 && PI == 1) {

        TERMINATE(4);
    }

    else if (TI == 0 && PI == 2) {

        TERMINATE(5);
    }

    else if (TI == 0 && PI == 3) {

        TERMINATE(6);
    }

    else if (TI == 2 && PI == 1) {

        TERMINATE(3);
    }

    else if (TI == 2 && PI == 2) {

        TERMINATE(3);
    }

    else if (TI == 2 && PI == 3) {

        TERMINATE(3);
    }

    cout << "=======================================\n";
}

// =====================================================
// EXECUTE USER PROGRAM
// =====================================================

void EXECUTE() {

    while (true) {

        cout << "\n=======================================\n";
        cout << "IC = " << IC << endl;
        cout << "=======================================\n";

        wait();

        int instructionRA = addressMap(IC);

        if (PI != 0) {

            MOS(0);
        }

        IR = M[instructionRA];

        cout << "Fetched Instruction = "
             << IR << endl;

        wait();

        IC++;

        TTC++;

        if (TTC > TTL) {

            TI = 2;
        }

        string opcode = IR.substr(0, 2);

        // =================================================
        // HALT
        // =================================================

        if (IR[0] == 'H') {

            SI = 3;

            MOS(0);
        }

        // =================================================
        // OPERAND
        // =================================================

        int VA;

        try {

            VA = stoi(IR.substr(2, 2));
        }
        catch (...) {

            PI = 2;

            MOS(0);
        }

        int RA = addressMap(VA);

        // =================================================
        // VALID PAGE FAULT
        // =================================================

        if (PI == 3 &&
            (opcode == "GD" || opcode == "SR")) {

            cout << "\nVALID PAGE FAULT\n";

            int page = VA / 10;

            int frame = allocateFrame();

            M[PTR + page] = to_string(frame);

            cout << "Allocated Frame = "
                 << frame << endl;

            printPageTable();

            wait();

            PI = 0;

            RA = addressMap(VA);
        }

        // =================================================
        // INVALID PAGE FAULT
        // =================================================

        else if (PI == 3) {

            TERMINATE(6);
        }

        // =================================================
        // LR
        // =================================================

        if (opcode == "LR") {

            cout << "\nLR INSTRUCTION\n";

            wait();

            R = M[RA];

            cout << "Register = " << R << endl;
        }

        // =================================================
        // SR
        // =================================================

        else if (opcode == "SR") {

            cout << "\nSR INSTRUCTION\n";

            wait();

            M[RA] = R;

            printMemory();
        }

        // =================================================
        // CR
        // =================================================

        else if (opcode == "CR") {

            cout << "\nCR INSTRUCTION\n";

            wait();

            if (R == M[RA])
                C = true;
            else
                C = false;

            cout << "Condition = " << C << endl;
        }

        // =================================================
        // BT
        // =================================================

        else if (opcode == "BT") {

            cout << "\nBT INSTRUCTION\n";

            wait();

            if (C) {

                IC = VA;

                cout << "BRANCH TAKEN\n";
            }
            else {

                cout << "NO BRANCH\n";
            }
        }

        // =================================================
        // GD
        // =================================================

        else if (opcode == "GD") {

            SI = 1;

            MOS(VA);
        }

        // =================================================
        // PD
        // =================================================

        else if (opcode == "PD") {

            SI = 2;

            MOS(VA);
        }

        // =================================================
        // OPCODE ERROR
        // =================================================

        else {

            PI = 1;

            MOS(0);
        }
    }
}

// =====================================================
// LOAD
// =====================================================

void LOAD(string filename) {

    fin.open(filename);

    string line;

    bool program = false;
    bool data = false;

    int page = 0;

    while (getline(fin, line)) {

        // =================================================
        // $AMJ
        // =================================================

        if (line.substr(0, 4) == "$AMJ") {

            INIT();

            PTR = allocateFrame() * 10;

            cout << "\nPROGRAM LOADING STARTED\n";

            cout << "PTR = " << PTR << endl;

            wait();

            program = true;
            data = false;

            page = 0;
        }

        // =================================================
        // $DTA
        // =================================================

        else if (line.substr(0, 4) == "$DTA") {

            program = false;
            data = true;

            cout << "\nPROGRAM LOADED SUCCESSFULLY\n";

            printPageTable();

            printMemory();

            wait();
        }

        // =================================================
        // $END
        // =================================================

        else if (line.substr(0, 4) == "$END") {

            cout << "\nEND OF JOB\n";
        }

        // =================================================
        // PROGRAM CARDS
        // =================================================

        else if (program) {

            int frame = allocateFrame();

            M[PTR + page] = to_string(frame);

            cout << "\nPAGE "
                 << page
                 << " ALLOCATED TO FRAME "
                 << frame
                 << endl;

            wait();

            int RA = frame * 10;

            int k = 0;

            for (int i = 0; i < line.size(); i += 4) {

                M[RA + k] = line.substr(i, 4);

                k++;
            }

            printMemory();

            page++;
        }

        // =================================================
        // DATA CARDS
        // =================================================

        else if (data) {

            dataCards.push_back(line);
        }
    }

    fin.close();
}

// =====================================================
// MAIN
// =====================================================

int main() {

    srand(time(0));

    fout.open("output.txt");

    LOAD("input.txt");

    EXECUTE();

    fout.close();

    return 0;
}