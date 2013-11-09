function test()
clc

[filename, pathname] = uigetfile('*.txt','请选择一个拓扑数据txt文件');
if isequal(filename,0)
   return;
end

% 加载网络
[V, E, from, to] = loadNetwork(fullfile(pathname, filename));

% 排除没有源汇网络（整个网络就是一个环）
if isempty(from) | isempty(to)
    disp('源点或汇点非法', '提示');
    return;
end

% 处理多源多汇网络
if length(from)>1 | length(to)>1   
    msg = sprintf('\n该网络是一个多源多汇网络:\n\t源点集合:%s\n\t汇点集合：%s\n', mat2str(from), mat2str(to));
    disp(msg);
    [V, E, from, to] = dealWithMultiSSNetwork(V, E, from, to);
end

% 创建网络表示方法的数据结构
A = createNetworkPresentationDataStructure(V, E); % 双向邻接表

% 输出网络信息
printNetworkInfo(V, E, from, to, A, 0);

% 查找单向回路
SC = findCycle_BySearch(E, A, from);

% 目前尚不能处理单向回路
if ~isempty(SC)
    disp('该网络存在单向回路')
    printCellArrayList(E, SC, 'C', 1);
    return;
end

% 计算网络的节点层次(采用最长路径法)
tic
layer = lpm(E, A, from);
t1 = toc;
printLayer(V, layer);

tic
% 分割网络
networkList = spliteNetwork(V, E, A, layer, from, to);
t2 = toc;

disp('网络链表:');
printNetwork(E, networkList); % 打印网络链表

disp('网络简化树状结构:');
printTreeNetwork(V, E, networkList); % 打印树状结构的网络

disp(sprintf('\n最长路径算法执行时间:%.3fs\t网络简化算法执行时间:%.3fs',t1, t2));
end