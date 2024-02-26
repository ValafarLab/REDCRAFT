#ifndef REDCRAFT_DIAGNOSTICS_H
#define REDCRAFT_DIAGNOSTICS_H

struct RunState {
    int mCurrentDepth;
    int mCurrentResidue;
    std::string mResidueName;
    bool mDecimationEnabled;
    bool mLJEnabled;
    bool mOTEstimation;

    bool operator==(const RunState& rhs) const {
        return (
                this->mCurrentDepth == rhs.mCurrentDepth &&
                this->mCurrentResidue == rhs.mCurrentResidue &&
                this->mResidueName == rhs.mResidueName &&
                this->mDecimationEnabled == rhs.mDecimationEnabled &&
                this->mLJEnabled == rhs.mLJEnabled &&
                this->mOTEstimation == rhs.mOTEstimation
                );
    }
};

#endif //REDCRAFT_DIAGNOSTICS_H
