function genOnce(dotExe, graphFile, graphFormat, pngFile)
% �����������ļ���������pngͼ��

genDotFile(graphFile);  % ����gd.gv�ļ�

cmd2 = ['!' dotExe ' -T' graphFormat ' gd.gv -o ' pngFile];
eval(cmd2);

% ɾ��gd.gv�ļ�
delete('gd.gv');

end