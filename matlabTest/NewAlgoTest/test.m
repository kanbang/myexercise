function test()
% 清屏
clc

[filename, pathname] = uigetfile('*.txt','请选择一个拓扑数据文件');
if isequal(filename,0)
     helpdlg('请选择一个拓扑数据txt文件!','提示');
     return;
end

% 从拓扑数据文件中加载网络
[V, E, from, to] = loadNetwork(fullfile(pathname, filename));

% 有些网络没有源汇（例如整个网络就是一个环）
% 需要排除这些情况
if isempty(from) | isempty(to)
    helpdlg('源点或汇点非法!','提示');
    return;
end

% 处理多源多汇网络
if length(from)>1 | length(to)>1
    [V, E, from, to] = dealWithMultiSSNetwork(V, E, from, to);
end

% 创建网络表示方法的数据结构
% 节点邻接矩阵、无向图节点邻接矩阵、关联矩阵、出边邻接表
[A, UA, B, Adj_list] = createNetworkPresentationDataStructure(V, E);

% 打印网络信息
printNetworkInfo(V, E, from, to, A, UA, B, Adj_list, 1);

% 通过矩阵方法查找源汇之间的所有通路
P = findAllPaths(A, E, from, to);

end
