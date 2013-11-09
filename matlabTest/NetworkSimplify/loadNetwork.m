function [V, E, from, to] = loadNetwork(filepath)
% 函数功能：从拓扑文件graph_topology.txt中加载网络
% 输出参数：
%   V        --     节点集合
%   E        --     分支集合
%   from     --     源点集合
%   to       --     汇点集合

% 定义分支集合（示例）
% E = [
%     % 格式
%     % 分支编号 -- 始节点编号，末节点编号 -- 分支属性（风量、阻力系数）
%     1     1    2  % 第1条分支
%     2     2    4  % 第2条分支
%     3     4    6  % 第3条分支
%     4     3    2  % 第4条分支
%     5     2    5  % 第5条分支
%     6     5    4  % 第6条分支
%     7     1    3  % 第7条分支
%     8     3    5  % 第8条分支
%     9     5    6  % 第9条分支
%     ];

InE = load(filepath); % 加载网络

n = size(InE,1); % 分支数

% 构造节点信息映射表
V=[];
for i=1:n
    V = union(V, InE(i, 2:3)); % 提取分支的始末节点，合并重复的节点
end

m = length(V); % 节点数

% 虚拟分支映射
E = zeros(n, 3); % 记录分支编号、虚拟分支的始末节点的虚拟编号
for i=1:n
    E(i,1) = InE(i,1);           % 记录分支的真实编号
    E(i,2) = find(V==InE(i,2));  % 始节点的编号在V中的索引位置
    E(i,3) = find(V==InE(i,3));  % 末节点的编号在V中的索引位置
end

% 节点度的信息，包括节点的出度outDegree和入度信息inDegree
VertexDegree=cell(m, 1);
for i=1:n 
    u = E(i, 2);  % 始节点（节点映射在V中的编号）
    v = E(i, 3);  % 末节点（节点映射在V中的编号）
    if isempty(VertexDegree{u})
        VertexDegree{u} = [VertexDegree{u}; [1 0]]; % 出度初始化为1，初始化为0
    else
        VertexDegree{u}(1) = VertexDegree{u}(1)+1; % 出度+1
    end
    
    if isempty(VertexDegree{v})
        VertexDegree{v} = [VertexDegree{v}; [0 1]]; % 出度初始化为0，初始化为1
    else
        VertexDegree{v}(2) = VertexDegree{v}(2)+1; % 入度+1
    end
end

% 计算源汇点（节点在V中的映射索引，并非真实的节点编号）
from=[];
to=[];
for u=1:m
    outDegree = VertexDegree{u}(1);  % 获取节点的出度和入度
    inDegree = VertexDegree{u}(2);
    if outDegree>0 & inDegree==0
        from = [from u];
    elseif outDegree==0 & inDegree>0
        to = [to u];
    end
end

end