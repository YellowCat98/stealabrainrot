#pragma once
// an easier way to handle saving


#include <Geode/Geode.hpp>

class SaveManager {
public:


    using MapData = std::unordered_map<std::string, std::string>;

    using GamingComplexMap = std::unordered_map<
        std::string, MapData
    >;

    using ComplexMap = std::unordered_map<
        std::string, // level ID
        GamingComplexMap
    >;

    static SaveManager* get();


    void getCurrentSave();
    void commitChanges();
    void pushChanges(const std::string& levelID, const std::string& brainrotID, MapData map);
    GamingComplexMap brainrotInLevel(const std::string& levelID); // returns {} if there's no brainrots
    void removeChange(const std::string& levelID, const std::string& brainrotID);

    void getCollectedBrainrots();
    void pushCollectedChanges(const std::string& brainrotID, MapData data);
    void commitCollectedChanges();
    GamingComplexMap getAllCollectedBrainrots();
    MapData getCollectedBrainrot(const std::string& brainrotID);
    std::vector<std::string> getAllTokens(); // this function helps so that you dont get a token you already had, its a low chance but oh well

    GamingComplexMap collectedUncommitted;
    ComplexMap uncommitted;
private:
    bool getCurrentSaveCalled;
    bool getCollectedCalled;
};