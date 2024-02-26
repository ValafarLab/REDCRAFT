#ifndef REDCRAFT_ANGLES_H
#define REDCRAFT_ANGLES_H

#include <string>
#include <unordered_map>
#include <vector>

enum RamSpace {
    RAM1,
    RAM2,
    RAMG,
    RAM_ALL
};

struct anglePair {
    double first;
    double second;
};

struct aminoInfo {
    std::string AAType;
    double J;
};

class Angles {

private:
    int m_NumOfMedia = 2;
    bool m_CustomRamspace = false;

    std::string m_RDCPrefix = "nefRDC";
    RamSpace m_RamSpace = RAM1;
    RamSpace m_RamSpaceGLY = RAMG;

    static bool FilterJ(double phi, double J, double error, bool Gly);

    double cutoff = 0;
    bool skip = false;

public:
    Angles();
    void generateSpaces();

    void readAAFromFile(const std::string& file_path, bool verbose = false);
    
    void setCustomRamspace(bool customRamspace);
    bool isCustomRamspace() const;

    RamSpace getRamSpace() const;
    void setRamSpace(RamSpace ramSpace);

    RamSpace getRamSpaceGly() const;
    void setRamSpaceGly(RamSpace ramSpaceGly);

    // Getters and setters
    int getNumOfMedia() const;

    void setNumOfMedia(int numOfMedia);

    const std::string &getRdcPrefix() const;

    void setRdcPrefix(const std::string &rdcPrefix);

    double getCutoff() const;

    void setCutoff(double cutoff);

    bool isSkip() const;

    void setSkip(bool skip);

    void printAminoAcidSequence();

    void printRAMSpacePairs();

    std::vector<aminoInfo> AASequence;
    std::vector<anglePair> ramSpacePairs;
    std::vector<anglePair> ramSpacePairsGLY;
    std::unordered_map<int, std::vector<anglePair>> ramSpaceCustom;

    void generateAngles(std::vector<std::vector<anglePair>> &retAngles, std::vector<std::vector<double>> &retRMSDs,
                        bool verbose = false);

    static void
    writeAngles(const std::vector<std::vector<anglePair>> &angleList, const std::vector<std::vector<double>> &rmsdList,
                bool verbose = false);

    void readRAMFromFiles();
};


#endif //REDCRAFT_ANGLES_H
