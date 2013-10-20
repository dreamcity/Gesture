#include "StdAfx.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/contrib/contrib.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

Mat input_image;
Mat input_image320;
Mat output_mask;
Mat output_image;
Mat tempImage1, tempImage2;
Mat BlurImage;
IplImage *src_image;
IplImage *dst_image;
void main()
{
    VideoCapture cam(0);
    if(!cam.isOpened())
        return;

    namedWindow("input image");
    namedWindow("output mask");
    namedWindow("output image");
    CvAdaptiveSkinDetector skin_detector(1, CvAdaptiveSkinDetector::MORPHING_METHOD_NONE);
    while(true) {
        cam >> input_image;
		////图像尺寸减半
		cv::resize(input_image, input_image320,cv::Size(320,240), 1, 1, CV_INTER_LINEAR);
		cv::medianBlur(input_image320, tempImage1, 3);
		//滤波
		int sigma = 3;
		int ksize = (sigma*3)|1;
		cv::GaussianBlur(tempImage1, tempImage2, cv::Size(ksize, ksize), sigma, sigma );
		tempImage2.copyTo(BlurImage);
		//convert mat to IplImage；
        src_image = &tempImage2.operator IplImage();
        if(dst_image == NULL)
            dst_image = cvCreateImage(cvSize(src_image->width, src_image->height), IPL_DEPTH_8U, 1);   
			//一定要先分配一个内存

        skin_detector.process(src_image, dst_image);
        output_mask = 255*Mat(dst_image);
        input_image320.copyTo(output_image, output_mask);

		  //  Mat dst_mask = 255*Mat(dst_img);  
    //data->BlurImage.copyTo(dst_mat,dst_mask); 


        imshow("input image", input_image320);
        imshow("output mask", output_mask);
        imshow("output image", output_image);
        output_image.setTo(0);
        if(27 == waitKey(30))
            return;
    }
    return;
}
