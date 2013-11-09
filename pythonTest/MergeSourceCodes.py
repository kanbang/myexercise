# coding=utf-8

import os

# Ҫ������·��
dir = "C:\\Users\\anheihb03dlj\\Desktop\\ARX_MVSS"

# ����ļ�·��
outFilePath = "C:\\Users\\anheihb03dlj\\Desktop\\�ϲ�Դ����.txt"

# �ļ��������
outFile = open(outFilePath, 'w')

# ��¼�к�
lineCount = 0

# �Ƿ�ʹ���к�
useLineCount = True 

# �Ƿ������Դ�ļ�
outputRC = False

# �Ƿ�ȥ���հ���
stripBlankLine = True 

# �Ƿ�ȥ��ע��
# �Ƚ��鷳����Ҫ�жϵ���ע��(//)�Ͷ���ע��(/* */)
#stripComment = False

def WriteSourceFile(filePath):
    # ʹ��ȫ�ֱ���
    global lineCount

    # ��ȡ�ļ� 
    inFile = open(filePath, 'r')

    # ����ѭ����ȡ�ļ�
    for line in inFile:
        # ȥ���հ���
        # �ж�����ǿ��У���д�뵽�ļ��� 
        if stripBlankLine:
            data = line.strip().split()
            if len(data)==0: continue

        # ���ʹ���кţ�������кţ����� "1.  " 
        if useLineCount:
            lineCount += 1
            lcStr = '%d' %lineCount
            outFile.write(lcStr + ".  " + line)
        else:
            outFile.write(line)
    # �ر��ļ�
    inFile.close()

def WalkFilesInChildDir(child_dir, ext):
    for root, dirs, files in os.walk(child_dir):
        for f in files:
            file_ext = os.path.splitext(f)[1][1:]
            # ��չ����׺�Ƚϣ������ִ�Сд
            if cmp(file_ext.upper(),ext.upper()) == 0:
                outFile.write("\n//" + os.path.split(child_dir)[1] + " -- " + f +"\n")
                # д���ļ�
                WriteSourceFile(os.path.join(root, f))
        outFile.write("\n")

for root, dirs, files in os.walk(dir):
    print "��Ŀ¼:", root
    outFile.write("Ŀ¼["+ os.path.split(root)[1]+"]�µ�����Դ����\n")
    # д��ͷ�ļ�
    WalkFilesInChildDir(root, "h")
    # д��Դ�ļ�
    WalkFilesInChildDir(root, "c")
    WalkFilesInChildDir(root, "cpp")
    if outputRC:
        # д����Դ�ļ�
        WalkFilesInChildDir(root, "rc")
        WalkFilesInChildDir(root, "rc2")

outFile.close();
