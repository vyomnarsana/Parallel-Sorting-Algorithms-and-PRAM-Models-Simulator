#include<bits/stdc++.h>
using namespace std;

class PRAMMODEL {

    protected:
    vector<int> memory;
    int nProcessors;
    int memorySize;

    private:
    void setModelRequirements() {
        cout << "Enter number of Processors: ";
        cin >> nProcessors;

        // Handle invalid input for nProcessors
        while (cin.fail() || nProcessors <= 0) {
            cout << "Invalid input. Please enter a positive integer for number of Processors: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin >> nProcessors;
        }

        cout << "Enter Memory Size: ";
        cin >> memorySize;

        // Handle invalid input for memorySize
        while (cin.fail() || memorySize <= 0) {
            cout << "Invalid input. Please enter a positive integer for Memory Size: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin >> memorySize;
        }

        // Resize memory vector and initialize with zeros
        memory.resize(memorySize, 0);

        cout << "Memory of size " << memorySize << " is initialized with 0" << endl;
    }

    public:
    PRAMMODEL() {
        setModelRequirements();
    }

    virtual void read() = 0;

    virtual void write() = 0;

    void printMemory() const {
        cout << "Memory contents:" << endl;
        for (int i = 0; i < memory.size(); ++i) {
            cout << "M[" << i << "]: " << memory[i] << " ";
        }
        cout << endl;
    }

    void menu() {
        while (true) {
            cout<<endl<<endl;
            cout << "Menu:" << endl;
            cout << "1. Perform READ operation" << endl;
            cout << "2. Perform WRITE operation" << endl;
            cout << "3. Print memory" << endl;
            cout << "4. Exit" << endl;

            int choice;
            cout << "Enter your choice (1-4): ";
            cin >> choice;

            // Validate the input
            if (cin.fail() || choice < 1 || choice > 4) {
                cin.clear(); // Clear the error flag
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
                cout << "Invalid choice. Please enter a number between 1 and 4." << endl;
                continue; // Skip the rest of the loop and prompt for a new choice
            }

            switch (choice) {
                case 1:
                    cout<<endl<<endl;
                    read();
                    break;
                case 2:
                    cout<<endl<<endl;
                    write();
                    break;
                case 3:
                    cout<<endl<<endl;
                    printMemory();
                    break;
                case 4:
                    cout<<endl<<endl;
                    return;
            }

            cout<<endl<<endl;
        }
    }
};

class EREW : public PRAMMODEL {
    public:

    EREW() {
        menu();
    }

    void read() {
        vector<bool> processorRead(nProcessors, false);
        vector<bool> memoryRead(memorySize, false);
        cout << "Enter 2-tuples {a, b} where 'a' denotes processor ID and 'b' denotes memory location to read.\n";
        cout << "Enter -1 for processor ID to finish inputting tuples.\n";

        while (true) {
            int a, b;
            cin >> a;

            if (a == -1)
                break;

            cin >> b;

            // Validate processor ID and memory location
            if (a < 0 || a >= nProcessors) {
                cout << "Invalid processor ID. Please enter a valid processor ID.\n";
                // Ignore the remaining input of the current invalid tuple
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            if (b < 0 || b >= memorySize) {
                cout << "Invalid memory location. Please enter a valid memory location.\n";
                // Ignore the remaining input of the current invalid tuple
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            if (processorRead[a]) {
                cout << "Error: Processor " << a << " has already performed a read operation.\n";
                cout << "Read operation unsuccessful.\n";
                return;
            } else {
                processorRead[a] = true;
            }

            if (memoryRead[b]) {
                cout << "Error: Memory location " << b << " has already been read by another processor.\n";
                cout << "Read operation unsuccessful.\n";
                return;
            } else {
                memoryRead[b] = true;
            }
        }
        cout << "Read operation successful.\n"<<endl;
    }


    void write() {
        cout << "Enter 3-tuples {a, b, c} where a denotes processor ID, b denotes memory location, and c denotes value to be written.\n";
        cout << "Enter -1 as processor ID to finish inputting tuples.\n";

        vector<vector<int>> operations;
        vector<bool> operationAssigned(nProcessors, false);

        while (true) {
            int a, b, c;
            cin >> a;

            if (a == -1)
                break;

            if (a < 0 || a >= nProcessors) {
                cout << "Invalid processor ID. Please enter a valid processor ID.\n";
                // Ignore the remaining input of the current invalid tuple
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            cin >> b >> c;

            if (b < 0 || b >= memorySize) {
                cout << "Invalid memory location. Please enter a valid memory location.\n";
                // Ignore the remaining input of the current invalid tuple
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            if (operationAssigned[a]) {
                cout << "Error: Processor " << a << " can perform only one write operation at a time.\n";
                cout << "Write operation unsuccessful.\n";
                return;
            }

            operationAssigned[a] = true;
            operations.push_back({a, b, c});
        }

        vector<bool> modified(memorySize, false);

        for (auto& op : operations) {
            int a = op[0], b = op[1], c = op[2];

            if (modified[b]) {
                cout << "Error: Multiple processors are trying to write to the same memory location.\n";
                cout << "System crashed. Memory locations are reinitialized to 0.\n";

                memory.assign(memorySize, 0);  // Reinitialize memory to 0
                return;
            }

            memory[b] = c;
            modified[b] = true;
        }

        printMemory();
    }  
};

class CREW : public PRAMMODEL {
    public:

    CREW() {
        menu();
    }

    void read(){
        vector<bool> processorRead(nProcessors, false);

        cout << "Enter 2-tuples {a, b} where 'a' denotes processor ID and 'b' denotes memory location to read.\n";
        cout << "Enter -1 for processor ID to finish inputting tuples.\n";

        while (true) {
            int a, b;
            cin >> a;

            if (a == -1)
                break;

            cin >> b;

            // Validate processor ID and memory location
            if (a < 0 || a >= nProcessors) {
                cout << "Invalid processor ID. Please enter a valid processor ID.\n";
                // Ignore the remaining input of the current invalid tuple
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            if (b < 0 || b >= memorySize) {
                cout << "Invalid memory location. Please enter a valid memory location.\n";
                // Ignore the remaining input of the current invalid tuple
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            if (processorRead[a]) {
                cout << "Error: Processor " << a << " has already performed a read operation.\n";
                cout << "Read operation unsuccessful.\n";
                return;
            } else {
                processorRead[a] = true;
            }
        }
        
        cout << "Read operation successful.\n";
    }

    void write() {
        cout << "Enter 3-tuples {a, b, c} where a denotes processor ID, b denotes memory location, and c denotes value to be written.\n";
        cout << "Enter -1 as processor ID to finish inputting tuples.\n";

        vector<vector<int>> operations;
        vector<bool> operationAssigned(nProcessors, false);

        while (true) {
            int a, b, c;
            cin >> a;

            if (a == -1)
                break;

            if (a < 0 || a >= nProcessors) {
                cout << "Invalid processor ID. Please enter a valid processor ID.\n";
                // Ignore the remaining input of the current invalid tuple
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            cin >> b >> c;

            if (b < 0 || b >= memorySize) {
                cout << "Invalid memory location. Please enter a valid memory location.\n";
                // Ignore the remaining input of the current invalid tuple
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            if (operationAssigned[a]) {
                cout << "Error: Processor " << a << " can perform only one write operation at a time.\n";
                cout << "Write operation unsuccessful.\n";
                return;
            }

            operationAssigned[a] = true;
            operations.push_back({a, b, c});
        }

        vector<bool> modified(memorySize, false);

        for (auto& op : operations) {
            int a = op[0], b = op[1], c = op[2];

            if (modified[b]) {
                cout << "Error: Multiple processors are trying to write to the same memory location.\n";
                cout << "System crashed. Memory locations are reinitialized to 0.\n";

                memory.assign(memorySize, 0);  // Reinitialize memory to 0
                return;
            }

            memory[b] = c;
            modified[b] = true;
        }

        printMemory();
    }
};

class PriorityCRCW : public PRAMMODEL {
    public:

    PriorityCRCW() {
        menu();
    }

    void read(){
        vector<bool> processorRead(nProcessors, false);

        cout << "Enter 2-tuples {a, b} where 'a' denotes processor ID and 'b' denotes memory location to read.\n";
        cout << "Enter -1 for processor ID to finish inputting tuples.\n";

        while (true) {
            int a, b;
            cin >> a;

            if (a == -1)
                break;

            cin >> b;

            // Validate processor ID and memory location
            if (a < 0 || a >= nProcessors) {
                cout << "Invalid processor ID. Please enter a valid processor ID.\n";
                // Ignore the remaining input of the current invalid tuple
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            if (b < 0 || b >= memorySize) {
                cout << "Invalid memory location. Please enter a valid memory location.\n";
                // Ignore the remaining input of the current invalid tuple
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            if (processorRead[a]) {
                cout << "Error: Processor " << a << " has already performed a read operation.\n";
                cout << "Read operation unsuccessful.\n";
                return;
            } else {
                processorRead[a] = true;
            }
        }
        
        cout << "Read operation successful.\n";
    }

    void write() {
        cout << "Enter 3-tuples {a, b, c} where a denotes processor ID, b denotes memory location, and c denotes value to be written.\n";
        cout << "Enter -1 as processor ID to finish inputting tuples.\n";

        vector<vector<int>> operations;
        vector<bool> operationAssigned(nProcessors, false);

        while (true) {
            int a, b, c;
            cin >> a;

            if (a == -1)
                break;

            if (a < 0 || a >= nProcessors) {
                cout << "Invalid processor ID. Please enter a valid processor ID.\n";
                // Ignore the remaining input of the current invalid tuple
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            cin >> b >> c;

            if (b < 0 || b >= memorySize) {
                cout << "Invalid memory location. Please enter a valid memory location.\n";
                // Ignore the remaining input of the current invalid tuple
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            if (operationAssigned[a]) {
                cout << "Error: Processor " << a << " can perform only one write operation at a time.\n";
                cout << "Write operation unsuccessful.\n";
                return;
            }

            operationAssigned[a] = true;
            operations.push_back({a, b, c});
        }

        vector<int> modified(memorySize, nProcessors);

        for (auto& op : operations) {
            int a = op[0], b = op[1], c = op[2];
            if (a < modified[b]) {
                memory[b] = c; 
                modified[b] = a;
            }
        }

        printMemory();
    }
};

class ArbitraryCRCW : public PRAMMODEL {
    public:

    ArbitraryCRCW() {
        menu();
    }

    void read(){
        vector<bool> processorRead(nProcessors, false);

        cout << "Enter 2-tuples {a, b} where 'a' denotes processor ID and 'b' denotes memory location to read.\n";
        cout << "Enter -1 for processor ID to finish inputting tuples.\n";

        while (true) {
            int a, b;
            cin >> a;

            if (a == -1)
                break;

            cin >> b;

            // Validate processor ID and memory location
            if (a < 0 || a >= nProcessors) {
                cout << "Invalid processor ID. Please enter a valid processor ID.\n";
                // Ignore the remaining input of the current invalid tuple
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            if (b < 0 || b >= memorySize) {
                cout << "Invalid memory location. Please enter a valid memory location.\n";
                // Ignore the remaining input of the current invalid tuple
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            if (processorRead[a]) {
                cout << "Error: Processor " << a << " has already performed a read operation.\n";
                cout << "Read operation unsuccessful.\n";
                return;
            } else {
                processorRead[a] = true;
            }
        }
        
        cout << "Read operation successful.\n";
    }

    void write() {
        cout << "Enter 3-tuples {a, b, c} where a denotes processor ID, b denotes memory location, and c denotes value to be written.\n";
        cout << "Enter -1 as processor ID to finish inputting tuples.\n";

        vector<vector<int>> operations;
        vector<bool> operationAssigned(nProcessors, false);

        while (true) {
            int a, b, c;
            cin >> a;

            if (a == -1)
                break;

            if (a < 0 || a >= nProcessors) {
                cout << "Invalid processor ID. Please enter a valid processor ID.\n";
                // Ignore the remaining input of the current invalid tuple
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            cin >> b >> c;

            if (b < 0 || b >= memorySize) {
                cout << "Invalid memory location. Please enter a valid memory location.\n";
                // Ignore the remaining input of the current invalid tuple
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            if (operationAssigned[a]) {
                cout << "Error: Processor " << a << " can perform only one write operation at a time.\n";
                cout << "Write operation unsuccessful.\n";
                return;
            }

            operationAssigned[a] = true;
            operations.push_back({a, b, c});
        }

        vector<vector<int>> modified(memorySize);

        for (auto& op : operations) {
            int a = op[0], b = op[1], c = op[2];
            modified[b].push_back(c);
        }

        for(int i = 0; i < memorySize; i++) {
            if(modified[i].size() > 0) {
                memory[i] = modified[i][rand()%(modified.size())];
            }
        }

        printMemory();
    }
};

class CommonCRCW : public PRAMMODEL {
    public:

    CommonCRCW() {
        menu();
    }

    void read(){
        vector<bool> processorRead(nProcessors, false);

        cout << "Enter 2-tuples {a, b} where 'a' denotes processor ID and 'b' denotes memory location to read.\n";
        cout << "Enter -1 for processor ID to finish inputting tuples.\n";

        while (true) {
            int a, b;
            cin >> a;

            if (a == -1)
                break;

            cin >> b;

            // Validate processor ID and memory location
            if (a < 0 || a >= nProcessors) {
                cout << "Invalid processor ID. Please enter a valid processor ID.\n";
                // Ignore the remaining input of the current invalid tuple
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            if (b < 0 || b >= memorySize) {
                cout << "Invalid memory location. Please enter a valid memory location.\n";
                // Ignore the remaining input of the current invalid tuple
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            if (processorRead[a]) {
                cout << "Error: Processor " << a << " has already performed a read operation.\n";
                cout << "Read operation unsuccessful.\n";
                return;
            } else {
                processorRead[a] = true;
            }
        }
        
        cout << "Read operation successful.\n";
    }

    void write() {
        cout << "Enter 3-tuples {a, b, c} where a denotes processor ID, b denotes memory location, and c denotes value to be written.\n";
        cout << "Enter -1 as processor ID to finish inputting tuples.\n";

        vector<vector<int>> operations;
        vector<bool> operationAssigned(nProcessors, false);

        while (true) {
            int a, b, c;
            cin >> a;

            if (a == -1)
                break;

            if (a < 0 || a >= nProcessors) {
                cout << "Invalid processor ID. Please enter a valid processor ID.\n";
                // Ignore the remaining input of the current invalid tuple
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            cin >> b >> c;

            if (b < 0 || b >= memorySize) {
                cout << "Invalid memory location. Please enter a valid memory location.\n";
                // Ignore the remaining input of the current invalid tuple
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            if (operationAssigned[a]) {
                cout << "Error: Processor " << a << " can perform only one write operation at a time.\n";
                cout << "Write operation unsuccessful.\n";
                return;
            }

            operationAssigned[a] = true;
            operations.push_back({a, b, c});
        }

        vector<pair<bool, int>> modified(memorySize, {false,INT_MAX});

        for (auto& op : operations) {
            int a = op[0], b = op[1], c = op[2];
            if(modified[b].first == false) {
                modified[b].first = true;
                modified[b].second = c;
            }
            else if(modified[b].second != c) {
                cout << "Error: Multiple processors are trying to write different values at the same memory location.\n";
                cout << "System crashed. Memory locations are reinitialized to 0.\n";

                memory.assign(memorySize, 0);  // Reinitialize memory to 0
                return;
            }
        }

        for(int i = 0; i < memorySize; i++) {
            if(modified[i].first == true) {
                memory[i] = modified[i].second;
            }
        }

        printMemory();
    }
};

class OddEvenTranspositionSort {
    private:
    int size;
    vector<int> array;

    // Function to perform Odd Even Transposition Sort
    void oddEvenTranspositionSort() {
        cout << "Odd-Even Transposition Sort is an O(n) time and O(n^2) work algorithm that requires n processors to sort an array of size n." << endl;
        cout << "It requires n steps to sort n numbers." << endl;
        for (int step = 1; step <= size; step++) {
            cout << "-----------------Step " << step << "-----------------" << endl;
            if (step % 2 == 1) {
                performAndPrintTransposition('O'); // Perform odd transpositions
            } else {
                performAndPrintTransposition('E'); // Perform even transpositions
            }
            cout << "----------------------------------------" << endl;
        }
    }

    // Function to perform odd or even transpositions and print results
    void performAndPrintTransposition(char type) {
        if (type == 'O') {
            // Odd Transposition
            for (int i = 0; i < size - 1; i += 2) {
                cout << array[i] << " <-> " << array[i + 1] << "   ";
            }
            if (size % 2 == 1) {
                cout << array[size - 1];
            }
            cout << endl;

            for (int i = 0; i < size - 1; i += 2) {
                if (array[i] > array[i + 1]) {
                    swap(array[i], array[i + 1]);
                }
                cout << array[i] << " " << array[i + 1] << " ";
            }
            if (size % 2 == 1) {
                cout << array[size - 1];
            }
            cout << endl;
        } else if (type == 'E') {
            // Even Transposition
            cout << array[0] << " ";
            for (int i = 1; i < size - 1; i += 2) {
                cout << array[i] << " <-> " << array[i + 1] << "   ";
            }
            if (size % 2 == 0) {
                cout << array[size - 1];
            }
            cout << endl;

            cout << array[0] << " ";
            for (int i = 1; i < size - 1; i += 2) {
                if (array[i] > array[i + 1]) {
                    swap(array[i], array[i + 1]);
                }
                cout << array[i] << " " << array[i + 1] << " ";
            }
            if (size % 2 == 0) {
                cout << array[size - 1];
            }
            cout << endl;
        }
    }   
    
    public:
    // Constructor to Initialize the array and call the sorting function
    OddEvenTranspositionSort() {
        cout<<"Enter the Size of Array:";
        cin>>size;
        cout<<endl;
        array.resize(size);
        cout<<"Enter the array: ";
        for(int i = 0; i < size; i++) {
            cin>>array[i];
        }
        oddEvenTranspositionSort();
    } 
};

class ShearSort {
    private:
    int rows, cols;
    vector<vector<int>> matrix;

    // Function to Print the Matrix
    void printMatrix() {
        for(int i = 0; i < rows; i++) {
            for(int j = 0; j < cols; j++) {
                cout<< matrix[i][j] <<" ";
            }
            cout<<endl;
        }
    }

    // Perform the Column Transpositions for all Rows
    void rowTransposition() {
        for(int i = 0; i < rows; i++) {
            if(i%2 == 0) {
                // Even Index row sorted in ascending order
                sort(matrix[i].begin(), matrix[i].end());
            }
            else {
                // Odd Index row sorted in descending order
                sort(matrix[i].begin(), matrix[i].end(), greater<int>());
            }
        }
    }

    // Perform the Column Transpositions for all Columns
    void colTransposition() {
        for (int col = 0; col <= cols; col++) {

            vector<int> tempColumn(rows);

            for (int row = 0; row < rows; row++) {
                tempColumn[row] = matrix[row][col];
            }

            sort(tempColumn.begin(), tempColumn.end());

            for (int row = 0; row < rows; ++row) {
                matrix[row][col] = tempColumn[row];
            }
        }
    }
    
    void shearSort() {
        cout<<endl<<endl;
        int n = 2*ceil(log2(rows)) + 1;
        cout << "Shear Sort is an O(nlogn) time and O(n^2 logn) work algorithm that requires n*m processors to sort a matrix of size n x m." << endl;
        cout << "It requires 2*ceil(logn)+2 times alternative row and column transpositions sort a n x m matrix." << endl << endl;
        for(int i = 1; i <= n; i++) {
            cout<<"Step "<<i<<": ";
            if(i%2 == 1) {
                cout<<"Row Transposition"<<endl;
                rowTransposition();
            }
            else {
                cout<<"Column Transposition"<<endl;
                colTransposition();
            }
            printMatrix();
            cout<<endl;
        }
    }

    public:
    // Constructor to Initialize the Matrix and call the sorting function
    ShearSort() {
        cout<<"Enter number of row and columns of the matrix:";
        cin>>rows>>cols;
        cout<<endl;
        matrix.resize(rows);
        for(int i = 0; i < rows; i++) {
            matrix[i].resize(cols);
        }
        cout<<"Enter the matrix values:"<<endl;
        for(int i = 0; i < rows; i++) {
            for(int j = 0; j < cols; j++) {
                cin>>matrix[i][j];
            }
        }
        shearSort();
    }

};

class MullerPreparataSort {

    private:
    int n;
    vector<int> array;
    vector<vector<int>> matrix;

    void mullerPreparataSort() {
        cout<<"Muller Preparata is a O(logn) time O(n^2) work algorithm which requires n^2 processors to sort n numbers"<<endl;
        cout<<"All the Numbers in the array should be unique";
        cout<<endl<<endl;
        cout<<"Your Entered Array is: "<<endl;
        printArray();
        cout<<endl;
        cout<<"Step 1: Initialize the matrix with 0's and 1's where processor at (i,j) compares numbers array[i] and array[j]"<<endl<<endl;
        initializeMatrix();
        cout<<"Initialized Matrix is:"<<endl<<endl;
        printMatrixAndArray();
        cout<<"Step 2: Sum the elements of each column which will finally placed at the first row of the matrix"<<endl<<endl;
        sumCols();
        cout<<"Matrix after adding the column elements and placing the sum to the first row is:"<<endl<<endl;
        printMatrixAndArray();
        cout<<"Step 3: Place the Array Elements at correct position according to the column sum"<<endl<<endl;
        placeElements();
        cout<<"Array after performing the last step is: "<<endl;
        printArray();
    }

    // Function to Initialize the Matrix with 0's and 1's
    void initializeMatrix() {
        for(int i = 0; i < n; i++) {
            for(int j = 0; j < n; j++) {
                if(array[j] <= array[i]) {
                    matrix[i][j] = 0;
                }
                else {
                    matrix[i][j] = 1;
                }
            }
        }
    }

    // Function to sum the column elements and place at the first row
    void sumCols() {
        for(int j = 0; j < n; j++) {
            int top = 0, bottom = n-1;

            while(top < bottom) {
                int middle = (top+bottom-1)/2;
                for(int i = 0; i <= middle; i++) {
                    matrix[i][j] = matrix[2*i][j] + matrix[2*i+1][j];
                } 
                if((bottom+1)%2 == 0) {
                    bottom = middle;
                }
                else {
                    matrix[middle+1][j] = matrix[bottom][j];
                    bottom = middle + 1;
                }
            }
        }
    }

    // Function to place the elements in the array according to the values in first row
    void placeElements() {
        vector<int> temp(n);
        for(int i = 0; i < n; i++) {
            temp[matrix[0][i]] = array[i];
        }
        for(int i = 0; i < n; i++) {
            array[i] = temp[i];
        }
    }

    // Function to print the array
    void printArray() {
        for(int i = 0; i < n; i++) {
            cout<<array[i]<<" ";
        }
        cout<<endl;
    }

    //Function to print the Matrix and Array
    void printMatrixAndArray() {
        for(int i = 0; i < n; i++) {
            cout<<array[i]<<" ";
        }
        cout<<endl;
        for(int i = 0; i < 3*n; i++) {
            cout<<"-";
        }
        cout<<endl;
        for(int i = 0; i < n; i++) {
            for(int j = 0; j < n; j++) {
                cout<<matrix[i][j]<<" ";
            }
            cout<<endl;
        }
        cout<<endl;
    }

    public:
    // Constructor to Initialize the array and call the sorting function
    MullerPreparataSort() {
        cout<<"Enter the Size of Array:";
        cin>>n;
        cout<<endl;
        array.resize(n);
        matrix.resize(n);
        set<int> st;
        for(int i = 0; i < n; i++) {
            matrix[i].resize(n);
        }
        cout<<"Enter the array: ";
        for(int i = 0; i < n; i++) {
            cin>>array[i];
            st.insert(array[i]);
        }
        if(st.size()<n) {
            cout<<"Error: This Algorithm can sort the array only if all the elements are distinct."<<endl;
            return;
        }
        mullerPreparataSort();
    }
};

class HirschbergSort {
    private:
    int n;
    vector<int> array;
    vector<vector<int>> matrix;

    // Function to Print the Matrix and Array
    void printMatrixAndArray() {
        for(int i = 0; i < n; i++) {
            cout<<array[i]<<" ";
        }
        cout<<endl;
        for(int i = 0; i < 4*n; i++) {
            cout<<"-";
        }
        cout<<endl;
        for(int i = 0; i < sqrt(n); i++) {
            for(int j = 0; j < n; j++) {
                cout<<matrix[i][j]<<" ";
            }
            cout<<endl;
        }
        cout<<endl;
    }

    // Function to Print the Array
    void printArray() {
        for(int i = 0; i < n; i++) {
            cout<<array[i]<<" ";
        }
        cout<<endl;
    }

    // Function to perform the Substep of Muller Preparata Sorting
    void parallelMullerPreparata() {
        cout<<"Step 1: The Array is divided into sqrt(n) equal parts and each part is sorted individually using Muller Preparata"<<endl<<endl;
        cout<<"Step 1.1: Initialize the Matrix with 0's and 1's to sort the subarrays according to Muller Preparata Algorithm"<<endl<<endl;
        parallelMullerInitialize();
        cout<<"Initialized Matrix is:"<<endl<<endl;
        printMatrixAndArray();
        cout<<"Step 1.2: Sum the numbers in each column and place the sum at the first row"<<endl<<endl;
        sumCols();
        cout<<"Matrix after adding the column elements and placing the sum to the first row is:"<<endl<<endl;
        printMatrixAndArray();
        cout<<"Step 1.3: Place the Array Elements at correct position according to the column sum"<<endl<<endl;
        placeElementsMuller();
        cout<<"Array after performing the last step is: "<<endl;
        printArray();
        cout<<endl<<endl;
    }

    // Function to Initialize the Matrix for the Muller Preparata Substep
    void parallelMullerInitialize() {
        for(int i = 0; i < sqrt(n); i++) {
            for(int j = 0; j < n; j++) {
                int n1 = array[j];
                int n2 = array[((j/(int)sqrt(n))*sqrt(n)) + i];
                if(n1<=n2) {
                    matrix[i][j]=0;
                }
                else {
                    matrix[i][j]=1;
                }
            }
        }
    }

    // Function to place the elements in the matrix according to the Muller Preparata Substep
    void placeElementsMuller() {
        vector<int> temp(n);
        for(int i = 0; i < n; i++) {
            temp[sqrt(n)*(i/((int)sqrt(n))) + matrix[0][i]] = array[i];
        }
        for(int i = 0; i < n; i++) {
            array[i] = temp[i];
        }
    }

    void hirschbergSort() {
        cout<<"Hirschberg Sort is a O(logn) time O(n*sqrt(n)*logn) work algorithm which requires n*sqrt(n) processors to sort n numbers"<<endl;
        cout<<"All the Numbers in the array should be unique";
        cout<<endl<<endl;
        parallelMullerPreparata();
        cout<<"Step 2: Initialize the matrix with 0's and 1's according to Hirschberg's Algorithm where processor at (i,j) compares array[j] with array[sqrt(n)*(j/sqrt(n)) + i]"<<endl<<endl;
        initializeMatrixHirschberg();
        cout<<"Initialized Matrix is:"<<endl<<endl;
        printMatrixAndArray();
        cout<<"Step 3: Sum the numbers in each column and place the sum at the first row"<<endl<<endl;
        sumCols();
        cout<<"Matrix after adding the column elements and placing the sum to the first row is:"<<endl<<endl;
        printMatrixAndArray();
        cout<<"Step 1.3: Place the Array Elements at correct position according to the column sum"<<endl<<endl;
        placeElementsHirschberg();
        cout<<"Array after performing the last step is: "<<endl;
        printArray();
    }

    // Function to Initialize the matrix for the Hirschberg Substep
    void initializeMatrixHirschberg() {
        for(int i = 0; i < sqrt(n); i++) {
            for(int j = 0; j < n; j++) {
                matrix[i][j] =greater_than_how_many(i * sqrt(n), i * sqrt(n) + sqrt(n) - 1, array[j]);
                cout<<matrix[i][j]<<" ";
            }
            cout<<endl;
        }
        cout<<endl<<endl;
    }

    // Function to place the elements in the array according to the values in the first row
    void placeElementsHirschberg() {
        vector<int> temp(n);
        for(int i = 0; i < n; i++) {
            temp[matrix[0][i]] = array[i];
        }
        for(int i = 0; i < n; i++) {
            array[i] = temp[i];
        }
    }

    // Function to sum the values in each column and placing at the first row of the matrix
    void sumCols() {
        for(int j = 0; j < n; j++) {
            int top = 0, bottom = sqrt(n)-1;

            while(top < bottom) {
                int middle = (top+bottom-1)/2;
                for(int i = 0; i <= middle; i++) {
                    matrix[i][j] = matrix[2*i][j] + matrix[2*i+1][j];
                } 
                if((bottom+1)%2 == 0) {
                    bottom = middle;
                }
                else {
                    matrix[middle+1][j] = matrix[bottom][j];
                    bottom = middle + 1;
                }
            }
        }
    }

    // Function to calculate that how many numbers are greater than a particular number in a given range in the matrix
    int greater_than_how_many(int low, int high, int num) {
        if(num<=array[low]) return 0;
        if(num>array[high]) return high - low + 1;
        if(num==array[high]) return high - low;
        int copy_low = low;
        int ind = high + 1;
        while(low <= high) {
            int mid = (low + high)/2;
            if(array[mid] == num) {
                return mid - copy_low;
            }
            else if(array[mid] < num) {
                ind = mid;
                low = mid + 1;
            }
            else {
                high = mid - 1;
            }
        }
        return ind - copy_low + 1;
    }
    
    public:
    // Constructor to take the input array as well as to call main sorting function
    HirschbergSort() {
        cout<<"Enter the Size of Array:";
        cin>>n;
        if(sqrt(n)*sqrt(n) != n) {
            cout<<"Hirschberg Sort can only arrays having size a perfect square number"<<endl;
            return;
        }
        cout<<endl;
        array.resize(n);
        matrix.resize(sqrt(n));
        set<int> st;
        for(int i = 0; i < sqrt(n); i++) {
            matrix[i].resize(n);
        }
        cout<<"Enter the array: ";
        for(int i = 0; i < n; i++) {
            cin>>array[i];
            st.insert(array[i]);
        }
        if(st.size()<n) {
            cout<<"Error: This Algorithm can sort the array only if all the elements are distinct."<<endl;
            return;
        }
        hirschbergSort();
    }
};

void parallelAlgorithmsMenu() {
    while(1) {
        cout<<"Choose Parallel Sorting Algorithms from the following:"<<endl;
        cout<<"Enter 1 for Odd Even Transposition Sort"<<endl;
        cout<<"Enter 2 for Shear Sort"<<endl;
        cout<<"Enter 3 for Muller Preparata Sort"<<endl;
        cout<<"Enter 4 for Hirschberg Sort"<<endl;
        cout<<"Enter 5 to Go to Main Menu"<<endl;
        int choice;
        cin>>choice;
        if (choice == 1) {
            cout << endl;
            OddEvenTranspositionSort OES;
        } else if (choice == 2) {
            cout << endl;
            ShearSort SS;
        } else if (choice == 3) {
            cout << endl;
            MullerPreparataSort MPS;
        } else if (choice == 4) {
            cout << endl;
            HirschbergSort HS;
        }
        else if (choice == 5) {
            return;
        }
        else {
            cout << "Invalid Choice.";
        }
    }
}

void PRAMModelsMenu() {
    while(1) {
        cout<<"Choose PRAM Model from the following:"<<endl;
        cout<<"Enter 1 for EREW PRAM Model"<<endl;
        cout<<"Enter 2 for CREW PRAM Model"<<endl;
        cout<<"Enter 3 for Priority CRCW PRAM Model"<<endl;
        cout<<"Enter 4 for Arbitrary CRCW PRAM Model"<<endl;
        cout<<"Enter 5 for Common CRCW PRAM Model"<<endl;
        cout<<"Enter 6 to Go to Main Menu"<<endl;
        int choice;
        cin>>choice;
        if (choice == 1) {
            cout << endl;
            EREW EE;
        } else if (choice == 2) {
            cout << endl;
            CREW CE;
        } else if (choice == 3) {
            cout << endl;
            PriorityCRCW PCC;
        } else if (choice == 4) {
            cout << endl;
            ArbitraryCRCW ACC;
        } else if (choice == 5) {
            cout << endl;
            CommonCRCW CCC;
        }
        else if (choice == 6) {
            return;
        }
        else {
            cout << "Invalid Choice.";
        }
    }
}

void mainMenu() {

    while(1) {
        cout<<"Enter 1 for List of Parallel Sorting Algorithms"<<endl;
        cout<<"Enter 2 for List of PRAM Models"<<endl;
        cout<<"Enter 3 to Exit"<<endl;
        int choice;
        cin>>choice;
        if (choice == 1) {
            cout << endl;
            parallelAlgorithmsMenu();
        } else if (choice == 2) {
            cout << endl;
            PRAMModelsMenu();
        }
        else if (choice == 3) {
            return;
        }
        else {
            cout << "Invalid Choice.";
        }
    }

} 

int main() {
    cout<<"Welcome to Parallel Sorting Algorithms and PRAM Models Simulator"<<endl<<endl;
    mainMenu();
    return 0;
}
