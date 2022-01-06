#include <iostream>
#include "RecordsFile.h"
#include <list>
#include <cstring>
#include <math.h>
using namespace std;

RecordsFile mainRec;

int charConverter(const char temp[]) {
    int res = 0;
    int size = strlen(temp) - 1;
    for (int i = size; i >= 0; i--)
    {
        switch (temp[i])
        {
            case '1':
                res += 1 * pow(10,size - i);
                break;
            case '2':
                res += 2 * pow(10, size - i);
                break;
            case '3':
                res += 3 * pow(10, size - i);
                break;
            case '4':
                res += 4 * pow(10, size - i);
                break;
            case '5':
                res += 5 * pow(10, size - i);
                break;
            case '6':
                res += 6 * pow(10, size - i);
                break;
            case '7':
                res += 7 * pow(10, size - i);
                break;
            case '8':
                res += 8 * pow(10, size - i);
                break;
            case '9':
                res += 9 * pow(10, size - i);
                break;
        }
    }
    return res;
}

void writeFile(string cIndexFile){
    fstream record;
    record.open(cIndexFile, ios::out);

    record << mainRec.getNonEmpty() << "|" << mainRec.getEmpty() << "|#|" ;
    int m = mainRec.getMaxBlocks();
    int n = mainRec.getBlockSize();
    list<Block> temp = mainRec.getAllBlocks();
    list<Block::data> t;

    int i;
    int cnt = 0;
    while(m > 0){

        i = n;
        record << temp.front().getNext() << "|" << temp.front().getMax() << "|";
        t = temp.front().getData();
        while(i > 0){
            if(t.front().state){
                record << t.front().iKey << "|";
                record << t.front().iVal << "|";
            }
            else{
                record << "*|*|";
            }
            t.pop_front();
            i--;
        }
        temp.pop_front();
        record <<"#|";
        m--;
    }
}
void readFile(string cIndexFile){
    mainRec.clear();
    int maxBlocks = -1;
    int recordsLength =-4;
    int Empty;
    int nonEmpty;
    Block t;
    fstream record;
    record.open(cIndexFile, ios::in);
    char temp;
    string tempS;
    int te,te2;
    while (record.peek() != EOF) {
        record >> temp;
        if(temp == '#') maxBlocks++;
    }
    mainRec.setMax(maxBlocks);
    record.clear();
    record.seekg(0);
    getline(record,tempS,'#');
    tempS = "*";
    while (tempS != "#" && record.peek() != EOF){
        getline(record,tempS,'|');
        recordsLength++;
    }
    recordsLength = recordsLength/2;
    mainRec.setBlockSize(recordsLength);

    record.clear();
    record.seekg(0);
    getline(record,tempS,'|');
    if(tempS[0] == '-'){
        tempS = tempS.substr(1, strlen(tempS.c_str()));
        nonEmpty = -1 * charConverter(tempS.c_str());
    }
    else
        nonEmpty = charConverter(tempS.c_str());

    mainRec.setNonEmpty(nonEmpty);

    getline(record,tempS,'|');
    if(tempS[0] == '-'){
        tempS = tempS.substr(1, strlen(tempS.c_str()));
        Empty = -1 * charConverter(tempS.c_str());
    }
    else
        Empty = charConverter(tempS.c_str());
    mainRec.setEmpty(Empty);



    int m = maxBlocks;
    int n = recordsLength;
    int i;
    while(m > 0){
        t.clear();
        i = n;
        getline(record,tempS,'|');
        getline(record,tempS,'|');
        if(tempS[0] == '-'){
            tempS = tempS.substr(1, strlen(tempS.c_str()));
            t.setNext(-1 * charConverter(tempS.c_str()));
        }
        else
            t.setNext(charConverter(tempS.c_str()));

        getline(record,tempS,'|');
        if(tempS[0] == '-'){
            tempS = tempS.substr(1, strlen(tempS.c_str()));
            t.setMax(-1 * charConverter(tempS.c_str()));
        }
        else
            t.setMax(charConverter(tempS.c_str()));

        while(i > 0){
            getline(record,tempS,'|');
            if(tempS == "*"){
                t.addRecord(-1,-1,false);
                getline(record,tempS,'|');
            }
            else{
                te = charConverter(tempS.c_str());
                getline(record,tempS,'|');
                te2 = charConverter(tempS.c_str());
                t.addRecord(te,te2, true);
            }
            i--;
        }
        mainRec.addBlock(t);
        m--;
    }
}

//Required Functions
bool CreateRecordFile(string cIndexFile, int m, int n){
    ifstream f(cIndexFile);
    if(!f.good()){
        int i;
        fstream record;
        record.open(cIndexFile, ios::out);
        record << "-1|1|";
        while(m > 0){
            i = n;
            record << "#|-1|-1|";
            while(i > 0){
                record << "*|*|";
                i--;
            }
            m--;
        }
        record << "#";
        return true;
    }
    else{
        cout << "already exist" << endl;
        return false;
    }
};

int InsertVal(string cIndexFile, int iToken, int iKey){
    readFile(cIndexFile);
    mainRec.insert(iKey,iToken);
    writeFile(cIndexFile);
};

int GetKey(string cIndexFile, int iBlock, int iRecord){
    readFile(cIndexFile);
    return mainRec.getKey(iBlock,iRecord);
};

int GetVal(string cIndexFile, int iBlock, int iRecord){
    readFile(cIndexFile);
    return  mainRec.getVal(iBlock,iRecord);
};

int GetBlockIndex (string cIndexFile, int iToken){
    readFile(cIndexFile);
    return mainRec.getBlockIndex(iToken);
}

int GetRecordIndex (string cIndexFile, int iToken){
    readFile(cIndexFile);
    mainRec.getRecordIndex(iToken);
}

void DeleteKey (string cIndexFile, int iToken){
    readFile(cIndexFile);
    mainRec.deleteKey(iToken);
    writeFile(cIndexFile);
}


int FirstEmptyBlock(string cIndexFile){
    readFile(cIndexFile);
    return mainRec.getEmpty();
}

int main() {
    //CreateRecordFile("Test.txt",2,3);
    //InsertVal("Test.txt", 99,10);
    //cout << GetKey("Test.txt",1,2) << endl;
    //cout << GetVal("Test.txt",1,2) << endl;
    //cout << GetBlockIndex("Test.txt",7) << endl;
    //cout << GetRecordIndex("Test.txt",7) << endl;
    //DeleteKey("Test.txt",10);
    //cout << FirstEmptyBlock("Test.txt") << endl;
}