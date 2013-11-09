function outEdges = findOutEdges(A, E, color, v, isColored)
% 函数功能：
% 根据当前的拓扑结构，查找当前节点v的出边分支
% 1）排除非0流量分支；2）查找颜色分支
% 输入参数：
%   A -- 节点邻接矩阵
%   v -- 要查找出边的节点
% 输出结果：
%   outEdges -- 节点v的出边分支

outEdges = A{v}; % 提取矩阵A的第v行（等价于获取节点v的出边信息）

% 修改!!!!!!!!!!!!!!!!!1
% 首先排除0流量分支
outEdges = outEdges(find(E(outEdges, 3)>10^-3));

% 查找颜色分支
outEdges = outEdges(find(color(outEdges)==isColored));

end