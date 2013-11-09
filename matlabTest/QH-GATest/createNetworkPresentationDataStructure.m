function A = createNetworkPresentationDataStructure(V, E)
% 创建网络表示方法数据结构
% 输入参数：
%   V -- 节点集合
%   E -- 分支集合（记录分支真实编号、始末节点）
% 输出参数：
%   A -- 邻接表

m = length(V);
n = size(E,1);

% 构造邻接表A
A=cell(m, 1);
for j=1:m
    for k=1:n
        v = E(k, 2);          % 分支的始节点
        if v == j             % 匹配实际的节点编号
            A{v}=[A{v}, k];   % 第k条分支是节点v的出边分支
        end
    end
end

end