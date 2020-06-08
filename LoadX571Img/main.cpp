#include <stdlib.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <windows.h>
#include <cstdlib> // atoi 函數所在函式庫
#include <algorithm>
#include <iomanip>
#include <io.h>
#include <dirent.h>

#define MAX_LENGTH 1024

using namespace std;

struct tag_Defect_Info
{
	string  iDFTCode;					// 3 bytes, Defect Code
	int   iPnNo;						// Panel number
	int   iDataNo;						// Defect location in the panel pixel column_cell_number
	int   iXCoord;              		// Defect X Coordinate
	int   iYCoord;						// Defect Y Coordinate
	string  ImageFile1;				// 20 bytes, Image file1
};

// config.ini Item
//========================
typedef struct configItem
{
	struct EQ
    {
    	string  EQID;
        string tempDFTPath;
        string tempDFTFile;
    }EQ;
    struct ORS
    {
        string  path;
    }ORS;

	struct FileServer
	{
		string  path;
	}FileServer;

	struct BC
	{
		string  path;
	}BC;

	struct tempProductInfo
	{
		string  product;
		string   operation;
	}tempProductInfo;
        
}configItem;
//========================

// ORS Current Pictures
typedef struct ORSCurrentPic
{
	string glassID;
	string No;
}ORSCurrentPic;

vector<string> split(const string& str, const string& delim);	//字串分割
vector<struct tag_Defect_Info> GetDefectList(string &DFTFilePath);	// 處理Defect List
bool DefectSort (tag_Defect_Info a, tag_Defect_Info b);	// 依照RVRP Defect List 排序

<<<<<<< HEAD
configItem configIniRead(string &configPath);
ORSCurrentPic GetORSCurrentPic(string Picpath);

=======
BOOL WINAPI WritePrivateProfileString(
  _In_ LPCTSTR lpAppName,
  _In_ LPCTSTR lpKeyName,
  _In_ LPCTSTR lpString,
  _In_ LPCTSTR lpFileName
);
//GetPrivateProfileInt(_T("NewProjectionReadioSel"), _T("Index"), 0,g_UISetting.m_strT6nsmgrInIPath);
>>>>>>> 02ae2c43c391feda4c0e9a229f88be3a4d7014e7
int main()
{ 
    //取得 config.ini 完整路徑 
    //================================================== 
    char tempStr[MAX_LENGTH];
    string configPath;       // path of config.ini 
    getcwd(tempStr, MAX_LENGTH);
    configPath.append(string(tempStr));
    memset(tempStr, 0, strlen(tempStr));	//清空陣列
    configPath.append("\\config.ini");
    cout << "configPath = " << configPath << endl;
    //==================================================
    
	configItem sconfigItem = configIniRead(configPath);
	//cout << "sconfigItem.EQ.tempDFTFile = " << sconfigItem.EQ.tempDFTFile << endl;

	//取得ORS 上傳路徑下Review圖片檔名
	//D:\git\temp\LoadX571Img\Q\FG5013GXS1\F505P\F505P175\F505P175BF\Image
	string ORSPath;
	ORSPath.clear();
	ORSPath.append(sconfigItem.ORS.path);
	ORSPath.append("\\");
	ORSPath.append(sconfigItem.EQ.EQID);
	ORSPath.append("*.jpg");
	ORSCurrentPic sORSCurrentPic = GetORSCurrentPic(ORSPath);
	
	//取得Defectfile 完整路徑
	//".\\Q\\FG5013GXS1\\F505P\\F505P175\\F505P175BF\\Format\\7571F505P175BF.cmo"
	string DFTFilePath;
	DFTFilePath.append(sconfigItem.FileServer.path); //Q:
	DFTFilePath.append("\\");	//"Q:\"
	DFTFilePath.append(sconfigItem.tempProductInfo.product);	//"Q:\FG5013GXS1"
	DFTFilePath.append("\\");	//"Q:\FG5013GXS1\"
	DFTFilePath.append("");
	
	//Q:\FG5013GXS1\F505P
	//Q:\FG5013GXS1
	
	//讀取Defect file --> DefectList
	vector<struct tag_Defect_Info> DefectList = GetDefectList(DFTFilePath);
	cout << "Defect List" << endl;
	cout << "Panel\tNo in panel\tX\tY\tCode\tX571IMG" << endl;
	cout << "====================================================================" << endl;
	for(int i = 0; i < DefectList.size(); i++)
	{
		cout << setw(5) << DefectList[i].iPnNo << "\t" \
			<< setw(11) << DefectList[i].iDataNo << "\t" \
			<< setw(7) << DefectList[i].iXCoord << "\t" \
			<< setw(7) << DefectList[i].iYCoord << "\t" \
			<< setw(4) << DefectList[i].iDFTCode << "\t" \
			<< DefectList[i].ImageFile1 << endl;
	}
	cout << "====================================================================" << endl;

	system("pause");
	return 0;
}

//String Split
vector<string> split(const string& str, const string& delim) {
	vector<string> res;
	if("" == str) return res;
	//先?要切割的字符串?string?型???char*?型
	char * strs = new char[str.length() + 1] ; //不要忘了
	strcpy(strs, str.c_str()); 
 
	char * d = new char[delim.length() + 1];
	strcpy(d, delim.c_str());
 
	char *p = strtok(strs, d);
	while(p) {
		string s = p; //分割得到的字符串???string?型
		res.push_back(s); //存入?果??
		p = strtok(NULL, d);
	}
 
	return res;
}

//Defect file processing
vector<struct tag_Defect_Info> GetDefectList(string &DFTFilePath)
{
	vector<struct tag_Defect_Info> DefectList;
	struct tag_Defect_Info pDefectInfo;

	ifstream DFTfile(DFTFilePath.c_str());
	string line;
	if(DFTfile) //檔案存在
	{
		int itempPnNo = 0;
		//int iPnlInfoStart = 0;
		while (getline (DFTfile, line)) // line不包含"\n"，@ 為檔案結束
		{
			if (line == "#"){
				itempPnNo += 1;
				//cout << itempPnNo << endl;
				getline (DFTfile, line);	// F505P175BF01 P ** **** *****RBDEF 8XP 0 1 0
				getline (DFTfile, line);	// %
			}
			else
			{
				if (itempPnNo > 0 && line != "@" && line != "")
				{
					vector<string> res = split(line, " ");
					if (res[15] == "P"){
						//cout << res[15] << endl;
						pDefectInfo.iDFTCode = res[17];					// 3 bytes, Defect Code
						pDefectInfo.iPnNo = itempPnNo;						// Panel number
						pDefectInfo.iDataNo = atoi(res[0].c_str());						// Defect location in the panel pixel column_cell_number
						pDefectInfo.iXCoord = atoi(res[3].c_str());              		// Defect X Coordinate
						pDefectInfo.iYCoord = atoi(res[4].c_str());						// Defect Y Coordinate
						getline (DFTfile, line);
						//cout << line << endl;
						pDefectInfo.ImageFile1 = split(line, " ")[0];			// 20 bytes, Image file1
						DefectList.push_back(pDefectInfo);
					}
					else	// panel judge != "p"
					{
						getline (DFTfile, line);
						continue;
					}
				}
			}
		}
		DFTfile.close();
		sort(DefectList.begin(),DefectList.end(), DefectSort);
		return DefectList;
	}
	else //檔案不存在
	{
		MessageBox(NULL, TEXT( "Load X571 file Failed!" ), TEXT( "Error" ), MB_OK );
		return DefectList;
	}
	
}

//依照RVRP Defect List 排序
bool DefectSort (tag_Defect_Info a, tag_Defect_Info b)
{
	if(a.iPnNo < b.iPnNo) return true;
	if(a.iPnNo == b.iPnNo){
		if(a.iXCoord <= b.iXCoord) return false;
		return true;
	}
	return false;
}

//取得Ini 變數值
configItem configIniRead(string &configPath)
{
	
    //取Ini Setting
    //memset(tempStr, 0, strlen(tempStr));	//清空陣列 
    //GetPrivateProfileString("EQ", "EQID", 0, tempStr, MAX_LENGTH, "D:\\git\\temp\\LoadX571Img\\config.ini");
    configItem sconfigItem;
	char tempStr[MAX_LENGTH];
	//EQ
	GetPrivateProfileString("EQ", "EQID", 0, tempStr, MAX_LENGTH, configPath.c_str());
	sconfigItem.EQ.EQID.clear();
	sconfigItem.EQ.EQID.append(string(tempStr));
	memset(tempStr, 0, strlen(tempStr));	//清空陣列
	GetPrivateProfileString("EQ", "tempDFTPath", 0, tempStr, MAX_LENGTH, configPath.c_str());
	sconfigItem.EQ.tempDFTPath.clear();
	sconfigItem.EQ.tempDFTPath.append(string(tempStr));
	memset(tempStr, 0, strlen(tempStr));	//清空陣列
	GetPrivateProfileString("EQ", "tempDFTFile", 0, tempStr, MAX_LENGTH, configPath.c_str());
	sconfigItem.EQ.tempDFTFile.clear();
	sconfigItem.EQ.tempDFTFile.append(string(tempStr));
	memset(tempStr, 0, strlen(tempStr));	//清空陣列
	
	//ORS
	GetPrivateProfileString("ORS", "path", 0, tempStr, MAX_LENGTH, configPath.c_str());
	sconfigItem.ORS.path.clear();
	sconfigItem.ORS.path.append(string(tempStr));
	memset(tempStr, 0, strlen(tempStr));	//清空陣列

	//FileServer
	GetPrivateProfileString("FileServer", "path", 0, tempStr, MAX_LENGTH, configPath.c_str());
	sconfigItem.FileServer.path.clear();
	sconfigItem.FileServer.path.append(string(tempStr));
	memset(tempStr, 0, strlen(tempStr));	//清空陣列

	//BC
	GetPrivateProfileString("BC", "path", 0, tempStr, MAX_LENGTH, configPath.c_str());
	sconfigItem.BC.path.clear();
	sconfigItem.BC.path.append(string(tempStr));
	memset(tempStr, 0, strlen(tempStr));	//清空陣列

	//tempProductInfo
	GetPrivateProfileString("tempProductInfo", "product", 0, tempStr, MAX_LENGTH, configPath.c_str());
	sconfigItem.tempProductInfo.product.clear();
	sconfigItem.tempProductInfo.product.append(string(tempStr));
	memset(tempStr, 0, strlen(tempStr));	//清空陣列
	GetPrivateProfileString("tempProductInfo", "operation", 0, tempStr, MAX_LENGTH, configPath.c_str());
	sconfigItem.tempProductInfo.operation.clear();
	sconfigItem.tempProductInfo.operation.append(string(tempStr));
	memset(tempStr, 0, strlen(tempStr));	//清空陣列
	
	return sconfigItem;
}

ORSCurrentPic GetORSCurrentPic(string Picpath)
{
	ORSCurrentPic sORSCurrentPic;
	HANDLE hFile = INVALID_HANDLE_VALUE; 
	WIN32_FIND_DATA pNextInfo;  

	hFile = FindFirstFile(Picpath.c_str(), &pNextInfo);
	
	if(INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(NULL, TEXT("ORS 資料夾下找不到對應的照片"), TEXT( "Error" ), MB_OK );
		return sORSCurrentPic;
	}

	char infPath[MAX_PATH] = {0};
	if(pNextInfo.cFileName[0] != '.')
	{
		cout << pNextInfo.cFileName.substr(2,5) << endl;
	}

	while(FindNextFile(hFile,&pNextInfo))  
	{  
		if(pNextInfo.cFileName[0] == '.')
		{
			continue;  
		}
		cout << pNextInfo.cFileName << endl;
	}
	
	//return FALSE;
	return sORSCurrentPic;
}
