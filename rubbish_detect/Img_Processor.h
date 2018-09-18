#include "Caffe_Net.h"


class Img_Processor{
private:
	//matlab model and net prototxt file .
	//const string rpn_prototxt_file = "F:/faster_rcnn/caffe-master/windows/rubbish_detect/rubbish_detect/models/proposal_test.prototxt";
	//const string rpn_model_file = "F:/faster_rcnn/caffe-master/windows/rubbish_detect/rubbish_detect/models/proposal_final";
	//const string fcnn_prototxt_file = "F:/faster_rcnn/caffe-master/windows/rubbish_detect/rubbish_detect/models/detection_test.prototxt";
	//const string fcnn_model_file = "F:/faster_rcnn/caffe-master/windows/rubbish_detect/rubbish_detect/models/detection_final";

	//const string rpn_prototxt_file = "../models/proposal_test.prototxt";
	//const string rpn_model_file = "../models/proposal_final";
	//const string fcnn_prototxt_file = "../models/detection_test.prototxt";
	//const string fcnn_model_file = "../models/detection_final";

	//mean_image.bmp is convert from the model.mat  in  matlab model directory 
	//you can save mean_image by command  imwrite(uint8(proposal_detection_model.image_means),"mean_image.bmp")
	//const string mean_image_file = "../models/mean_image.bmp";
	//const string mean_image_file = "F:/faster_rcnn/caffe-master/windows/rubbish_detect/rubbish_detect/models/mean_image.bmp";
	
	Caffe_Net net_;

public:
	Img_Processor()
	{
		//this->net_ = Caffe_Net(rpn_prototxt_file, rpn_model_file, fcnn_prototxt_file, fcnn_model_file, mean_image_file);


	}
	Mat image_process(Mat src);
	



};