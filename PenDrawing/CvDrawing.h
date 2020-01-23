//https://charlottehong.blogspot.com/2019/02/opencv-401-visual-studio-2019.html
#pragma once
#include <iostream>
#include <opencv2/opencv.hpp>
//#include "PenDrawing.h"
using namespace cv;
using namespace std;
//using namespace PenDrawing;
namespace CvDrawing {
	public ref class CvClass
	{
	public:
		void FillOil(unsigned char* ptr, const  int width, const  int height, const  int channel,
			const  int pointW, const  int pointH, const  int rate, unsigned char color_b, unsigned char color_g, unsigned char color_r)
		{
			unsigned char** fp = new unsigned char* [height];
			int Stride = width * channel, x = 0, y = 0, cv_x = 0;
			for (int j = 0; j < height; j++)
				fp[j] = ptr + (Stride * j);

			Mat img;
			img = Mat::zeros(height, width, CV_8UC3);
			for (y = 0; y < height; y++)
			{
				uchar* imgptr = img.ptr<uchar>(y);
				for (x = 0, cv_x = 0; x < Stride; x += channel, cv_x += 3)
				{
					imgptr[cv_x + 0] = fp[y][x];
					imgptr[cv_x + 1] = fp[y][x + 1];
					imgptr[cv_x + 2] = fp[y][x + 2];
				}
			}
			cv::Rect ccomp;
			//	cv::flip(img, img, -1);
			cv::floodFill(img,
				cv::Point(pointW, pointH),
				cv::Scalar(color_b, color_g, color_r), &ccomp, cv::Scalar(rate, rate, rate), cv::Scalar(rate, rate, rate));
			for (y = 0; y < height; y++)
			{
				uchar* imgptr = img.ptr<uchar>(y);
				for (x = 0, cv_x = 0; x < Stride; x += channel, cv_x += 3)
				{
					fp[y][x] = imgptr[cv_x + 0];
					fp[y][x + 1] = imgptr[cv_x + 1];
					fp[y][x + 2] = imgptr[cv_x + 2];
				}
			}
			img.release();
		}

	};
};