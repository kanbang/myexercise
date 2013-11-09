function A = createNetworkPresentationDataStructure(V, E)
% 创建网络表示方法数据结构
% 输入参数：
%   V -- 节点集合
%   E -- 分支集合（记录分支真实编号、始末节点）
% 输出参数：
%   A -- 邻接表(出边和入边邻的双向邻接表)

m = length(V);
n = size(E,1);

% 构造邻接表A
A=cell(m, 2);
for u=1:m
    for e=1:n
        if u == E(e, 2)
            A{u,1} = [A{u,1} e]; % 记录出边分支
        end
        
        if u == E(e, 3)
            A{u,2} = [A{u,2} e]; % 记录入边分支
        end
    end
end

end