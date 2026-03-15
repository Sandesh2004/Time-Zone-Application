#ifndef FAVORITE_MANAGER_H
#define FAVORITE_MANAGER_H

#include <vector>
#include <string>

class FavoriteManager {

public:
    static std::vector<std::string> load();
    static void save(std::string city);
};

#endif