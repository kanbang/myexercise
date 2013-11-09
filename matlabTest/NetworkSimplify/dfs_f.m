function [sg, tentacle] = dfs_f(E, A, layer, CE, u, v)
% 深度优先搜索(正向搜索-查找出边)

n = size(E, 1);
color = zeros(1, n);
color(CE) = 1; % 将已搜索过的子网络中的所有分支着色
[sg, tentacle] = dfs1(E, A, layer, u, v, [], [], [], color, false);
tentacle = removeErrorTentacleEdges1(E, A, sg, tentacle, u);

    function [outSG, outTentacle, outTempTentacle, outColor, outGoBack] = dfs1(E, A, layer, u, v, sg, tentacle, tempTentacle, color, goBack)
        % 深度优先搜索(正向搜索-查找出边)
        if u == v
            % 表示已到达汇点
            outSG = sg;
            outTentacle = tentacle;
            outTempTentacle = tempTentacle;
            outColor = color;
            outGoBack = goBack;
            return;
        end

        if layer(u) >= layer(v)
            outSG = sg;
            outTentacle = tentacle;
            outTempTentacle = tempTentacle;
            outColor = color;
            outGoBack = true;  % 已经超过最大路径长度!
            return;
        end

        % 遍历出边
        for e=A{u,1}
            if color(e)
                continue;
            end

            % 可以找到出路
            goBack = false;
            color(e) = 1;

            if ~isempty(tempTentacle)
                tentacle = [tentacle tempTentacle(end)];
                tempTentacle = [];
            end

            sg = [sg e]; % 添加到网络的分支集合中
            [outSG, outTentacle, outTempTentacle, outColor, outGoBack] = dfs1(E, A, layer, E(e,3), v, sg, tentacle, tempTentacle, color, goBack);
            sg = outSG;
            tentacle = outTentacle;
            tempTentacle = outTempTentacle;
            color = outColor;
            goBack = outGoBack;

            if goBack & sg(end) == e % 表示分支没有成功的向前走
                % 需要后退，将分支弹出
                tempTentacle = [tempTentacle e]; % 多余的"触手"分支
                sg = sg(1:end-1);
            end
        end
        
        if ~isempty(tempTentacle)
            tentacle = [tentacle tempTentacle(end)];
            tempTentacle = [];
        end

        outSG = sg;
        outTentacle = tentacle;
        outTempTentacle = tempTentacle;
        outColor = color;
        outGoBack = goBack;
    end

    function outTentacle = removeErrorTentacleEdges1(E, A, sg, tentacle, from)
        % 删除错误的出边"触手"分支
        % 有2类：1) 始节点不在子网络上的分支
        %       2) 以源点作为始节点的出边"触手"分支
        color = zeros(1, size(A, 1));
        for e = sg
            color(E(e, 2)) = 1;
            color(E(e, 3)) = 1;
        end

        outTentacle = [];
        for e = tentacle
            u = E(e, 2);
            if  color(u) & u ~= from   % 分支的始节点是否在子网络上
                outTentacle = [outTentacle e];
            end
        end
    end

end