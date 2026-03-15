#include "TimeZoneDatabase.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>

using namespace std;

TimeZoneDatabase::TimeZoneDatabase(){

    ifstream file("zones.txt");
    string line;

    while(getline(file,line)){

        stringstream ss(line);

        string key,city,zone,offset,dst,country;

        getline(ss,key,',');
        getline(ss,city,',');
        getline(ss,zone,',');
        getline(ss,offset,',');
        getline(ss,dst,',');
        getline(ss,country,',');

        for(char& c : key)
            c = tolower(static_cast<unsigned char>(c));

        zones[key] = TimeZone(key,city,zone,stod(offset),stoi(dst),country);
    }
}

TimeZoneDatabase& TimeZoneDatabase::getInstance(){

    static TimeZoneDatabase instance;
    return instance;
}

bool TimeZoneDatabase::hasCity(const string& city) const{

    return zones.find(city)!=zones.end();
}

TimeZone TimeZoneDatabase::getZone(const string& city) const{

    auto it=zones.find(city);

    if(it!=zones.end())
        return it->second;

    throw runtime_error("City not found");
}

void TimeZoneDatabase::listCities() const{

    cout<<"\nAvailable Cities/Counties\n";

    for(auto &z:zones)
        cout<<setw(15)<<setfill(' ')<<left<<z.first<<" "
            <<z.second.zoneName<<" "
            <<z.second.country<<"\n";
}

vector<string> TimeZoneDatabase::search(const string& q) const{

    vector<string> r;

    for(auto &z:zones)
        if(z.first.find(q)!=string::npos)
            r.push_back(z.first);

    return r;
}