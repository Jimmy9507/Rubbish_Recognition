#ifndef CAFFE_REG_H
#define CAFFE_REG_H
#include "caffe/common.hpp"  
#include "caffe/layers/input_layer.hpp"  
namespace caffe
{

	extern INSTANTIATE_CLASS(InputLayer);
	REGISTER_LAYER_CLASS(Input);
}
#endif