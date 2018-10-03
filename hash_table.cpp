/*
 * =====================================================================================
 *
 *       Filename:  hash_table.cpp
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  2018年10月03日 23时42分16秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (),
 *   Organization:
 *
 * =====================================================================================
 */

#include <list>
#include <iostream>
#include <algorithm>
#include <string>
#include <google/profiler.h>
using namespace std;
extern "C" {
#include <ctype.h>
#include <string.h>
}

template <typename T>
class HashTable {
    private:
        std::list< std::pair<std::string, T> >* ht;
        static const int dict_hash_function_seed = 5381;
        int size;
    public:
        HashTable(int s)
            : size(s)
        {
            ht = new std::list< std::pair<std::string, T> >[size];
        }

        /* And a caseinsensitive hash function (based on djb hash) */
        //来源于Redis
        unsigned int dictGenCaseHashFunction(const std::string& key) {
            const char* buf = key.c_str();
            int len = key.length();
            unsigned int hash = (unsigned int)dict_hash_function_seed;

            while (len--)
                hash = ((hash << 5) + hash) +(tolower(*buf++)); /* hash * 33 + c */
            return hash % size;
        }

        bool hash(const std::string &key, const T& t) {
            auto slot = &ht[dictGenCaseHashFunction(key)];
            for(auto it = slot->begin(); it != slot->end(); it++) {
                if(key == it->first) {
                    it->second = t;
                    return true;
                }
            }
            slot->push_back(std::pair<std::string, T>(key, t));
            return true;
            //std::cout<<ht[slotPos].size()<<std::endl;
        }

        bool get(const std::string& key, T& t) {
            auto slot = &ht[dictGenCaseHashFunction(key)];
            auto it = slot->begin();
            for(it = slot->begin(); it != slot->end(); it++) {
                if(key == it->first) {
                    t = it->second;
                    std::cout<<t<<std::endl;
                    return true;
                }
            }
            return false;
        }

        bool remove(const std::string& key) {
            auto slot = &ht[dictGenCaseHashFunction(key)];
            auto it = slot->begin();
            for(it = slot->begin(); it != slot->end(); it++) {
                if(key == it->first) {
                    //std::cout<<it->second<<std::endl;
                    return true;
                }
            }
            return false;
        }

};

int main(int argc, const char** argv) {
    HashTable<int> hashTable(1000000);
    for(int i = 0; i < 10000000; i++) {
        if(i%2 == 0) {
            continue;
        }
        std::string key = std::to_string(i);
        hashTable.hash(key, i*10);
    }
    ProfilerStart("profiler");
    for(int i = 8999999; i < 10000000; i++) {
        std::string key = std::to_string(i);
        int a = 0;
        hashTable.get(key, a);
    }
    ProfilerStop();
    int a = 0;
    hashTable.get("127", a);
    hashTable.hash("127", 127);
    hashTable.get("127", a);
    return 0;
}
