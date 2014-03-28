/*
	Functions that are being used from the openCV library:
	void calcHist(const Mat* images, int nimages, const int* channels, 					  InputArray mask, SparseMat& hist, int dims, const 				  int* histSize, const float** ranges, bool 				  uniform=true, bool accumulate=false )
	Definitions of the parameters can be found: 
		http://docs.opencv.org/modules/imgproc/doc/histograms.html?highlight=histogram#void%20cvCalcBackProjectPatch%28IplImage**%20images,%20CvArr*%20dst,%20CvSize%20patch_size,%20CvHistogram*%20hist,%20int%20method,%20double%20factor%29
*/

#include "articulo.h"
/*
	Constructor
	Input parameters:
		filename: name of the file that contains the video being analized.
*/
///DUMB CONSTRUCTOR
prisImage::prisImage(){

}

/*
	Constructor
	Input parameters:
		filename: name of the file that contains the video being analized.
*/
prisImage::prisImage(const string filename){
	if(file.open(filename)){
		cout<<"File opened succesfuly"<<endl;	
	}
	else{
		cout<<"Error opening the file"<<endl;
	}
}

/*
	Destructor
	Release the video file
*/
prisImage::~prisImage(){
	if(file.isOpened()){
		file.release();
		cout<<"File released"<<endl;
	}
}
/*
	Function to open a video
	input: filename
*/
int prisImage::loadVideo(string filename){
	if(file.open(filename)){
		cout<<"File opened succesfuly"<<endl;	
	}
	else{
		cout<<"Error opening the file"<<endl;
	}	
	return 0;
}
/*
	Loads a new frame to the internal value image.
	returns 0 if correct decodification.
*/
int prisImage::extractNextImage(){
	///Load image
	file>>image;
	if( !image.data ){ 
		return -1; 
	}

	return 0;
}

/*
	Calculate the histogram to the actual image, with the actual mask.
	Inputs:
		colorSystem: define the color representation being used, for example "HSV", "RGB", etc.
		isMask: true defines if it uses the mask loaded to mask previously. 
	RGB:
		hist would contain the histogram for each channel (R,G,B) in that order.

	The number of bins is differnt for each ColorSystem
	In RGB is called histSize
	In HSV is called hbins and sbins
*/
int prisImage::histogram(const string colorSystem, bool useMask){
	///RGB Histogram
	if(colorSystem.compare("RGB")==0){
		/// Separate the image in 3 places ( B, G and R )

		/// Establish the number of bins
		int histSize = 256;
		int histBinSizes[] = {histSize, histSize, histSize};
		/// Set the ranges ( for B,G,R) )
		float range[] = { 0, 256 } ;
		const float* ranges[] = {range,range,range};
		bool uniform = true; 
		bool accumulate = false;
		int channels[] = {0, 1, 2};

		/// Compute the histograms:
		if(useMask){///Use mask
			calcHist( &image, 1, channels, mask, // do not use mask
				hist, 2, histBinSizes, ranges,
			    uniform, // the histogram is uniform
			    accumulate);
		}
		else{///Don't use mask
			calcHist( &image, 1, channels, Mat(), // do not use mask
			    hist, 2, histBinSizes, ranges,
			    uniform, // the histogram is uniform
			    accumulate);
		}
	}
	else if(colorSystem.compare("HSV")==0){
		Mat hsv;

		// Quantize the hue to 30 levels
		// and the saturation to 32 levels
		int hbins = 30, sbins = 32;
		int histSize[] = {hbins, sbins};
		// hue varies from 0 to 179, see cvtColor
		float hranges[] = { 0, 180 };
		// saturation varies from 0 (black-gray-white) to
		// 255 (pure spectrum color)
		float sranges[] = { 0, 256 };
		const float* ranges[] = { hranges, sranges };
		MatND hist;
		// we compute the histogram from the 0-th and 1-st channels
		int channels[] = {0, 1};
		if(useMask){
			calcHist( &hsv, 1, channels, mask, // do not use mask
				     hist, 2, histSize, ranges,
				     true, // the histogram is uniform
				     false );
		}
		else{
			calcHist( &hsv, 1, channels, Mat(), // do not use mask
				     hist, 2, histSize, ranges,
				     true, // the histogram is uniform
				     false );
		}
	}
	else{
		cout<<"Color Representation is not yet Implemented"<<endl;
	}	
	return 0;
}

int prisImage::setMask(const string file){
	return 0;
}

/*
	This function transforms a contour to a Mat, to use it later as a mask for the histogram, it stored in the private atribute mask and is return as value.
	Inputs:
		contour: the contour being transformed
		type: the image type for example CV_8UC1
*/

Mat prisImage::contoursToMask(vector<Point> contour, int type = CV_8UC1){
	////Create the vector of contours (in this case only one), push back the input
	vector< vector<Point> > contours;
	contours.push_back(contour);
    // Get bounding box for contour
    //Rect roi = boundingRect(contour); // This is a OpenCV function

    // Create a mask for each contour to mask out that region from image.
    mask = Mat::zeros(image.size(), type);
    drawContours(mask, contours, 0, Scalar(255), CV_FILLED); // This is a OpenCV function filled with white

    // At this point, mask has value of 255 for pixels within the contour and value of 0 for those not in contour.
 	return  mask; 
}

Mat prisImage::getHist(){
	return hist;
}
blopList::blopList(){

}
/*
	Constructor of the blopList
*/
blopList::blopList(vector<blop> list, int k, int dimensions){
	this->list = list;
	this->k = k;
	this->dimensions = dimensions;
	closestCentroidIndex.reserve(list.size());
}

/*
	Get the closes centroid to the blop at blopIndex
*/
int blopList::getClosestCentroid(int blopIndex){
	/*
		minValue: result of the current minimum distance calculation
		tempMin: result of the current distance calculation
		result: index of the current minimum distance centroid
	*/
	double minValue, tempMin;
	int result=0;
	///Initialize with the max double value
	minValue = numeric_limits<double>::max();
	///Iterates over all the centroids
	for(int i=0;i<k;++i){
		///Calculate distance
		tempMin = list[blopIndex].distance(centroids[i]);
		//cout<<tempMin<<endl; 
		if(tempMin < minValue){/// Update if smaller
			result = i;
			minValue = tempMin;
		}
	}
			
	return result;
}
int blopList::calculateDimension(){
	this->dimensions=list.size();
	return 0;
}
/*
	This function calculate the new value for the centroids
*/
int blopList::getNewCentroids(){
  vector<double> totalBlops;
  int position;
  double scale;
  totalBlops.reserve(k);
  centroids.clear();
  centroids.reserve(k);
  Mat firstBlopHisto = list[0].getHisto();
  ///Initialize each centroid with zeros.
  for(int i=0; i < k; ++i){
  	centroids.push_back(blop(Mat::zeros(firstBlopHisto.size(),firstBlopHisto.type()),&bhattacharyyaDistance));

  }

///Add each correspondant blop to the centroid
  for (int i = 0 ; i < (signed int) list.size(); ++i){
  	position = closestCentroidIndex[i];
  	totalBlops[position]+=1;
 	centroids[position].addBlops(list[i].getHisto());///Addblops  is equivalente to centroids[position]=centroids[position]+list[i].getHisto
  }

  //cout << "Matrix = "<< endl << " "  << (centroids[1].getHisto()) << endl << endl;
  ///Divide by N
  for(int i=0; i< k; ++i){
  	scale= 1/totalBlops[k];
  	centroids[i].multiplyScalar(scale);
  }
  return 0;
}
/*
	K-means clustering function
	Parameters
	*Condition: is one of the values of the enum TERMINATION_T {NUM_ITERATIONS, DISTANCE}, which defines the type of finish conditions used in the algorithm
	*LimitCondtion: depending on the value selected in the first parameter it can mean the number of iterations, of the minimum distance that the centroids have to move before the algorithm finishes.
 
*/
int blopList::kmeans(TERMINATION_T condition, double limitCondition){
	this->randomInitializeCentroids();
	if(condition == NUM_ITERATIONS){
		double numIterations = 0;
		while(numIterations < limitCondition){
			cout<<numIterations<<endl;
			for (int i=0 ; i < (signed int) list.size(); ++i){
  				closestCentroidIndex[i] = getClosestCentroid(i);
			}
			getNewCentroids();
			++numIterations;
		}
	}
	else if(condition == DISTANCE){
		double tempDistance = numeric_limits<int>::max(); 
		while(tempDistance > limitCondition){
			cout<<"Not yet implemented: Correction is needed :D"<<endl;
		}
	}
	else{
		cout<<"Wrong condition identifier"<<endl;
	}
	return 0;
}
/*
	Initialize each centroid with a random value from the actual blop list.
*/
int blopList::randomInitializeCentroids(){
	centroids.clear();
	int size = list.size();
	int randomNumber;
	/* initialize random seed: */
  	srand(time(NULL));

  	for(int i=0; i < k; ++i){
		/* generate secret number between 0 and size: */
  		randomNumber = rand() % size;
  		centroids.push_back(list[randomNumber]);
	}
	return 0;
}
/*
	Add the blop newBlop to the vector list.
*/
int blopList::addBlop(blop newBlop){
	list.push_back(newBlop);
	return 0;
}

/*
	Returns the blop at position i.
*/
blop blopList::getBlop(int i){
	return list[i];
}

/*
	This function returns the centroids calculated before of the k - clusters
*/
vector<blop> blopList::getCentroids(){
	return centroids;
}

int blopList::setK(int newK){
	this->k = newK;
	return 0;
}

int blopList::reserve(){
	closestCentroidIndex.reserve(list.size());
	return 0;
}

XMLVideo::XMLVideo(const char *xmlFilename){
	file<> xmlFile(xmlFilename);
	xml_document<> doc;
	doc.parse<0>(xmlFile.data());	
	///Parse root node
	xml_node<> *pRoot = doc.first_node(); ///In this case is scene
	///Root children video
	xml_node<> *pNode=pRoot->first_node("video");///First children of video;
	///Childrens of video
	xml_node<> *widthNode = pNode->first_node("width");
	xml_node<> *heightNode = pNode->first_node("height");
	xml_node<> *lengthNode = pNode->first_node("length");
	this->imageWidth = atoi((widthNode->value()));
	this->imageHeight = atoi((heightNode->value()));
	this->imageLength = atoi((lengthNode->value()));
	///Root children layers
	xml_node<> *pLayers=pRoot->first_node("layers");///First children of video;	
	///Iterate over layers childresns
	for(xml_node<> * layer_node=pLayers->first_node("layer"); layer_node; layer_node=layer_node->next_sibling()){
		///Get the attribute
		///Inside this node we only need to use the first_node, there is no multiple siblings, we can check the values of depth, name, description, url, color_green,color_red, color_blue
		xml_node<> *numberNode = layer_node->first_node("number");
		xml_node<> *nameNode = layer_node->first_node("name");
		int numberValue = atoi(numberNode->value());
		std::string name = nameNode->value();
		layersNames[numberValue] = name;
				
	}
	///The same process is applied for the masks
	xml_node<> *masksNode=pRoot->first_node("masks");///There is only one child of this type
	///Iterate over masks childrens, called mask
	for(xml_node<> * mask_node=masksNode->first_node("mask");mask_node;mask_node=mask_node->next_sibling()){
		///Here we can get the values of the layer, frame, vertex count and polygon points. The last object has the atribute points
		vector<Point> tempPoints;
		xml_node<> *pointsNode = mask_node->first_node("polygon");
		string maskPoints = pointsNode->first_attribute("points")->value();
		//xml_node<> *layerNode = mask_node->first_node("layer");
		//int layerNumber = atoi((layerNode->value()));
		xml_node<> *frameNode = mask_node->first_node("frame");
		int frameNumber = atoi((frameNode->value()));
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
		/*****
		TO DO
			FALTA ALMACENAR LA RELACION ENTRE LOS PUNTOS Y SU LAYER CORRESPONDIENTE PARA HACER LA VALIDACION
		****/
	}
}
int XMLVideo::kmeans(int k,TERMINATION_T condition ,double limitCondition){
	blops.setK(k);
	blops.reserve();
	blops.kmeans(condition,limitCondition);
	return 0;
}

blopList XMLVideo::getBlopList(){
	return blops;	
}

int XMLVideo::loadVideo(string filename){
	video.loadVideo(filename);
	int frameCounter = 0;
	std::multimap<int,vector<Point> >::iterator it,itlow,itup;
	while(video.extractNextImage()==0){
		itlow = contours.lower_bound (frameCounter);  // itlow points to b
  		itup = contours.upper_bound (frameCounter);   // itup points to e (not d)
		// print range [itlow,itup):
  		for (it=itlow; it!=itup; ++it){
  			video.contoursToMask((*it).second);///Calculate new mask
  			video.histogram("RGB",true);///Using RGB and masks
  			blop temp(video.getHist(),&bhattacharyyaDistance);
  			blops.addBlop(temp);
    	}
		++frameCounter;
	}
	blops.calculateDimension();
	return 0;
}

vector<blop> XMLVideo::getCentroids(){
	return blops.getCentroids();
}