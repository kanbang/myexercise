function P = dfs_visit(Adj_list, E, u, v, color)
path=[];
P={};
color= zeros(1, size(E, 1));  % 节点着色集合(初始化为0，默认节点都未着色)

while true
    outEdges = findOutEdges(Adj_list, color, u, 0); % 寻找节点u的未着色出边分支
    if isempty(outEdges)
        if isempty(path)
            return;
        else
            outEdges = findOutEdges(Adj_list, color, u, 1); % 寻找节点u的着色分支集合
            color(outEdges(1))=0; % 任选一条分支退色（这里选择拓扑顺序的第一条着色分支）
            u = E(path(end), 1); % 通路的最后一条分支的始节点作为寻边始节点
            path = path(1:end-1);
        end
    else
        for e = outEdges
            if color(e)
                continue;
            end

            color(e) = 1;
            path = [path, e];       % 将分支添加到路径中
            u = E(e, 2);            % 查询分支的末节点

            if u == v
                P{length(P)+1} = path;
                u = E(path(end), 1); % 通路的最后一条分支的始节点作为寻边始节点
                path = path(1:end-1);
            end
            break;
        end
    end
end
end