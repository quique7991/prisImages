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
*/
int prisImage::histogram(const string colorSystem, bool useMask){
	///RGB Histogram
	if(colorSystem.compare("RGB")==0){
		/// Separate the image in 3 places ( B, G and R )
		vector<Mat> bgr_planes;
		split( image, bgr_planes );

		/// Establish the number of bins
		int histSize = 256;

		/// Set the ranges ( for B,G,R) )
		float range[] = { 0, 256 } ;
		const float* histRange = { range };

		bool uniform = true; bool accumulate = false;

		Mat b_hist, g_hist, r_hist;

		/// Compute the histograms:
		if(useMask){///Use mask
			calcHist( &bgr_planes[0], 1, 0, mask, b_hist, 1, &histSize, &histRange, uniform, accumulate );
			calcHist( &bgr_planes[1], 1, 0, mask, g_hist, 1, &histSize, &histRange, uniform, accumulate );
			calcHist( &bgr_planes[2], 1, 0, mask, r_hist, 1, &histSize, &histRange, uniform, accumulate );
			hist.clear();
			hist.push_back(r_hist);
			hist.push_back(g_hist);
			hist.push_back(b_hist);
		}
		else{///Don't use mask
			calcHist( &bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate );
			calcHist( &bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate );
			calcHist( &bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate );
			hist.clear();
			hist.push_back(r_hist);
			hist.push_back(g_hist);
			hist.push_back(b_hist);
		}
	}
	else if(colorSystem.compare("HSV")==0){
		Mat hsv;
		cvtColor(image, hsv, CV_BGR2HSV);

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
			calcHist( &hsv, 1, channels, Mat(), // do not use mask
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
}

int prisImage::setMask(const string file){

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
    Rect roi = boundingRect(contour); // This is a OpenCV function

    // Create a mask for each contour to mask out that region from image.
    mask = Mat::zeros(image.size(), type);
    drawContours(mask, contours, 0, Scalar(255), CV_FILLED); // This is a OpenCV function filled with white

    // At this point, mask has value of 255 for pixels within the contour and value of 0 for those not in contour.
 	return  mask; 
}
