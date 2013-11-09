function [P, lastPath, lastNode, lastE] = standalone_dfs(A, E, start, target)
% 独立通路法
% 输入参数：记录的是节点的出边
%   A         -- 邻接表（用户需保证网络是单源单汇的），；邻接表是随着编码串的变化而变化的
%   V         -- 图的节点集合
%   E         -- 图的分支集合
%   start     -- 网络的源点
%   target    -- 网络的汇点
% 输出参数：
%   P -- 独立通路集合(使用分支表示通路)

[P, lastPath, lastNode, lastE] = dfs_visit(A, E, start, target); % 求出独立通路集合

    function [P, lastPath, lastNode, lastE] = dfs_visit(A, E, u, v)
        lastPath = [];
        lastNode = [];
        lastE = [];
        path=[];
        P={};
        theta = 10^-3; % 流量偏差

        w = u;
        we=[];
        % 颜色分支(当前正在处理的分支，也即在path中的分支)
        color = zeros(1, size(E, 1));

        while true
            % 寻找节点u的流量大于0，且流量最少的分支
            outEdges = findOutEdges(A, E, color, w, theta);
            if isempty(outEdges)
                if isempty(path)
                    lastPath = path;
                    lastNode = w;
                    lastE = E;
                    return;
                else
                    %                     disp(sprintf('后退1...\n'));
                    w = E(path(end), 1); % 后退
                    path = path(1:end-1);
                    continue;
                end
            end

            for e=outEdges
%             e = outEdges;
            color(e) = 1;
            path = [path e];       % 将分支添加到路径中
            w = E(e, 2);            % 查询分支的末节点

            if w == v
                pathNum = size(P,1)+1;
                disp(sprintf('找到一条通路:%s\n', mat2str(path)));
                P{pathNum, 1} = path; % 记录通路

                [minQ we]= caclPathFlowWidth(path, E, theta); % 计算通路中的最小流量宽度
                P{pathNum, 2} = minQ;                     % 记录通路的最小流量宽度
                P{pathNum, 3} = path(we);                 % 记录宽度分支
                E = removeFlow(E, path, minQ);            % 通路减去流量minQ
                w = u;
                path = [];
                color = zeros(1, size(E, 1));
            end
            break;
        end
        end

    end

    function outEdges = findOutEdges(A, E, color, v, theta)
        % 函数功能：
        % 根据当前的拓扑结构，查找当前节点v的出边分支
        % 1）排除非0流量分支
        % 输入参数：
        %   A -- 节点邻接矩阵
        %   v -- 要查找出边的节点
        % 输出结果：
        %   outEdges -- 节点v的出边分支

                outEdges = A{v}; % 提取矩阵A的第v行（等价于获取节点v的出边信息）

        % 排除颜色分支(当前正在处理的分支，也即在path中的分支)
                outEdges = outEdges(find(color(outEdges)==0));

        % 排除0流量分支
                outEdges = outEdges(find(E(outEdges, 3)>theta));

%         outEdges = [];
%         for e=A{v}
%             if color(e) == 1 | E(e,3) <= theta
%                 continue;
%             end
% 
%             if isempty(outEdges)
%                 outEdges = e;
%             elseif E(outEdges, 3) > E(e,3)
%                 outEdges = e;
%             end
%         end
    end

    function [width ei] = caclPathFlowWidth(path, E, theta)
        % 计算通路的最小流量宽度
        % theta -- 误差（可根据实际情况设定，例如0.1）
        q = [];
        for e = path
            q = [q E(e,3)]; % 提取分支的流量
        end
        q = q(find(q>theta));
        [width ei] = min(q); % 求最小的流量已经所在的位置index
    end

    function outE = removeFlow(E, path, q)
        % 通路path的分支减去流量q
        for e = path
            E(e,3) = E(e,3) - q;
        end
        outE = E;
    end

end
