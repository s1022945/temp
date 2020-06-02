#include <stdlib.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <windows.h>
#include <cstdlib> // atoi 函數所在函式庫
#include <algorithm>
#include <iomanip>

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

vector<string> split(const string& str, const string& delim);	//字串分割
vector<struct tag_Defect_Info> GetDefectList(string &DFTFilePath);	// 處理Defect List
bool DefectSort (tag_Defect_Info a, tag_Defect_Info b);	// 依照RVRP Defect List 排序

BOOL WINAPI WritePrivateProfileString(
  _In_ LPCTSTR lpAppName,
  _In_ LPCTSTR lpKeyName,
  _In_ LPCTSTR lpString,
  _In_ LPCTSTR lpFileName
);

int main()
{
	//取得Defectfile 完整路徑
	string DFTFilePath = ".\\Q\\FG5013GXS1\\F505P\\F505P175\\F505P175BF\\Format\\7571F505P175BF.cmo";

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
