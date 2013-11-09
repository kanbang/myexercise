function test()
% 测试ga和平衡图绘制

clc


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%网络初始化%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

[filename, pathname] = uigetfile('*.txt','请选择一个拓扑数据txt文件');
if isequal(filename,0)
   return;
end

% 加载网络
[V, E, from, to] = loadNetwork(fullfile(pathname, filename));

% disp(V)
% disp(E)
% 有些网络没有源汇（例如整个网络就是一个环）
% 需要排除这些情况
if isempty(from) | isempty(to)
    disp('源点或汇点非法', '提示');
    return;
end

% 检查源汇
isBalanced = checkBalance(V, E);
if ~isBalanced
%     return;
end

% 处理多源多汇网络
if length(from)>1 | length(to)>1
    msg = sprintf('\n该网络是一个多源多汇网络:\n\t源点集合:%s\n\t汇点集合：%s\n', mat2str(V(from)), mat2str(V(to)));
    disp(msg);
    [V, E, from, to] = dealWithMultiSSNetwork(V, E, from, to);
end

% 创建网络表示方法的数据结构
A = createNetworkPresentationDataStructure(V, E);

% 输出网络信息
printNetworkInfo(V, E, from, to, A, 1);

% 查找单向回路
SC = findCycle_BySearch(E, A, from);

% 目前尚不能处理单向回路
if ~isempty(SC)
    disp('该网络存在单向回路')
    printCellArrayList(E, SC, 'C', 1);
    return;
end

% 构造一个虚拟的分支集合用于搜索算法
VE = E(1:end, 2:4);

useGA = false;
reply = input('请选择是否使用ga? Y/N [N]: ','s');
if isempty(reply)
    useGA = false;
elseif strcmpi(reply, 'y')
    useGA = true;
elseif strcmpi(reply, 'n')
    useGA = false;
end

if useGA
    disp(sprintf('\t用户选择使用ga优化网络拓扑\n'));
    result_A = runGA(A, VE, from , to);
else
    disp(sprintf('\t用户选择不使用ga优化网络拓扑\n'));
    result_A = A;
end

[P, lastPath, lastNode, lastE]= standalone_dfs(result_A, VE, from, to);             % 搜索独立通路

disp(sprintf('\n最后的一条通路:%s\n', mat2str(E(lastPath))));
disp(sprintf('\n最后到达的节点:%d\n', V(lastNode)));
outE = [];
for i = 1:size(lastE,1)
    outE = [outE; [E(i,1) lastE(i, 3)]];
end
disp(outE)

P1 = removeAllVirtualEdges(E, P(1:end, 1));
disp('打印独立通路:')
printCellArrayList(E, P1, 'P', 1);                      % 需要翻译
%printNodePath(V, E, P1, 'P');

% figure
splitedEdges = drawQHGraph(A, V, E, from, P, 1);        % 求被分割的分支，(但不)绘制平衡图

% 打印被分割的分支
disp('被分割的分支:')
disp(splitedEdges)

end