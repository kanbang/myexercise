function P = dfs_visit(A, E, u, v, color)

    function w = caclPathFlowWidth(path, E, theta)
        % 计算通路的最小流量宽度
        % theta -- 误差（可根据实际情况设定，例如0.1）
        q = [];
        for e = path
            q = [q E(e,3)]; % 提取分支的流量
        end
%         q = q(find(q>theta));
        w = min(q); % 求最小的流量
    end

    function outE = removeFlow(E, path, q)
        % 通路path的分支减去流量q
        for e = path
            E(e,3) = E(e,3) - q;
        end
        outE = E;
    end

path=[];
P={};
color= zeros(1, size(E, 1));  % 节点着色集合(初始化为0，默认节点都未着色)
theta = 10^-3; % 流量偏差

while true
    outEdges = findOutEdges(A, E, color, u, 0); % 寻找节点u的未着色出边分支
    if isempty(outEdges)
        if isempty(path)
            return;
        else
            outEdges = findOutEdges(A, E, color, u, 1); % 寻找节点u的着色分支集合
            if isempty(outEdges)               
                continue;
            end
            
%             color(outEdges(1))=0; % 任选一条分支退色（这里选择拓扑顺序的第一条着色分支）
            u = E(path(end), 1);  % 通路的最后一条分支的始节点作为寻边始节点
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
                minQ = caclPathFlowWidth(path, E, theta);
                if minQ > theta % 不存在流量等于0的分支
                    pathNum = size(P,1)+1;
                    P{pathNum, 1} = path;

                    % 修改！！！！！！！！！！！！
                    % 计算通路的最小流量宽度

                    P{pathNum, 2} = minQ;        % 记录通路的最小流量宽度
                    E = removeFlow(E, path, minQ);   % 通路减去流量minQ
                end
                
                u = E(path(end), 1);             % 通路的最后一条分支的始节点作为寻边始节点
                path = path(1:end-1);
            end
            break;
        end
    end
end
end