#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>
#include <ctime>
#include <iomanip>
#include <thread>
#include <chrono>
#include <cmath>
#include <conio.h>

using namespace std;

class TimeZone {
public:
    string key;
    string city;
    string zoneName;
    double offset;
    bool hasDST;
    string country;

    TimeZone() {}

    TimeZone(string k,string c,string z,double o,bool d,string co)
        :key(k),city(c),zoneName(z),offset(o),hasDST(d),country(co){}
};

class TimeZoneDatabase {

private:
    map<string,TimeZone> zones;

    TimeZoneDatabase() {

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
            
            for(char& c : key){
                c = tolower(static_cast<unsigned char>(c));
            }
            zones[key]=TimeZone(key,city,zone,stod(offset),stoi(dst),country);
        }
    }

public:

    static TimeZoneDatabase& getInstance(){

        static TimeZoneDatabase instance;
        return instance;
    }

    bool hasCity(const string& city) const{

        return zones.find(city)!=zones.end();
    }

    TimeZone getZone(const string& city) const{

        auto it=zones.find(city);

        if(it!=zones.end())
            return it->second;

        throw runtime_error("City not found");
    }

    void listCities() const{

        cout<<"\nAvailable Cities\n";

        for(auto &z:zones)
            cout<<setw(15)<<setfill(' ')<<left<<z.first<<" "
                <<z.second.zoneName<<" "
                <<z.second.country<<"\n";
    }

    vector<string> search(const string& q) const{

        vector<string> r;

        for(auto &z:zones)
            if(z.first.find(q)!=string::npos)
                r.push_back(z.first);

        return r;
    }
};

class TimeConverter {

public:

    static tm getUTC(){

        time_t now=time(0);

        tm* utc=gmtime(&now);

        return *utc;
    }

    static tm applyOffset(tm utc, double offsetHours)
    {
        time_t raw;

    #ifdef _WIN32
        raw = _mkgmtime(&utc);
    #else
        raw = timegm(&utc);
    #endif

        raw += static_cast<int>(round(offsetHours * 3600));

        tm* result = gmtime(&raw);

        return *result;
    }

    static void show(tm t,const TimeZone& z,bool dst){

        cout<<"\n====================================\n";

        cout<<"City: "<<z.city<<" ("<<z.country<<")\n";

        cout<<"Zone: "<<z.zoneName<<"\n";

        cout<<"Date: "
            <<setw(2)<<setfill('0')<<t.tm_mday<<"-"
            <<setw(2)<<setfill('0')<<(t.tm_mon + 1)<<"-"
            <<t.tm_year + 1900 << "\n";

        cout<<"Time: "
            <<setw(2)<<t.tm_hour<<":"
            <<setw(2)<<t.tm_min<<":"
            <<setw(2)<<t.tm_sec<<"\n";

        cout<<"DST: "<<(dst?"Applied":"Not Applied")<<"\n";

        cout<<"====================================\n";
    }
};

class FavoriteManager {

public:

    static vector<string> load(){

        vector<string> f;

        ifstream file("favorites.txt");

        string city;

        while(getline(file,city))
            f.push_back(city);

        return f;
    }

    static void save(string city){

        ofstream file("favorites.txt",ios::app);

        file<<city<<endl;
    }
};

class Application {

private:

    bool dstEnabled=false;

    TimeZoneDatabase& db;

    vector<string> favorites;

public:

    Application():db(TimeZoneDatabase::getInstance()){

        favorites=FavoriteManager::load();
    }

    string getCity(){

        string city;

        while(true){

            cout<<"\nEnter city (or type list): ";

            getline(cin,city);

            if(city=="list"){
                db.listCities();
                continue;
            }

            if(!db.hasCity(city)){
                cout<<"City not found\n";
                continue;
            }

            return city;
        }
    }

    void viewTime(){

        cout<<"\n1 View time for a city\n";
        cout<<"2 View time for favorite cities\n";

        int choice;
        cin>>choice;
        cin.ignore();

        if(choice == 1){

            string city = getCity();

            TimeZone z = db.getZone(city);

            tm utc = TimeConverter::getUTC();

            double off = z.offset;

            if(dstEnabled && z.hasDST)
                off += 1;

            tm local = TimeConverter::applyOffset(utc, off);

            TimeConverter::show(local, z, dstEnabled && z.hasDST);
        }

        else if(choice == 2){

            if(favorites.empty()){
                cout<<"\nNo favorite cities added\n";
                return;
            }

            tm utc = TimeConverter::getUTC();

            for(auto &city : favorites){

                string key = city;

                for(char &c : key)
                    c = tolower(static_cast<unsigned char>(c));

                if(!db.hasCity(key)){
                    cout<<"\nCity not found in database: "<<city<<"\n";
                    continue;
                }

                TimeZone z = db.getZone(key);

                double off = z.offset;

                if(dstEnabled && z.hasDST)
                    off += 1;

                tm local = TimeConverter::applyOffset(utc, off);

                TimeConverter::show(local, z, dstEnabled && z.hasDST);
            }
        }
    }

    void convertTime(){

        int day,month,year;
        int hour,minute;

        cout<<"Enter Day: ";
        cin>>day;

        cout<<"Enter Month: ";
        cin>>month;

        cout<<"Enter Year: ";
        cin>>year;

        cout<<"Enter Hour (0-23): ";
        cin>>hour;

        cout<<"Enter Minute (0-59): ";
        cin>>minute;

        cin.ignore();

        string from=getCity();
        string to=getCity();

        TimeZone f=db.getZone(from);
        TimeZone t=db.getZone(to);

        tm timeInput={0};

        timeInput.tm_mday=day;
        timeInput.tm_mon=month-1;
        timeInput.tm_year=year-1900;
        timeInput.tm_hour=hour;
        timeInput.tm_min=minute;
        timeInput.tm_sec=0;

    #ifdef _WIN32
        time_t raw=_mkgmtime(&timeInput);
    #else
        time_t raw=timegm(&timeInput);
    #endif

        int fromOffset=f.offset*3600;
        int toOffset=t.offset*3600;

        if(dstEnabled && f.hasDST)
            fromOffset+=3600;

        if(dstEnabled && t.hasDST)
            toOffset+=3600;

        raw=raw-fromOffset;
        raw=raw+toOffset;

        tm* result=gmtime(&raw);

        cout<<"\nConverted Date & Time: "
            <<setw(2)<<setfill('0')<<result->tm_mday<<"-"
            <<setw(2)<<result->tm_mon+1<<"-"
            <<result->tm_year+1900<<" "
            <<setw(2)<<result->tm_hour<<":"
            <<setw(2)<<result->tm_min
            <<" "<<t.zoneName<<"\n";
    }

    void toggleDST(){

        cout<<"\n1 Enable DST\n2 Disable DST\n";

        int c;
        cin>>c;
        cin.ignore();

        dstEnabled=(c==1);
    }

    void addFavorite(){

        string city=getCity();

        favorites.push_back(city);

        FavoriteManager::save(city);
    }

    void showFavorites(){

        cout<<"\nFavorites\n";

        for(int i=0;i<favorites.size();i++)
            cout<<i+1<<". "<<favorites[i]<<"\n";
    }

    void searchCity(){

        string q;

        cout<<"Search: ";
        getline(cin,q);

        auto r = db.search(q);

        if(r.empty()){
            cout<<"City not found\n";
        }
        else{
            cout<<"\nFound Cities:\n";

            for(auto &c : r)
                cout<<c<<"\n";
        }
    }

    void liveWorldClock(){

        vector<string> cities = {
            "india",
            "london",
            "new york",
            "tokyo",
            "sydney"
        };

        cout<<"\nLive World Clock (Press q to return to menu)\n";

        while(true){

            if(_kbhit()){          // check if key pressed
                char ch = _getch();
                if(ch == 'q' || ch == 'Q')
                    break;         // exit clock
            }

            system("cls");

            cout<<"========= LIVE WORLD CLOCK =========\n\n";

            tm utcBase = TimeConverter::getUTC();

            for(string city : cities){

                TimeZone z = db.getZone(city);

                double off = z.offset;

                if(dstEnabled && z.hasDST)
                    off += 1;

                tm local = TimeConverter::applyOffset(utcBase, off);

                cout << left << setw(12) << setfill(' ') << city
                    << put_time(&local, "%H:%M:%S")
                    << "   " << z.zoneName << "\n";
            }

            cout<<"\nPress Q to return to menu\n";

            this_thread::sleep_for(chrono::seconds(1));
        }
    }

    void run(){

        while(true){

            cout<<"\n========== GLOBAL TIME ZONE ==========\n";

            cout<<"1 View Current Time\n";
            cout<<"2 Convert Time\n";
            cout<<"3 Toggle DST\n";
            cout<<"4 Add Favorite\n";
            cout<<"5 View Favorites\n";
            cout<<"6 Search City\n";
            cout<<"7 Live World Clock\n";
            cout<<"8 Exit\n";

            int c;

            cout<<"Choice: ";

            cin>>c;
            cin.ignore();

            switch(c){

                case 1:viewTime();break;
                case 2:convertTime();break;
                case 3:toggleDST();break;
                case 4:addFavorite();break;
                case 5:showFavorites();break;
                case 6:searchCity();break;
                case 7:liveWorldClock(); break;
                case 8:return;
            }
        }
    }
};

int main(){

    Application app;

    app.run();
}