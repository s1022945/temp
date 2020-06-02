#include <stdlib.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <windows.h>
#include <cstdlib> // atoi ��ƩҦb�禡�w
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

vector<string> split(const string& str, const string& delim);	//�r�����
vector<struct tag_Defect_Info> GetDefectList(string &DFTFilePath);	// �B�zDefect List
bool DefectSort (tag_Defect_Info a, tag_Defect_Info b);	// �̷�RVRP Defect List �Ƨ�

BOOL WINAPI WritePrivateProfileString(
  _In_ LPCTSTR lpAppName,
  _In_ LPCTSTR lpKeyName,
  _In_ LPCTSTR lpString,
  _In_ LPCTSTR lpFileName
);

int main()
{
	//���oDefectfile ������|
	string DFTFilePath = ".\\Q\\FG5013GXS1\\F505P\\F505P175\\F505P175BF\\Format\\7571F505P175BF.cmo";

	//Ū��Defect file --> DefectList
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
	//��?�n���Ϊ��r�Ŧ�?string?��???char*?��
	char * strs = new char[str.length() + 1] ; //���n�ѤF
	strcpy(strs, str.c_str()); 
 
	char * d = new char[delim.length() + 1];
	strcpy(d, delim.c_str());
 
	char *p = strtok(strs, d);
	while(p) {
		string s = p; //���αo�쪺�r�Ŧ�???string?��
		res.push_back(s); //�s�J?�G??
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
	if(DFTfile) //�ɮצs�b
	{
		int itempPnNo = 0;
		//int iPnlInfoStart = 0;
		while (getline (DFTfile, line)) // line���]�t"\n"�A@ ���ɮ׵���
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
	else //�ɮפ��s�b
	{
		MessageBox(NULL, TEXT( "Load X571 file Failed!" ), TEXT( "Error" ), MB_OK );
		return DefectList;
	}
	
}

//�̷�RVRP Defect List �Ƨ�
bool DefectSort (tag_Defect_Info a, tag_Defect_Info b)
{
	if(a.iPnNo < b.iPnNo) return true;
	if(a.iPnNo == b.iPnNo){
		if(a.iXCoord <= b.iXCoord) return false;
		return true;
	}
	return false;
}
