/*
    This file contains the header for the function required for the april 1st article.
    Functions that are going to be included:
    *Histogram
    *K-means
    *Bhattacharyya distance.
    It is based on openCV 2.4.5, base version on ubuntu 13.10
*/
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>       /* time */
#include <limits>       // std::numeric_limits
#include "opencv2/core/core_c.h"
#include "opencv2/core/core.hpp"
#include "opencv2/flann/miniflann.hpp"
#include "opencv2/imgproc/imgproc_c.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/video/video.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/ml/ml.hpp"
#include "opencv2/highgui/highgui_c.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "rapidxml-1.13/rapidxml.hpp"
#include "rapidxml-1.13/rapidxml_print.hpp"
#include "rapidxml-1.13/rapidxml_utils.hpp"

using namespace rapidxml;

using namespace std;

using namespace cv;

#ifndef __ARTICULO_H_INCLUDED__   // if x.h hasn't been included yet...
#define __ARTICULO_H_INCLUDED__   //   #define this so the compiler knows it has been included


class prisImage{
private:
    Mat image;///Contains all the files related to the image
    VideoCapture file;///File where is extracted the image. 
    Mat mask;///Mask matrix
    Mat hist;///Vector of arrays that contains the last histogram calculated.
public:
    prisImage();
    prisImage(const string filename);///Contructor
    ~prisImage();///Destructor: free the video query
    int loadVideo(string filename);
    int extractNextImage();/// Get next image
    int histogram(const string colorSystem, bool useMask);///Get histogram. Review .cpp for more information.
    int setMask(const string file);///NOT YET IMPLEMENTED
    Mat contoursToMask(vector<Point> contour, int type);///Transform contours into binary matrixes (masks)
    Mat getHist();///Returns the internal variable hist
    
};

double bhattacharyyaDistance(Mat& H1,Mat& H2){////Actual function distance
    if(!checkRange(H1)){
        cout<<"Shit1"<<endl;
    }
    if(!checkRange(H2)){
        cout<<"Shit2"<<endl;
    }
    return compareHist(H1, H2, CV_COMP_BHATTACHARYYA);
}

class blop{
private:
    Mat histo;///Histogram of the blop
    int index;
    int layer;///index of the blop. Could be useful later to identify blops
    double (*distanceFunc)(Mat&, Mat& ); ///Pointer to a function, the actual distance function.
    int frame;///Frame in which the blop is present
public:
    blop(Mat histo,double (*distanceFunc)(Mat&, Mat&)){///Constructor

        normalize(histo,this->histo, 0, 1,  NORM_L2, -1, Mat() );  
    
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
        return 0;
    }
    double distance(blop &other){///Calculate the distance from another blop histogram
        return (*distanceFunc)(this->histo,other.getHisto());
    }
    int addBlops(Mat &otherHisto){
        (this->histo)+= otherHisto;
        return 0;
    }
    int multiplyScalar(double Scalar){///Multiply by a scalar
        scaleAdd(this->histo,Scalar,Mat::zeros((this->histo).size(),(this->histo).type()),this->histo);
        return 0;
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
    int randomInitializeCentroids();///Initialize centroids with random value.
public:
    blopList();
    blopList(vector<blop> list, int k,int dimensions);///Constructor
    int kmeans(TERMINATION_T condition ,double limitCondition);///K-means function
    int addBlop(blop newBlop);///Add a new blop to the list.
    int calculateDimension();
    int setK(int newK);///Change the number of clusters
    blop getBlop(int i);///get blop on index i.
    vector<blop> getCentroids();///return the centroids stored.
    int getNewCentroids();
    int reserve();
};

class XMLVideo{
private:
    int imageWidth;
    int imageHeight;
    int imageLength;
    string xmlFilename;   
    map<int,string> layersNames;
    multimap<int,vector<Point> > contours;//This multimap contain each array of points for each contour, it is indexed by the frame number
    blopList blops;
    prisImage video;
public:
    XMLVideo(const char* xmlFilename);
    int kmeans(int k,TERMINATION_T condition ,double limitCondition);
    int printXML();
    int loadVideo(string filename);///This function loads
    blopList getBlopList();
    vector<blop> getCentroids();
};
#include "articulo.cpp"
#endif 