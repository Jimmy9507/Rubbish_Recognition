#include "stdafx.h"
#include <iostream>
#include <fstream>
#include "Caffe_Net.h"
#include"Img_Processor.h"

using namespace std;

Mat Img_Processor::image_process(Mat src){
	Mat dst;
	
	// ifstream fin("F:/faster rcnn/caffe-master/windows/Project1/testImage.txt",std::ios_base::in);
	//ifstream fin("F:/faster rcnn/caffe-master/windows/Project1/faster-demo/testImage.txt", std::ios_base::in);
	//string lines;
	//while (getline(fin, lines)){
	//Mat img = imread(lines.c_str());
	
	if (src.empty()){
			cout << "input image error;"  << endl;
			exit(0);
		}
	//cvWaitKey(0);
	dst = this->net_.fasterrcnn(src);

	//cvWaitKey(0);
	
	return dst;
}
