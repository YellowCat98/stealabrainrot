#pragma once
// an easier way to handle saving


#include <Geode/Geode.hpp>

class SaveManager {
public:


    using MapData = std::map<std::string, std::string>;
    using ComplexMap = std::unordered_map<
        std::string, std::vector<MapData>
    >;

    static SaveManager* get();
    void getCurrentSave();
    void commitChanges();
    void pushChanges(std::string levelID, MapData map);
    ComplexMap uncommitted;
    std::vector<MapData> brainrotInLevel(std::string levelID); // returns {} if there's no brainrots
private:
    

    bool getCurrentSaveCalled;
};