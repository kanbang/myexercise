function P = findAllPaths_BySearch(Adj_list, E, from, to, hasCycle)
% 搜索网络的全部通路，采用dfs搜索
% 既能用于无单向回路的网络，也能用于有单向回路的网络
% 参考文献：[1] 《流体网络》第3章--3.2.3小节: 44-45
%          [2] 《通风网络含有单向回路时的通路算法》--刘剑，贾进章,于斌
% 输入参数:
%   Adj_list -- 邻接表
%   E -- 分支集合
%   from, to -- 源汇节点
%   hasCycle -- 是否有单向回路
% 输出参数：
%   P -- 通路集合
% celldisp(P)

% 搜索全部通路
P = dfs_visit(Adj_list, E, from, to, hasCycle);

    function nodes = getNodesOfColoredEdges(E, color)
        nodes = [];
        for ee=1:size(E,1)
            if color(ee)
                 % 只添加着色分支的始节点
                 % 文献2没有说清楚(!!!!)
                nodes = union(nodes, E(ee,2));
            end
        end
    end

    function outEdges = findColoredOutEdges(Adj_list, color, u)
        % 函数功能：
        % 根据当前的拓扑结构，查找当前节点u的出边分支
        % 1）查找有颜色分支
        % 输入参数：
        %   Adj_list -- 邻接表
        %   u -- 要查找出边的节点
        % 输出结果：
        %   outEdges -- 节点u的出边分支

        outEdges = Adj_list{u}; % 提取矩阵A的第u行（等价于获取节点v的出边信息）

        % 查找颜色分支
        outEdges = outEdges(find(color(outEdges)==1));

    end

    function outEdges = findUnColoredOutEdges_NoCycle(Adj_list, color, u)
        % 函数功能：
        % 根据当前的拓扑结构，查找当前节点u的出边分支
        % 1）查找无颜色分支-- 无单向回路的情况

        outEdges = Adj_list{u};
        outEdges = outEdges(find(color(outEdges)==0));
    end

    function outEdges = findUnColoredOutEdges(Adj_list, E, color, u)
        % 函数功能：
        % 根据当前的拓扑结构，查找当前节点u的出边分支
        % 1）查找无颜色分支-- 有单向回路的情况

        outEdges = [];

        nodes = getNodesOfColoredEdges(E, color);
        % 查找颜色分支
        for ee = Adj_list{u}
            if color(ee) | ~isempty(find(nodes==E(ee, 3)))
                continue;
            end

            outEdges = [outEdges ee];
        end
    end

    function P = dfs_visit(Adj_list, E, u, v, hasCycle)
        path=[];
        P={};
        color= zeros(1, size(E, 1));  % 节点着色集合(初始化为0，默认节点都未着色)

        while true
            outEdges = [];
            % 修改后的dfs可以同时适用于有无单向回路的情况
            % findUnColoredOutEdges完全可以代替findUnColoredOutEdges_NoCycle
            % 从效率的角度考虑，做成了if/else的形式(!!!!!!!!!!!!)
            if hasCycle
                outEdges = findUnColoredOutEdges(Adj_list, E, color, u);       % 寻找有单向回路情况的节点u的未着色出边分支
            else
                outEdges = findUnColoredOutEdges_NoCycle(Adj_list, color, u);   % 寻找无单向回路情况的节点u的未着色出边分支
            end

            if isempty(outEdges)
                if isempty(path)
                    return;
                else
                    outEdges = findColoredOutEdges(Adj_list, color, u);    % 寻找节点u的着色分支集合
                    color(outEdges) = 0; % 全部褪色
                    u = E(path(end), 2); % 通路的最后一条分支的始节点作为寻边始节点
                    path = path(1:end-1);
                end
            else
                for e = outEdges
                    if color(e)
                        continue;
                    end

                    color(e) = 1;
                    path = [path, e];       % 将分支添加到路径中
                    u = E(e, 3);            % 查询分支的末节点

                    if u == v
                        P{length(P)+1} = path;
                        u = E(path(end), 2); % 通路的最后一条分支的始节点作为寻边始节点
                        path = path(1:end-1);
                    end
                    break;
                end
            end
        end
    end

end