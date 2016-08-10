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
	string folder = "E:\\VQ\\GY_WZ\\gourp_15";//"F:\\RECORD\\data\\test";
	vector<string> files, filesname;
	std::cout << "get all files from :" << folder << std::endl;

	if (filemgr){
#if 0
		filemgr->getFiles(folder);
#else
		cout << "readFolderFiles" << endl;
		filemgr->readFolderFiles(folder, files, filesname, ".ts");
#endif
	}
	else
		cout << "filemgr is null" << endl;
	cout << "done" << endl;
#if 1
	if (!files.empty()){
		filemgr->showFiles(files);
	}
	else
		cout << "files is empty" << endl;
#endif
	return 0;
}

