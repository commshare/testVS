#pragma once
#include <vector>


using namespace std;
class FileMgr{
public:
	vector<string> getFiles(string cate_dir);
	int readFolderFiles(string folder, vector<string> &files, vector<string> &filesname, string format);
	void  showFiles(vector<string> files);
};