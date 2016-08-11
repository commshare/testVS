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
	/*Ŀ¼�µ��ļ�����Ŀ¼�����ļ����� ���Ƶ���һ��Ŀ¼�����ݹ���Ŀ¼*/
	int copyFileToDir(vector<string> filesname, string src_dir, string dst_dir);
	/*�ļ���ȫ·����ֱ�Ӹ��Ƶ�����һ��Ŀ¼��,��Ҫ�ṩ�ļ�������dstĿ¼�����ļ�*/
	int copyFileToDir(vector<string> files, vector<string> filenames, string dst_dir);
};