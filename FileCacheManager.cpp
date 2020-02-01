#include <fstream>
#include <cstring>
#include <iostream>
#include "FileCacheManager.h"
#include "SplitClass.h"

FileCacheManager::FileCacheManager() {
    pthread_mutex_init(&mutex, nullptr);
    loadDataMap();
}

string FileCacheManager::getSolution(string problem) {
    pthread_mutex_lock(&mutex);
    string sol = data.find(problem)->second;
    pthread_mutex_unlock(&mutex);
    return sol;
}

bool FileCacheManager::hasSolution(string prob) {
    pthread_mutex_lock(&mutex);
    for (auto pair: data) {
        if(pair.first == prob){
            pthread_mutex_unlock(&mutex);
            return true;
        }
    }
    pthread_mutex_unlock(&mutex);
    return false;
}

void FileCacheManager::loadDataMap() {
    string buffer;
    string p;
    string s;
    bool inSol = false;
    ifstream fileSolutions;
    fileSolutions.open("solutions.txt", ifstream::in | istream::app);
    if (!fileSolutions) {
        throw "Failed opening file";
    }
    while (getline(fileSolutions, buffer)) {
        while (strcmp(buffer.c_str(), "$$")  != 0){
            if (strcmp(buffer.c_str(), "$")  == 0) {
                inSol = true;
            }
            else if (!inSol) {
                p += buffer;
            }
            else {
                s += buffer;
            }
            getline(fileSolutions, buffer);
        }
        pthread_mutex_lock(&mutex);
        data.insert(pair<string, string>(p, s));
        pthread_mutex_unlock(&mutex);
        inSol = false;
        p = "";
        s = "";
    }
    fileSolutions.close();
}

void FileCacheManager::updateData(string prob, string solution) {
    pthread_mutex_lock(&mutex);
    data.insert(pair<string, string>(prob, solution));
    pthread_mutex_unlock(&mutex);
}

void FileCacheManager::saveToDisk(string prob, string solution) {
    ofstream fileSolution;
    fileSolution.open("solutions.txt" , ofstream::out | ostream::app);
    if(!fileSolution){
        throw "failed opening file";
    }
    
    fileSolution<<prob << endl;
    fileSolution<<"$"<<endl;
    fileSolution << solution << endl;
    fileSolution << "$$" << endl;

    fileSolution.close();
}