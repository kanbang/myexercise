function outEdges = findOutEdges(Adj_list, E, color, v, isColored)
% 函数功能：
% 根据当前的拓扑结构，查找当前节点v的出边分支
% 1）查找颜色分支
% 输入参数：
%   Adj_list -- 邻接表
%   v -- 要查找出边的节点
% 输出结果：
%   outEdges -- 节点v的出边分支

outEdges = Adj_list{v}; % 提取矩阵A的第v行（等价于获取节点v的出边信息）

% 查找颜色分支
outEdges = outEdges(find(color(outEdges)==isColored));

end