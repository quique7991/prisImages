/*
    This file contains the header for the function required for the april 1st article.
    Functions that are going to be included:
    *Histogram
    *K-means
    *Bhattacharyya distance.
*/
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>       /* time */
#include <limits>       // std::numeric_limits

using namespace std;

class prisImage{
private:
    Mat image;///Contains all the files related to the image
    VideoCapture file;///File where is extracted the image. 
    Mat mask;///Mask matrix
    vector<Mat> hist;///Vector of arrays that contains the last histogram calculated.
public:
    prisImage(const string filename);///Contructor
    ~prisImage();///Destructor: free the video query
    int extractNextImage();/// Get next image
    int histogram(const string colorSystem, bool useMask);///Get histogram. Review .cpp for more information.
    int setMask(const string file);///NOT YET IMPLEMENTED
    Mat contoursToMask(vector<Point> contour, int type);///Transform contours into binary matrixes (masks)
    
};

double bhattacharyyaDistance(Mat& H1,Mat& H2){////Actual function distance
    return compareHist(H1, H2, CV_COMP_BHATTACHARYYA);
}

class blop{
private:
    Mat histo;///Histogram of the blop
    int index;///index of the blop. Could be useful later to identify blops
    double (*distanceFunc)(Mat&, Mat& *) ///Pointer to a function, the actual distance function.
public:
    blop(Mat histo,double (*distanceFunc)(Mat&, Mat& *)){///Constructor
        this->histo = histo;    
        this->distanceFunc = distanceFunc;
    }
    int setIndex(int index){///Set the value of index
        this->index = index;
        return 0;
    }
    int getIndex(){/// Get the value of index
        return (this->index);
    }
    Mat& getHisto(){/// Return the histogram of the blop
        return histo; 
    }
    int setHisto(Mat newHisto){///Assign a different histogram
        this->histo = newHisto;
    }
    double distance(blop &other){///Calculta the distance from another blop histogram
        return (*distanceFunc)(this->histo,other.getHisto());
    }
};

/*
    This enum defines the type of conditions to finish the k-measn that can beused to call the kmeans(...) function.
*/
enum TERMINATION_T {NUM_ITERATIONS, DISTANCE};

class blopList{
private:
    vector<blop> list;///List with all the blops.
    vector<blop> centroids;///Centrois of the the k- clusters.
    vector<int> closestCentroidIndex;///Contain the index to closest centroid for each blop in the list.
    int k;/// Number of clusters to calculate
    int dimensions;
    int getClosestCentroid(int blopIndex);//Private function to determine the closest centroid index
public:
    blopList(vector<blop> list, int k,int dimensions);///Constructor
    int kmeans(TERMINATION_T condition ,int limitCondition);///K-means function
    int randomInitializeCentroids();///Initialize centroids with random value.
    int addBlop(blop newBlop);///Add a new blop to the list.
    int setK(int newK);///Change the number of clusters
    blop getBlop(index i);///get blop on index i.
    vector<blop> getCentroids();///return the centroids stored.
};

