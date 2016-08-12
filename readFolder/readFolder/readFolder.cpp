// readFolder.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <string> //for cout<<std::string
#include "stdafx.h"
#include"FileMgr.h"
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{

	//printf("hellow world\n");
	FileMgr *filemgr = new FileMgr();
	string test1 = "J:\\media\\XLIVE\\windows\\test";
	string folder = "J:\\MYSELF\\webcamoid\\libAvKys\\Plugins";//"E:\\VQ\\GY_WZ\\gourp_15";//"F:\\RECORD\\data\\test";
	string dst_folder = "J:\\MYSELF\\webcamoid\\lib\\avkys";//"J:\\media\\XLIVE\\windows\\webcamoid\\lib\\avkys";// "F:\\RECORD\\testoutdll";
	vector<string> files, filesname; /*Á½¸övector*/
	std::cout << "get all files from :" << folder << std::endl;
	string format = ".dll";

	if (filemgr){
#if 0
		filemgr->getFiles(folder);
#else
		cout << "readFolderFiles" << endl;
		bool outputFileName = false;
		filemgr->readFolderFiles(folder, files, filesname, format, outputFileName);
#endif
	}
	else
		cout << "filemgr is null" << endl;
	cout << "done" << endl;
#if 0
	if (!files.empty()){
		filemgr->showFiles(files);
	}
	else
		cout << "files is empty" << endl;
#endif
	filemgr->copyFileToDir(files, filesname, dst_folder);

	cout << "ALL DONE,print anything to exit " << endl;
	getchar();
	return 0;
}

