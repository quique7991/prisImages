#include "articulo.h"

int main(){
	XMLVideo xml("files/cut1_360.xml");
	xml.loadVideo("files/cut1_360.mp4");
	xml.kmeans(2,NUM_ITERATIONS,20);
	//vector<blop> solution = xml.getCentroids();
	/*for (std::vector<blop>::iterator it = solution.begin() ; it != solution.end(); ++it){
		cout << "Matrix = "<< endl << " "  << ((*it).getHisto()) << endl << endl;
	}*/
	return 0;
}