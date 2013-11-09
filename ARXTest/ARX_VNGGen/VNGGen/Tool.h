#pragma once

#include <string>
#include <vector>
using namespace std;
typedef vector<string> StringArray;

/*
 * ���е�λͳһ�ɺ���(mm)
 * ��λ����:{ 1in = 2.54cm = 25.4mm = 72pt }
 */

/* Ӣ�� --> ���� */
#define INCH_2_MM(v) v*25.4

/* ���� --> Ӣ�� */
#define MM_2_INCH(v) v*0.0393700787401575

extern void SplitString(const string& content, const string& token, StringArray& cc);
extern string GetAppPathDir();
extern string BuildPath(const string& dir, const string& fileName);
extern void RemoveFile(const string& filePath);
extern bool ReadGraphvizInstallPath(string& path);

extern double StringToDouble(const string& value);
extern int StringToInt(const string& value);

