function pathNum = countAllPathsNum_BySearch(Adj_list, E, from, to, hasCycle)
% 计算网络的通路总数，采用dfs搜索
% 既能用于无单向回路的网络，也能用于有单向回路的网络
% 参考文献：[1] 《流体网络》第3章--3.2.3小节: 44-45
%          [2] 《通风网络含有单向回路时的通路算法》--刘剑，贾进章,于斌
% 输入参数：
%   Adj_list -- 邻接表
%   E -- 分支集合
%   from, to -- 源汇节点
%   hasCycle -- 是否有单向回路
% 输出参数：
%   pathNum -- 通路总数

% 直接调用dfs搜索全部通路
P = findAllPaths_BySearch(Adj_list, E, from, to, hasCycle);

% 最后计算通路总数
pathNum = length(P);
end