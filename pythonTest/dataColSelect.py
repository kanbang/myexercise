# coding=utf-8

"""
	�����ļ����¸�ʽ��
	˼·��1) ÿ�ζ�ȡ�ļ����һ��string
		  2) ȥ����β�Ŀհ�-strip
		  3) ���ַ������зָ�(split), �õ�һ��list
		  4) ʹ��join������list���Ʊ��\t��������
		  5) ������ļ���
"""

def GetCols():
	inputStr = raw_input('��ѡ��Ҫ�������[ʹ�ÿհ׽��зָ�]:')
	inputData = inputStr.strip().split()
	selectedCols = []
	if len(inputData)!=0:
		for s in inputData:
			selectedCols.append(int(s))
	return selectedCols

def GetData(data, selectedCols):
	tempData = []; # һ���յ���ʱlist
	for i in selectedCols:
		tempData.append(data[i])
	return tempData;

# ѡ��Ҫ�������
selectedCols = GetCols()
if len(selectedCols)==0:
	exit();

# �����ļ�·��
inFilePath = 'test.txt'

# ����ļ�·��
outFilePath = 'outputData.txt'

# �����������
inFile = open(inFilePath, 'r')
outFile = open(outFilePath, 'w')

# ����ѭ����ȡ�ļ�
for line in inFile:
	# ȥ����β�հײ��ָ�õ�һ��list
	data = line.strip().split()
	if len(data)==0:
		continue
	outFile.write('\t'.join(GetData(data, selectedCols)) + '\n')

inFile.close();
outFile.close();
