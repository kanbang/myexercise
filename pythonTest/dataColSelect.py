# coding=utf-8

"""
	数据文件重新格式化
	思路：1) 每次读取文件获得一个string
		  2) 去掉首尾的空白-strip
		  3) 将字符串进行分割(split), 得到一个list
		  4) 使用join方法将list用制表符\t连接起来
		  5) 输出到文件中
"""

def GetCols():
	inputStr = raw_input('请选择要输出的列[使用空白进行分隔]:')
	inputData = inputStr.strip().split()
	selectedCols = []
	if len(inputData)!=0:
		for s in inputData:
			selectedCols.append(int(s))
	return selectedCols

def GetData(data, selectedCols):
	tempData = []; # 一个空的临时list
	for i in selectedCols:
		tempData.append(data[i])
	return tempData;

# 选择要输出的列
selectedCols = GetCols()
if len(selectedCols)==0:
	exit();

# 输入文件路径
inFilePath = 'test.txt'

# 输出文件路径
outFilePath = 'outputData.txt'

# 输入输出对象
inFile = open(inFilePath, 'r')
outFile = open(outFilePath, 'w')

# 按行循环读取文件
for line in inFile:
	# 去掉首尾空白并分割得到一个list
	data = line.strip().split()
	if len(data)==0:
		continue
	outFile.write('\t'.join(GetData(data, selectedCols)) + '\n')

inFile.close();
outFile.close();
