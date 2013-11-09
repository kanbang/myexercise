function ST=findSpanTree(Adj_list, E, from)
% 查找生成树，采用dfs
    function [treeEdges, outColor] = dfs_visit(Adj_list, E, inColor, u, inTreeEdges)
        % 执行dfs搜索，查找树边分支
        if color(u) == 0
            color(u) = 1; % 着色
            for e = Adj_list{u}
                v = E(e, 3); % 出边分支的末节点
                if color(v) == 0
                    inTreeEdges = [inTreeEdges e];
                    [inTreeEdges, outColor] = dfs_visit(Adj_list, E, color, v, inTreeEdges);
                    color = outColor;
                end
            end
        end
        treeEdges = inTreeEdges;
        outColor = color;
    end

% 节点颜色（0-未着色, 1-着色)
color = zeros(size(Adj_list,1),1);

% 查找生成树
ST = dfs_visit(Adj_list, E, color, from, []);
end