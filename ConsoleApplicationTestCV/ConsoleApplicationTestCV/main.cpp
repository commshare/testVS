#include<iostream>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"


using namespace cv;//it is cv

#include"cv_lib_turn.h"

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
	//ȥɫ
	cvtColor(src, gray0, CV_BGR2GRAY);
	//��ɫ
	addWeighted(gray0, -1, NULL, 0, 255, gray1);
	//��˹ģ��,��˹�˵�Size������Ч���й�
	GaussianBlur(gray1, gray1, Size(11, 11), 0);

	//�ںϣ���ɫ����
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
	//imshow("����", img);
	//waitKey();
	//imwrite("D:/����.jpg", img);
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
		string::size_type idx = name.find(".jpg");

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
			printf("��������ȷ������\n ");
			printf("-m ѡ����ģʽ 1Ϊ���� 2Ϊ��ͨ");
			printf("-b 1Ϊ���� 2Ϊ��ͨ ���������ļ���");
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
					/*д��ͼƬ*/
					imwrite(dstfolder, dst);
				}


			}




		}
		else{
			printf("��������ȷ������\n ");
			printf("-m ѡ����ģʽ 1Ϊ���� 2Ϊ��ͨ");
			printf("-b 1Ϊ���� 2Ϊ��ͨ ���������ļ���");
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
#include <windows.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>  
#ifdef WIN32
#include <direct.h>
#include <io.h>
#endif
using namespace std;
using namespace cv;
/*
ԭ�����Ƿ��غ�׺�����ڼ�һ��.�󷵻�
*/
std::string GetFilePosfix(const char* path)
{
	const char* pos = strrchr(path, '.');
	if (pos)
	{
		std::string str(pos + 1);
		//1.ת��ΪСд  
		//http://blog.csdn.net/infoworld/article/details/29872869  
		std::transform(str.begin(), str.end(), str.begin(), ::tolower);
		//std:cout << "str[" + str + "]" << endl;
		return "." + str;
	}
	return std::string();
}
//��ȡ�ض���ʽ���ļ���
void GetAllFormatFiles(string path, vector<string>& files, vector<string>& filesname, string format)
{
	//�ļ����  
	/*refer to http://blog.csdn.net/linj_m/article/details/48046913 for exception*/
	intptr_t   hFile = 0;
	//�ļ���Ϣ  
	struct _finddata_t fileinfo;
	string p;
#if 0
	if ((hFile = _findfirst(p.assign(path).append("\\*" + format).c_str(), &fileinfo)) != -1)
#else
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
#endif
	{
		do
		{
			string name = fileinfo.name;

			/*�������ļ�������*/
			//cout << "fileinfo.name[" << name<< "]" << endl;
			/*�������Ŀ¼*/
			if ((fileinfo.attrib &  _A_SUBDIR))
			{
				//cout << "sub dir" << endl;
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
				{
					//files.push_back(p.assign(path).append("\\").append(fileinfo.name) );
					GetAllFormatFiles(p.assign(path).append("\\").append(fileinfo.name), files, filesname, format);
				}
			}
			else
			{
				//cout << "file" << endl;
				if (GetFilePosfix(name.c_str()) == format){
					//cout << "got [" + name + "]" << endl;
					filesname.push_back(fileinfo.name);
					/*�ļ���ȫ·����ͨ��folder����+\\+�ļ�����ƴ�Ӷ���*/
					files.push_back(p.assign(path).append("\\").append(fileinfo.name));
				}
				//else
				//	cout << "not dll" << endl;

			}
		} while (_findnext(hFile, &fileinfo) == 0);

		_findclose(hFile);
	}
	else
		cout << "hFile error" << endl;
}
int readFolderFiles(string folder, vector<string> &files/*�õ�ȫ·��*/, vector<string> &filesname/*�������ļ���*/, string format, bool outputName){

	char * distAll = "AllFilesName.txt";
	GetAllFormatFiles(folder, files, filesname, format);
#if 1
	/*�ļ����д��txt��*/
	ofstream ofn(distAll);
	/*��ӡ�ļ�����·���������ȫ·��*/
	int size = files.size();
	ofn << size << endl;
	for (int i = 0; i < size; i++)
	{
		if (outputName)	/*������ļ���*/
			ofn << filesname[i] << endl;
		else
			ofn << files[i] << endl;
		//cout << "output [" + files[i] + "]" << endl;
	}
	ofn.close();
#endif
	return 0;
}
/*
F:\\RECORD\\cut F:\\RECORD\\cutout
*/
int main(int argc, char *argv[])
{
	char* folder = argv[1];
	printf("src folder [%s]\n",folder);
	string orgfolder = folder;

	    std::vector<std::string>fileName;
		std::vector<std::string>names;

		readFolderFiles(orgfolder,fileName,names,".jpg",true);
	cout << "file num[" <<fileName.size() << "]";
	for (int i = 0; i < fileName.size(); i++){
		string dstfolder = argv[2];
#if 0
		string orgf = orgfolder.append(fileName[i]);
		cout << "orgf=" << orgf << endl;
#else
		string orgf = orgfolder + "\\" + names[i];
		cout << "orgf=" << orgf << endl;

#endif

		/*�����ļ�*/
		IplImage *image = cvLoadImage(orgf.c_str());// ("F:\\RECORD\\cut\\������.jpg");

		if (image == NULL)
			cout << "error load image" << endl;
		else{
			//cvShowImage("image1", image);
		}

		//��ROI����ͼ�񱣴���image��:���Ͻ�x�����Ͻ�y�����ο�ȡ��߶�
		cvSetImageROI(image, cvRect(45, 145, 710, 686));
		//cvShowImage("imageROI", image);
#if 0
		//ִ��cvSetImageROI����֮����ʾimageͼ����ֻ��ʾROI��ʶ��һ���֣����ı���ָ��image��
		//�������Ծɱ�����ԭ��ͼ�����Ϣ����ִ����һ��cvResetImageROI(image),֮��imageָʾԭ����ͼ����Ϣ��
		cvResetImageROI(image);
		cvShowImage("image2", image);
#endif
		// ������ͼ��
		IplImage *result;
		result = cvCreateImage(cvSize(710, 686), image->depth, image->nChannels);
		cvCopy(image, result);
		cvResetImageROI(image);
#if TEST
		cvShowImage("image copy out", result);
		char *outfilename = "F:\\RECORD\\cutout\\teswt.jpg";
		if (!cvSaveImage(outfilename, result)) printf("Could not save: %s\n", outfilename);
#endif

#if 0
		dstfolder.append(fileName[i]);
#else
		string outfile = dstfolder + "\\" + names[i];
#endif
		if (!cvSaveImage(outfile.c_str(), result)) printf("Could not save: %s\n", outfile.c_str());
	}

	cvWaitKey(0);
	getchar();
	return 0;
}

/*
opencv_ml2413d.lib
opencv_calib3d2413d.lib
opencv_contrib2413d.lib
opencv_core2413d.lib
opencv_features2d2413d.lib
opencv_flann2413d.lib
opencv_gpu2413d.lib
opencv_highgui2413d.lib
opencv_imgproc2413d.lib
opencv_legacy2413d.lib
opencv_objdetect2413d.lib
opencv_ts2413d.lib
opencv_video2413d.lib
opencv_nonfree2413d.lib
opencv_ocl2413d.lib
opencv_photo2413d.lib
opencv_stitching2413d.lib
opencv_superres2413d.lib
opencv_videostab2413d.lib
opencv_objdetect2413.lib
opencv_ts2413.lib
opencv_video2413.lib
opencv_nonfree2413.lib
opencv_ocl2413.lib
opencv_photo2413.lib
opencv_stitching2413.lib
opencv_superres2413.lib
opencv_videostab2413.lib
opencv_calib3d2413.lib
opencv_contrib2413.lib
opencv_core2413.lib
opencv_features2d2413.lib
opencv_flann2413.lib
opencv_gpu2413.lib
opencv_highgui2413.lib
opencv_imgproc2413.lib
opencv_legacy2413.lib
opencv_ml2413.lib
kernel32.lib
user32.lib
gdi32.lib
winspool.lib
comdlg32.lib
advapi32.lib
shell32.lib
ole32.lib
oleaut32.lib
uuid.lib
odbc32.lib
odbccp32.lib


*/