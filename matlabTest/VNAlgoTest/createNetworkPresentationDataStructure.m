function [A, UA, B, Adj_list] = createNetworkPresentationDataStructure(V, E)
% 创建网络表示方法的数据结构
% 输入参数：
%   V        --     节点集合
%   E        --     分支集合
% 输出参数:
%   A        --     有向图节点邻接矩阵
%   UA       --     无向图节点邻接矩阵（对称矩阵）
%   B        --     完全关联矩阵
%   Adj_list --     出边邻接表

m = length(V); % 节点数
n = size(E,1); % 分支数

% 构造邻接表Adj_list
Adj_list=cell(m, 1);
for i=1:m
    for j=1:n
        v = E(j,2);          % 分支的始节点
        if v == i
            Adj_list{v}=[Adj_list{v}, j];   % 第j条分支是节点v的出边分支
        end
    end
end

% 构造邻接矩阵A和关联矩阵B
A=zeros(m, m);
B=zeros(m, n);
for i=1:n
    u = E(i,2); % 分支的始节点
    v = E(i,3); % 分支的末节点
    A(u,v)=1;
    B(u,i)=1;
    B(v,i)=-1;
end

% 构造无向图节点邻接矩阵(对称矩阵)
UA = A;
for i=1:m
    for j = 1:i
        if UA(i,j) == UA(j,i) % 同时等于0或同时等于1
            continue;
        end
        UA(i,j)=1;
        UA(j,i)=1;
    end
end

end