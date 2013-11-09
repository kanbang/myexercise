function genOnce(dotExe, graphFile, graphFormat, pngFile)
% 处理单个拓扑文件，并生成png图形

genDotFile(graphFile);  % 生成gd.gv文件

cmd2 = ['!' dotExe ' -T' graphFormat ' gd.gv -o ' pngFile];
eval(cmd2);

% 删除gd.gv文件
delete('gd.gv');

end