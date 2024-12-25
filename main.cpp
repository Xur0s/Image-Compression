#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
using namespace std;

class distanceSkeleton {
    public:
        int numRows;
        int numCols;
        int minVal;
        int maxVal;
        int newMinVal;
        int newMaxVal;
        int** ZFAry;
        int** skeletonAry;
        int distanceChoice;
    
    public:
        void setZero(int** ary) {
            int numRows = this->numRows+2;
            int numCols = this->numCols+2;
            for(int i = 0; i < numRows; i++) {
                for(int j = 0; j < numCols; j++) {
                    ary[i][j] = 0;
                }
            }
        }

        void loadImage(ifstream & inFile) {
            int numRows = this->numRows+2;
            int numCols = this->numCols+2;
            for(int i = 1; i < numRows-1; i++) {
                for(int j = 1; j < numCols-1; j++) {
                    inFile >> ZFAry[i][j];
                }
            }
        }

        void prettyPrint(int** ary, ofstream & printFile, int minVal, int maxVal) {
            int numRows = this->numRows+2;
            int numCols = this->numCols+2;
            printFile << numRows << "  " << numCols << "  " << minVal << "  " << maxVal << endl;
            string str = to_string(maxVal);
            int Width = str.length();
            for(int i = 0; i < numRows; i++) { 
                for(int j = 0; j < numCols; j++) {
                    printFile << ary[i][j];
                    str = to_string(ary[i][j]);
                    int WW = str.length();
                    while(WW <= Width) {
                        printFile << " ";
                        WW++;
                    }
                }
                printFile << endl;
            }
            printFile << endl;
        }

        void distancePass1(int distanceChoice, ofstream & logFile) {
            logFile << "Entering distancePass1 () method.\n";
            int numRows = this->numRows+2;
            int numCols = this->numCols+2;
            newMinVal = 0;
            newMaxVal = 0;
            for(int i = 1; i < numRows-1; i++) {
                for(int j = 1; j < numCols-1; j++) {
                    if (ZFAry[i][j] > 0) {
                        int a = ZFAry[i-1][j-1];
                        int b = ZFAry[i-1][j];
                        int c = ZFAry[i-1][j+1];
                        int d = ZFAry[i][j-1];
                        switch(distanceChoice) {
                            case 4:
                                a += 2;
                                b += 1;
                                c += 2;
                                d += 1;
                                break;
                            case 8:
                                a += 1;
                                b += 1;
                                c += 1;
                                d += 1;
                                break;
                            case 10:
                                a += round(sqrt(2));
                                b += 1;
                                c += round(sqrt(2));
                                d += 1;
                                break;
                        }
                        int passMinVal = min(min(a,b), min(c,d));
                        ZFAry[i][j] = passMinVal;
                        if (ZFAry[i][j] < newMinVal)
                            newMinVal = ZFAry[i][j];
                        if (ZFAry[i][j] > newMaxVal)
                            newMaxVal = ZFAry[i][j];
                    }
                }
            }
            logFile << "Leaving distancePass1 () method.\n";
        }

        void distancePass2(int distanceChoice, ofstream & logFile) {
            logFile << "Entering distancePass2 () method.\n";
            int numRows = this->numRows+2;
            int numCols = this->numCols+2;
            newMinVal = 0;
            newMaxVal = 0;
            for(int i = numRows-2; i > 0; i--) {
                for(int j = numCols-2; j > 0; j--) {    
                    int e = ZFAry[i][j+1];
                    int f = ZFAry[i+1][j-1];
                    int g = ZFAry[i+1][j];
                    int h = ZFAry[i+1][j+1];
                    switch(distanceChoice) {
                        case 4:
                            e += 1;
                            f += 2;
                            g += 1;
                            h += 2;
                            break;
                        case 8:
                            e += 1;
                            f += 1;
                            g += 1;
                            h += 1;
                            break;
                        case 10:
                            e += 1;
                            f += round(sqrt(2));
                            g += 1;
                            h += round(sqrt(2));
                            break;
                    }
                    int passMinVal = min(min(e,f), min(g,h));
                    passMinVal = min(passMinVal, ZFAry[i][j]);
                    ZFAry[i][j] = passMinVal;
                    if (ZFAry[i][j] < newMinVal)
                        newMinVal = ZFAry[i][j];
                    if (ZFAry[i][j] > newMaxVal)
                        newMaxVal = ZFAry[i][j];
                }
            }
            logFile << "Leaving distancePass2 () method.\n";
        }

        void distanceTransform(int distanceChoice, ofstream & printFile, ofstream & logFile) {
            logFile << "Entering DistanceTransform () method.\n";
            distancePass1(distanceChoice, logFile);
            printFile << "1st pass distance transform with choice = " << distanceChoice << endl;
            prettyPrint(ZFAry, printFile, 0, newMaxVal);
            distancePass2(distanceChoice, logFile);
            printFile << "2nd pass distance transform with choice = " << distanceChoice << endl;
            prettyPrint(ZFAry, printFile, 0, newMaxVal);
            logFile << "Leaving DistanceTransform () method.\n";
        }

        bool isLocalMaxima(int row, int col) {
            if(ZFAry[row][col] > 0) {
                int a = ZFAry[row-1][col-1];
                int b = ZFAry[row-1][col];
                int c = ZFAry[row-1][col+1];
                int d = ZFAry[row][col-1];
                int e = ZFAry[row][col+1];
                int f = ZFAry[row+1][col-1];
                int g = ZFAry[row+1][col];
                int h = ZFAry[row+1][col+1];
                int maxNeighborValue;
                if(distanceChoice == 4) {
                    maxNeighborValue = max(max(b,d), max(e,g));
                    if(ZFAry[row][col] >= maxNeighborValue)
                        return true;
                }
                if(distanceChoice == 8 || distanceChoice == 10) {
                    maxNeighborValue = max(max(a,b), max(c,d));
                    maxNeighborValue = max(maxNeighborValue, max(e,f));
                    maxNeighborValue = max(maxNeighborValue, max(g,h));
                    if(ZFAry[row][col] >= maxNeighborValue)
                        return true;  
                }
            }
            return false;
        }
        
        void computeLocalMaxima(ofstream & logFile) {
            logFile << "Entering computeLocalMaxima () method.\n";
            int numRows = this->numRows+2;
            int numCols = this->numCols+2;
            for(int i = 1; i < numRows-1; i++) {
                for(int j = 1; j < numCols-1; j++) {
                    if(isLocalMaxima(i,j)) {
                        skeletonAry[i][j] = ZFAry[i][j];
                    } else {
                        skeletonAry[i][j] = 0;
                    }
                }
            }
            logFile << "Leaving computeLocalMaxima () method.\n";
        }

        void extractSkeleton(ofstream & skeletonFile, ofstream & logFile) {
            logFile << "Entering extractSkeleton () method.\n";
            int numRows = this->numRows+2;
            int numCols = this->numCols+2;
            skeletonFile << numRows << " " << numCols << " " << newMinVal << " " << newMaxVal << endl;
            string str = to_string(max(numRows, numCols));
            int Width = str.length();
            for(int i = 1; i < numRows-1; i++) {
                for(int j = 1; j < numCols-1; j++) {
                    if(skeletonAry[i][j] > 0) {
                        skeletonFile << i; // write i
                        str = to_string(i);
                        int WW = str.length();
                        while(WW <= Width){
                            skeletonFile << " ";
                            WW++;
                        }
                        skeletonFile << j; // write j
                        str = to_string(j);
                        WW = str.length();
                        while(WW <= Width){
                            skeletonFile << " ";
                            WW++;
                        }
                        skeletonFile << skeletonAry[i][j] << endl;
                    }
                }
            }
            logFile << "Leaving extractSkeleton () method.\n";
        }

        void compression(ofstream & skeletonFile, ofstream & printFile, ofstream & logFile) {
            logFile << "Entering compression () method.\n";
            computeLocalMaxima(logFile);
            printFile << "Local maxima, skeletonAry with choice = " << distanceChoice << endl;
            prettyPrint(skeletonAry, printFile, 0, newMaxVal);
            extractSkeleton(skeletonFile, logFile);
            logFile << "Below is skeleton Array with choice = " << distanceChoice << endl << endl;
            prettyPrint(skeletonAry, logFile, 0, newMaxVal);
            logFile << "Leaving compression () method.\n";
        }

        void loadSkeleton(ifstream & skeletonFile, ofstream & logFile) {
            logFile << "Entering loadSkeleton () method.\n";
            int row;
            int col;
            int value;
            string header;
            getline(skeletonFile, header);
            while(skeletonFile >> row >> col >> value) {
                ZFAry[row][col] = value;
            }
            logFile << "Leaving loadSkeleton () method.\n";
        }

        void expensionPass1(ofstream & logFile) {
            logFile << "Entering expensionPass1 () method.\n";
            int numRows = this->numRows+2;
            int numCols = this->numCols+2;
            for(int i = 1; i < numRows-1; i++) {
                for(int j = 1; j < numCols-1; j++) {
                    if(ZFAry[i][j] == 0) {
                        int a = ZFAry[i-1][j-1];
                        int b = ZFAry[i-1][j];
                        int c = ZFAry[i-1][j+1];
                        int d = ZFAry[i][j-1];
                        int e = ZFAry[i][j+1];
                        int f = ZFAry[i+1][j-1];
                        int g = ZFAry[i+1][j];
                        int h = ZFAry[i+1][j+1];
                        int valHolder;
                        switch(distanceChoice){
                            case 4:
                                a = a - 2;
                                b = b - 1;
                                c = c - 2;
                                d = d - 1;
                                e = e - 1;
                                f = f - 2;
                                g = g - 1;
                                h = h - 2;
                                valHolder = max(max(a,b), max(c,d));
                                valHolder = max(valHolder, max(e,f));
                                valHolder = max(valHolder, max(g,h));
                                valHolder = max(valHolder, ZFAry[i][j]);
                                ZFAry[i][j] = valHolder;
                                break;
                            case 8:
                                a = a - 1;
                                b = b - 1;
                                c = c - 1;
                                d = d - 1;
                                e = e - 1;
                                f = f - 1;
                                g = g - 1;
                                h = h - 1;
                                valHolder = max(max(a,b), max(c,d));
                                valHolder = max(valHolder, max(e,f));
                                valHolder = max(valHolder, max(g,h));
                                valHolder = max(valHolder, ZFAry[i][j]);
                                ZFAry[i][j] = valHolder;
                                break;
                            case 10:
                                a = round(a - sqrt(2));
                                b = b - 1;
                                c = round(c - sqrt(2));
                                d = d - 1;
                                e = e - 1;
                                f = round(f - sqrt(2));
                                g = g - 1;
                                h = round(h - sqrt(2));
                                valHolder = max(max(a,b), max(c,d));
                                valHolder = max(valHolder, max(e,f));
                                valHolder = max(valHolder, max(g,h));
                                valHolder = max(valHolder, ZFAry[i][j]);
                                ZFAry[i][j] = valHolder;
                                break;
                        }
                    }
                }
            }
            logFile << "Leaving expensionPass1 () method.\n";
        }

        void expensionPass2(ofstream & logFile) {
            logFile << "Entering expensionPass2 () method.\n";
            int numRows = this->numRows+2;
            int numCols = this->numCols+2;
            for(int i = numRows-2; i > 0; i--) {
                for(int j = numCols-2; j > 0; j--) {
                    int a = ZFAry[i-1][j-1];
                    int b = ZFAry[i-1][j];
                    int c = ZFAry[i-1][j+1];
                    int d = ZFAry[i][j-1];
                    int e = ZFAry[i][j+1];
                    int f = ZFAry[i+1][j-1];
                    int g = ZFAry[i+1][j];
                    int h = ZFAry[i+1][j+1];
                    int valHolder;
                    switch(distanceChoice){
                        case 4:
                            a = a - 2;
                            b = b - 1;
                            c = c - 2;
                            d = d - 1;
                            e = e - 1;
                            f = f - 2;
                            g = g - 1;
                            h = h - 2;
                            valHolder = max(max(a,b), max(c,d));
                            valHolder = max(valHolder, max(e,f));
                            valHolder = max(valHolder, max(g,h));
                            valHolder = max(valHolder, ZFAry[i][j]);
                            ZFAry[i][j] = valHolder;
                            break;
                        case 8:
                            a = a - 1;
                            b = b - 1;
                            c = c - 1;
                            d = d - 1;
                            e = e - 1;
                            f = f - 1;
                            g = g - 1;
                            h = h - 1;
                            valHolder = max(max(a,b), max(c,d));
                            valHolder = max(valHolder, max(e,f));
                            valHolder = max(valHolder, max(g,h));
                            valHolder = max(valHolder, ZFAry[i][j]);
                            ZFAry[i][j] = valHolder;
                            break;
                        case 10:
                            a = round(a - sqrt(2));
                            b = b - 1;
                            c = round(c - sqrt(2));
                            d = d - 1;
                            e = e - 1;
                            f = round(f - sqrt(2));
                            g = g - 1;
                            h = round(h - sqrt(2));
                            valHolder = max(max(a,b), max(c,d));
                            valHolder = max(valHolder, max(e,f));
                            valHolder = max(valHolder, max(g,h));
                            valHolder = max(valHolder, ZFAry[i][j]);
                            ZFAry[i][j] = valHolder;
                            break;
                    }
                }
            }
            logFile << "Leaving expensionPass2 () method.\n";
        }

        void deCompression(ofstream & printFile, ofstream & logFile) {
            logFile << "Entering deCompression () method.\n";
            expensionPass1(logFile);
            printFile << "1st pass Expansion with choice = " 
                        << distanceChoice << endl;
            prettyPrint(ZFAry, printFile, 0, newMaxVal);
            expensionPass2(logFile);
            printFile << "2nd pass Expansion with choice = " 
                        << distanceChoice << endl;
            prettyPrint(ZFAry, printFile, 0, newMaxVal);
            logFile << "Leaving deCompression () method.\n";
        }

        void binThreshold(ofstream & logFile, ofstream & deCompressedFile) {
            logFile << "Entering binThreshold () method.\n";
            int numRows = this->numRows+2;
            int numCols = this->numCols+2;
            for(int i = 1; i < numRows-1; i++) {
                for(int j = 1; j < numCols-1; j++) {
                    if (ZFAry[i][j] >= 1) {
                        deCompressedFile << "1 ";
                    } else {
                        deCompressedFile << "0 ";
                    }
                }
                deCompressedFile << endl;
            }
            logFile << "Leaving binThreshold () method.\n";
        }
};

main (int argc, char** argv) {
    if(argc != 7) {
        cout << "Your command line needs to include 6 parameters: " << 
                "input file, distance choice, print file, skeleton file, " <<
                "decompressed file, and log file\n";
        exit(1);
    }

    ifstream inFile(argv[1]);
    if(!inFile.is_open()) {
        cout << "Unable to open the input file" << endl;
    }
    ofstream prettyPrintFile(argv[3]);
    if(!prettyPrintFile.is_open()) {
        cout << "Unable to open the print file" << endl;
    }
    ofstream skeletonFile(argv[4]);
    if(!skeletonFile.is_open()) {
        cout << "Unable to open the skeleton file" << endl;
    }
    ofstream deCompressedFile(argv[5]);
    if(!deCompressedFile.is_open()) {
        cout << "Unable to open the decompressed file" << endl;
    }
    ofstream logFile(argv[6]);
    if(!logFile.is_open()) {
        cout << "Unable to open the log file" << endl;
    }

    distanceSkeleton* data = new distanceSkeleton();
    inFile >> data->numRows >> data->numCols >> data->minVal >> data->maxVal;
    data->ZFAry = new int*[data->numRows+2];
    for(int i = 0; i < data->numRows+2; i++) {
        data->ZFAry[i] = new int[data->numCols + 2];
    }
    data->skeletonAry = new int*[data->numRows+2];
    for(int i = 0; i < data->numRows+2; i++) {
        data->skeletonAry[i] = new int[data->numCols + 2];
    }
    data->distanceChoice = atoi(argv[2]);
    int distanceChoice = data->distanceChoice;

    data->setZero(data->ZFAry);
    data->setZero(data->skeletonAry);

    data->loadImage(inFile);
    prettyPrintFile << "** Below is input image**\n";
    data->prettyPrint(data->ZFAry, prettyPrintFile, data->minVal, data->maxVal);

    data->distanceTransform(distanceChoice, prettyPrintFile, logFile);

    data->compression(skeletonFile, prettyPrintFile, logFile);
    skeletonFile.close();

    ifstream skeletonWFile("skeletonFile.txt");
    if(!skeletonWFile.is_open()) {
        cout << "Unable to open the skeleton file" << endl;
    }

    data->setZero(data->ZFAry);
    data->loadSkeleton(skeletonWFile, logFile);
    prettyPrintFile << "** Below is the loaded skeleton with choice = " 
                    << distanceChoice << " **\n";
    data->prettyPrint(data->ZFAry, prettyPrintFile, 0, data->newMaxVal);

    data->deCompression(prettyPrintFile, logFile);
    deCompressedFile << data->numRows << " " << data->numCols << " "
                        << data->minVal << " " << data->maxVal << endl;
    
    data->binThreshold(logFile, deCompressedFile);

    inFile.close();
    prettyPrintFile.close();
    skeletonWFile.close();
    deCompressedFile.close();
    logFile.close();

    return 0;
}