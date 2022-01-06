#include "fstream"
#include "list"
#include "Block.h"
#include <iostream>
#include <cmath>
using namespace std;

class RecordsFile {
    int NonEmpty;
    int Empty;
    list<Block> blocks;
    int maxBlocks;
    int blockSize;

public:
    //required function helpers
    int getKey(int iBlock, int iRecord){
        if(iRecord > blockSize || iRecord < 0){
            cout << "iRecord out of range" << endl;
            return -1;
        }
        else if(iBlock > maxBlocks || iBlock < 0){
            cout << "iBlock out of range" << endl;
            return -1;
        }
        else{
            list<Block> search = blocks;
            list<Block::data> result;
            Block searchB;
            while (iBlock - 1 > 0){
                search.pop_front();
                iBlock--;
            }
            searchB = search.front();
            result = searchB.getData();
            while(iRecord - 1 > 0){

                result.pop_front();
                iRecord--;
            }
            if(!result.front().state){
                return -1;
            }
            else{
                return result.front().iKey;
            }
        }
    }
    int getVal(int iBlock, int iRecord){
        if(iRecord > blockSize || iRecord < 0){
            cout << "iRecord out of range" << endl;
            return -1;
        }
        else if(iBlock > maxBlocks || iBlock < 0){
            cout << "iBlock out of range" << endl;
            return -1;
        }
        else{

            list<Block> search = blocks;
            list<Block::data> result;
            Block searchB;
            while (iBlock - 1 > 0){
                search.pop_front();
                iBlock--;
            }
            searchB = search.front();
            result = searchB.getData();
            while(!result.front().state){
                result.pop_front();

            }
            while(iRecord - 1 > 0){
                result.pop_front();
                iRecord--;
            }
            if(!result.front().state){
                return -1;
            }
            else{
                return result.front().iVal;
            }
        }
    }
    int insert(int iKey,int iVal){
        int preNext;
        int index = -1,count,Ncount = 0,max;
        list<Block> search = blocks, updatedB;
        list<Block::data> test, updatedD;
        Block temp;
        int m = maxBlocks;
        int i;
        while(m > 0){
            Ncount++;
            count = -1;
            updatedD.clear();
            i = blockSize;
            test = search.front().getData();
            max = search.front().getMax();
            preNext = search.front().getNext();
            while(i > 0){
                count++;
                if(!test.front().state && index == -1){
                    test.front().iVal = iVal;
                    test.front().iKey = iKey;
                    test.front().state = true;
                    index = count;
                    if(updatedB.size() > 0){
                        if(updatedB.back().getNext() != Ncount) preNext = updatedB.back().getNext();
                        updatedB.back().setNext(Ncount);
                    }
                    if(iKey > max) max = iKey;
                }
                updatedD.push_back(test.front());
                test.pop_front();
                i--;
            }
            temp.setData(updatedD,max,preNext);
            temp.sort();
            //cout << "*";

            updatedB.push_back(temp);
            search.pop_front();
            m--;
        }
        blocks = updatedB;
        calcEmpties();

        return index;
    }
    int getBlockIndex(int iToken){
        list<Block> search = blocks;
        int count = 0;
        while(!search.empty()){
            count++;
            if(search.front().getMax() >= iToken){
                list<Block::data> searchD = search.front().getData();
                while(!searchD.empty()){
                    if(searchD.front().iKey == iToken && searchD.front().state) return count;
                    searchD.pop_front();
                }
            }
            search.pop_front();
        }
        return -1;
    }
    int getRecordIndex(int iToken){
        list<Block> search = blocks;

        while(!search.empty()){
            if(search.front().getMax() >= iToken){
                list<Block::data> searchD = search.front().getData();
                int count = 0;
                while(!searchD.empty()){
                    count++;
                    if(searchD.front().iKey == iToken && searchD.front().state) return count;
                    searchD.pop_front();
                }
            }
            search.pop_front();
        }
        return -1;
    }
    void deleteKey(int iToken){
        list<Block> searchDel = blocks;
        list<Block> searchMerge = blocks;
        list<Block> result;
        int search = getBlockIndex(iToken);
        if(search != -1){
            int block = search - 1;
            int preBlock = block - 1;

            while(block > 0){
                searchDel.pop_front();
                block--;
            }

            bool state;
            int tempK,tempV;
            searchDel.front().deleteRecord(iToken);
            if(search > 1){
                while(preBlock > 0){
                    result.push_back(searchMerge.front());
                    searchMerge.pop_front();
                    preBlock--;
                }
                if(searchDel.front().getValuesAmount() < ceil(blockSize/2.0) && searchMerge.front().getValuesAmount() < blockSize){
                    while(searchMerge.front().getValuesAmount() < blockSize){
                        tempK = searchDel.front().getData().front().iKey;
                        tempV = searchDel.front().getData().front().iVal;
                        state = searchMerge.front().insert(tempK,tempV);
                        if(state){
                            searchDel.front().deleteRecord(tempK);
                            if(searchDel.front().getValuesAmount() == 0){
                                searchMerge.front().setNext(searchDel.front().getNext());
                                searchDel.front().setNext(-1);
                                break;
                            }
                        }
                    }
                }
                searchMerge.front().sort();
                result.push_back(searchMerge.front());
            }
            while(!searchDel.empty()){
                result.push_back(searchDel.front());
                searchDel.pop_front();
            }



            blocks = result;
            calcEmpties();
        }
    }

    /////////////////////////////////////////////////////////////

    void calcEmpties(){
        list<Block> search= blocks;
        int count;
        Empty = -1;
        count = 0;
        for (int i = 0; i < maxBlocks; i++) {
            count++;
            if(search.front().getMax() == -1){
                Empty = count;
                break;
            }
            search.pop_front();
        }
        search= blocks;
        count = 0;
        NonEmpty = -1;
        for (int i = 0; i < maxBlocks; i++) {
            count++;
            if(search.front().getMax() != -1){
                NonEmpty = count;
                break;
            }
            search.pop_front();
        }
    }
    void setMax(int max){
        maxBlocks = max;
    }
    void setBlockSize(int size){
        blockSize = size;
    }
    void setEmpty(int emp){
        Empty = emp;
    }
    void setNonEmpty(int emp){
        NonEmpty = emp;
    }
    list<Block> getAllBlocks(){
        return blocks;
    }
    void clear(){
        blocks.clear();
        NonEmpty = 0;
        Empty = 0;
        maxBlocks = 0;
        blockSize = 0;
    }
    bool addBlock(Block t){
        if(maxBlocks > blocks.size()) {
            blocks.push_back(t);
            return true;
        }
        return false;
    }
    int getNonEmpty() {
        return NonEmpty;
    }
    int getEmpty() {
        return Empty;
    }
    int getMaxBlocks() {
        return maxBlocks;
    }
    int getBlockSize() {
        return blockSize;
    }
};
