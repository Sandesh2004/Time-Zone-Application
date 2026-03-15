#include "FavoriteManager.h"
#include <fstream>
#include <iostream>

using namespace std;

vector<string> FavoriteManager::load(){

    vector<string> f;

    ifstream file("favorites.txt");

    string city;

    while(getline(file,city))
        f.push_back(city);

    return f;
}

void FavoriteManager::save(string city){

    ofstream file("favorites.txt",ios::app);

    file<<city<<endl;
}