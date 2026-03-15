#include "Application.h"
#include "TimeConverter.h"
#include "FavoriteManager.h"

#include <iostream>
#include <iomanip>
#include <thread>
#include <chrono>
#include <conio.h>
#include <ctime>

using namespace std;

Application::Application()
    : db(TimeZoneDatabase::getInstance()), dstEnabled(false)
{
    favorites = FavoriteManager::load();
}

string Application::getCity(){

    string city;

    while(true){

        cout << "\nEnter city/country (or type list): ";
        getline(cin,city);

        if(city == "list"){
            db.listCities();
            continue;
        }

        if(!db.hasCity(city)){
            cout << "City/Country not found\n";
            continue;
        }

        return city;
    }
}

void Application::viewTime(){

    cout << "\n1 View time for a city\n";
    cout << "2 View time for favorite cities\n";

    int choice;
    cin >> choice;
    cin.ignore();

    if(choice == 1){

        string city = getCity();

        TimeZone z = db.getZone(city);

        tm utc = TimeConverter::getUTC();

        double off = z.offset;

        if(dstEnabled && z.hasDST)
            off += 1;

        tm local = TimeConverter::applyOffset(utc,off);

        TimeConverter::show(local,z,dstEnabled && z.hasDST);
    }

    else if(choice == 2){

        if(favorites.empty()){
            cout << "\nNo favorite cities added\n";
            return;
        }

        tm utc = TimeConverter::getUTC();

        for(auto &city : favorites){

            string key = city;

            for(char &c : key)
                c = tolower(static_cast<unsigned char>(c));

            if(!db.hasCity(key)){
                cout << "\nCity not found in database: " << city << "\n";
                continue;
            }

            TimeZone z = db.getZone(key);

            double off = z.offset;

            if(dstEnabled && z.hasDST)
                off += 1;

            tm local = TimeConverter::applyOffset(utc,off);

            TimeConverter::show(local,z,dstEnabled && z.hasDST);
        }
    }
}

void Application::convertTime(){

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

    string from = getCity();
    string to = getCity();

    TimeZone f = db.getZone(from);
    TimeZone t = db.getZone(to);

    tm timeInput = {};

    timeInput.tm_mday = day;
    timeInput.tm_mon = month - 1;
    timeInput.tm_year = year - 1900;
    timeInput.tm_hour = hour;
    timeInput.tm_min = minute;

#ifdef _WIN32
    time_t raw = _mkgmtime(&timeInput);
#else
    time_t raw = timegm(&timeInput);
#endif

    int fromOffset = f.offset * 3600;
    int toOffset = t.offset * 3600;

    if(dstEnabled && f.hasDST)
        fromOffset += 3600;

    if(dstEnabled && t.hasDST)
        toOffset += 3600;

    raw = raw - fromOffset;
    raw = raw + toOffset;

    tm* result = gmtime(&raw);

    cout << "\nConverted Date & Time: "
         << put_time(result,"%d-%m-%Y %H:%M")
         << " " << t.zoneName << "\n";
}

void Application::toggleDST(){

    cout<<"\n1 Enable DST\n2 Disable DST\n";

    int c;
    cin>>c;
    cin.ignore();

    dstEnabled = (c==1);
}

void Application::addFavorite(){

    string city = getCity();

    favorites.push_back(city);

    FavoriteManager::save(city);
}

void Application::showFavorites(){

    cout << "\nFavorites\n";

    for(int i=0;i<favorites.size();i++)
        cout << i+1 << ". " << favorites[i] << "\n";
}

void Application::searchCity(){

    string q;

    cout << "Search: ";
    getline(cin,q);

    auto r = db.search(q);

    if(r.empty()){
        cout << "City/Country not found\n";
    }
    else{

        cout << "\nFound Cities/Countries:\n";

        for(auto &c : r)
            cout << c << "\n";
    }
}

void Application::liveWorldClock(){

    vector<string> cities = {
        "india",
        "london",
        "new york",
        "tokyo",
        "sydney"
    };

    cout<<"\nLive World Clock (Press q to return to menu)\n";

    while(true){

        if(_kbhit()){
            char ch = _getch();
            if(ch=='q' || ch=='Q')
                break;
        }

        system("cls");

        cout<<"========= LIVE WORLD CLOCK =========\n\n";

        tm utcBase = TimeConverter::getUTC();

        for(string city : cities){

            TimeZone z = db.getZone(city);

            double off = z.offset;

            if(dstEnabled && z.hasDST)
                off += 1;

            tm local = TimeConverter::applyOffset(utcBase,off);

            cout << left << setw(12) << setfill(' ') << city
                 << put_time(&local,"%H:%M:%S")
                 << "   " << z.zoneName << "\n";
        }

        cout<<"\nPress Q to return to menu\n";

        this_thread::sleep_for(chrono::seconds(1));
    }
}

void Application::run(){

    while(true){

        cout<<"\n========== GLOBAL TIME ZONE ==========\n";

        cout<<"1 View Current Time\n";
        cout<<"2 Convert Time\n";
        cout<<"3 Toggle DST\n";
        cout<<"4 Add Favorite\n";
        cout<<"5 View Favorites\n";
        cout<<"6 Search City/Country\n";
        cout<<"7 Live World Clock\n";
        cout<<"8 Exit\n";

        int c;

        cout<<"Choice: ";
        cin>>c;
        cin.ignore();

        switch(c){

            case 1: viewTime(); break;
            case 2: convertTime(); break;
            case 3: toggleDST(); break;
            case 4: addFavorite(); break;
            case 5: showFavorites(); break;
            case 6: searchCity(); break;
            case 7: liveWorldClock(); break;
            case 8: return;
        }
    }
}