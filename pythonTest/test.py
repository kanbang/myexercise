# coding=utf-8

import os

# ����ļ�·��
outFilePath = "C:\\Users\\anheihb03dlj\\Desktop\\st.txt"

# �����������
outFile = open(outFilePath, 'w')

def WriteSourceFile(filePath):
    # �����������
    inFile = open(filePath, 'r')

    # ����ѭ����ȡ�ļ�
    for line in inFile:
        # ȥ����β�հ�
        data = line.strip().split()
        print "��ȡһ������:", data, len(data)
        if len(data)==0: continue

        # ��listת����string        
        #outFile.write("".join(data))        
        outFile.write(line)

    inFile.close()

WriteSourceFile("C:\\Users\\anheihb03dlj\\Desktop\\aa.txt")

outFile.close()
