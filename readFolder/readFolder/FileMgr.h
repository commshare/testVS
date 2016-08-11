#pragma once
#include <vector>
#include <iostream>

using namespace std;
class FileMgr{
public:
	vector<string> getFiles(string cate_dir);
	int readFolderFiles(string folder, vector<string> &files, vector<string> &filesname, string format,bool outputName);
	void  showFiles(vector<string> files);
};