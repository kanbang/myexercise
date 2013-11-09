function genMore(dotExe, graphDir, graphFormat, pngDir)
% 批量处理读取某个目录下的拓扑数据文件，然后使用graphviz软件生成png图像
% 输入参数:
%   graphDir -- 存放拓扑数据的目录
%   pngDir    -- png图形的保存目录

txtFiles = dir(fullfile(graphDir,'*.txt'));
n = length(txtFiles);

for i = 1:n
    graphTopofile = fullfile(graphDir, txtFiles(i).name);
   genDotFile(graphTopofile); % 生成gd.gv文件
    
    % 分割文件路径
    [pathstr,name,ext,versn] = fileparts(graphTopofile);  
    pngFile = fullfile(pngDir, [name '.' graphFormat]);
    
    cmd2 = ['!' dotExe ' -T' graphFormat ' gd.gv -o ' pngFile];        
    eval(cmd2);
end


% 删除gd.gv文件
delete('gd.gv');

end