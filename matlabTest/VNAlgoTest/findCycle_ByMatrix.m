function SC = findCycle_ByMatrix(A, E, Adj_list)
% 采用节点邻接矩阵计算单向回路
% (***)该矩阵算法还是有缺陷，单向回路会被计算多次
% 参考文献：《A new matrix-based mathematical model for determining
%            unidirectional circuits in a ventilation network》 -- 贾进章
%            《单向回路数学模型.doc》 -- 英文文献重点部分的中文说明
% 输入参数：
%   A -- 节点邻接矩阵
%   E -- 分支集合
%   Adj_list -- 邻接表
% 输出参数：
%   SC -- 单向回路集合
% celldisp(SC)

m = size(A,1);
SC={};
TA=1;
for i=1:m-1
    TA=TA*A;
    nodesOfCycle = findNodesOfCycle(TA);
    if isempty(nodesOfCycle)
        continue;
    end

    % 单向回路的节点不一定是按照顺序排列的
    % 该矩阵算法还是有缺陷，单向回路会被计算多次
    edgesOfCycle = findEdgesOfCycle(E, Adj_list, nodesOfCycle);
    cycleNum = length(edgesOfCycle);
    for i=1:cycleNum
        SC{end+1} = edgesOfCycle{i};
    end
end


% 子函数
    function nodesOfCycle = findNodesOfCycle(A_k)
        % 查找单向回路上的节点
        % 用节点表示单向回路
        nodesOfCycle =[];
        n = size(A_k,1);
        for j=1:n
            if A_k(j,j)~=0
                nodesOfCycle = [nodesOfCycle j];
            end
        end
    end

    function edgesOfCycle = findEdgesOfCycle(E, Adj_list, nodesOfCycle)
        % 根据节点单向回路查找分支单向回路
        % 采用一个修改的dfs算法来查找由节点nodesOfCycle构成的单向回路
        % 参考文献：《流体网络》第3章-3.2.4小节：44-45
        edgesOfCycle = {};
        color = zeros(1, size(Adj_list,1));
        visited = zeros(1, size(E,1)); % 是否访问过
        color(nodesOfCycle) = 1; % 标记单向回路中的节点
        nodesOfPath = [];
        path=[];

        cycleEdges = [];
        u = nodesOfCycle(1);
        currentStartNode = u;
        otherCycleNodes = [];

        while true
            outEdges = findOutEdgesOfCycle_unVisited(E, Adj_list, color, visited, nodesOfPath, u);
            if isempty(outEdges)
                if isempty(path)
                    return;
                else
                    outEdges = findOutEdgesOfCycle_visited(E, Adj_list, color, visited, u);
                    for e = outEdges
                        visited(e)=0;
                    end
                    u = E(path(end), 2);
                    path = path(1:end-1);
                    nodesOfPath = nodesOfPath(1:end-1);
                end
            else
                for e = outEdges
                    u = E(e, 3);
                    visited(e)= 1;
                    path = [path e];
                    nodesOfPath = [nodesOfPath u];

                    if u == currentStartNode % 找到一个单向回路
                        edgesOfCycle{end+1} = path;
                        cycleEdges = [cycleEdges path]; % 记录已查到的单向回路

                        if isempty(nodesOfCycle)
                            return;
                        end
                        currentStartNode = findStartNode(E, nodesOfCycle, cycleEdges);
                        if isempty(currentStartNode)
                            return;
                        end
                        u = currentStartNode;
                        path = [];
                        nodesOfPath = [];
                        color = zeros(1, size(Adj_list,1));
                        visited = zeros(1, size(E,1)); % 是否访问过
                        color(nodesOfCycle) = 1; % 标记单向回路中的节点
                    end
                    break;
                end
            end
        end
    end

    function startNode = findStartNode(E, nodesOfCycle, cycleEdges)
        tnodes = [];
        for e = cycleEdges
            tnodes = union(tnodes, E(e,2:3));
        end

        startNode = [];
        nodes = [];
        for v = nodesOfCycle
            if(isempty(find(tnodes==v)))
                startNode = v;
                break;
            end
        end
    end

    function outEdges = findOutEdgesOfCycle_unVisited(E, Adj_list, color, visited, nodesOfPath, u)
        % 查找节点u的在单向回路，且未访问过的节点之间的分支
        outEdges = [];
        for e = Adj_list{u}
            v = E(e, 3);
            if ~isempty(find(nodesOfPath==v))
                continue;
            end

            if color(v) & ~visited(e)
                outEdges = [outEdges e];
            end
        end
    end

    function outEdges = findOutEdgesOfCycle_visited(E, Adj_list, color, visited, u)
        % 查找节点u的在单向回路，且已经访问过的节点之间的分支
        outEdges = [];
        for e = Adj_list{u}
            v = E(e, 3);
            if color(v) & visited(e)
                outEdges = [outEdges e];
            end
        end
    end

end