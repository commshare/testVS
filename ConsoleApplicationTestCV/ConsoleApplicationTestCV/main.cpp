#include<iostream>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"


using namespace cv;//it is cv

int main1(){

#if 0 //test ok
	//read an image
	Mat img = imread("F:\\pic.jpg");
	if (img.empty()){
		std::cout << "imread fail" << std::endl;
	}
	//create a window
	namedWindow("test");
	//show the image in window
	imshow("test",img);
	//wait 6000ms and the window shall close itself
	waitKey(6000);
#endif
	Mat image, finalImage;
	char * t1 = "D:\\img\\vlcsnap-2016-06-02-15h59m08s111.png";

	image = imread(t1, CV_LOAD_IMAGE_GRAYSCALE);
	if (image.empty()) {
		std::cout << "img read fail" << std::endl;
		return 1;
	}

	Laplacian(image, finalImage, CV_8U);

	int rows = finalImage.rows;
	int cols = finalImage.cols;
	char *pixels = reinterpret_cast<char *>(finalImage.data);
	int maxLap = -16777216; //just too small number
	for (int i = 0; i < (rows*cols); i++) {
		if (pixels[i] > maxLap)
			maxLap = pixels[i];
	}

	//You can fine tune maxLap by seeing its value
	//in my case 20 is optimal
	//std::cout << argv[1] << " " << maxLap << "\n";

	if (maxLap < 20) {
		//blurry
		std::cout << "blurry" << std::endl;
		return 0;
	}
	else {
		//sharp
		std::cout << "sharp" << std::endl;
		return 1;
	}

}

#include "opencv2\opencv.hpp"  
#include<iostream>
//#include <dirent.h>
/*
https://github.com/tronkko/dirent
*/
#include"include/dirent.h"
#include <stdio.h>

using namespace cv;
using namespace std;

/*
http://blog.csdn.net/yangtrees/article/details/9115321
*/

Mat sumiao(Mat src){

	int width = src.cols;
	int heigh = src.rows;
	Mat gray0, gray1;
	//去色
	cvtColor(src, gray0, CV_BGR2GRAY);
	//反色
	addWeighted(gray0, -1, NULL, 0, 255, gray1);
	//高斯模糊,高斯核的Size与最后的效果有关
	GaussianBlur(gray1, gray1, Size(11, 11), 0);

	//融合：颜色减淡
	Mat img(gray1.size(), CV_8UC1);
	for (int y = 0; y<heigh; y++)
	{

		uchar* P0 = gray0.ptr<uchar>(y);
		uchar* P1 = gray1.ptr<uchar>(y);
		uchar* P = img.ptr<uchar>(y);
		for (int x = 0; x<width; x++)
		{
			int tmp0 = P0[x];
			int tmp1 = P1[x];
			P[x] = (uchar)min((tmp0 + (tmp0*tmp1) / (256 - tmp1)), 255);
		}

	}
	//imshow("素描", img);
	//waitKey();
	//imwrite("D:/素描.jpg", img);
	return img;

}


Mat carton(Mat image){

	Mat gray;
	cvtColor(image, gray, CV_BGR2GRAY);
	const int MEDIAN_BLUR_FILTER_SIZE = 7;
	medianBlur(gray, gray, MEDIAN_BLUR_FILTER_SIZE);

	Mat edges;
	const int LAPLACIAN_FILTER_SIZE = 5;
	Laplacian(gray, edges, CV_8U, LAPLACIAN_FILTER_SIZE);

	Mat mask;
	const int EDGE_THRESHOLD = 90;
	threshold(edges, mask, EDGE_THRESHOLD, 255, THRESH_BINARY_INV);

	Size size = image.size();
	Size smallSize;
	smallSize.width = size.width / 2;
	smallSize.height = size.height / 2;
	Mat smallImg = Mat(smallSize, CV_8UC3);
	resize(image, smallImg, smallSize, 0, 0, INTER_LINEAR);

	Mat tmp = Mat(smallSize, CV_8UC3);
	int repetitions = 9;
	for (int i = 0; i < repetitions; i++)
	{
		int ksize = 9;
		double sigmaColor = 9;
		double sigmaSpace = 7;
		bilateralFilter(smallImg, tmp, ksize, sigmaColor, sigmaSpace);
		bilateralFilter(tmp, smallImg, ksize, sigmaColor, sigmaSpace);
	}

	Mat bigImg;
	resize(smallImg, bigImg, image.size(), 0, 0, INTER_LINEAR);

	Mat cartoon = Mat(image.size(), CV_8UC3);
	memset((char*)cartoon.data, 0, cartoon.step * cartoon.rows);

	bigImg.copyTo(cartoon, mask);

	return cartoon;


}


int ImageReader(const char* path, vector<string>& filenames){

	DIR * dp;
	struct dirent *filename;

	dp = opendir(path);
	if (!dp)
	{
		fprintf(stderr, "open directory error\n");
		return 0;
	}
	while (filename = readdir(dp))
	{
		string name = filename->d_name;
		string::size_type idx = name.find(".png");

		if (idx != string::npos){
			filenames.push_back(name);
		}
		else{
		}


	}
	closedir(dp);

	//sort(filenames.begin(), filenames.end(), less_second);


	return 0;

}

vector<string> getFileNames(const char *folder){

	vector<string> filename;
	ImageReader(folder, filename);

	return filename;
}

int main1(int argc, char** argv)
{

	try{
		if (argc <= 4){
			printf("请输入正确的命令\n ");
			printf("-m 选择风格模式 1为素描 2为卡通");
			printf("-b 1为素描 2为卡通 批量处理文件夹");
			return 0;
		}


		if (strcmp(argv[1], "-m") == 0){
			Mat src;
			Mat dst;
			if (strcmp(argv[2], "1") == 0){
				src = imread(argv[3], 1);
				dst = sumiao(src);

			}
			else if (strcmp(argv[2], "2") == 0){
				src = imread(argv[3], 1);
				dst = carton(src);
			}

			imwrite(argv[4], dst);
		}
		else if (strcmp(argv[1], "-b") == 0) {
			Mat src;
			Mat dst;
			char* folder = argv[3];
			vector<string>fileName = getFileNames(folder);
			if (strcmp(argv[2], "1") == 0){
				for (int i = 0; i < fileName.size(); i++){
					string orgfolder = folder;
					string dstfolder = argv[4];
					string orgf = orgfolder.append(fileName[i]);
					cout << "orgf=" << orgf << endl;
					src = imread(orgf, 1);
					dst = sumiao(src);
					dstfolder.append(fileName[i]);
					imwrite(dstfolder, dst);
				}

			}
			else if (strcmp(argv[2], "2") == 0){
				for (int i = 0; i < fileName.size(); i++){
					string orgfolder = folder;
					string dstfolder = argv[4];
					string orgf = orgfolder.append(fileName[i]);
					cout << "orgf=" << orgf << endl;
					src = imread(orgf, 1);
					dst = carton(src);
					dstfolder.append(fileName[i]);
					/*写入图片*/
					imwrite(dstfolder, dst);
				}


			}




		}
		else{
			printf("请输入正确的命令\n ");
			printf("-m 选择风格模式 1为素描 2为卡通");
			printf("-b 1为素描 2为卡通 批量处理文件夹");
			return 0;
		}


	}
	catch (exception& e){

		cout << "\nexception thrown!" << endl;
		cout << e.what() << endl;
	}

}

#include <iostream>
#include <highgui.h>

using namespace std;
using namespace cv;

int main()
{
	IplImage *image = cvLoadImage("F:\\RECORD\\cut\\靖慎怀.jpg");
	if (image == NULL)
		cout << "error load image" << endl;
	else{
		//cvShowImage("image1", image);
	}

	//将ROI区域图像保存在image中:左上角x、左上角y、矩形宽度、高度
	cvSetImageROI(image, cvRect(45,145, 710, 686));
	//cvShowImage("imageROI", image);
#if 0
	//执行cvSetImageROI（）之后显示image图像是只显示ROI标识的一部分，即改变了指针image，
	//但是它仍旧保留有原来图像的信息，在执行这一句cvResetImageROI(image),之后，image指示原来的图像信息。
	cvResetImageROI(image);
	cvShowImage("image2", image);
#endif
	// 创建子图像
	IplImage *result;
	result = cvCreateImage(cvSize(710,686), image->depth, image->nChannels);
	cvCopy(image, result);
	cvResetImageROI(image);
	cvShowImage("image copy out", result);
	char *outfilename = "F:\\RECORD\\cutout\\teswt.jpg";
	if (!cvSaveImage(outfilename, result)) printf("Could not save: %s\n", outfilename);
	cvWaitKey(0);
//	getchar();
	return 0;
}