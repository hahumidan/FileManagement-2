#include <list>
using namespace std;

class Block {
public:
    struct data{
        int iKey;
        int iVal;
        bool state = false;
        bool operator<(const data& comp) const {
            return iKey < comp.iKey;
        }
        bool operator>(const data& comp) const {
            return iKey > comp.iKey;
        }
        bool operator==(const data& comp) const {
            return state == comp.state;
        }
    };
private:
    int max;
    int next;

    list<data> blockData;
public:
    void addRecord(int key,int val,bool state){
        data temp;
        if(key > max) max = key;
        temp.iKey = key;
        temp.iVal = val;
        temp.state = state;
        blockData.push_back(temp);
    }
    void deleteRecord(int iToken){
        list<data> search = blockData;
        bool cam = true;
        int preMax = -1;
        list<data> result;
        while(!search.empty()){
            if(search.front().iKey == iToken && search.front().state && cam){
                search.front().state = false;
                search.front().iKey = -1;
                search.front().iVal = -1;
                cam = false;
                if(max == iToken) max = preMax;
            }
            preMax = search.front().iKey;
            result.push_back(search.front());
            search.pop_front();
        }
        blockData = result;
        sort();
    }
    bool insert(int key,int val){
        list<data> temp = blockData;
        list<data> result;
        int answer = true;
        while(!temp.empty()){
            if(!temp.front().state && answer){
                temp.front().state = true;
                temp.front().iKey = key;
                temp.front().iVal = val;
                if(key > max) max = key;
                answer = false;
            }
            result.push_back(temp.front());
            temp.pop_front();
        }
        blockData = result;
        return !answer;
    }
    void setData(list<data> t, int m, int n){
        max = m;
        next = n;
        blockData = t;
    }
    void sort(){
        if(getValuesAmount() > 0){
            blockData.sort();
            while(!blockData.front().state){
                blockData.push_back(blockData.front());
                blockData.pop_front();
            }
        }
    }
    int getValuesAmount(){
        list<data> temp = blockData;
        int count = 0;
        while(!temp.empty()){
            if(temp.front().state){
                count++;
            }
            temp.pop_front();
        }
        return count;
    }
    list<data> getData(){
        return blockData;
    }
    void setMax(int m){
        max = m;
    }
    void setNext(int n){
        next = n;
    }
    int getMax(){
        return max;
    }
    int getNext(){
        return next;
    }
    void clear(){
        blockData.clear();
    }
};
