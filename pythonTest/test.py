# coding=utf-8

import os

# 输出文件路径
outFilePath = "C:\\Users\\anheihb03dlj\\Desktop\\st.txt"

# 输入输出对象
outFile = open(outFilePath, 'w')

def WriteSourceFile(filePath):
    # 输入输出对象
    inFile = open(filePath, 'r')

    # 按行循环读取文件
    for line in inFile:
        # 去掉首尾空白
        data = line.strip().split()
        print "读取一行数据:", data, len(data)
        if len(data)==0: continue

        # 将list转换成string        
        #outFile.write("".join(data))        
        outFile.write(line)

    inFile.close()

WriteSourceFile("C:\\Users\\anheihb03dlj\\Desktop\\aa.txt")

outFile.close()
