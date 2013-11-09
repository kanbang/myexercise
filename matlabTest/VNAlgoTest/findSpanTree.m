function ST=findSpanTree(Adj_list, E, from)
% ����������������dfs
    function [treeEdges, outColor] = dfs_visit(Adj_list, E, inColor, u, inTreeEdges)
        % ִ��dfs�������������߷�֧
        if color(u) == 0
            color(u) = 1; % ��ɫ
            for e = Adj_list{u}
                v = E(e, 3); % ���߷�֧��ĩ�ڵ�
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

% �ڵ���ɫ��0-δ��ɫ, 1-��ɫ)
color = zeros(size(Adj_list,1),1);

% ����������
ST = dfs_visit(Adj_list, E, color, from, []);
end