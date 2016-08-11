#pragma once
#include <vector>
#include <iostream>

using namespace std;
class FileMgr{
public:
	vector<string> getFiles(string cate_dir);
	int readFolderFiles(string folder, vector<string> &files, vector<string> &filesname, string format,bool outputName);
	void  showFiles(vector<string> files);
	int FileMgr::copyFile(const char*src_file, const char*dst_file);
	/*目录下的文件根据目录名和文件名， 复制到另一个目录，不递归子目录*/
	int copyFileToDir(vector<string> filesname, string src_dir, string dst_dir);
	/*文件（全路径）直接复制到另外一个目录下,需要提供文件名以在dst目录生成文件*/
	int copyFileToDir(vector<string> files, vector<string> filenames, string dst_dir);
};