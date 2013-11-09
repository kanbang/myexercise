function SC = findCycle_BySearch(E, Adj_list, from)
% 通过搜索的方法计算单向回路

m = size(Adj_list,1);
dfn = zeros(1,m); % 已访问节点的顺序
color = zeros(1,size(E, 1)); % 节分支是否已被访问过

parent(1:m)=-1;   % 记录节点的父节点均未知

k = 1; % 计数器
SC={};
for u=1:m
    if dfn(u)==0
        [SC, dfn, color, k, parent]=dfs(Adj_list, E, from, dfn, color, k, parent, SC);
    end
end

    function outSC=dfs(Adj_list, E, u, SC)
         w = u;
         color = zeros(1, size(E, 1));                    % 分支是否着色
         visited = zeros(1,size(Adj_list,1));             % 节点是否已访问
         path=[];
        while true
            outEdges = findOutEdges(A, E, color, w);
            if isempty(outEdges)
                if isempty(path)
                    return;
                else
                    w = E(path(end), 1); % 后退
                    path = path(1:end-1);
                    continue;
                end
            end

            for e = outEdges
                w = E(e, 3);
                if color(e)
                    % 找到一个单向回路
                    nodesOfCycle = findNodesOfCycle(parent, u, w);
                    edgesOfCycle = findEdgesOfCycle(E, Adj_list, nodesOfCycle);
                    if ~isempty(edgesOfCycle)
                        SC{end+1} = edgesOfCycle;
                    end
                elseif dfn(w)==0
                    parent(w)=u;
                    [outSC, outDfn, outColor, outK, outParent]=dfs(Adj_list, E, w, dfn, color, k, parent, SC);
                    dfn=outDfn;
                    color=outColor;
                    k=outK;
                    parent=outParent;
                    SC=outSC;
                end
            end
            color(u)=0;

            outDfn=dfn;
            outColor=color;
            outK=k;
            outParent=parent;
            outSC=SC;
        end
    end

    function outEdges = findOutEdges(A, E, color, v)
        outEdges = A{v}; % 提取矩阵A的第v行（等价于获取节点v的出边信息）

        % 排除颜色分支(当前正在处理的分支，也即在path中的分支)
        outEdges = outEdges(find(color(outEdges)==0));
    end

    function nodesOfCycle = findNodesOfCycle(parent, u, v)
        % 查找单向回路u->v->u上的节点
        % 用节点表示单向回路
        nodesOfCycle =[];
        while u ~= v
            nodesOfCycle = [u nodesOfCycle]; % 逆向
            u = parent(u);
        end
        nodesOfCycle = [nodesOfCycle v];
    end

    function edgesOfCycle = findEdgesOfCycle(E, Adj_list, nodesOfCycle)
        % 根据节点单向回路查找分支单向回路
        % 用分支表示单向回路
        edgesOfCycle = [];
        if length(nodesOfCycle) <= 1 % 必须至少存在2个节点
            return;
        end

        % 求解单向回路的分支路径
        u = nodesOfCycle(1); % 取出第1个节点作为回路的开始点
        nodesOfCycle = [nodesOfCycle u]; % 构成一个环
        for v = nodesOfCycle
            if u == v
                continue;
            end

            for e = Adj_list{u}
                if E(e, 3) == v
                    edgesOfCycle = [edgesOfCycle e];
                end
            end
            u = v;
        end
    end

end
