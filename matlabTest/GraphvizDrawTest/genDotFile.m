function genDotFile(graphFile)
% 生成dot文件（扩展名为.gv)
% 根据需要可以选择是否需要分支编号
E = load(graphFile);

n = size(E,1);

fid=fopen('gd.gv', 'w');  % 新建文件，准备写入
fprintf(fid, 'digraph G {\n');
fprintf(fid, 'graph[rankdir=BT];\n');

for i=1:n
    % 有分支编号
    fprintf(fid,'\tv%d -> v%d [label="e%d"];\n', E(i,2), E(i,3), E(i,1));
    
    % 无分支编号
    %fprintf(fid,'\tv%d -> v%d;\n', E(i,2), E(i,3));
end

fprintf(fid, '}');
fclose(fid); % 关闭文件

end