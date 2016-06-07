#include "hdf5.h"
#include<iostream>
#include<fstream>
#include<iomanip>
#include<string>
using namespace std;

#define DIM0 1624
#define DIM1 76
#define DIM2 4

#pragma comment(lib,"hdf5.lib")
#pragma comment(lib,"libszip.lib")
#pragma comment(lib,"libzlib.lib")

int main ()
{
    hid_t       file, dset, attr;            /* Handles */
    herr_t      status;

	string FileName = "H2A_SM2B20120530_03337.h5";
	string TableName = "wvc_lat";
	string StringName = "row_time";
	string DataAttName = "scale_factor";
	string StringAttName = "unit";

	file = H5Fopen (FileName.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);		//打开文件
	#pragma region 读取字符串数据
	dset = H5Dopen (file, "row_time", H5P_DEFAULT);		//打开表
	hid_t filetype;
	size_t sdim;
	filetype = H5Dget_type (dset);		//Get the datatype
	sdim = H5Tget_size (filetype);		//Get the datatype's size
	sdim++;			//Make room for null terminator
	hsize_t dims[1] = {DIM0};
	hid_t space;
	int ndims;
	space = H5Dget_space(dset);
	ndims = H5Sget_simple_extent_dims(space, dims, NULL);
	//申请内存，存放字符串
	char ** rdata = new char * [DIM0];
	rdata[0] = new char[DIM0*sdim];
	for(int i=1; i<DIM0; i++)
		rdata[i] = rdata[0] + i*sdim;
	hid_t memtype;
	memtype = H5Tcopy (H5T_C_S1);
	status = H5Tset_size (memtype, sdim);
	status = H5Dread (dset, memtype, H5S_ALL, H5S_ALL, H5P_DEFAULT, rdata[0]);
	ofstream fout;
	fout.open("time.txt");
	for(int i=0; i<DIM0; i++)
		if(strlen(rdata[i]) != 0)
			fout << i << '\t' << rdata[i] << endl;
	fout.close();
	delete [] rdata[0];
	delete [] rdata;
	status = H5Tclose (memtype);
	status = H5Tclose (filetype);
    status = H5Dclose (dset);		//关闭表
	#pragma endregion
	#pragma region 读取Table数据
	dset = H5Dopen (file, TableName.c_str(), H5P_DEFAULT);		//打开表
	int (*Rlat)[DIM1] = new int[DIM0][DIM1];			//申请内存，存放纬度
	status = H5Dread (dset, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, (void *)Rlat);		//读取表
	delete [] Rlat;			//释放内存
	#pragma endregion
	#pragma region 读取Talbe数据普通属性
	dset = H5Dopen (file, TableName.c_str(), H5P_DEFAULT);		//打开表
	attr = H5Aopen (dset, DataAttName.c_str(), H5P_DEFAULT);	//打开属性
	char name[20];
	status = H5Aget_name(attr,20,name);		//获取属性的名称
	cout << name << endl;
	H5A_info_t ainfo;
	status = H5Aget_info(attr,&ainfo);
	cout << ainfo.corder << "\t" << ainfo.corder_valid << "\t" << ainfo.cset << "\t" << ainfo.data_size << endl;
	int f;
	H5Aread(attr,H5T_NATIVE_INT,(void *)&f);		//读取属性的值
	cout << f << endl;
	status = H5Aclose (attr);		//关闭属性
	status = H5Dclose (dset);		//关闭表
	#pragma endregion
	status = H5Fclose (file);		//关闭文件

	system("pause");
    return 0;
}

