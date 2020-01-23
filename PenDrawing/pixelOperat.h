#pragma once
#include <math.h> 
#define BGR2BGR 0
#define BGR2RGB 1
#define BGR2RBG 2
#define BGR2GRB 3
#define BGR2GBR 4
#define BGR2BRG 5
#include <thread>
#include <iostream>  
#include<cmath>
#include<algorithm>
#include <vector>
using namespace System;
using namespace std;
namespace pix {

	public ref class pixelOperate
	{
		const double PI = 3.141592653589793238463;
		// TODO: 請在此新增此類別的方法。
	public:
		void colorTo255(unsigned char* ptr, int width, int height, int channel)
		{
			unsigned char** fp = new unsigned char* [height];
			int Stride = width * channel, x = 0, y = 0;
			for (int j = 0; j < height; j++)
				fp[j] = ptr + (Stride * j);
			for (y = 0; y < height; y++)
			{
				for (x = 0; x < Stride; x += channel)
				{
					fp[y][x] = 255 - fp[y][x];
					fp[y][x + 1] = 255 - fp[y][x + 1];
					fp[y][x + 2] = 255 - fp[y][x + 2];
				}
			}
			delete[] fp;
		}
		void removeBackGround(unsigned char* ptr, const  int width, const  int height, const  int channel,
			const  int pointW, const  int pointH, const  int rate)
		{
			unsigned char** fp = new unsigned char* [height];
			int Stride = width * channel, x = 0, y = 0;
			for (int j = 0; j < height; j++)
				fp[j] = ptr + (Stride * j);
			vector<int> w2;
			vector<int> h2;
			w2.push_back(pointW);
			h2.push_back(pointH);
			int range = rate;
			int b = fp[pointH][pointW * 4];
			int g = fp[pointH][pointW * 4 + 1];
			int r = fp[pointH][pointW * 4 + 2];
			//fp[pointH][pointW * 4 + 3] ;
			BackToRero(fp, w2, h2, width, height, b, g, r, range);
			delete[] fp;
		}
		inline bool checkColorRange(int originColor, int color, int range) {
			if (originColor + range >= color && originColor - range <= color)
				return true;
			return false;
		}
		void BackToRero(unsigned char** fp, vector<int> w, vector<int> h, int width, int height, int b, int g, int r, int range) {
			if (w.size() < 1)return;
			//if (w.size() > 5000)return;
			vector<int> w2;
			vector<int> h2;
			for (int i = 0;i < w.size();i++)
			{
				if (w[i] < 0 || w[i] >= width || h[i] < 0 || h[i] >= height)
				{
					continue;
				}
				else {
					if (checkColorRange(fp[h[i]][w[i] * 4], b, range) && checkColorRange(fp[h[i]][w[i] * 4 + 1], g, range) && checkColorRange(fp[h[i]][w[i] * 4 + 2], r, range))
					{
						fp[h[i]][w[i] * 4] = 0;
						fp[h[i]][w[i] * 4 + 1] = 0;
						fp[h[i]][w[i] * 4 + 2] = 0;
						fp[h[i]][w[i] * 4 + 3] = 0;
					}
					if (h[i] - 1 > 0)
					{
						if (checkColorRange(fp[h[i] - 1][w[i] * 4], b, range) && checkColorRange(fp[h[i] - 1][w[i] * 4 + 1], g, range) && checkColorRange(fp[h[i] - 1][w[i] * 4 + 2], r, range)
							&& fp[h[i] - 1][w[i] * 4 + 3] != 0 && FindByVector(w2, h2, w[i], h[i] - 1) == false)
						{
							h2.push_back(h[i] - 1);w2.push_back(w[i]);
						}
					}
					if (w[i] + 1 < width)
					{
						if (checkColorRange(fp[h[i]][(w[i] + 1) * 4], b, range) && checkColorRange(fp[h[i]][(w[i] + 1) * 4 + 1], g, range) && checkColorRange(fp[h[i]][(w[i] + 1) * 4 + 2], r, range)
							&& fp[h[i]][(w[i] + 1) * 4 + 3] != 0 && FindByVector(w2, h2, w[i] + 1, h[i]) == false)
						{
							h2.push_back(h[i]);w2.push_back(w[i] + 1);
						}
					}
					if (w[i] - 1 > 0)
					{
						if (checkColorRange(fp[h[i]][(w[i] - 1) * 4], b, range) && checkColorRange(fp[h[i]][(w[i] - 1) * 4 + 1], g, range) && checkColorRange(fp[h[i]][(w[i] - 1) * 4 + 2], r, range)
							&& fp[h[i]][(w[i] - 1) * 4 + 3] != 0 && FindByVector(w2, h2, w[i] - 1, h[i]) == false)
						{
							h2.push_back(h[i]);w2.push_back(w[i] - 1);
						}
					}
					if (h[i] + 1 < height)
					{
						if (checkColorRange(fp[h[i] + 1][w[i] * 4], b, range) && checkColorRange(fp[h[i] + 1][w[i] * 4 + 1], g, range) && checkColorRange(fp[h[i] + 1][w[i] * 4 + 2], r, range)
							&& fp[h[i] + 1][w[i] * 4 + 3] != 0 && FindByVector(w2, h2, w[i], h[i] + 1) == false)
						{
							h2.push_back(h[i] + 1);w2.push_back(w[i]);
						}
					}
				}
			}
			w.clear();
			w.shrink_to_fit();
			h.clear();
			h.shrink_to_fit();
			BackToRero(fp, w2, h2, width, height, b, g, r, range);

		}
		void FillTest(unsigned char* ptr, int width, int height, int channel)
		{
			unsigned char** fp = new unsigned char* [height];
			int Stride = width * channel, x = 0, y = 0;
			for (int j = 0; j < height; j++)
				fp[j] = ptr + (Stride * j);
			vector<int> w2;
			vector<int> h2;
			w2.push_back(10);
			h2.push_back(10);
			FT(fp, w2, h2, width, height);
			delete[] fp;
		}
		bool FindByVector(vector<int> v, vector<int> v2, int n, int n2) {
			for (int i = 0;i < v.size();i++) {
				if (v[i] == n && v2[i] == n2)return true;
			}
			return false;
		}
		void FT(unsigned char** fp, vector<int> w, vector<int> h, int width, int height) {
			if (w.size() < 1)return;
			//if (w.size() > 5000)return;
			vector<int> w2;
			vector<int> h2;

			for (int i = 0;i < w.size();i++)
			{
				if (w[i] < 0 || w[i] >= width || h[i] < 0 || h[i] >= height)
				{
					continue;
				}
				else {
					fp[h[i]][w[i] * 4] = 0;
					fp[h[i]][w[i] * 4 + 1] = 0;
					fp[h[i]][w[i] * 4 + 2] = 0;
					fp[h[i]][w[i] * 4 + 3] = 0;
					if (h[i] - 1 > 0)
					{
						if (fp[h[i] - 1][w[i] * 4] != 0 && fp[h[i] - 1][w[i] * 4 + 1] != 0 && fp[h[i] - 1][w[i] * 4 + 2] != 0
							&& FindByVector(w2, h2, w[i], h[i] - 1) == false)
						{
							h2.push_back(h[i] - 1);w2.push_back(w[i]);
						}
					}
					if (w[i] + 1 < width)
					{
						if (fp[h[i]][(w[i] + 1) * 4] != 0 && fp[h[i]][(w[i] + 1) * 4 + 1] != 0 && fp[h[i]][(w[i] + 1) * 4 + 2] != 0
							&& FindByVector(w2, h2, w[i] + 1, h[i]) == false)
						{
							h2.push_back(h[i]);w2.push_back(w[i] + 1);
						}
					}
					if (w[i] - 1 > 0)
					{
						if (fp[h[i]][(w[i] - 1) * 4] != 0 && fp[h[i]][(w[i] - 1) * 4 + 1] != 0 && fp[h[i]][(w[i] - 1) * 4 + 2] != 0
							&& FindByVector(w2, h2, w[i] - 1, h[i]) == false)
						{
							h2.push_back(h[i]);w2.push_back(w[i] - 1);
						}
					}
					if (h[i] + 1 < height)
					{
						if (fp[h[i] + 1][w[i] * 4] != 0 && fp[h[i] + 1][w[i] * 4 + 1] != 0 && fp[h[i] + 1][w[i] * 4 + 2] != 0
							&& FindByVector(w2, h2, w[i], h[i] + 1) == false)
						{
							h2.push_back(h[i] + 1);w2.push_back(w[i]);
						}
					}
				}
			}
			w.clear();
			w.shrink_to_fit();
			h.clear();
			h.shrink_to_fit();
			FT(fp, w2, h2, width, height);
		}
		void Overexposed(unsigned char* ptr, int width, int height, int channel)
		{
			unsigned char** fp = new unsigned char* [height];
			int Stride = width * channel, x = 0, y = 0;
			for (int j = 0; j < height; j++)
				fp[j] = ptr + (Stride * j);
			for (y = 0; y < height; y++)
			{
				for (x = 0; x < Stride; x += channel)
				{
					fp[y][x] = abs(128 - abs(128 - fp[y][x]));
					fp[y][x + 1] = abs(128 - abs(128 - fp[y][x + 1]));
					fp[y][x + 2] = abs(128 - abs(128 - fp[y][x + 2]));
				}
			}
			delete[] fp;
		}
		void rgbNormal(unsigned char* ptr, int width, int height, int channel, int valueR, int valueG, int valueB)
		{
			unsigned char** fp = new unsigned char* [height];
			int Stride = width * channel, x = 0, y = 0;
			for (int j = 0; j < height; j++)
				fp[j] = ptr + (Stride * j);
			for (y = 0; y < height; y++)
			{
				for (x = 0; x < Stride; x += channel)
				{
					if (fp[y][x] + valueB >= 255)fp[y][x] = 255;
					else if (fp[y][x] + valueB < 0)fp[y][x] = 0;
					else fp[y][x] = fp[y][x] + valueB;

					if (fp[y][x + 1] + valueG >= 255)fp[y][x + 1] = 255;
					else if (fp[y][x + 1] + valueG < 0)fp[y][x + 1] = 0;
					else fp[y][x + 1] = fp[y][x + 1] + valueG;

					if (fp[y][x + 2] + valueR >= 255)fp[y][x + 2] = 255;
					else if (fp[y][x + 2] + valueR < 0)fp[y][x + 2] = 0;
					else fp[y][x + 2] = fp[y][x + 2] + valueR;
				}
			}
			delete[] fp;
		}
		void Binarization(unsigned char* ptr, int width, int height, int channel, int value)
		{
			const int value2 = value * 3;
			unsigned char** fp = new unsigned char* [height];
			int Stride = width * channel, x = 0, y = 0;
			for (int j = 0; j < height; j++)
				fp[j] = ptr + (Stride * j);
			for (y = 0; y < height; y++)
			{
				for (x = 0; x < Stride; x += channel)
				{
					if (fp[y][x] + fp[y][x + 1] + fp[y][x + 2] >= value2)
					{
						fp[y][x] = 255;
						fp[y][x + 1] = 255;
						fp[y][x + 2] = 255;
					}
					else
					{
						fp[y][x] = 0;
						fp[y][x + 1] = 0;
						fp[y][x + 2] = 0;
					}
				}
			}
			delete[] fp;
		}
		void  colorToGray(unsigned char* ptr, int width, int height, int channel)
		{
			unsigned char** fp = new unsigned char* [height];
			int Stride = width * channel, x = 0, y = 0;
			for (int j = 0; j < height; j++)
				fp[j] = ptr + (Stride * j);
			for (y = 0; y < height; y++)
			{
				for (x = 0; x < Stride; x += channel)
				{
					fp[y][x] = (0.299 * fp[y][x + 2] + 0.587 * fp[y][x + 1] + 0.114 * fp[y][x]);
					fp[y][x + 1] = fp[y][x];
					fp[y][x + 2] = fp[y][x];
				}
			}
			delete[] fp;
		}
		void horizontalFlip(unsigned char* ptr, unsigned char* ptr2, int width, int height, int channel)
		{
			unsigned char** fp = new unsigned char* [height];
			unsigned char** fp2 = new unsigned char* [height];
			int Stride = width * channel, x = 0, y = 0;
			for (int j = 0; j < height; j++)
				fp[j] = ptr + (Stride * j);
			for (int j = 0; j < height; j++)
				fp2[j] = ptr2 + (Stride * j);
			for (y = 0; y < height; y++)
			{
				for (x = 0; x < Stride; x += channel)
				{
					fp[y][x] = fp2[y][Stride - channel - x];
					fp[y][x + 1] = fp2[y][Stride - channel - x + 1];
					fp[y][x + 2] = fp2[y][Stride - channel - x + 2];
				}
			}
		}
		void AllFlip(unsigned char* ptr, unsigned char* ptr2, int width, int height, int channel)
		{
			unsigned char** fp = new unsigned char* [height];
			unsigned char** fp2 = new unsigned char* [height];
			int Stride = width * channel, x = 0, y = 0;
			for (int j = 0; j < height; j++)
				fp[j] = ptr + (Stride * j);
			for (int j = 0; j < height; j++)
				fp2[j] = ptr2 + (Stride * j);
			for (y = 0; y < height; y++)
			{
				for (x = 0; x < Stride; x += channel)
				{
					fp[y][x] = fp2[height - y - 1][Stride - x - channel];
					fp[y][x + 1] = fp2[height - y - 1][Stride - x - channel + 1];
					fp[y][x + 2] = fp2[height - y - 1][Stride - x - channel + 2];
				}
			}
		}
		void verticalFlip(unsigned char* ptr, unsigned char* ptr2, int width, int height, int channel)
		{
			unsigned char** fp = new unsigned char* [height];
			unsigned char** fp2 = new unsigned char* [height];
			int Stride = width * channel, x = 0, y = 0;
			for (int j = 0; j < height; j++)
				fp[j] = ptr + (Stride * j);
			for (int j = 0; j < height; j++)
				fp2[j] = ptr2 + (Stride * j);
			for (y = 0; y < height; y++)
			{
				for (x = 0; x < Stride; x += channel)
				{
					fp[y][x] = fp2[(height - 1) - y][x];
					fp[y][x + 1] = fp2[(height - 1) - y][x + 1];
					fp[y][x + 2] = fp2[(height - 1) - y][x + 2];
				}
			}
		}
		void kaleidoscope(unsigned char* ptr, unsigned char* ptr2, int width, int height, int channel)
		{
			unsigned char** fp = new unsigned char* [height];
			unsigned char** fp2 = new unsigned char* [height];
			int Stride = width * channel, x = 0, y = 0;
			unsigned long int up = 0;
			unsigned long int down = 0;
			for (int j = 0; j < height; j++)
				fp[j] = ptr + (Stride * j);
			for (int j = 0; j < height; j++)
				fp2[j] = ptr2 + (Stride * j);
			for (y = 0; y < height; y++)
			{
				for (x = 0; x < Stride; x += channel)
				{
					fp[y][x] = abs(fp2[(height - 1) - y][x] - fp2[y][x]);
					fp[y][x + 1] = abs(fp2[(height - 1) - y][x + 1] - fp2[y][x + 1]);
					fp[y][x + 2] = abs(fp2[(height - 1) - y][x + 2] - fp2[y][x + 2]);
				}
			}
			for (y = 0; y < height; y++)
			{
				for (x = 0; x < Stride; x += channel)
				{
					if (fp2[y][x] + fp2[y][x + 1] + fp2[y][x + 2] >= 127 * 3)
						up++;
					else
						down++;
				}
			}
			for (y = 0; y < height; y++)
			{
				for (x = 0; x < Stride; x += channel)
				{
					fp2[y][x] = abs(fp[y][Stride - channel - x] - fp[y][x]);
					fp2[y][x + 1] = abs(fp[y][Stride - channel - x + 1] - fp[y][x + 1]);
					fp2[y][x + 2] = abs(fp[y][Stride - channel - x + 2] - fp[y][x + 2]);
				}
			}
			if (up < down)
			{
				for (y = 0; y < height; y++)
				{
					for (x = 0; x < Stride; x += channel)
					{
						fp[y][x] = fp2[y][x];
						fp[y][x + 1] = fp2[y][x + 1];
						fp[y][x + 2] = fp2[y][x + 2];
					}
				}
			}
			else
			{
				for (y = 0; y < height; y++)
				{
					for (x = 0; x < Stride; x += channel)
					{
						fp[y][x] = 255 - fp2[y][x];
						fp[y][x + 1] = 255 - fp2[y][x + 1];
						fp[y][x + 2] = 255 - fp2[y][x + 2];
					}
				}
			}
		}
		void  brightness(unsigned char* ptr, int width, int height, int channel, int value)
		{
			unsigned char** fp = new unsigned char* [height];
			int Stride = width * channel, x = 0, y = 0;
			for (int j = 0; j < height; j++)
				fp[j] = ptr + (Stride * j);
			if (value > 0)
			{
				for (y = 0; y < height; y++)
				{
					for (x = 0; x < Stride; x += channel)
					{
						fp[y][x] = fp[y][x] + value > 255 ? 255 : fp[y][x] + value;
						fp[y][x + 1] = fp[y][x + 1] + value > 255 ? 255 : fp[y][x + 1] + value;
						fp[y][x + 2] = fp[y][x + 2] + value > 255 ? 255 : fp[y][x + 2] + value;
					}
				}
			}
			else
			{
				for (y = 0; y < height; y++)
				{
					for (x = 0; x < Stride; x += channel)
					{
						fp[y][x] = fp[y][x] + value < 0 ? 0 : fp[y][x] + value;
						fp[y][x + 1] = fp[y][x + 1] + value < 0 ? 0 : fp[y][x + 1] + value;
						fp[y][x + 2] = fp[y][x + 2] + value < 0 ? 0 : fp[y][x + 2] + value;
					}
				}
			}
			delete[] fp;
		}
		void  brightness2(unsigned char* ptr, int width, int height, int channel, double value)
		{
			unsigned char** fp = new unsigned char* [height];
			int Stride = width * channel, x = 0, y = 0;
			for (int j = 0; j < height; j++)
				fp[j] = ptr + (Stride * j);
			if (value > 0)
			{
				for (y = 0; y < height; y++)
				{
					for (x = 0; x < Stride; x += channel)
					{
						fp[y][x] = fp[y][x] * value > 255 ? 255 : fp[y][x] * value;
						fp[y][x + 1] = fp[y][x + 1] * value > 255 ? 255 : fp[y][x + 1] * value;
						fp[y][x + 2] = fp[y][x + 2] * value > 255 ? 255 : fp[y][x + 2] * value;
					}
				}
			}
			else
			{
				for (y = 0; y < height; y++)
				{
					for (x = 0; x < Stride; x += channel)
					{
						fp[y][x] = fp[y][x] * value < 0 ? 0 : fp[y][x] * value;
						fp[y][x + 1] = fp[y][x + 1] * value < 0 ? 0 : fp[y][x + 1] * value;
						fp[y][x + 2] = fp[y][x + 2] * value < 0 ? 0 : fp[y][x + 2] * value;
					}
				}
			}
			delete[] fp;
		}
		void contrast(unsigned char* ptr, int width, int height, int channel, int value)
		{
			unsigned char** fp = new unsigned char* [height];
			int Stride = width * channel, x = 0, y = 0;
			unsigned char* colorList = new unsigned char[256];
			for (int i = 0; i < 256; i++)
			{
				colorList[i] = unsigned char((double)i * ((double)(255 - value * 1) / 255.0));
			}
			for (int j = 0; j < height; j++)
				fp[j] = ptr + (Stride * j);
			double color = 0;
			int colorB = 0, colorG = 0, colorR = 0;
			for (y = 0; y < height; y++)
			{
				for (x = 0; x < Stride; x += channel)
				{
					color = (double)colorList[(int)((double)(fp[y][x] + fp[y][x + 1] + fp[y][x + 2]) / 3.0)] / (double)((double)(fp[y][x] + fp[y][x + 1] + fp[y][x + 2]) / 3.0);
					colorB = (fp[y][x] * color);
					colorG = (fp[y][x + 1] * color);
					colorR = (fp[y][x + 2] * color);
					if (colorB > 255)colorB = 255;
					if (colorG > 255)colorG = 255;
					if (colorR > 255)colorR = 255;
					fp[y][x] = colorB;
					fp[y][x + 1] = colorG;
					fp[y][x + 2] = colorR;
				}
			}
		}
		/*0-100 100
		20-80  60
			1*(60/100)=1*0.6=0.6
			20*(60/100)=20*0.6=12+20=32
			80*(60/100)=80*0.6=48+20=68
			100*(60/100)=100*0.6=60*/
		void BrightnessContrast(unsigned char* ptr, int width, int height, int channel, int value, int value2)
		{
			unsigned char** fp = new unsigned char* [height];
			int Stride = width * channel, x = 0, y = 0;
			unsigned char* colorList = new unsigned char[256];
			double color = 0;
			int c = 0;
			int colorB = 0, colorG = 0, colorR = 0;
			for (int j = 0; j < height; j++)
				fp[j] = ptr + (Stride * j);
			for (int i = 0; i < 256; i++)
			{
				if (value2 > 0)
				{
					c = int((double)(i + value) * ((double)(255 - value2 * 1) / 255.0));
					if (c > 255)c = 255;
					if (c < 0)c = 0;
					colorList[i] = c;
				}
				else if (value2 == -255)
				{
					if (i > 127)
						colorList[i] = 255;
					else
						colorList[i] = 0;
				}
				else
				{
					int TempValue = 255 - (-value2);
					if (TempValue < 3)colorList[i] = 0;
					double temp = (double)(255.0 / (double)TempValue);
					if (i > 127)
						c = 127 + (int)(temp * (double)(i - 127)) + value;
					else
						c = 127 - (int)(temp * (double)(127 - i)) + value;
					if (c > 255)c = 255;
					if (c < 0)c = 0;
					colorList[i] = c;
				}
			}
			for (y = 0; y < height; y++)
			{
				for (x = 0; x < Stride; x += channel)
				{
					color = (double)colorList[(int)((double)(fp[y][x] + fp[y][x + 1] + fp[y][x + 2]) / 3.0)] / (double)((double)(fp[y][x] + fp[y][x + 1] + fp[y][x + 2]) / 3.0);
					colorB = (fp[y][x] * color);
					colorG = (fp[y][x + 1] * color);
					colorR = (fp[y][x + 2] * color);
					if (colorB > 255)colorB = 255;
					if (colorG > 255)colorG = 255;
					if (colorR > 255)colorR = 255;
					fp[y][x] = colorList[fp[y][x]];
					fp[y][x + 1] = colorList[fp[y][x + 1]];
					fp[y][x + 2] = colorList[fp[y][x + 2]];
				}
			}
			delete[] fp;
			delete[] colorList;
		}
		void ScanningLine(unsigned char* ptr, int width, int height, int channel, int value, int value2)
		{
			unsigned char** fp = new unsigned char* [height];
			int Stride = width * channel, x = 0, y = 0;
			for (int j = 0; j < height; j++)
				fp[j] = ptr + (Stride * j);
			if (value == 0 || value == 2)
			{
				for (y = 0; y < height; y++)
				{
					for (x = 0; x < Stride; x += channel * value2)
					{
						fp[y][x] = 0;
						fp[y][x + 1] = 0;
						fp[y][x + 2] = 0;
					}
				}
			}
			if (value == 1 || value == 2)
			{
				for (y = 0; y < height; y += value2)
				{
					for (x = 0; x < Stride; x += channel)
					{
						fp[y][x] = 0;
						fp[y][x + 1] = 0;
						fp[y][x + 2] = 0;
					}
				}
			}
		}
		void airbrush(unsigned char* ptr, unsigned char* ptr2, int width, int height, const int channel, bool iscolor, const int value)
		{
			unsigned char** fp = new unsigned char* [height];
			unsigned char** fp2 = new unsigned char* [height];
			int Stride = width * channel, x = 0, y = 0, x2 = 0, y2 = 0;
			for (int j = 0; j < height; j++)
				fp[j] = ptr + (Stride * j);
			for (int j = 0; j < height; j++)
				fp2[j] = ptr2 + (Stride * j);
			if (iscolor == false) {
				for (y = 0; y < height; y++)
				{
					for (x = 0; x < Stride; x += channel)
					{
						x2 = std::rand() % value * channel - ((value / 2) * channel);
						y2 = (std::rand() % value - ((value / 2)));
						if (y + y2 < 0 || y + y2 >= height || x + x2 < 0 || x + x2 >= Stride)
							x2 = y2 = 0;
						fp[y][x] = fp2[y + y2][x + x2];
						fp[y][x + 1] = fp2[y + y2][x + x2 + 1];
						fp[y][x + 2] = fp2[y + y2][x + x2 + 2];
					}
				}
			}
			else
			{
				for (y = 0; y < height; y++)
				{
					for (x = 0; x < Stride; x += channel)
					{
						x2 = std::rand() % value * channel - ((value / 2) * channel);
						y2 = (std::rand() % value - ((value / 2)));
						if (y + y2 < 0 || y + y2 >= height || x + x2 < 0 || x + x2 >= Stride)
							x2 = y2 = 0;
						fp[y][x] = fp2[y + y2][x + x2];
						x2 = std::rand() % value * channel - ((value / 2) * channel);
						y2 = (std::rand() % value - ((value / 2)));
						if (y + y2 < 0 || y + y2 >= height || x + x2 < 0 || x + x2 >= Stride)
							x2 = y2 = 0;
						fp[y][x + 1] = fp2[y + y2][x + x2 + 1];
						x2 = std::rand() % value * channel - ((value / 2) * channel);
						y2 = (std::rand() % value - ((value / 2)));
						if (y + y2 < 0 || y + y2 >= height || x + x2 < 0 || x + x2 >= Stride)
							x2 = y2 = 0;
						fp[y][x + 2] = fp2[y + y2][x + x2 + 2];
					}
				}
			}
		}
		void sharp(unsigned char* ptr, unsigned char* ptr2, int width, int height, int channel, int value)
		{
			unsigned char** fp = new unsigned char* [height];
			unsigned char** fp2 = new unsigned char* [height];
			const int recSize = ((value * 2 + 1) * (value * 2 + 1));
			const int recWidth = value * channel;
			const int rec = (value * 2 + 1) * (value * 2 + 1) + 1;
			int Stride = width * channel, x = 0, y = 0;
			for (int j = 0; j < height; j++)
				fp[j] = ptr + (Stride * j);
			for (int j = 0; j < height; j++)
				fp2[j] = ptr2 + (Stride * j);
			int x2 = 0; int y2 = 0;
			int countB = 0;
			int countG = 0;
			int countR = 0;
			for (y = 0; y < height; y++)
			{
				for (x = 0; x < Stride; x += channel)
				{
					for (y2 = -value; y2 <= value; y2++)
					{
						for (x2 = -recWidth; x2 <= recWidth; x2 += channel)
						{
							if (y + y2 < 0 || y + y2 >= height || x + x2 < 0 || x + x2 >= Stride)
								continue;
							countB += fp2[y + y2][x + x2];
							countG += fp2[y + y2][x + x2 + 1];
							countR += fp2[y + y2][x + x2 + 2];
						}
					}
					countB = countB * -1 + fp[y][x] * rec;
					countG = countG * -1 + fp[y][x + 1] * rec;
					countR = countR * -1 + fp[y][x + 2] * rec;
					if (countB > 255)countB = 255;
					if (countG > 255)countG = 255;
					if (countR > 255)countR = 255;
					if (countB < 0)countB = 0;
					if (countG < 0)countG = 0;
					if (countR < 0)countR = 0;
					fp[y][x] = countB;
					fp[y][x + 1] = countG;
					fp[y][x + 2] = countR;
					countB = countG = countR = 0;
				}
			}
			delete[] fp;
			delete[] fp2;
		}
		void  blurry(unsigned char* ptr, int width, int height, int channel, int value)
		{
			unsigned char** fp = new unsigned char* [height];
			const int recSize = ((value * 2 + 1) * (value * 2 + 1));
			const int recWidth = value * channel;
			int Stride = width * channel, x = 0, y = 0;
			for (int j = 0; j < height; j++)
				fp[j] = ptr + (Stride * j);
			int x2 = 0; int y2 = 0;
			int countB = 0;
			int countG = 0;
			int countR = 0;
			for (y = 0; y < height; y++)
			{
				for (x = 0; x < Stride; x += channel)
				{
					for (y2 = -value; y2 <= value; y2++)
					{
						for (x2 = -recWidth; x2 <= recWidth; x2 += channel)
						{
							if (y + y2 < 0 || y + y2 >= height || x + x2 < 0 || x + x2 >= Stride)
								continue;
							countB += fp[y + y2][x + x2];
							countG += fp[y + y2][x + x2 + 1];
							countR += fp[y + y2][x + x2 + 2];
						}
					}
					fp[y][x] = countB / recSize;
					fp[y][x + 1] = countG / recSize;
					fp[y][x + 2] = countR / recSize;
					countB = countG = countR = 0;
				}
			}
			delete[] fp;
		}
		void ColorNoise(unsigned char* ptr, const int width, const int height, const int channel, const double value)
		{
			srand(time(NULL));
			thread ThreadW0H0(ColorNoiseThread, ptr, width, height, channel, value, 0, 0);
			thread ThreadW1H0(ColorNoiseThread, ptr, width, height, channel, value, 1, 0);
			thread ThreadW0H1(ColorNoiseThread, ptr, width, height, channel, value, 0, 1);
			thread ThreadW1H1(ColorNoiseThread, ptr, width, height, channel, value, 1, 1);
			ThreadW0H0.join();
			ThreadW1H0.join();
			ThreadW0H1.join();
			ThreadW1H1.join();
		}
		static void ColorNoiseThread(unsigned char* ptr, const int width, const int height, const int channel, const double value, int halfwidth, int halfheight)
		{
			int Stride = width * channel, x = 0, y = 0;
			int heightBegin = 0;
			int heightEnd = height;
			if (halfheight == 0)heightBegin = 0; else heightBegin = height / 2;
			if (halfheight == 0)heightEnd = height / 2; else heightEnd = height;
			int widthBegin = 0;
			int widthEnd = Stride;
			int halfStride = Stride / 2;
			if (halfStride % 4 != 0)
			{
				for (int i = 0; i < 10; i++)
				{
					if (i == 9)return;
					if ((halfStride - i) % 4 == 0)
					{
						halfStride -= i;
						break;
					}
				}
			}
			if (halfwidth == 0)widthBegin = 0; else widthBegin = halfStride;
			if (halfwidth == 0)widthEnd = halfStride; else widthEnd = Stride;
			unsigned char** fp = new unsigned char* [height];
			for (int j = 0; j < height; j++)
				fp[j] = ptr + (Stride * j);
			const double unValue = 1 - value;
			for (y = heightBegin; y < heightEnd; y++)
			{
				for (x = widthBegin; x < widthEnd; x += channel)
				{
					fp[y][x] = (std::rand() % 256) * value + (fp[y][x]) * unValue;
					fp[y][x + 1] = (std::rand() % 256) * value + (fp[y][x + 1]) * unValue;
					fp[y][x + 2] = (std::rand() % 256) * value + (fp[y][x + 2]) * unValue;
				}
			}
			delete[] fp;
		}
		void oilpaint(unsigned char* ptr, unsigned char* ptr2, int width, int height, int channel, int value, double value2)
		{
			thread ThreadB1(oilpaintThread, ptr, ptr2, width, height, channel, value, value2, 0, 0);
			thread ThreadG1(oilpaintThread, ptr, ptr2, width, height, channel, value, value2, 1, 0);
			thread ThreadR1(oilpaintThread, ptr, ptr2, width, height, channel, value, value2, 2, 0);
			thread ThreadB2(oilpaintThread, ptr, ptr2, width, height, channel, value, value2, 0, 1);
			thread ThreadG2(oilpaintThread, ptr, ptr2, width, height, channel, value, value2, 1, 1);
			thread ThreadR2(oilpaintThread, ptr, ptr2, width, height, channel, value, value2, 2, 1);
			ThreadB1.join();
			ThreadG1.join();
			ThreadR1.join();
			ThreadB2.join();
			ThreadG2.join();
			ThreadR2.join();
		}
		static void oilpaintThread(unsigned char* ptr, unsigned char* ptr2, int width, int height, int channel, int value, double value2, int bgr, int half)
		{
			int adopt = (int)((double)(value * value) * value2);
			if (adopt > value* value - 1)adopt = value * value - 1;
			if (adopt < 0)adopt = 0;
			unsigned char** fp = new unsigned char* [height];
			unsigned char** fp2 = new unsigned char* [height];
			unsigned char* colorListB = new unsigned char[value * value];
			unsigned char* colorListG = new unsigned char[value * value];
			unsigned char* colorListR = new unsigned char[value * value];
			for (int i = 0; i < value * value; i++)
				colorListB[i] = colorListG[i] = colorListR[i] = 0;
			const int recSize = ((value * 2 + 1) * (value * 2 + 1));
			const int recWidth = value * channel;
			int Stride = width * channel, x = 0, y = 0;
			for (int j = 0; j < height; j++)
				fp[j] = ptr + (Stride * j);
			for (int j = 0; j < height; j++)
				fp2[j] = ptr2 + (Stride * j);
			int x2 = 0; int y2 = 0;
			int countB = 0;
			int countG = 0;
			int countR = 0;
			int count = 0;
			int heightBegin = 0;
			int heightEnd = height;
			if (half == 0)heightBegin = 0; else heightBegin = height / 2;
			if (half == 0)heightEnd = height / 2; else heightEnd = height;
			if (bgr == 0)
			{
				for (y = heightBegin; y < heightEnd; y++)
				{
					for (x = 0; x < Stride; x += channel)
					{
						count = 0;
						for (y2 = 0; y2 < value; y2++)
						{
							for (x2 = 0; x2 < recWidth; x2 += channel)
							{
								if (y + y2 < 0 || y + y2 >= height || x + x2 < 0 || x + x2 >= Stride)
								{
									colorListB[count] = 0;
									count++;
									continue;
								}
								colorListB[count] = fp2[y + y2][x + x2];
								count++;
							}
						}
						sort(colorListB, colorListB + value * value);
						fp[y][x] = colorListB[adopt];
					}
				}
			}
			if (bgr == 1)
			{
				for (y = heightBegin; y < heightEnd; y++)
				{
					for (x = 0; x < Stride; x += channel)
					{
						count = 0;
						for (y2 = 0; y2 < value; y2++)
						{
							for (x2 = 0; x2 < recWidth; x2 += channel)
							{
								if (y + y2 < 0 || y + y2 >= height || x + x2 < 0 || x + x2 >= Stride)
								{
									colorListG[count] = 0;
									count++;
									continue;
								}
								colorListG[count] = fp2[y + y2][x + x2 + 1];
								count++;
							}
						}
						sort(colorListG, colorListG + value * value);
						fp[y][x + 1] = colorListG[adopt];
					}
				}
			}
			if (bgr == 2)
			{
				for (y = heightBegin; y < heightEnd; y++)
				{
					for (x = 0; x < Stride; x += channel)
					{
						count = 0;
						for (y2 = 0; y2 < value; y2++)
						{
							for (x2 = 0; x2 < recWidth; x2 += channel)
							{
								if (y + y2 < 0 || y + y2 >= height || x + x2 < 0 || x + x2 >= Stride)
								{
									colorListR[count] = 0;
									count++;
									continue;
								}
								colorListR[count] = fp2[y + y2][x + x2 + 2];
								count++;
							}
						}
						sort(colorListR, colorListR + value * value);
						fp[y][x + 2] = colorListR[adopt];
					}
				}
			}
			delete[] fp;
			delete[] fp2;
		}
		void pasteImage(unsigned char* ptr, unsigned char* ptr2, int width, int height, int width2, int height2, int pasteX, int pasteY, int channel)
		{
			unsigned char** fp = new unsigned char* [height];
			unsigned char** fp2 = new unsigned char* [height2];
			int Stride = width * channel, x = 0, y = 0;
			int Stride2 = width2 * channel;
			for (int j = 0; j < height; j++)
				fp[j] = ptr + (Stride * j);
			for (int j = 0; j < height2; j++)
				fp2[j] = ptr2 + (Stride2 * j);
			int x2 = 0; int y2 = 0;
			int pasteXPoint = pasteX * channel;
			for (y = 0, y2 = 0; y < height, y2 < height2; y++, y2++)
			{
				for (x = 0, x2 = 0; x < Stride, x2 < Stride2; x += channel, x2 += channel)
				{
					if (x + pasteXPoint >= Stride || x + pasteXPoint < 0 || y + pasteY >= height || y + pasteY < 0)continue;
					if (x2 >= Stride2 || x2 < 0 || y2 >= height2 || y2 < 0)continue;
					fp[y + pasteY][x + pasteXPoint] = fp2[y2][x2];
					fp[y + pasteY][x + pasteXPoint + 1] = fp2[y2][x2 + 1];
					fp[y + pasteY][x + pasteXPoint + 2] = fp2[y2][x2 + 2];
				}
			}
			delete[] fp;
			delete[] fp2;
		}
		void tile(unsigned char* ptr, int width, int height, int channel, int value, int value2)
		{
			unsigned char** fp = new unsigned char* [height];
			const int recSize = value * value;
			int Stride = width * channel, x = 0, y = 0;
			for (int j = 0; j < height; j++)
				fp[j] = ptr + (Stride * j);
			int x2 = 0; int y2 = 0;
			int B = 0;
			int G = 0;
			int R = 0;
			for (y = 0; y < height; y += value)
			{
				for (x = 0; x < Stride; x += channel * value)
				{
					for (y2 = 0; y2 <= value; y2++)
					{
						for (x2 = 0; x2 <= value2 * channel; x2 += channel)
						{
							if (y + y2 < 0 || y + y2 >= height || x + x2 < 0 || x + x2 >= Stride)
								continue;
							B = fp[y + y2][x + x2] * (x2 / channel) / value2 + 255 * (value2 - x2 / 4) / value2;
							G = fp[y + y2][x + x2 + 1] * (x2 / channel) / value2 + 255 * (value2 - x2 / 4) / value2;
							R = fp[y + y2][x + x2 + 2] * (x2 / channel) / value2 + 255 * (value2 - x2 / 4) / value2;
							if (B > 255)B = 255;
							if (B < 0) B = 0;
							if (G > 255)G = 255;
							if (G < 0) G = 0;
							if (R > 255)R = 255;
							if (R < 0) R = 0;
							fp[y + y2][x + x2] = B;
							fp[y + y2][x + x2 + 1] = G;
							fp[y + y2][x + x2 + 2] = R;
						}
					}
					for (y2 = 0; y2 <= value2; y2++)
					{
						for (x2 = 0; x2 <= value * channel; x2 += channel)
						{
							if (y + y2 < 0 || y + y2 >= height || x + x2 < 0 || x + x2 >= Stride)
								continue;
							B = fp[y + y2][x + x2] * (y2) / value2 + 255 * (value2 - y2) / value2;
							G = fp[y + y2][x + x2 + 1] * (y2) / value2 + 255 * (value2 - y2) / value2;
							R = fp[y + y2][x + x2 + 2] * (y2) / value2 + 255 * (value2 - y2) / value2;
							if (B > 255)B = 255;
							if (B < 0) B = 0;
							if (G > 255)G = 255;
							if (G < 0) G = 0;
							if (R > 255)R = 255;
							if (R < 0) R = 0;
							fp[y + y2][x + x2] = B;
							fp[y + y2][x + x2 + 1] = G;
							fp[y + y2][x + x2 + 2] = R;

						}
					}
					for (y2 = 0; y2 <= value; y2++)
					{
						for (x2 = (value - value2) * channel; x2 <= value * channel; x2 += channel)
						{
							if (y + y2 < 0 || y + y2 >= height || x + x2 < 0 || x + x2 >= Stride)
								continue;
							int temptra = x2 - ((value - value2) * channel);
							B = fp[y + y2][x + x2] * (value2 - temptra / channel) / value2 + (temptra / channel) / value2;
							G = fp[y + y2][x + x2 + 1] * (value2 - temptra / channel) / value2 + (temptra / channel) / value2;
							R = fp[y + y2][x + x2 + 2] * (value2 - temptra / channel) / value2 + (temptra / channel) / value2;
							if (B > 255)B = 255;
							if (B < 0) B = 0;
							if (G > 255)G = 255;
							if (G < 0) G = 0;
							if (R > 255)R = 255;
							if (R < 0) R = 0;
							fp[y + y2][x + x2] = B;
							fp[y + y2][x + x2 + 1] = G;
							fp[y + y2][x + x2 + 2] = R;
						}
					}
					for (y2 = (value - value2); y2 <= value; y2++)
					{
						for (x2 = 0; x2 <= value * channel; x2 += channel)
						{
							if (y + y2 < 0 || y + y2 >= height || x + x2 < 0 || x + x2 >= Stride)
								continue;
							int temptra = (y2 - (value - value2)) * channel;
							B = fp[y + y2][x + x2] * (value2 - temptra / channel) / value2 + (temptra / channel) / value2;
							G = fp[y + y2][x + x2 + 1] * (value2 - temptra / channel) / value2 + (temptra / channel) / value2;
							R = fp[y + y2][x + x2 + 2] * (value2 - temptra / channel) / value2 + (temptra / channel) / value2;
							if (B > 255)B = 255;
							if (B < 0) B = 0;
							if (G > 255)G = 255;
							if (G < 0) G = 0;
							if (R > 255)R = 255;
							if (R < 0) R = 0;
							fp[y + y2][x + x2] = B;
							fp[y + y2][x + x2 + 1] = G;
							fp[y + y2][x + x2 + 2] = R;
						}
					}

				}
			}
		}
		void mosaic(unsigned char* ptr, unsigned char* ptr2, const int width, const int height, const int channel, const int value)
		{
			unsigned char** fp = new unsigned char* [height];
			unsigned char** fp2 = new unsigned char* [height];
			const int recSize = value * value;
			//const int recWidth = value * channel;
			int Stride = width * channel, x = 0, y = 0;
			for (int j = 0; j < height; j++)
				fp[j] = ptr + (Stride * j);
			for (int j = 0; j < height; j++)
				fp2[j] = ptr2 + (Stride * j);
			int x2 = 0; int y2 = 0;
			int countB = 0;
			int countG = 0;
			int countR = 0;
			int count = 0;
			for (y = 0; y < height; y += value)
			{
				for (x = 0; x < Stride; x += channel * value)
				{
					for (y2 = 0; y2 < value; y2++)
					{
						for (x2 = 0; x2 < value * channel; x2 += channel)
						{
							if (y + y2 < 0 || y + y2 >= height || x + x2 < 0 || x + x2 >= Stride)
							{
								count++;
								continue;
							}
							countB += fp2[y + y2][x + x2];
							countG += fp2[y + y2][x + x2 + 1];
							countR += fp2[y + y2][x + x2 + 2];
						}
					}
					countB /= recSize - count;
					countG /= recSize - count;
					countR /= recSize - count;
					count = 0;
					for (y2 = 0; y2 < value; y2++)
					{
						for (x2 = 0; x2 < value * channel; x2 += channel)
						{
							if (y + y2 < 0 || y + y2 >= height || x + x2 < 0 || x + x2 >= Stride)
								continue;
							fp[y + y2][x + x2] = countB;
							fp[y + y2][x + x2 + 1] = countG;
							fp[y + y2][x + x2 + 2] = countR;
						}
					}
					countB = countG = countR = 0;
				}
			}
			delete[] fp;
		}
		void emboss(unsigned char* ptr, unsigned char* ptr2, int width, int height, int channel, int pasteY, int pasteX, bool isgray)
		{
			//int pasteY = 5;
			//int pasteX = 5;
			unsigned char** fp = new unsigned char* [height];
			unsigned char** fp2 = new unsigned char* [height];
			int Stride = width * channel, x = 0, y = 0;
			for (int j = 0; j < height; j++)
				fp[j] = ptr + (Stride * j);
			for (int j = 0; j < height; j++)
				fp2[j] = ptr2 + (Stride * j);
			int x2 = 0; int y2 = 0;
			int pasteXPoint = pasteX * channel;
			unsigned char gray = 0;
			int bb = 0, gg = 0, rr = 0;
			int tempB = 0, tempG = 0, tempR = 0;
			if (isgray == false)
			{
				for (y = 0; y < height; y++)
				{
					for (x = 0; x < Stride; x += channel)
					{
						if (x + pasteXPoint >= Stride || x + pasteXPoint < 0 || y + pasteY >= height || y + pasteY < 0)continue;
						gray = (fp2[y + pasteY][x + pasteXPoint] + fp2[y + pasteY][x + pasteXPoint + 1] + fp2[y + pasteY][x + pasteXPoint + 2]) / 3;
						tempB = 255 - (gray);
						tempG = 255 - (gray);
						tempR = 255 - (gray);
						bb = ((tempB)+fp[y][x]) / 2;
						gg = ((tempG)+fp[y][x + 1]) / 2;
						rr = ((tempR)+fp[y][x + 2]) / 2;
						gray = (bb + gg + rr) / 3;
						bb = bb * 2 - gray;
						gg = gg * 2 - gray;
						rr = rr * 2 - gray;
						if (rr > 255)rr = 255;
						if (gg > 255)gg = 255;
						if (bb > 255)bb = 255;
						if (rr < 0)rr = 0;
						if (gg < 0)gg = 0;
						if (bb < 0)bb = 0;
						fp[y][x] = bb;
						fp[y][x + 1] = gg;
						fp[y][x + 2] = rr;
					}
				}
			}
			else
			{
				for (y = 0; y < height; y++)
				{
					for (x = 0; x < Stride; x += channel)
					{
						if (x + pasteXPoint >= Stride || x + pasteXPoint < 0 || y + pasteY >= height || y + pasteY < 0)continue;
						fp[y + pasteY][x + pasteXPoint] = ((255 - fp2[y][x]) + fp[y + pasteY][x + pasteXPoint]) / 2;
						fp[y + pasteY][x + pasteXPoint + 1] = ((255 - fp2[y][x + 1]) + fp[y + pasteY][x + pasteXPoint + 1]) / 2;
						fp[y + pasteY][x + pasteXPoint + 2] = ((255 - fp2[y][x + 2]) + fp[y + pasteY][x + pasteXPoint + 2]) / 2;
					}
				}
			}
			delete[] fp;
			delete[] fp2;
		}
		void ToneSeparation(unsigned char* ptr, int width, int height, int channel, int value)
		{
			unsigned char* colorList = new unsigned char[256];
			for (int i = 0; i < 256; i++)
				colorList[i] = i;
			value = 75 - value;
			if (value <= 0)value = 1;
			int newValue = 255 / (value);
			for (int i = 0; i < 128; i++)
			{
				for (int j = 0; j < 256; j += newValue)
				{
					if (i >= j)
						colorList[i] = j;
				}
			}
			for (int i = 128; i < 256; i++)
			{
				for (int j = 255; j >= 0; j -= newValue)
				{
					if (i <= j)
						colorList[i] = j;
				}
			}
			unsigned char** fp = new unsigned char* [height];
			int Stride = width * channel, x = 0, y = 0;
			for (int j = 0; j < height; j++)
				fp[j] = ptr + (Stride * j);
			for (y = 0; y < height; y++)
			{
				for (x = 0; x < Stride; x += channel)
				{
					fp[y][x] = colorList[fp[y][x]];
					fp[y][x + 1] = colorList[fp[y][x + 1]];
					fp[y][x + 2] = colorList[fp[y][x + 2]];
				}
			}
			delete[] fp;
		}

		static void blurry3BGR(unsigned char** fp, unsigned char** fp2, int width, int height, int channel, int value, int bgr, int half)
		{
			int Stride = width * channel;
			const int recSize = ((value * 2 + 1) * (value * 2 + 1));
			const int recWidth = value * channel;
			int x = 0; int y = 0;
			int x2 = 0; int y2 = 0;
			int countB = 0;
			int countG = 0;
			int countR = 0;
			int reduceRecSize = 0;
			int reduceRecSize1 = 0;
			int reduceRecSize2 = 0;
			int boxSize = 0;
			int heightBegin = 0;
			int heightEnd = height;
			if (half == 0)heightBegin = 0; else heightBegin = height / 2;
			if (half == 0)heightEnd = height / 2; else heightEnd = height;
			if (bgr == 0)
			{
				for (y = heightBegin; y < heightEnd; y++)
				{
					for (x = 0; x < Stride; x += channel)
					{
						if (x == 0)
						{
							for (y2 = -value; y2 <= value; y2++)
							{
								for (x2 = -recWidth; x2 <= recWidth; x2 += channel)
								{
									if (y + y2 < 0 || y + y2 >= height || x + x2 < 0 || x + x2 >= Stride)
									{
										reduceRecSize2++;
										continue;
									}
									countB += fp2[y + y2][x + x2];
								}
							}
							fp[y][x] = countB / (recSize - reduceRecSize2);
							reduceRecSize2 = 0;
						}
						else
						{
							for (y2 = -value, x2 = -recWidth - channel; y2 <= value; y2++)
							{
								if (y + y2 < 0 || y + y2 >= height || x + x2 < 0 || x + x2 >= Stride)
								{
									reduceRecSize1++;
									continue;
								}
								countB -= fp2[y + y2][x + x2];
							}
							for (y2 = -value, x2 = recWidth; y2 <= value; y2++)
							{
								if (y + y2 < 0 || y + y2 >= height || x + x2 < 0 || x + x2 >= Stride)
								{
									reduceRecSize++;
									continue;
								}
								countB += fp2[y + y2][x + x2];
							}
							fp[y][x] = countB / (recSize + reduceRecSize - reduceRecSize1);
						}
						reduceRecSize = 0;
						reduceRecSize1 = 0;
					}
					countB = countG = countR = 0;
					reduceRecSize2 = 0;
				}
			}
			if (bgr == 1)
			{
				for (y = heightBegin; y < heightEnd; y++)
				{
					for (x = 0; x < Stride; x += channel)
					{
						if (x == 0)
						{
							for (y2 = -value; y2 <= value; y2++)
							{
								for (x2 = -recWidth; x2 <= recWidth; x2 += channel)
								{
									if (y + y2 < 0 || y + y2 >= height || x + x2 < 0 || x + x2 >= Stride)
									{
										reduceRecSize2++;
										continue;
									}
									countG += fp2[y + y2][x + x2 + 1];
								}
							}
							fp[y][x + 1] = countG / (recSize - reduceRecSize2);
							reduceRecSize2 = 0;
						}
						else
						{
							for (y2 = -value, x2 = -recWidth - channel; y2 <= value; y2++)
							{
								if (y + y2 < 0 || y + y2 >= height || x + x2 < 0 || x + x2 >= Stride)
								{
									reduceRecSize1++;
									continue;
								}
								countG -= fp2[y + y2][x + x2 + 1];
							}
							for (y2 = -value, x2 = recWidth; y2 <= value; y2++)
							{
								if (y + y2 < 0 || y + y2 >= height || x + x2 < 0 || x + x2 >= Stride)
								{
									reduceRecSize++;
									continue;
								}
								countG += fp2[y + y2][x + x2 + 1];
							}
							fp[y][x + 1] = countG / (recSize + reduceRecSize - reduceRecSize1);
						}
						reduceRecSize = 0;
						reduceRecSize1 = 0;
					}
					countB = countG = countR = 0;
					reduceRecSize2 = 0;
				}
			}
			if (bgr == 2)
			{
				for (y = heightBegin; y < heightEnd; y++)
				{
					for (x = 0; x < Stride; x += channel)
					{
						if (x == 0)
						{
							for (y2 = -value; y2 <= value; y2++)
							{
								for (x2 = -recWidth; x2 <= recWidth; x2 += channel)
								{
									if (y + y2 < 0 || y + y2 >= height || x + x2 < 0 || x + x2 >= Stride)
									{
										reduceRecSize2++;
										continue;
									}
									countR += fp2[y + y2][x + x2 + 2];
								}
							}
							fp[y][x + 2] = countR / (recSize - reduceRecSize2);
							reduceRecSize2 = 0;
						}
						else
						{
							for (y2 = -value, x2 = -recWidth - channel; y2 <= value; y2++)
							{
								if (y + y2 < 0 || y + y2 >= height || x + x2 < 0 || x + x2 >= Stride)
								{
									reduceRecSize1++;
									continue;
								}
								countR -= fp2[y + y2][x + x2 + 2];
							}
							for (y2 = -value, x2 = recWidth; y2 <= value; y2++)
							{
								if (y + y2 < 0 || y + y2 >= height || x + x2 < 0 || x + x2 >= Stride)
								{
									reduceRecSize++;
									continue;
								}
								countR += fp2[y + y2][x + x2 + 2];
							}
							fp[y][x + 2] = countR / (recSize + reduceRecSize - reduceRecSize1);
						}
						reduceRecSize = 0;
						reduceRecSize1 = 0;
					}
					countB = countG = countR = 0;
					reduceRecSize2 = 0;
				}
			}
		}
		static void blurry4BGR(unsigned char** fp, unsigned char** fp2, int width, int height, int channel, int value, int bgr, int half)
		{
			int Stride = width * channel;
			const int recSize = ((value * 2 + 1) * (value * 2 + 1));
			const int recWidth = value * channel;
			int x = 0; int y = 0;
			int x2 = 0; int y2 = 0;
			int countB = 0;
			int countG = 0;
			int countR = 0;
			int reduceRecSize = 0;
			int reduceRecSize1 = 0;
			int reduceRecSize2 = 0;
			int boxSize = 0;
			int heightBegin = 0;
			int heightEnd = height;
			unsigned char* pointY;
			unsigned char* pointRec;
			if (half == 0)heightBegin = 0; else heightBegin = height / 2;
			if (half == 0)heightEnd = height / 2; else heightEnd = height;
			if (bgr == 0)
			{
				for (y = heightBegin; y < heightEnd; y++)
				{
					pointY = &fp[y][0];
					for (x = 0; x < Stride; x += channel)
					{
						if (x == 0)
						{
							for (y2 = -value; y2 <= value; y2++)
							{
								pointRec = &fp2[y + y2][0];
								for (x2 = -recWidth; x2 <= recWidth; x2 += channel)
								{
									if (y + y2 < 0 || y + y2 >= height || x + x2 < 0 || x + x2 >= Stride)
									{
										reduceRecSize2++;
										continue;
									}
									countB += *(pointRec + x + x2);
								}
							}
							*(pointY + x) = countB / (recSize - reduceRecSize2);
							//fp[y][x] = countB / (recSize - reduceRecSize2);
							reduceRecSize2 = 0;
						}
						else
						{
							for (y2 = -value, x2 = -recWidth - channel; y2 <= value; y2++)
							{
								pointRec = &fp2[y + y2][0];
								if (y + y2 < 0 || y + y2 >= height || x + x2 < 0 || x + x2 >= Stride)
								{
									reduceRecSize1++;
									continue;
								}
								countB -= *(pointRec + x + x2);
							}
							for (y2 = -value, x2 = recWidth; y2 <= value; y2++)
							{
								pointRec = &fp2[y + y2][0];
								if (y + y2 < 0 || y + y2 >= height || x + x2 < 0 || x + x2 >= Stride)
								{
									reduceRecSize++;
									continue;
								}
								countB += *(pointRec + x + x2);
							}
							*(pointY + x) = countB / (recSize + reduceRecSize - reduceRecSize1);
							//fp[y][x] = countB / (recSize + reduceRecSize - reduceRecSize1);
						}
						reduceRecSize = 0;
						reduceRecSize1 = 0;
					}
					countB = countG = countR = 0;
					reduceRecSize2 = 0;
				}
			}
			if (bgr == 1)
			{
				for (y = heightBegin; y < heightEnd; y++)
				{
					pointY = &fp[y][0];
					for (x = 0; x < Stride; x += channel)
					{
						if (x == 0)
						{
							for (y2 = -value; y2 <= value; y2++)
							{
								pointRec = &fp2[y + y2][0];
								for (x2 = -recWidth; x2 <= recWidth; x2 += channel)
								{
									if (y + y2 < 0 || y + y2 >= height || x + x2 < 0 || x + x2 >= Stride)
									{
										reduceRecSize2++;
										continue;
									}
									countG += *(pointRec + x + x2 + 1);
								}
							}
							*(pointY + x + 1) = countG / (recSize - reduceRecSize2);
							//fp[y][x + 1] = countG / (recSize - reduceRecSize2);
							reduceRecSize2 = 0;
						}
						else
						{
							for (y2 = -value, x2 = -recWidth - channel; y2 <= value; y2++)
							{
								pointRec = &fp2[y + y2][0];
								if (y + y2 < 0 || y + y2 >= height || x + x2 < 0 || x + x2 >= Stride)
								{
									reduceRecSize1++;
									continue;
								}
								countG -= *(pointRec + x + x2 + 1);
							}
							for (y2 = -value, x2 = recWidth; y2 <= value; y2++)
							{
								pointRec = &fp2[y + y2][0];
								if (y + y2 < 0 || y + y2 >= height || x + x2 < 0 || x + x2 >= Stride)
								{
									reduceRecSize++;
									continue;
								}
								countG += *(pointRec + x + x2 + 1);
							}
							*(pointY + x + 1) = countG / (recSize + reduceRecSize - reduceRecSize1);
							//fp[y][x + 1] = countG / (recSize + reduceRecSize - reduceRecSize1);
						}
						reduceRecSize = 0;
						reduceRecSize1 = 0;
					}
					countB = countG = countR = 0;
					reduceRecSize2 = 0;
				}
			}
			if (bgr == 2)
			{
				for (y = heightBegin; y < heightEnd; y++)
				{
					pointY = &fp[y][0];
					for (x = 0; x < Stride; x += channel)
					{
						if (x == 0)
						{
							for (y2 = -value; y2 <= value; y2++)
							{
								pointRec = &fp2[y + y2][0];
								for (x2 = -recWidth; x2 <= recWidth; x2 += channel)
								{
									if (y + y2 < 0 || y + y2 >= height || x + x2 < 0 || x + x2 >= Stride)
									{
										reduceRecSize2++;
										continue;
									}
									countR += *(pointRec + x + x2 + 2);
								}
							}
							*(pointY + x + 2) = countR / (recSize - reduceRecSize2);
							//fp[y][x + 2] = countR / (recSize - reduceRecSize2);
							reduceRecSize2 = 0;
						}
						else
						{
							for (y2 = -value, x2 = -recWidth - channel; y2 <= value; y2++)
							{
								pointRec = &fp2[y + y2][0];
								if (y + y2 < 0 || y + y2 >= height || x + x2 < 0 || x + x2 >= Stride)
								{
									reduceRecSize1++;
									continue;
								}
								countR -= *(pointRec + x + x2 + 2);
							}
							for (y2 = -value, x2 = recWidth; y2 <= value; y2++)
							{
								pointRec = &fp2[y + y2][0];
								if (y + y2 < 0 || y + y2 >= height || x + x2 < 0 || x + x2 >= Stride)
								{
									reduceRecSize++;
									continue;
								}
								countR += *(pointRec + x + x2 + 2);
							}
							*(pointY + x + 2) = countR / (recSize + reduceRecSize - reduceRecSize1);
							//fp[y][x + 2] = countR / (recSize + reduceRecSize - reduceRecSize1);
						}
						reduceRecSize = 0;
						reduceRecSize1 = 0;
					}
					countB = countG = countR = 0;
					reduceRecSize2 = 0;
				}
			}
		}
		void  inline blurry3(unsigned char* ptr, unsigned char* ptr2, int width, int height, int channel, int value)
		{
			unsigned char** fp = new unsigned char* [height];
			unsigned char** fp2 = new unsigned char* [height];
			const int recSize = ((value * 2 + 1) * (value * 2 + 1));
			const int recWidth = value * channel;
			int Stride = width * channel, x = 0, y = 0;
			for (int j = 0; j < height; j++)
				fp2[j] = ptr2 + (Stride * j);
			for (int j = 0; j < height; j++)
				fp[j] = ptr + (Stride * j);
			int x2 = 0; int y2 = 0;
			int countB = 0;
			int countG = 0;
			int countR = 0;
			int reduceRecSize = 0;
			int reduceRecSize1 = 0;
			int reduceRecSize2 = 0;
			int boxSize = 0;
			thread ThreadB1(blurry3BGR, fp, fp2, width, height, channel, value, 0, 0);
			thread ThreadG1(blurry3BGR, fp, fp2, width, height, channel, value, 1, 0);
			thread ThreadR1(blurry3BGR, fp, fp2, width, height, channel, value, 2, 0);
			thread ThreadB2(blurry3BGR, fp, fp2, width, height, channel, value, 0, 1);
			thread ThreadG2(blurry3BGR, fp, fp2, width, height, channel, value, 1, 1);
			thread ThreadR2(blurry3BGR, fp, fp2, width, height, channel, value, 2, 1);
			ThreadB1.join();
			ThreadG1.join();
			ThreadR1.join();
			ThreadB2.join();
			ThreadG2.join();
			ThreadR2.join();
			delete[] fp;
			delete[] fp2;
		}
		static void colorOrder(unsigned char& b, unsigned char& g, unsigned char& r, int order)
		{
			unsigned char temp;
			if (order == BGR2BGR)
			{
				return;
			}
			else if (order == BGR2BRG)
			{
				temp = g;
				g = r;
				r = temp;
			}
			else if (order == BGR2RGB)
			{
				temp = b;
				b = r;
				r = temp;
			}
			else if (order == BGR2RBG)
			{
				temp = b;
				b = g;
				g = temp;
				temp = r;
				r = g;
				g = temp;
			}
			else if (order == BGR2GRB)
			{
				temp = b;
				b = r;
				r = temp;
				temp = g;
				g = r;
				r = temp;
			}
			else if (order == BGR2GBR)
			{
				temp = b;
				b = g;
				g = temp;
			}
		}
		static void ConvertHSV_(unsigned char* ptr, int width, int height, int H, int S, int V, int channel, bool fix, int order, int halfheight, int halfwidth)
		{
			unsigned char** fp = new unsigned char* [height];
			int Stride = width * channel, x = 0, y = 0;
			for (int j = 0; j < height; j++)
				fp[j] = ptr + (Stride * j);
			int heightBegin = 0;
			int heightEnd = height;
			if (halfheight == 0)heightBegin = 0; else heightBegin = height / 2;
			if (halfheight == 0)heightEnd = height / 2; else heightEnd = height;
			int widthBegin = 0;
			int widthEnd = Stride;
			int halfStride = Stride / 2;
			if (halfStride % 4 != 0)
			{
				for (int i = 0; i < 10; i++)
				{
					if (i == 9)return;
					if ((halfStride - i) % 4 == 0)
					{
						halfStride -= i;
						break;
					}
				}
			}
			if (halfwidth == 0)widthBegin = 0; else widthBegin = halfStride;
			if (halfwidth == 0)widthEnd = halfStride; else widthEnd = Stride;
			for (y = heightBegin; y < heightEnd; y++)
			{
				for (x = widthBegin; x < widthEnd; x += channel)
				{
					colorOrder(fp[y][x], fp[y][x + 1], fp[y][x + 2], order);
					BGRToHSV(H, S, V, fp[y][x], fp[y][x + 1], fp[y][x + 2], fix);
				}
			}
			delete[] fp;
		}
		void ConvertHSV(unsigned char* ptr, int width, int height, int H, int S, int V, int channel, bool fix, int order)
		{
			thread ThreadW0H0(ConvertHSV_, ptr, width, height, H, S, V, channel, fix, order, 0, 0);
			thread ThreadW0H1(ConvertHSV_, ptr, width, height, H, S, V, channel, fix, order, 0, 1);
			thread ThreadW1H0(ConvertHSV_, ptr, width, height, H, S, V, channel, fix, order, 1, 0);
			thread ThreadW1H1(ConvertHSV_, ptr, width, height, H, S, V, channel, fix, order, 1, 1);
			ThreadW0H0.join();
			ThreadW0H1.join();
			ThreadW1H0.join();
			ThreadW1H1.join();
		}
		/////////////////////////////
		//////////////////////////////

		static inline double HSVMin(double a, double b) {
			return a <= b ? a : b;
		}

		static inline double HSVMax(double a, double b) {
			return a >= b ? a : b;
		}

		static inline void BGRToHSV(int H, int S, int V, unsigned char& colorB, unsigned char& colorG, unsigned char& colorR, bool fix)
		{
			double delta, min;
			double h = 0, s, v;
			min = HSVMin(HSVMin(colorR, colorG), colorB);
			v = HSVMax(HSVMax(colorR, colorG), colorB);
			delta = v - min;
			if (v == 0.0)
				s = 0;
			else
				s = delta / v;
			if (s == 0)
				h = 0.0;
			else
			{
				if (colorR == v)
					h = (colorG - colorB) / delta;
				else if (colorG == v)
					h = 2 + (colorB - colorR) / delta;
				else if (colorB == v)
					h = 4 + (colorR - colorG) / delta;
				h *= 60;
				if (h < 0.0)
					h = h + 360;
			}
			if (fix == false)
				h += H;
			else
				h = H;
			if (h < 0.0)
				h = h + 360;
			if (h >= 360.0)
				h = h - 360;
			s += s * S / 100;
			if (s > 1.0) s = 1.0;
			if (s < 0) s = 0;
			v += V;
			if (v > 255) v = 255;
			if (v < 0) v = 0;
			HSVToBGR(h, s, v, colorB, colorG, colorR);
		}

		static inline void  HSVToBGR(double H, double S, double V, unsigned char& colorB, unsigned char& colorG, unsigned char& colorR)
		{
			if (S == 0)
			{
				colorR = V;
				colorG = V;
				colorB = V;
			}
			else
			{
				int i;
				double f, p, q, t;

				if (H == 360)
					H = 0;
				else
					H = H / 60;

				i = (int)trunc(H);
				f = H - i;

				p = V * (1.0 - S);
				q = V * (1.0 - (S * f));
				t = V * (1.0 - (S * (1.0 - f)));

				switch (i)
				{
				case 0:
					colorR = V;
					colorG = t;
					colorB = p;
					break;

				case 1:
					colorR = q;
					colorG = V;
					colorB = p;
					break;
				case 2:
					colorR = p;
					colorG = V;
					colorB = t;
					break;

				case 3:
					colorR = p;
					colorG = q;
					colorB = V;
					break;

				case 4:
					colorR = t;
					colorG = p;
					colorB = V;
					break;

				default:
					colorR = V;
					colorG = p;
					colorB = q;
					break;
				}
			}
		}
		void Fluorescent(unsigned char* ptr, unsigned char* ptr2, int width, int height, int channel, int value)
		{
			unsigned char** fp = new unsigned char* [height];
			unsigned char** fp2 = new unsigned char* [height];
			const int recSize = ((value * 2 + 1) * (value * 2 + 1));
			const int recWidth = value * channel;
			const int rec = (value * 2 + 1) * (value * 2 + 1);
			int Stride = width * channel, x = 0, y = 0;
			for (int j = 0; j < height; j++)
				fp[j] = ptr + (Stride * j);
			for (int j = 0; j < height; j++)
				fp2[j] = ptr2 + (Stride * j);
			int x2 = 0; int y2 = 0;
			int countB = 0;
			int countG = 0;
			int countR = 0;
			for (y = 0; y < height; y++)
			{
				for (x = 0; x < Stride; x += channel)
				{
					for (y2 = -value; y2 <= value; y2++)
					{
						for (x2 = -recWidth; x2 <= recWidth; x2 += channel)
						{
							if (y + y2 < 0 || y + y2 >= height || x + x2 < 0 || x + x2 >= Stride)
								continue;
							countB += fp2[y + y2][x + x2];
							countG += fp2[y + y2][x + x2 + 1];
							countR += fp2[y + y2][x + x2 + 2];
						}
					}
					countB = countB * -1 + fp[y][x] * rec;
					countG = countG * -1 + fp[y][x + 1] * rec;
					countR = countR * -1 + fp[y][x + 2] * rec;
					if (countB > 255)countB = 255;
					if (countG > 255)countG = 255;
					if (countR > 255)countR = 255;
					if (countB < 0)countB = 0;
					if (countG < 0)countG = 0;
					if (countR < 0)countR = 0;
					fp[y][x] = countB;
					fp[y][x + 1] = countG;
					fp[y][x + 2] = countR;
					countB = countG = countR = 0;
				}
			}
			delete[] fp;
			delete[] fp2;
		}
		void sharptest(unsigned char* ptr, unsigned char* ptr2, int width, int height, int channel, int vW, int vH)
		{
			unsigned char** fp = new unsigned char* [height];
			unsigned char** fp2 = new unsigned char* [height];
			//const int recSize = ((value * 2 + 1) * (value * 2 + 1));
			//

			int Stride = width * channel, x = 0, y = 0;
			for (int j = 0; j < height; j++)
				fp[j] = ptr + (Stride * j);
			for (int j = 0; j < height; j++)
				fp2[j] = ptr2 + (Stride * j);
			int x2 = 0; int y2 = 0;
			int countB = 0;
			int countG = 0;
			int countR = 0;
			int rand = -2;
			int value = 1;
			const int rec = (value * 2 + 1) * (value * 2 + 1);
			const int recWidth = value * channel;
			for (y = 0; y < height; y++)
			{
				for (x = 0; x < Stride; x += channel)
				{
					for (y2 = -value; y2 <= value; y2++)
					{
						for (x2 = -recWidth; x2 <= recWidth; x2 += channel)
						{
							if (y + y2 < 0 || y + y2 >= height || x + x2 < 0 || x + x2 >= Stride)
								continue;
							countB += fp2[y + y2][x + x2];
							countG += fp2[y + y2][x + x2 + 1];
							countR += fp2[y + y2][x + x2 + 2];
						}
					}
					fp2[y][x] = countB / ((value * 2 + 1) * (value * 2 + 1));
					fp2[y][x + 1] = countG / ((value * 2 + 1) * (value * 2 + 1));
					fp2[y][x + 2] = countR / ((value * 2 + 1) * (value * 2 + 1));
					countB = countG = countR = 0;
				}
			}
			//int vW = 1;
			//int vH = 4;
			int recWidth2 = vW * channel;
			for (y = 0; y < height; y++)
			{
				for (x = 0; x < Stride; x += channel)
				{
					for (y2 = -vH; y2 <= vH; y2++)
					{
						for (x2 = -recWidth2; x2 <= recWidth2; x2 += channel)
						{
							if (y + y2 < 0 || y + y2 >= height || x + x2 < 0 || x + x2 >= Stride)
								continue;
							countB += fp2[y + y2][x + x2];
							countG += fp2[y + y2][x + x2 + 1];
							countR += fp2[y + y2][x + x2 + 2];
						}
					}
					countB = (countB)+(fp2[y][x] * -(vW * 2 + 1) * (vH * 2 + 1));
					countG = (countG)+(fp2[y][x + 1] * -(vW * 2 + 1) * (vH * 2 + 1));
					countR = (countR)+(fp2[y][x + 2] * -(vW * 2 + 1) * (vH * 2 + 1));
					if (countB > 255)countB = 255;
					if (countG > 255)countG = 255;
					if (countR > 255)countR = 255;
					if (countB < 0)countB = 0;
					if (countG < 0)countG = 0;
					if (countR < 0)countR = 0;
					fp[y][x] = 255 - (countB + countG + countR) / 3;
					fp[y][x + 1] = fp[y][x];
					fp[y][x + 2] = fp[y][x];
					countB = countG = countR = 0;
				}
			}
			delete[] fp;
			delete[] fp2;
		}
		//放射線
		void sharptest00(unsigned char* ptr, unsigned char* ptr2, int width, int height, int channel, int value)
		{
			unsigned char** fp = new unsigned char* [height];
			unsigned char** fp2 = new unsigned char* [height];
			const int recSize = ((value * 2 + 1) * (value * 2 + 1));
			const int recWidth = value * channel;
			const int rec = (value * 2 + 1) * (value * 2 + 1);
			int Stride = width * channel, x = 0, y = 0;
			for (int j = 0; j < height; j++)
				fp[j] = ptr + (Stride * j);
			for (int j = 0; j < height; j++)
				fp2[j] = ptr2 + (Stride * j);
			int x2 = 0; int y2 = 0;
			int countB = 0;
			int countG = 0;
			int countR = 0;
			int rand = -2;
			/*for (y = 0; y < height; y++)
			{
				for (x = 0; x < Stride; x += channel)
				{
					for (y2 = -value; y2 <= value; y2++)
					{
						for (x2 = -recWidth; x2 <= recWidth; x2 += channel)
						{
							if (y + y2 < 0 || y + y2 >= height || x + x2 < 0 || x + x2 >= Stride)
								continue;
							countB += fp2[y + y2][x + x2];
							countG += fp2[y + y2][x + x2 + 1];
							countR += fp2[y + y2][x + x2 + 2];
						}
					}
					//rand = std::rand() % 10;
					//if (rand == (value * 2 + 1) * (value * 2 + 1))rand = 1;
					fp[y][x] = countB / ((value * 2 + 1) * (value * 2 + 1) - rand);
					fp[y][x + 1] = countG / ((value * 2 + 1) * (value * 2 + 1) - rand);
					fp[y][x + 2] = countR / ((value * 2 + 1) * (value * 2 + 1) - rand);
					countB = countG = countR = 0;
				}
			}*/
			for (y = 0; y < height; y++)
			{
				for (x = 0; x < Stride; x += channel)
				{
					for (y2 = -value; y2 <= value; y2++)
					{
						for (x2 = -recWidth; x2 <= recWidth; x2 += channel)
						{
							if (y + y2 < 0 || y + y2 >= height || x + x2 < 0 || x + x2 >= Stride)
								continue;
							countB += fp2[y + y2][x + x2];
							countG += fp2[y + y2][x + x2 + 1];
							countR += fp2[y + y2][x + x2 + 2];
						}
					}
					countB = (countB)+(fp[y][x] * -rec);
					countG = (countG)+(fp[y][x + 1] * -rec);
					countR = (countR)+(fp[y][x + 2] * -rec);
					if (countB > 255)countB = 255;
					if (countG > 255)countG = 255;
					if (countR > 255)countR = 255;
					if (countB < 0)countB = 0;
					if (countG < 0)countG = 0;
					if (countR < 0)countR = 0;
					fp[y][x] = 255 - countB;
					fp[y][x + 1] = 255 - countG;
					fp[y][x + 2] = 255 - countR;
					//countB = countG = countR = 0;
				}
			}
			delete[] fp;
			delete[] fp2;
		}
		//污漬
		void sharptest0(unsigned char* ptr, unsigned char* ptr2, int width, int height, int channel, int value)
		{
			unsigned char** fp = new unsigned char* [height];
			unsigned char** fp2 = new unsigned char* [height];
			const int recSize = ((value * 2 + 1) * (value * 2 + 1));
			const int recWidth = value * channel;
			const int rec = (value * 2 + 1) * (value * 2 + 1) + 2;
			int Stride = width * channel, x = 0, y = 0;
			for (int j = 0; j < height; j++)
				fp[j] = ptr + (Stride * j);
			for (int j = 0; j < height; j++)
				fp2[j] = ptr2 + (Stride * j);
			int x2 = 0; int y2 = 0;
			int countB = 0;
			int countG = 0;
			int countR = 0;
			int rand = -0;
			for (y = 0; y < height; y++)
			{
				for (x = 0; x < Stride; x += channel)
				{
					for (y2 = -value; y2 <= value; y2++)
					{
						for (x2 = -recWidth; x2 <= recWidth; x2 += channel)
						{
							if (y + y2 < 0 || y + y2 >= height || x + x2 < 0 || x + x2 >= Stride)
								continue;
							countB += fp2[y + y2][x + x2];
							countG += fp2[y + y2][x + x2 + 1];
							countR += fp2[y + y2][x + x2 + 2];
						}
					}
					//rand = std::rand() % 10;
					//if (rand == (value * 2 + 1) * (value * 2 + 1))rand = 1;
					fp[y][x] = countB / ((value * 2 + 1) * (value * 2 + 1) - rand);
					fp[y][x + 1] = countG / ((value * 2 + 1) * (value * 2 + 1) - rand);
					fp[y][x + 2] = countR / ((value * 2 + 1) * (value * 2 + 1) - rand);
				}
			}
			for (y = 0; y < height; y++)
			{
				for (x = 0; x < Stride; x += channel)
				{
					for (y2 = -value; y2 <= value; y2++)
					{
						for (x2 = -recWidth; x2 <= recWidth; x2 += channel)
						{
							if (y + y2 < 0 || y + y2 >= height || x + x2 < 0 || x + x2 >= Stride)
								continue;
							countB += fp2[y + y2][x + x2];
							countG += fp2[y + y2][x + x2 + 1];
							countR += fp2[y + y2][x + x2 + 2];
						}
					}
					countB = (countB)+(fp[y][x] * -rec);
					countG = (countG)+(fp[y][x + 1] * -rec);
					countR = (countR)+(fp[y][x + 2] * -rec);
					if (countB > 255)countB = 255;
					if (countG > 255)countG = 255;
					if (countR > 255)countR = 255;
					if (countB < 0)countB = 0;
					if (countG < 0)countG = 0;
					if (countR < 0)countR = 0;
					fp[y][x] = countB;
					fp[y][x + 1] = countG;
					fp[y][x + 2] = countR;
					countB = countG = countR = 0;
				}
			}
			delete[] fp;
			delete[] fp2;
		}
		void sharp2222(unsigned char* ptr, unsigned char* ptr2, int width, int height, int channel, int value)
		{
			unsigned char** fp = new unsigned char* [height];
			unsigned char** fp2 = new unsigned char* [height];
			const int recSize = ((value * 2 + 1) * (value * 2 + 1));
			const int recWidth = value * channel;
			int Stride = width * channel, x = 0, y = 0;
			for (int j = 0; j < height; j++)
				fp[j] = ptr + (Stride * j);
			for (int j = 0; j < height; j++)
				fp2[j] = ptr2 + (Stride * j);
			int x2 = 0; int y2 = 0;
			int countB = 0;
			int countG = 0;
			int countR = 0;
			for (y = 0; y < height; y++)
			{
				for (x = 0; x < Stride; x += channel)
				{
					for (y2 = -value; y2 <= value; y2++)
					{
						for (x2 = -recWidth; x2 <= recWidth; x2 += channel)
						{
							if (y + y2 < 0 || y + y2 >= height || x + x2 < 0 || x + x2 >= Stride)
								continue;
							countB += fp2[y + y2][x + x2];
							countG += fp2[y + y2][x + x2 + 1];
							countR += fp2[y + y2][x + x2 + 2];
						}
					}
					countB = countB * -1 + fp[y][x] * 9;
					countG = countG * -1 + fp[y][x + 1] * 9;
					countR = countR * -1 + fp[y][x + 2] * 9;
					if (countB > 255)countB = 255;
					if (countG > 255)countG = 255;
					if (countR > 255)countR = 255;
					if (countB < 0)countB = 0;
					if (countG < 0)countG = 0;
					if (countR < 0)countR = 0;
					fp[y][x] = countB;
					fp[y][x + 1] = countG;
					fp[y][x + 2] = countR;
					countB = countG = countR = 0;
				}
			}
			delete[] fp;
			delete[] fp2;
		}
	};
};


/* //淺綠特效
void blurry(unsigned char* ptr, int width, int height, int channel, int value)
		{
			unsigned char** fp = new unsigned char* [height];
			int Stride = width * channel, x = 0, y = 0;
			for (int j = 0; j < height; j++)
				fp[j] = ptr + (Stride * j);
			int x2 = 0; int y2 = 0;
			int countB = 0;
			int countG = 0;
			int countR = 0;
			for (y = 0; y < height; y++)
			{
				for (x = 0; x < Stride; x += channel)
				{
					for (y2 = -value; y2 <= value; y2++)
					{
						for (x2 = -value; x2 <= value; x2++)
						{
							if (y + y2 < 0 || y + y2 >= height || x + x2 < 0 || x + x2 >= Stride)
								continue;
							countB += fp[y+y2][x+x2];
							countG += fp[y+y2][x+x2+1];
							countR += fp[y+y2][x+x2+2];
						}
					}
					fp[y][x] = countB / ((value*2+1)* (value * 2 + 1));
					fp[y][x + 1] = countG / ((value * 2 + 1) * (value * 2 + 1));
					fp[y][x + 2] = countR / ((value * 2 + 1) * (value * 2 + 1));
					countB = countG = countR = 0;
				}
			}
			delete[] fp;

		}
*/
/*//墨綠特效
void blurry(unsigned char* ptr, int width, int height, int channel, int value)
		{
			unsigned char** fp = new unsigned char* [height];
			int Stride = width * channel, x = 0, y = 0;
			for (int j = 0; j < height; j++)
				fp[j] = ptr + (Stride * j);
			int x2 = 0; int y2 = 0;
			int countB = 0;
			int countG = 0;
			int countR = 0;
			for (y = 0; y < height; y++)
			{
				for (x = 0; x < Stride; x += channel)
				{
					for (y2 = -value; y2 <= value; y2++)
					{
						for (x2 = -value; x2 <= value*channel; x2+=channel)
						{
							if (y + y2 < 0 || y + y2 >= height || x + x2 < 0 || x + x2 >= Stride)
								continue;
							countB += fp[y + y2][x + x2];
							countG += fp[y + y2][x + x2 + 1];
							countR += fp[y + y2][x + x2 + 2];
						}
					}
					fp[y][x] = countB / ((value * 2 + 1) * (value * 2 + 1));
					fp[y][x + 1] = countG / ((value * 2 + 1) * (value * 2 + 1));
					fp[y][x + 2] = countR / ((value * 2 + 1) * (value * 2 + 1));
					countB = countG = countR = 0;
				}
			}
			delete[] fp;

		}
*/
/*麵包版
void tile(unsigned char* ptr, unsigned char* ptr2, int width, int height, int channel, int value)
{
	int value2 = 5;
	unsigned char** fp = new unsigned char* [height];
	unsigned char** fp2 = new unsigned char* [height];
	const int recSize = value * value;
	int Stride = width * channel, x = 0, y = 0;
	for (int j = 0; j < height; j++)
		fp[j] = ptr + (Stride * j);
	for (int j = 0; j < height; j++)
		fp2[j] = ptr2 + (Stride * j);
	int x2 = 0; int y2 = 0;
	int B = 0;
	int G = 0;
	int R = 0;
	for (y = 0; y < height; y += value)
	{
		for (x = 0; x < Stride; x += channel * value)
		{
			for (y2 = 0; y2 < value; y2++)
			{
				for (x2 = 0; x2 < value2 * channel; x2 += channel)
				{
					fp[y + y2][x + x2] = fp2[y + y2][x + x2] * (x2 / channel) / value2 + 255 * (value2 - x2 / 4) / value2;
					fp[y + y2][x + x2 + 1] = fp2[y + y2][x + x2 + 1] * (x2 / channel) / value2 + 255 * (value2 - x2 / 4) / value2;
					fp[y + y2][x + x2 + 2] = fp2[y + y2][x + x2 + 2] * (x2 / channel) / value2 + 255 * (value2 - x2 / 4) / value2;
				}
			}
			for (y2 = 0; y2 < value2; y2++)
			{
				for (x2 = 0; x2 < value * channel; x2 += channel)
				{
					B = fp2[y + y2][x + x2] * (x2 / channel) / value2 + 255 * (value2 - x2 / 4) / value2;
					G = fp2[y + y2][x + x2 + 1] * (x2 / channel) / value2 + 255 * (value2 - x2 / 4) / value2;
					R = fp2[y + y2][x + x2 + 2] * (x2 / channel) / value2 + 255 * (value2 - x2 / 4) / value2;
					if (B > 255)B = 255;
					if (B < 0) B = 0;
					if (G > 255)G = 255;
					if (G < 0) G = 0;
					if (R > 255)R = 255;
					if (R < 0) R = 0;
					fp[y + y2][x + x2] = B;
					fp[y + y2][x + x2 + 1] = G;
					fp[y + y2][x + x2 + 2] = R;

				}
			}
		}
	}*/
	/*void  inline blurry2(unsigned char* ptr, unsigned char* ptr2, int width, int height, int channel, int value)
			{
				unsigned char** fp = new unsigned char* [height];
				unsigned char** fp2 = new unsigned char* [height];
				const int recSize = ((value * 2 + 1) * (value * 2 + 1));
				const int recWidth = value * channel;
				int Stride = width * channel, x = 0, y = 0;
				for (int j = 0; j < height; j++)
					fp2[j] = ptr2 + (Stride * j);
				for (int j = 0; j < height; j++)
					fp[j] = ptr + (Stride * j);
				int x2 = 0; int y2 = 0;
				int countB = 0;
				int countG = 0;
				int countR = 0;
				int reduceRecSize = 0;
				int reduceRecSize1 = 0;
				int reduceRecSize2 = 0;
				int boxSize = 0;
				for (y = 0; y < height; y++)
				{
					for (x = 0; x < Stride; x += channel)
					{
						if (x == 0)
						{
							for (y2 = -value; y2 <= value; y2++)
							{
								for (x2 = -recWidth; x2 <= recWidth; x2 += channel)
								{
									if (y + y2 < 0 || y + y2 >= height || x + x2 < 0 || x + x2 >= Stride)
									{
										reduceRecSize2++;
										continue;
									}
									countB += fp2[y + y2][x + x2];
									countG += fp2[y + y2][x + x2 + 1];
									countR += fp2[y + y2][x + x2 + 2];
								}
							}
							fp[y][x] = countB / (recSize - reduceRecSize2);
							fp[y][x + 1] = countG / (recSize - reduceRecSize2);
							fp[y][x + 2] = countR / (recSize - reduceRecSize2);
							reduceRecSize2 = 0;
						}
						else
						{
							for (y2 = -value, x2 = -recWidth - channel; y2 <= value; y2++)
							{
								if (y + y2 < 0 || y + y2 >= height || x + x2 < 0 || x + x2 >= Stride)
								{
									reduceRecSize1++;
									continue;
								}
								countB -= fp2[y + y2][x + x2];
								countG -= fp2[y + y2][x + x2 + 1];
								countR -= fp2[y + y2][x + x2 + 2];
							}
							for (y2 = -value, x2 = recWidth; y2 <= value; y2++)
							{
								if (y + y2 < 0 || y + y2 >= height || x + x2 < 0 || x + x2 >= Stride)
								{
									reduceRecSize++;
									continue;
								}
								countB += fp2[y + y2][x + x2];
								countG += fp2[y + y2][x + x2 + 1];
								countR += fp2[y + y2][x + x2 + 2];
							}
							fp[y][x] = countB / (recSize + reduceRecSize - reduceRecSize1);
							fp[y][x + 1] = countG / (recSize + reduceRecSize - reduceRecSize1);
							fp[y][x + 2] = countR / (recSize + reduceRecSize - reduceRecSize1);
						}
						reduceRecSize = 0;
						reduceRecSize1 = 0;
					}
					countB = countG = countR = 0;
					reduceRecSize2 = 0;
				}
				delete[] fp;
			}
			static void blurry3B(unsigned char** fp, unsigned char** fp2, int width, int height, int channel, int value, int half)
			{
				int Stride = width * channel;
				const int recSize = ((value * 2 + 1) * (value * 2 + 1));
				const int recWidth = value * channel;
				int x = 0; int y = 0;
				int x2 = 0; int y2 = 0;
				int countB = 0;
				int countG = 0;
				int countR = 0;
				int reduceRecSize = 0;
				int reduceRecSize1 = 0;
				int reduceRecSize2 = 0;
				int boxSize = 0;
				int heightBegin = 0;
				int heightEnd = height;
				if (half == 0)heightBegin = 0; else heightBegin = height / 2;
				if (half == 0)heightEnd = height / 2; else heightEnd = height;
				for (y = heightBegin; y < heightEnd; y++)
				{
					for (x = 0; x < Stride; x += channel)
					{
						if (x == 0)
						{
							for (y2 = -value; y2 <= value; y2++)
							{
								for (x2 = -recWidth; x2 <= recWidth; x2 += channel)
								{
									if (y + y2 < 0 || y + y2 >= height || x + x2 < 0 || x + x2 >= Stride)
									{
										reduceRecSize2++;
										continue;
									}
									countB += fp2[y + y2][x + x2];
								}
							}
							fp[y][x] = countB / (recSize - reduceRecSize2);
							reduceRecSize2 = 0;
						}
						else
						{
							for (y2 = -value, x2 = -recWidth - channel; y2 <= value; y2++)
							{
								if (y + y2 < 0 || y + y2 >= height || x + x2 < 0 || x + x2 >= Stride)
								{
									reduceRecSize1++;
									continue;
								}
								countB -= fp2[y + y2][x + x2];
							}
							for (y2 = -value, x2 = recWidth; y2 <= value; y2++)
							{
								if (y + y2 < 0 || y + y2 >= height || x + x2 < 0 || x + x2 >= Stride)
								{
									reduceRecSize++;
									continue;
								}
								countB += fp2[y + y2][x + x2];
							}
							fp[y][x] = countB / (recSize + reduceRecSize - reduceRecSize1);
						}
						reduceRecSize = 0;
						reduceRecSize1 = 0;
					}
					countB = countG = countR = 0;
					reduceRecSize2 = 0;
				}
			}
			static void blurry3G(unsigned char** fp, unsigned char** fp2, int width, int height, int channel, int value, int half)
			{
				int Stride = width * channel;
				const int recSize = ((value * 2 + 1) * (value * 2 + 1));
				const int recWidth = value * channel;
				int x = 0; int y = 0;
				int x2 = 0; int y2 = 0;
				int countB = 0;
				int countG = 0;
				int countR = 0;
				int reduceRecSize = 0;
				int reduceRecSize1 = 0;
				int reduceRecSize2 = 0;
				int boxSize = 0;
				int heightBegin = 0;
				int heightEnd = height;
				if (half == 0)heightBegin = 0; else heightBegin = height / 2;
				if (half == 0)heightEnd = height / 2; else heightEnd = height;
				for (y = heightBegin; y < heightEnd; y++)
				{
					for (x = 0; x < Stride; x += channel)
					{
						if (x == 0)
						{
							for (y2 = -value; y2 <= value; y2++)
							{
								for (x2 = -recWidth; x2 <= recWidth; x2 += channel)
								{
									if (y + y2 < 0 || y + y2 >= height || x + x2 < 0 || x + x2 >= Stride)
									{
										reduceRecSize2++;
										continue;
									}
									countG += fp2[y + y2][x + x2 + 1];
								}
							}
							fp[y][x + 1] = countG / (recSize - reduceRecSize2);
							reduceRecSize2 = 0;
						}
						else
						{
							for (y2 = -value, x2 = -recWidth - channel; y2 <= value; y2++)
							{
								if (y + y2 < 0 || y + y2 >= height || x + x2 < 0 || x + x2 >= Stride)
								{
									reduceRecSize1++;
									continue;
								}
								countG -= fp2[y + y2][x + x2 + 1];
							}
							for (y2 = -value, x2 = recWidth; y2 <= value; y2++)
							{
								if (y + y2 < 0 || y + y2 >= height || x + x2 < 0 || x + x2 >= Stride)
								{
									reduceRecSize++;
									continue;
								}
								countG += fp2[y + y2][x + x2 + 1];
							}
							fp[y][x + 1] = countG / (recSize + reduceRecSize - reduceRecSize1);
						}
						reduceRecSize = 0;
						reduceRecSize1 = 0;
					}
					countB = countG = countR = 0;
					reduceRecSize2 = 0;
				}
			}
			static void blurry3R(unsigned char** fp, unsigned char** fp2, int width, int height, int channel, int value, int half)
			{
				int Stride = width * channel;
				const int recSize = ((value * 2 + 1) * (value * 2 + 1));
				const int recWidth = value * channel;
				int x = 0; int y = 0;
				int x2 = 0; int y2 = 0;
				int countB = 0;
				int countG = 0;
				int countR = 0;
				int reduceRecSize = 0;
				int reduceRecSize1 = 0;
				int reduceRecSize2 = 0;
				int boxSize = 0;
				int heightBegin = 0;
				int heightEnd = height;
				if (half == 0)heightBegin = 0; else heightBegin = height / 2;
				if (half == 0)heightEnd = height / 2; else heightEnd = height;
				for (y = heightBegin; y < heightEnd; y++)
				{
					for (x = 0; x < Stride; x += channel)
					{
						if (x == 0)
						{
							for (y2 = -value; y2 <= value; y2++)
							{
								for (x2 = -recWidth; x2 <= recWidth; x2 += channel)
								{
									if (y + y2 < 0 || y + y2 >= height || x + x2 < 0 || x + x2 >= Stride)
									{
										reduceRecSize2++;
										continue;
									}
									countR += fp2[y + y2][x + x2 + 2];
								}
							}
							fp[y][x + 2] = countR / (recSize - reduceRecSize2);
							reduceRecSize2 = 0;
						}
						else
						{
							for (y2 = -value, x2 = -recWidth - channel; y2 <= value; y2++)
							{
								if (y + y2 < 0 || y + y2 >= height || x + x2 < 0 || x + x2 >= Stride)
								{
									reduceRecSize1++;
									continue;
								}
								countR -= fp2[y + y2][x + x2 + 2];
							}
							for (y2 = -value, x2 = recWidth; y2 <= value; y2++)
							{
								if (y + y2 < 0 || y + y2 >= height || x + x2 < 0 || x + x2 >= Stride)
								{
									reduceRecSize++;
									continue;
								}
								countR += fp2[y + y2][x + x2 + 2];
							}
							fp[y][x + 2] = countR / (recSize + reduceRecSize - reduceRecSize1);
						}
						reduceRecSize = 0;
						reduceRecSize1 = 0;
					}
					countB = countG = countR = 0;
					reduceRecSize2 = 0;
				}
			}*/