#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <string>
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
using namespace cv;
using namespace std;

int main(){
	file<> xmlFile("/home/quique/Desktop/files/cut1_360.xml");
	multimap<int,vector<Point> > contours;//This multimap contain each array of points for each contour, it is indexed by the frame number
	xml_document<> doc;
	doc.parse<0>(xmlFile.data());	
	///Parse root node
	xml_node<> *pRoot = doc.first_node(); ///In this case is scene
	///Root children video
	xml_node<> *pNode=pRoot->first_node("video");///First children of video;
	///Childrens of video
	xml_node<> *widthNode = pNode->first_node("width");
	xml_node<> *heightNode = pNode->first_node("height");
	//xml_node<> *lengthNode = pNode->first_node("length");
	int imageWidth = atoi((widthNode->value()));
	int imageHeight = atoi((heightNode->value()));
	//int imageLength = atoi((lengthNode->value()));
	///Root children layers
	//xml_node<> *pLayers=pRoot->first_node("layers");///First children of video;	
	///Iterate over layers childresns
	/*for(xml_node<> * layer_node=pLayers->first_node("layer"); layer_node; layer_node=layer_node->next_sibling()){
		///Get the attribute
		///Inside this node we only need to use the first_node, there is no multiple siblings, we can check the values of depth, name, description, url, color_green,color_red, color_blue
		xml_node<> *numberNode = layer_node->first_node("number");
		xml_node<> *nameNode = layer_node->first_node("name");
		int numberValue = atoi(numberNode->value());
		std::string name = nameNode->value();
				
	}*/
	///The same process is applied for the masks
	xml_node<> *masksNode=pRoot->first_node("masks");///There is only one child of this type
	///Iterate over masks childrens, called mask
	int frameCounter=0;
	for(xml_node<> * mask_node=masksNode->first_node("mask");mask_node;mask_node=mask_node->next_sibling()){
		///Here we can get the values of the layer, frame, vertex count and polygon points. The last object has the atribute points
		vector<Point> tempPoints;
		xml_node<> *pointsNode = mask_node->first_node("polygon");
		string maskPoints = pointsNode->first_attribute("points")->value();
		//xml_node<> *layerNode = mask_node->first_node("layer");
		//int layerNumber = atoi((layerNode->value()));
		xml_node<> *frameNode = mask_node->first_node("frame");
		int frameNumber = atoi((frameNode->value()));
		if(frameNumber > frameCounter){
			frameCounter = frameNumber;
		}
		std::istringstream ss(maskPoints);
		std::string token;
		std::string token2;
		while(std::getline(ss, token, ' ')) {///Each pair is separated by a whitespace
    		Point temp;
    		int counter = 0;
			std::istringstream ss2(token);
    		while(std::getline(ss2,token2, ',')){
    			if(counter == 0){
    				temp.x=atoi(token2.c_str());
    			}
    			else if(counter == 1){
    				temp.y=atoi(token2.c_str());
    			}
    			++counter;
    		}
    		counter = 0;
    		tempPoints.push_back(temp);
		}
		contours.insert(std::pair<int,vector<Point> >(frameNumber,tempPoints));
	}

	Size videoSize(imageWidth,imageHeight);
	VideoWriter binaryVideo("/home/quique/Desktop/binarias.mpeg", CV_FOURCC('P','I','M','1'), 25, videoSize, true);
	Mat tempBinaryMask, tempResult;
	std::multimap<int,vector<Point> >::iterator it,itlow,itup;
	if(binaryVideo.isOpened()){
		for(int i=0; i < frameCounter;++i){	
			tempResult = Mat::zeros(videoSize, CV_8UC3);
			itlow = contours.lower_bound (i);  // itlow points to b
				itup = contours.upper_bound (i);   // itup points to e (not d)
				for (it=itlow; it!=itup; ++it){
					vector< vector<Point> > tempVector;
					tempVector.push_back((*it).second);
					// Get bounding box for contour
					//Rect roi = boundingRect(contour); // This is a OpenCV function
					// Create a mask for each contour to mask out that region from image.
					tempBinaryMask = Mat::zeros(videoSize, CV_8UC3);
					drawContours(tempBinaryMask, tempVector, 0, Scalar(255), CV_FILLED); // This is a OpenCV function filled with white		
					tempResult += tempBinaryMask;
				}

			            
			binaryVideo << tempResult;
		}
		cout<<"Video Ready"<<endl;
	}
	else{
		cout<<"Video was not opened correctly"<<endl;
	}
	return 0;
}