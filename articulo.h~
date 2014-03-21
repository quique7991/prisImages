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

class prisImage
{
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
