#include <gtest/gtest.h>
#include <angles.h>
#include <matrix.h>
#include <vector>
#include "redcraft.h"

Angles *angles;
TEST(StageOne, ReadResiduesFromFile) {
    angles = new Angles();
    angles->readAAFromFile("nefRDC.1");
    angles->printAminoAcidSequence();
    // Fail if no angles extracted from file
    EXPECT_GT(angles->AASequence.size(), 0);
}

TEST(StageOne, GenerateRAMSpace) {
    angles->setRamSpace(RAM1);
    angles->generateSpaces();
    angles->printRAMSpacePairs();
    if (angles->ramSpacePairs.empty()) {
        FAIL() << "Normal RAM Space pairs not initialized";
    }
    if (angles->ramSpacePairsGLY.empty()) {
        FAIL() << "Glycine RAM Space pairs not initialized";
    }
    EXPECT_EQ(angles->ramSpacePairs[0].first, -170);
    EXPECT_EQ(angles->ramSpacePairsGLY[0].first, -180);
}

std::vector<std::vector<anglePair>> angleList;
std::vector<std::vector<double>> rmsdList;

TEST(StageOne, GenerateAngles) {
    angles->generateAngles(angleList, rmsdList, true);
    EXPECT_EQ(angleList.size(), rmsdList.size());
    EXPECT_EQ(angleList.size(), angles->AASequence.size());
    EXPECT_GT(angleList[0].size(), 0);
}

TEST(StageOne, WriteAngles) {
    Angles::writeAngles(angleList, rmsdList, true);
    std::ifstream infile;
    infile.open("1.angles", std::ifstream::in);
    if (!infile.is_open()) {
        FAIL() << "File was not properly written";
    }
    infile.close();
}

Redcraft* rc_default;
TEST(StageTwo, InitialState) {
    std::string test_path = std::string(TEST_PATH) + "sample_1a1z_1Hz/";
    std::cout << test_path;
    rc_default = new Redcraft(test_path + "default.conf");
    struct RunState _1a1z = {
            .mCurrentDepth = 100,
            .mCurrentResidue = 1,
            .mResidueName = "ASP ",
            .mDecimationEnabled = false,
            .mLJEnabled = true,
            .mOTEstimation = false
    };
    EXPECT_EQ(rc_default->get_run_state(), _1a1z);
}



// Test from https://developer.ibm.com/articles/gpu-programming-with-openmp/
TEST(GPU, MatrixMultiplication) {
    int N = 1000;
    int i = 0;
    int j = 0;
    int k = 0;

    int pA[N*N];
    int pB[N*N];
    int pC[N*N];

    for (int i = 0; i < N*N; i++) {
        pA[i] = (rand() % 99) + 10;
        pB[i] = (rand() % 99) + 10;
        pC[i] = (rand() % 99) + 10;
    }


#define pA(i, j) (pA[((i)*N) + (j)])
#define pB(i, j) (pB[((i)*N) + (j)])
#define pC(i, j) (pC[((i)*N) + (j)])

    double dtime = omp_get_wtime();

//#pragma omp target data map (to: pA[0:N*N], pB[0:N*N]) map (tofrom: pC[0:N*N])
//#pragma omp target
//#pragma omp teams distribute parallel for collapse(2) private(i, j, k)
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            for (k = 0; k < N; k++) {
                pC(i, j) += pA(i, k) * pB(k, j);
            }
        }
    }
    std::cout << "Time taken in parallel, with GPU offloading: " << omp_get_wtime() - dtime << std::endl;

//    for (i = 0; i < N; i++) {
//        cout << "| ";
//        for (j = 0; j < N; j++) {
//            cout << pC(i, j) << " ";
//        }
//        cout << "|" << endl;
//    }

//    int pA[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
//    int pB[9] = {2, 3, 9, 4, 5, 8, 9, 2, 9};
//    int pC[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
}

TEST(Matrix, InvertOpenMP) {
//    auto test_matrix = generate_random_double_matrix(5);
//    generate_inverse_parallel(test_matrix, 8);
    Matrix* test = new Matrix(5, 5, 2);
    invertSquareMatrix(test);
}

//TEST(Curl, MakeRequest) {
//    CURL *curl;
//    FILE *fp;
//    CURLcode res;
//    curl_global_init(CURL_GLOBAL_ALL);
//    curl = curl_easy_init();
//    if (curl) {
//        curl_easy_setopt(curl, CURLOPT_URL, "https://ifestos.cse.sc.edu/v1/api/query/DAYAQWLKDGGPSSGRPPPS");
//        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "show=status&show=queryID");
//        res = curl_easy_perform(curl);
//        if (res != CURLE_OK) {
//            fprintf(stderr, "curl_easy_perform() failed: %s\n",
//                    curl_easy_strerror(res));
//        }
//
//        curl_easy_cleanup(curl);
//    }
//    curl_global_cleanup();
//
//}

// This will just prevent the test from leaking
TEST(Destructor, Destruct) {
    delete rc_default;
    delete angles;
    EXPECT_TRUE(true);
}