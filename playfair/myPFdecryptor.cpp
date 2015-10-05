#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <stdlib.h>
#include <time.h>
#include <math.h>
using namespace std;

char alphabet[26] = "ABCDEFGHIKLMNOPQRSTUVWXYZ";  // Exclude "J"
char key[5][5];
char key_best[5][5];
int rowC[26], rowC_best[26];
int colC[26], colC_best[26];
float trigStat[17576], score, score_best;


int readText(char* filename, string &mytext){
    string line;
    ifstream myfile(filename);
    int length;
    
    if (myfile.is_open()) {
        while ( getline (myfile,line) ){
            mytext += line;
        }
        myfile.close();
    } else {
        cout << "Error opening " << filename << endl;
        return -1;
    }
    
    if ( mytext.length() % 2 != 0) {
        cout << "Length of cipher text is not multiple of 2! Padding X at the end of text" << endl;
        cin.ignore();
        mytext += 'X';
    }
    
    return 0;
}

int readStat(char* filename){
    stringstream ss;
    string tgram, line;
    float logRatio;
    ifstream myfile(filename);
    int i = 0;
    
    if (myfile.is_open()) {
        while ( getline(myfile,line) ){
            ss << line;
            ss >> tgram >> logRatio;
            ss.clear();
            trigStat[i] = logRatio;
            i++;
        }
        myfile.close();
    } else {
        cout << "Error opening " << filename << endl;
        return -1;
    }
    
    return 0;
}

void getPos(){
    int i, j;
    for(int m=0; m<25; m++){
        for (i=0; i<5; i++) {
            for (j=0; j<5; j++) {
                if (alphabet[m] == key[i][j]) {
                    rowC[alphabet[m]-65] = i;
                    colC[alphabet[m]-65] = j;
                    break;
                }
            }
        }
    }
    return;
}

void randomKey(){
    int i, j, k, m = 0;
    char *c = new char[26];
    strcpy(c, alphabet);
    
    for (i=0; i<5; i++) {
        for (j=0; j<5; j++) {
            k = rand () % (25 - m);
            key[i][j] = c[k];
            //cout << key[i][j];
            c[k] = c[24 - m];
            m++;
        }
    }
    delete[] c;
    return;
}

int getRow(char c){
    return rowC[c-65];
}

int getCol(char c){
    return colC[c-65];
}

void decryptPair(char* myPair){
    int row0 = getRow(myPair[0]);
    int column0 = getCol(myPair[0]);
    int row1 = getRow(myPair[1]);
    int column1 = getCol(myPair[1]);
    
    if (row0 == row1) {
        if (column0 == 0)
            column0 = 5;
        if (column1 == 0)
            column1 = 5;
        myPair[0] = key[row0][(column0-1)];
        myPair[1] = key[row1][(column1-1)];
    } else if (column0 == column1) {
        if (row0 == 0)
            row0 = 5;
        if (row1 == 0)
            row1 = 5;
        myPair[0] = key[(row0-1)][column0];
        myPair[1] = key[(row1-1)][column1];
    } else {
        myPair[0] = key[row0][column1];
        myPair[1] = key[row1][column0];
    }
    
    return;
}

string decryptText(string cipherT){
    string plainT, pair;
    char* cPair = new char[3];
    int txtLength = cipherT.length();
    
    for (int i=0; i<txtLength/2; i++) {
        pair = cipherT.substr(i*2, 2);
        strcpy(cPair, pair.c_str());
        //cout << cPair << endl;
        decryptPair(cPair);
        plainT += cPair;
    }
    
    delete[] cPair;
    return plainT;
}

int tgram2int(string t) {
    int num;
    num = (t[0]-64)*26*26 + (t[1]-64)*26 + (t[2]-64) -703; // 703 is offset of AAA
    return num;
}

float getScore(string p) {
    float score = 0;
    int index, totalTgram = p.length()-2;
    string tgram;
    
    for (int i=0; i<totalTgram; i++) {
        tgram = p.substr(i, 3);
        //    cout << i << " " << tgram << endl;
        index = tgram2int(tgram);
        score += trigStat[index];
    }
    
    return score;
}

void copyKRC(char mode){
    if (mode == 'n') {                              // n stants for normal
        memcpy( &key_best, &key, sizeof(key) );
        memcpy( &rowC_best, &rowC, sizeof(rowC) );
        memcpy( &colC_best, &colC, sizeof(colC) );
        score_best = score;
    } else if (mode == 'r') {                       // r stants for reverse
        memcpy( &key, &key_best, sizeof(key) );
        memcpy( &rowC, &rowC_best, sizeof(rowC) );
        memcpy( &colC, &colC_best, sizeof(colC) );
        score = score_best;
    } else {
        cout << "Copy mode undefined" << endl;
        cin.ignore();
    }
    return;
}

void printKey(char mode) {
    if (mode == 'm')
        cout << "=key=" << endl;
    else
        cout << ";  Key = ";
    for (int i=0; i<5; i++) {
        for (int j=0; j<5; j++) {
            cout << key[i][j];
        }
        if (mode == 'm')
            cout << endl;
    }
    if (mode == 'm')
        cout << "=====" << endl;
    return;
}

void keySwap(char mode) {
    int i,j,k;
    char tmp;
    
    if (mode == 's') {
        i = rand () % 25;
        do{
            j = rand () % 25;
        }while (j == i);
        
        tmp = key[i/5][i%5];
        key[i/5][i%5] = key[j/5][j%5];
        key[j/5][j%5] = tmp;
    } else if (mode == 'd') {
        i = rand () % 25;
        do{
            j = rand () % 25;
        }while (j == i);
        do{
            k = rand () % 25;
        }while ((k == i) || (k == j));
        
        tmp = key[i/5][i%5];
        key[i/5][i%5] = key[j/5][j%5];
        key[j/5][j%5] = key[k/5][k%5];
        key[k/5][k%5] = tmp;
    } else if (mode == 'r') {
        i = rand () % 5;
        do{
            j = rand () % 5;
        }while (j == i);
        
        for (int m=0; m<5; m++) {
            tmp = key[i][m];
            key[i][m] = key[j][m];
            key[j][m] = tmp;
        }
    } else if (mode == 'c') {
        i = rand () % 5;
        do{
            j = rand () % 5;
        }while (j == i);
        
        for (int m=0; m<5; m++) {
            tmp = key[m][i];
            key[m][i] = key[m][j];
            key[m][j] = tmp;
        }
    } else {
        cout << "keySwap mode undefined" << endl;
        cin.ignore();
    }
    
    getPos(); // refresh key position
    return;
}

void keyTurn(char mode) {
    char keyTemp[5][5];
    for (int i=0; i<5; i++) {
        for (int j=0; j<5; j++) {
            if (mode == 't') {
                keyTemp[i][j] = key[j][i];
            } else if (mode == 'r') {
                keyTemp[i][j] = key[4-i][j];
            } else if (mode == 'c') {
                keyTemp[i][j] = key[i][4-j];
            } else {
                cout << "keyTurn mode undefined" << endl;
                cin.ignore();
            }
        }
    }
    memcpy( &key, &keyTemp, sizeof(key) );
    getPos(); // refresh key position
    return;
}

void keyFlip(char mode) {
    int i = rand () % 5;
    char tmp;
    for(int m=0; m<2; m++){
        if (mode == 'r') {
            tmp = key[i][m];
            key[i][m] = key[i][4-m];
            key[i][4-m] = tmp;
        } else if (mode == 'c') {
            tmp = key[m][i];
            key[m][i] = key[4-m][i];
            key[4-m][i] = tmp;
        } else {
            cout << "keyFlip mode undefined" << endl;
            cin.ignore();
        }
    }
    getPos(); // refresh key position
    return;
}

int main(){
    string plainTXT, cipherTXT;    
    srand (time(NULL));
    char cipherFile[] = "playfair.txt";
    char trigramFile[] = "countSorted.txt";
    float Temperature,diff;
    int better, x;
    ofstream myResult("ResultPF.txt");
    
    // read cipher text from file
    if (readText(cipherFile, cipherTXT) == -1)
        return -1;
    
    // read trigram statistics from file in to array (percentage of trigram)
    if (readStat(trigramFile) == -1)
        return -1;
    
    randomKey();
    Temperature = 100;
    getPos();
    plainTXT = decryptText(cipherTXT);
    score = getScore(plainTXT);
    copyKRC('n');
    //printKey();

    while(1){        
        copyKRC('r');
        plainTXT = decryptText(cipherTXT);
        score = getScore(plainTXT);
        cout << plainTXT << endl << "Score = " << 40000+score << ";  Temperature = " << Temperature;
        printKey('l');
        cout << endl << endl;
        
        better=0;
        
        for (int i=0; i<50000; i++) {
            x = rand () % 1000;
            if (x < 5)
                keyTurn('t');
            else if (x < 10)
                keyTurn('c');
            else if (x < 15)
                keyTurn('r');
            else if (x < 20)
                keyFlip('c');
            else if (x < 30)
                keyFlip('r');
            else if (x < 230)
                keySwap('r');
            else if (x < 430)
                keySwap('c');
            else if (x < 630)
                keySwap('s');
            else if (x >= 900)
                keySwap('d');
            else
                keySwap('s');
            
            plainTXT = decryptText(cipherTXT);
            score = getScore(plainTXT);
            diff = score - score_best;
            
            if (diff>0)
                better++;
            
            if ( diff>0 || ((float)rand()/RAND_MAX < exp(diff/Temperature)) )
                copyKRC('n');
            else
                copyKRC('r');
            
            if (better > 100)
                break;
        }
        
        Temperature -= 1;
        
        if (better == 0) {
            copyKRC('r');
            plainTXT = decryptText(cipherTXT);
            score = getScore(plainTXT);
            if (myResult.is_open()) {
                myResult << plainTXT << endl << "Score = " << 40000+score << ";  Temperature = " << Temperature << endl;
                for (int i=0; i<5; i++) {
                    for (int j=0; j<5; j++) {
                        myResult << key[i][j];
                    }
                    myResult << endl;
                }
                myResult << endl;
                myResult.close();
            } else {
                cout << "Unable to open file";
                cin.ignore();
            }
            
            break;
        }
    }
    
    return 0;
} 
