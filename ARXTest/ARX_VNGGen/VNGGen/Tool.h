#pragma once

#include <string>
#include <vector>
using namespace std;
typedef vector<string> StringArray;

/*
 * 所有单位统一成毫米(mm)
 * 单位换算:{ 1in = 2.54cm = 25.4mm = 72pt }
 */

/* 英寸 --> 毫米 */
#define INCH_2_MM(v) v*25.4

/* 毫米 --> 英寸 */
#define MM_2_INCH(v) v*0.0393700787401575

extern void SplitString(const string& content, const string& token, StringArray& cc);
extern string GetAppPathDir();
extern string BuildPath(const string& dir, const string& fileName);
extern void RemoveFile(const string& filePath);
extern bool ReadGraphvizInstallPath(string& path);

extern double StringToDouble(const string& value);
extern int StringToInt(const string& value);

