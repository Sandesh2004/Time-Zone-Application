#ifndef APPLICATION_H
#define APPLICATION_H

#include <vector>
#include <string>
#include "TimeZoneDatabase.h"

class Application {

private:

    bool dstEnabled;
    TimeZoneDatabase& db;
    std::vector<std::string> favorites;

public:

    Application();

    void run();

private:

    std::string getCity();

    void viewTime();
    void convertTime();
    void toggleDST();
    void addFavorite();
    void showFavorites();
    void searchCity();
    void liveWorldClock();
};

#endif