/*
http://blog.5ibc.net/p/39392.html
*/
#include <windows.h>
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
* @function: 获取cate_dir目录下的所有文件名
* @param: cate_dir - string类型
* @result：vector<string>类型
*/
vector<string> FileMgr::getFiles(string cate_dir)
{
	cout << "folder is :" << cate_dir << endl;
	vector<string> files;//存放文件名

#ifdef WIN32
	_finddata_t file;
	long lf;
	//输入文件夹路径
	if ((lf = _findfirst(cate_dir.c_str(), &file)) == -1) {
		std::cout << "cate_dir" << cate_dir << " not found!!!" << std::endl;
	}
	else {
		while (_findnext(lf, &file) == 0) {
			//输出文件名
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

	/*sort函数没有第三个参数，实现的是从小到大*/
	//排序，按从小到大排序
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

//////上面的那个不工作
//获取所有的文件名
void GetAllFiles(string path, vector<string>& files)
{

	long   hFile = 0;
	//文件信息  
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			if ((fileinfo.attrib &  _A_SUBDIR))
			{
				cout << "sub dir" << endl;
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
				{
					files.push_back(p.assign(path).append("\\").append(fileinfo.name));
					GetAllFiles(p.assign(path).append("\\").append(fileinfo.name), files);
				}
			}
			else
			{
				cout << "file" << endl;
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
			}

		} while (_findnext(hFile, &fileinfo) == 0);

		_findclose(hFile);
	}
	else
		cout << "hFile error" << endl;

}
#include <string.h> 
/*
原函数是返回后缀，现在加一个.后返回
*/
std::string GetFilePosfix(const char* path)
{
	const char* pos = strrchr(path, '.');
	if (pos)
	{
		std::string str(pos + 1);
		//1.转换为小写  
		//http://blog.csdn.net/infoworld/article/details/29872869  
		std::transform(str.begin(), str.end(), str.begin(), ::tolower);
		//std:cout << "str[" + str + "]" << endl;
		return "."+str;
	}
	return std::string();
}
//获取特定格式的文件名
void GetAllFormatFiles(string path, vector<string>& files, vector<string>& filesname,string format)
{
	//文件句柄  
	/*refer to http://blog.csdn.net/linj_m/article/details/48046913 for exception*/
	intptr_t   hFile = 0;
	//文件信息  
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

			/*仅仅是文件的名字*/
			//cout << "fileinfo.name[" << name<< "]" << endl;
			/*如果是子目录*/
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
					/*文件的全路径是通过folder名字+\\+文件名字拼接而成*/
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
int FileMgr::readFolderFiles(string folder, vector<string> &files/*拿到全路径*/, vector<string> &filesname/*仅仅是文件名*/, string format,bool outputName){

	char * distAll = "AllFilesName.txt";
	GetAllFormatFiles(folder, files, filesname, format);
#if 1
	/*文件结果写入txt中*/
	ofstream ofn(distAll);
	/*打印文件绝对路径，这个是全路径*/
	int size = files.size();
	ofn << size << endl;
	for (int i = 0; i < size; i++)
	{
		if (outputName)	/*这个是文件名*/
			ofn << filesname[i] << endl;
		else
			ofn << files[i] << endl;
		//cout << "output [" + files[i] + "]" << endl;
	}
	ofn.close();
#endif
	return 0;
}
void  FileMgr::showFiles(vector<string> files){
	/*打印文件路径*/
	cout << "show the abosolute path of all files : BEGIN" << endl;

	int size = files.size();
	cout << "size:" << size << endl;
	for (int i = 0; i < size; i++)
	{
		cout << files[i] << endl;
	}
	cout << "show the abosolute path of all files :  END" << endl;
}




/*
http://1350579085.iteye.com/blog/1993507
*复制文件
filename :要复制的文件名
newfile :要复制到的文件名

没有一个循环读取文件直到写完的过程
*/
int FileMgr::copyFile(const char*src_file, const char*dst_file)
{
	ifstream in;
	ofstream out;
	/**
	open函数的原型是
	open(const char*filename,ios_base::openmode mode=ios_base::in) ;
	in_stream.open("file.txt");这样写参数实际上是指向这个常量字符串的指针
	*/
	//打开文件
	in.open(src_file);
	//打开文件失败
	if (in.fail()){
		cout << "打开文件" + string(src_file)+"失败" << endl;
		in.close();
		out.close();
		return -1;
	}
	out.open(dst_file);
	if (out.fail()){
		cout << "创建文件" + string(dst_file) + "失败" << endl;
		in.close();
		out.close();
		return -1;
	}
	else{//复制文件
		out << in.rdbuf();
		out.close();
		in.close();
		return 0;
	}
}
/*
1>------ Build started: Project: readFolder, Configuration: Debug Win32 ------
1>  FileMgr.cpp
1>d:\program files (x86)\microsoft visual studio 12.0\vc\include\xutility(2132): error C4996: 'std::_Copy_impl': Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'
1>          d:\program files (x86)\microsoft visual studio 12.0\vc\include\xutility(2113) : see declaration of 'std::_Copy_impl'
1>          j:\myself\testvs\readfolder\readfolder\filemgr.cpp(342) : see reference to function template instantiation '_OutIt std::copy<std::_String_const_iterator<std::_String_val<std::_Simple_types<char>>>,LPWSTR>(_InIt,_InIt,_OutIt)' being compiled
1>          with
1>          [
1>              _OutIt=LPWSTR
1>  ,            _InIt=std::_String_const_iterator<std::_String_val<std::_Simple_types<char>>>
1>          ]
========== Build: 0 succeeded, 1 failed, 0 up-to-date, 0 skipped ==========
*/
#if 0
LPWSTR ConvertToLPWSTR(const std::string& s)
{
	LPWSTR ws = new wchar_t[s.size() + 1]; // +1 for zero at the end
	copy(s.begin(), s.end(), ws);
	ws[s.size()] = 0; // zero at the end
	return ws;
}
#endif

//C++ string to WINDOWS UNICODE string
std::wstring s2ws(const std::string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}
int FileMgr::copyFileToDir(vector<string> filesname, string src_dir, string dst_dir){
	vector<string>::iterator it;
	int ret=0;
	for (it = filesname.begin(); it != filesname.end(); ++it){
		 string src = src_dir + "\\" + *it;
		 string dst = dst_dir + "\\" + *it;
		//  char *_src = (char *) src.c_str();
		ret=  copyFile(src.c_str(), dst.c_str());
	}
	return ret;
}
int FileMgr::copyFileToDir(vector<string> files, vector<string> filenames, string dst_dir){
	int size = files.size();
	int ret = 0;
	for (int i = 0; i < size;++i){
		//string src = src_dir + "\\" + *it;
		string dst = dst_dir + "\\" + filenames[i];
#if 0
		ret = copyFile(files[i].c_str(), dst.c_str());
#else
		/* http://stackoverflow.com/questions/27484269/expected-primary-expression-before-bool
		files[i].c_str(), dst.c_str() cannot work! 
		cannot convert argument 1 from 'const char *' to 'LPCWSTR'*/
		/*convert from string to LPCWSTR*/
		std::wstring stemp = (s2ws(files[i]));
		LPCWSTR  _src = stemp.c_str();
		std::wstring stemp_dst = s2ws(dst);
		LPCWSTR  _dst = stemp_dst.c_str();
		/*
		https://msdn.microsoft.com/zh-cn/library/windows/desktop/aa363851(v=vs.85).aspx
		*/
		BOOL b = CopyFile(_src, _dst, FALSE);
		if (b){
		}
		else
			cout << "copy error :"<< GetLastError() << endl;

#endif
	}
	return ret;
}