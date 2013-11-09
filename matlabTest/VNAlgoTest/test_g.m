function test_g()
% 测试

% 从拓扑数据文件中加载网络
[V, E, from, to]=loadNetwork('graph_topology.txt');

% 有些网络没有源汇（例如整个网络就是一个环）
% 需要排除这些情况
if isempty(from) | isempty(to)
    disp('源点或汇点非法')
    return;
end

% 处理多源多汇网络
if length(from)>1 | length(to)>1
    disp('==================================================================')
    disp('该网络是一个多源多汇网络:')
    disp('源点集合:')
    disp(from)
    disp('汇点集合:')
    disp(to)
    disp('==================================================================')
    [V, E, from, to] = dealWithMultiSSNetwork(V, E, from, to);
end

% 创建网络表示方法的数据结构
[A, UA, B, Adj_list] = createNetworkPresentationDataStructure(V, E);

% 输出网络信息
printNetworkInfo(V, E, from, to, A, UA, B, Adj_list);

% 有错误（不能用于有单向回路的图）
% % 查找一颗生成树
% ST = findSpanTree(Adj_list, E, from);
% % 删除虚拟分支，同时将分支转换成真实的分支编号
% ST = removeVirtualEdges(E, ST);
% disp('树枝:')
% disp(ST)
% 
% C = solveBasicCircuitMatrix(B, ST);
% disp('回路矩阵:')
% disp(C)

SC=findCycle_ByMatrix(A, E, Adj_list);
SC = removeAllVirtualEdges(E, SC);  % 删除虚拟分支，同时将分支转换成真实的分支编号
disp('采用节点邻接矩阵计算单向回路: ')
printCellArrayList(E, SC, 'C');

SC=findCycle_BySearch(E, Adj_list, from);
SC = removeAllVirtualEdges(E, SC);  % 删除虚拟分支，同时将分支转换成真实的分支编号
disp('采用dfs搜索单向回路: ')
printCellArrayList(E, SC, 'C');

isConnectedGraph = checkGraphConnection_ByMatrix(UA);
disp('采用无向图节点邻接计算图的连通性:')
if isConnectedGraph
    disp(sprintf('\t===>图是连通的'))
else
    disp(sprintf('\t===>图是不连通的'))
end

isConnectedGraph = checkGraphConnection_BySearch(E);
disp('采用搜索的方法计算图的连通性:')
if isConnectedGraph
    disp(sprintf('\t===>图是连通的'))
else
    disp(sprintf('\t===>图是不连通的'))
end

% 判断网络是否有单向回路
hasCycle = 0;
if isempty(SC)
    hasCycle = 1;
end

if ~hasCycle
    P = findAllPaths_NoCycle_ByMatrix(A, E, from, to);
    P = removeAllVirtualEdges(E, P);
    disp('采用矩阵方法计算无单向回路网络的通路:')
    printCellArrayList(E, P, 'P');
end

P = findAllPaths_BySearch(Adj_list, E, from, to, hasCycle); % 同时适用于有无单向回路的情况
P = removeAllVirtualEdges(E, P);
disp('采用深度优先搜索(有/无)单向回路网络的通路:')
printCellArrayList(E, P, 'P');

if ~hasCycle
    pathNum = countAllPathsNum_NoCycle_ByMatrix(A, from, to);
    disp('采用矩阵方法计算无单向回路通路总数:')
    disp(pathNum)
end

pathNum = countAllPathsNum_BySearch(Adj_list, E, from, to, hasCycle); % 同时适用于有无单向回路的情况
disp('采用深度优先搜索计算(有/无)单向回路通路总数:')
disp(pathNum)

end
