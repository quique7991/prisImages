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

class prisImage{
private:
    Mat image;///Contains all the files related to the image
    VideoCapture file;///File where is extracted the image. 
    Mat mask;///Mask matrix
    vector<Mat> hist;///Vector of arrays that contains the last histogram calculated.
public:
    prisImage(const string filename);
    ~prisImage();
    int extractNextImage();
    int histogram(const string colorSystem, bool useMask);
    int setMask(const string file);///NOT YET IMPLEMENTED
    Mat contoursToMask(vector<Point> contour, int type);
    
};

double bhattacharyyaDistance(Mat& H1,Mat& H2){
    return compareHist(H1, H2, CV_COMP_BHATTACHARYYA);
}

class blop{
private:
    Mat histo;///Histogram of the blop
    int index;
    int name;
    double (*distanceFunc)(Mat&, Mat& *)
public:
    blop(Mat histo,double (*distanceFunc)(Mat&, Mat& *)){
        this->histo = histo;    
        this->distanceFunc = distanceFunc;
    }
    int setIndex(int index){
        this->index = index;
        return 0;
    }
    int getIndex(){
        return (this->index);
    }
    int setName(int name){
        this->name = name;
        return 0;
    }
    int getName(){
        return name;
    }
    Mat& getHisto(){
        return histo; 
    }
    int setHisto(Mat newHisto){
        this->histo = newHisto;
    }
    double distance(blop &other){
        return (*distanceFunc)(this->histo,other.getHisto());
    }
};

class blopList{
private:
    vector<blop> list;
    vector<blop> centroids;
public:
    blopList(vector<blop> list);
    
};

