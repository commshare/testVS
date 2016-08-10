/*
http://blog.5ibc.net/p/39392.html
*/

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
/* http://stackoverflow.com/questions/8679473/error-c2679-binary-no-operator-found-which-takes-a-right-hand-operand-of
The std::string operators are defined in the <string> header.
The header <string.h> is for C-style string functions.
*/
#include <string>
#include<algorithm> //for sort
#ifdef linux
#include <unistd.h>
#include <dirent.h>
#endif
#ifdef WIN32
#include <direct.h>
#include <io.h>
#endif
#include <fstream>  
#include"FileMgr.h"
using namespace std;

/**
* @function: ��ȡcate_dirĿ¼�µ������ļ���
* @param: cate_dir - string����
* @result��vector<string>����
*/
vector<string> FileMgr::getFiles(string cate_dir)
{
	cout << "folder is :" << cate_dir << endl;
	vector<string> files;//����ļ���

#ifdef WIN32
	_finddata_t file;
	long lf;
	//�����ļ���·��
	if ((lf = _findfirst(cate_dir.c_str(), &file)) == -1) {
		std::cout << "cate_dir" << cate_dir << " not found!!!" << std::endl;
	}
	else {
		while (_findnext(lf, &file) == 0) {
			//����ļ���
			cout << "file name:" << file.name << endl;

			if (strcmp(file.name, ".") == 0 || strcmp(file.name, "..") == 0)
				continue;
			files.push_back(file.name);
		}
	}
	_findclose(lf);
#endif

#ifdef linux
	DIR *dir;
	struct dirent *ptr;
	char base[1000];

	if ((dir = opendir(cate_dir.c_str())) == NULL)
	{
		perror("Open dir error...");
		exit(1);
	}

	while ((ptr = readdir(dir)) != NULL)
	{
		if (strcmp(ptr->d_name, ".") == 0 || strcmp(ptr->d_name, "..") == 0)    ///current dir OR parrent dir
			continue;
		else if (ptr->d_type == 8)    ///file
			//printf("d_name:%s/%s\n",basePath,ptr->d_name);
			files.push_back(ptr->d_name);
		else if (ptr->d_type == 10)    ///link file
			//printf("d_name:%s/%s\n",basePath,ptr->d_name);
			continue;
		else if (ptr->d_type == 4)    ///dir
		{
			files.push_back(ptr->d_name);
			/*
			memset(base,'\0',sizeof(base));
			strcpy(base,basePath);
			strcat(base,"/");
			strcat(base,ptr->d_nSame);
			readFileList(base);
			*/
		}
	}
	closedir(dir);
#endif

	/*sort����û�е�����������ʵ�ֵ��Ǵ�С����*/
	//���򣬰���С��������
	sort(files.begin(), files.end());
	return files;
}

#if 0
/*
http://stackoverflow.com/questions/306533/how-do-i-get-a-list-of-files-in-a-directory-in-c
*/
/* Returns a list of files in a directory (except the ones that begin with a dot) */

void GetFilesInDirectory(std::vector<string> &out, const string &directory)
{
#ifdef WIN32 //WINDOWS
	HANDLE dir;
	WIN32_FIND_DATA file_data;

	if ((dir = FindFirstFile((directory + "/*").c_str(), &file_data)) == INVALID_HANDLE_VALUE)
		return; /* No files found */

	do {
		const string file_name = file_data.cFileName;
		const string full_file_name = directory + "/" + file_name;
		const bool is_directory = (file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;

		if (file_name[0] == '.')
			continue;

		if (is_directory)
			continue;

		out.push_back(full_file_name);
	} while (FindNextFile(dir, &file_data));

	FindClose(dir);
#else
	DIR *dir;
	class dirent *ent;
	class stat st;

	dir = opendir(directory);
	while ((ent = readdir(dir)) != NULL) {
		const string file_name = ent->d_name;
		const string full_file_name = directory + "/" + file_name;

		if (file_name[0] == '.')
			continue;

		if (stat(full_file_name.c_str(), &st) == -1)
			continue;

		const bool is_directory = (st.st_mode & S_IFDIR) != 0;

		if (is_directory)
			continue;

		out.push_back(full_file_name);
	}
	closedir(dir);
#endif
} // GetFilesInDirectory

#endif

//////������Ǹ�������
//��ȡ���е��ļ���
void GetAllFiles(string path, vector<string>& files)
{

	long   hFile = 0;
	//�ļ���Ϣ  
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			if ((fileinfo.attrib &  _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
				{
					files.push_back(p.assign(path).append("\\").append(fileinfo.name));
					GetAllFiles(p.assign(path).append("\\").append(fileinfo.name), files);
				}
			}
			else
			{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
			}

		} while (_findnext(hFile, &fileinfo) == 0);

		_findclose(hFile);
	}

}

//��ȡ�ض���ʽ���ļ���
void GetAllFormatFiles(string path, vector<string>& files, vector<string>& filesname,string format)
{
	//�ļ����  
	/*refer to http://blog.csdn.net/linj_m/article/details/48046913 for exception*/
	intptr_t   hFile = 0;
	//�ļ���Ϣ  
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*" + format).c_str(), &fileinfo)) != -1)
	{
		do
		{
			cout << "fileinfo.name[" << fileinfo.name << "]" << endl;
			if ((fileinfo.attrib &  _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
				{
					//files.push_back(p.assign(path).append("\\").append(fileinfo.name) );
					GetAllFormatFiles(p.assign(path).append("\\").append(fileinfo.name), files, filesname, format);
				}
			}
			else
			{
				filesname.push_back(fileinfo.name);
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0);

		_findclose(hFile);
	}
}
int FileMgr::readFolderFiles(string folder, vector<string> &files, vector<string> &filesname, string format){
	string filePath = folder;// "testimages\\water";
	//vector<string> files;
	char * distAll = "AllFiles.txt";

	//��ȡ���е��ļ����������ļ����ļ�
	//GetAllFiles(filePath, files);

	//��ȡ���и�ʽΪjpg���ļ�
	//string format = ".jpg";
	GetAllFormatFiles(filePath, files, filesname, format);
#if 1
	/*�ļ����д��txt��*/
	ofstream ofn(distAll);
	/*��ӡ�ļ�·��*/
	int size = files.size();
	ofn << size << endl;
	for (int i = 0; i < size; i++)
	{
		ofn << filesname[i] << endl;
		//	cout << files[i] << endl;
	}
	ofn.close();
#endif
	return 0;
}
void  FileMgr::showFiles(vector<string> files){
	/*��ӡ�ļ�·��*/
	int size = files.size();
	cout << "size:" << size << endl;
	for (int i = 0; i < size; i++)
	{
		cout << files[i] << endl;
	}
}