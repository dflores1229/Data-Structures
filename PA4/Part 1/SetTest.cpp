#include <cstdlib>
#include <iostream>
#include <list>
#include <map>
#include <time.h>
#include <vector>
#include "Set.h"
using namespace std;
const string ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const unsigned int NUM_STR = 1000;
const unsigned int NUM_REM = 100;
const unsigned int STR_LEN = 10;

/**
 * Test the Set implementations
 */
int main() {
    // create Set objects
    ArrayListSet arr_set;
    HashTableSet ht_set;
    LinkedListSet linked_set;
    MultiwayTrieSet mwt_set;
    RedBlackTreeSet rbt_set;

    // create map for convenience in later tests
    map<string,Set*> sets = {
        {"ArrayListSet",    &arr_set},
        {"HashTableSet",    &ht_set},
        {"LinkedListSet",   &linked_set},
        {"MultiwayTrieSet", &mwt_set},
        {"RedBlackTreeSet", &rbt_set}
    };
    unordered_map<string,bool> success;
    for(auto pair : sets) {
        success[pair.first] = true;
    }

    // build list of random strings to add
    srand(time(NULL));
    unordered_set<string> str_ht;
    for(unsigned int i = 0; i < NUM_STR; ++i) {
        string curr;
        for(unsigned int j = 0; j < STR_LEN; ++j) {
            curr += ALPHABET[rand() % ALPHABET.size()];
        }
        str_ht.insert(curr);
    }
    list<string> str_ll;
    vector<string> str_vec;
    for(auto curr : str_ht) {
        str_ll.push_back(curr);
        str_vec.push_back(curr);
    }

    // add the words to each set
    for(auto pair : sets) {
        unsigned int count = 0;
        for(auto curr : str_ll) {
            if(pair.second->find(curr)) {
                success[pair.first] = false;
                break;
            }
            pair.second->insert(curr);
            pair.second->insert(curr); // duplicate insertion (shouldn't do anything)
            if(!(++count == pair.second->size() && pair.second->find(curr))) {
                success[pair.first] = false;
                break;
            }
        }
    }

    // remove non-existent word from each set and check size
    for(auto pair : sets) {
        if(!success[pair.first]) {
            continue;
        }
        pair.second->remove("niema");
        if(str_ll.size() != pair.second->size()) {
            success[pair.first] = false;
        }
    }


    // remove words from each set
    for(unsigned int count = str_ll.size()-1; count >= str_ll.size()-NUM_REM; --count) {
        auto curr = str_ll.back();
        str_ll.pop_back();
        str_vec.pop_back();
        str_ht.erase(curr);
        for(auto pair : sets) {
            if(!success[pair.first]) {
                continue;
            }
            pair.second->remove(curr);
            if(count != pair.second->size() || pair.second->find(curr)) {
                success[pair.first] = false;
            }
        }
    }

    // output results
    for(auto pair : success) {
        if(!success[pair.first]) {
            cout << pair.first << " failed" << endl;
        }
    }
    return 0;
}
