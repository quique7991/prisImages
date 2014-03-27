#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <string>
#include "rapidxml-1.13/rapidxml.hpp"
#include "rapidxml-1.13/rapidxml_print.hpp"
#include "rapidxml-1.13/rapidxml_utils.hpp"

using namespace rapidxml;
using namespace std;

int main(){
	file<> xmlFile("files/cut1_360.xml");
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
	///Root children layers
	xml_node<> *pLayers=pRoot->first_node("layers");///First children of video;	
	///Iterate over layers childresns
	for(xml_node<> * layer_node=pLayers->first_node("layer"); layer_node; layer_node=layer_node->next_sibling()){
		///Get the attrib ute
		cout<<"El ID de este layer es:";
		///Inside this node we only need to use the first_node, there is no multiple siblings, we can check the values of depth, name, description, url, color_green,color_red, color_blue
		xml_node<> *numberNode = layer_node->first_node("number");
		std::string strValue = numberNode->value();
		cout<< strValue <<endl;
	}
	///The same process is applied for the masks
	xml_node<> *masksNode=pRoot->first_node("masks");///There is only one child of this type
	///Iterate over masks childrens, called mask
	for(xml_node<> * mask_node=masksNode->first_node("mask");mask_node;mask_node=mask_node->next_sibling()){
		///Here we can get the values of the layer, frame, vertex count and polygon points. The last object has the atribute points
		xml_node<> *pointsNode = mask_node->first_node("polygon");
		string value = pointsNode->first_attribute("points")->value();
		cout<<value<<"HOLA"<<endl;
	}
	return 0;
}