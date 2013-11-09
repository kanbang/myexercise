function score = qh_fitness(X, A, VE, vertexPosInTopology, from , to)
% 评价函数 -- 计算交叉的块数
% 输入参数：
%   A -- 原始的拓扑
%   E -- 分支集合信息
%   vertexPosInToplogy -- 节点在编码串x中的位置

% disp('种群适应值计算开始')

A = adjustTopology(A, X, vertexPosInTopology); % 调整拓扑

% disp('调整拓扑完毕!!!')
P = standalone_dfs(A, VE, from, to); % 搜索独立通路

% disp('搜索独立通路完毕!!!')
% score = 1/caclBlockCrossNum(P); % 计算块交叉数
score = 1500 - caclBlockCrossNum(P); % 计算块交叉数

% disp(score)
    function blockCrossNum = caclBlockCrossNum(P)
        % 计算块交叉数
        len = length(P);

        blockCrossNum = length(P{1});

        if len>1
            for i=2:len
                cp = intersect(P{i}, P{i-1});
                blockCrossNum = blockCrossNum + length(P{i}) - length(cp);
            end
        end
    end

% disp('种群适应值计算结束')
end