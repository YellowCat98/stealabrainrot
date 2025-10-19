#pragma once
// an easier way to handle saving


#include <Geode/Geode.hpp>

class SaveManager {
public:

    using ComplexMap = std::unordered_map<
        std::string, std::vector<std::map<std::string, std::string>>
    >;

    using MapData = std::map<std::string, std::string>;

    static SaveManager* get();
    void getCurrentSave();
    void commitChanges();
    void pushChanges(std::string levelID, MapData map);
private:
    ComplexMap uncommitted;

    bool getCurrentSaveCalled;
};