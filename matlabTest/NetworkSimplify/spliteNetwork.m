function networkList = simplifyNetwork(V, E, A, layer, from, to)
% 分割网络
% 输入参数：
%   E -- 分支集合
%   A -- 双向邻接表
%   from -- 源点
%   to -- 汇点
%   layer -- 节点的层次属性
%  输出参数：
%   networkList -- 简化后的网络链表
%   采用cell表示，类似于2维数组
%   第1列 -- 源点、汇点
%   第2列 -- 网络类型(串联-1; 并联-2; 角联-3)
%   第3列 -- 父网络的编号(0表示没有父网络，也表示该网络处于最顶层位置)
%   第4列 -- 虚拟分支编号
%   第5列 -- 网络的独立分支集合

SS = buildSSArray(E, A, layer); % 构建源汇节点对
SS(end+1, 1:end) = [from to];   % 将原始网络的源汇添加到SS中

disp('层次间距和层次之和较小的优先排序');
disp('源汇数组对:');
msg = sprintf('\t源点\t汇点\t层次间距\t层次之和\n');
for i=1:size(SS,1)
    u = SS(i,1);
    v = SS(i,2);
    lu = layer(u);
    lv = layer(v);
    msg = [msg sprintf('\t%d\t\t%d\t\t%d\t\t%d\n', u, v, lv-lu, lu+lv)];
end
disp(msg);

networkList = findChildNetwork(E, A, SS); % 查找子网络，并保存到网络链表中

% 子函数
    function networkList = findChildNetwork(E, A, SS)
        % 查找子网络，并保存到网络链表中
        networkList = {};
        CE = []; % 已确定的子网络的分支集合

        pairCount = size(SS, 1);
        for i=1:pairCount
            % 正向搜索
            u = SS(i, 1);
            v = SS(i, 2);
            disp(sprintf('==>搜索源汇%d->%d\n', u, v));

            % 搜索子网络以及出边触手分支和入边触手分支（核心算法）
            [outSG, outTentacle, inSG, inTentacle] = dfs(E, A, layer, CE, u, v);

            % 源汇u->v无法构成独立的子网络
            if isempty(outSG) | isempty(inSG) | ~isempty(outTentacle) | ~isempty(inTentacle)
                continue;
            end

            % 排除只有一条分支的子网络
            if length(outSG)<2
                continue;
            end

            % 通过正向和反向搜索，已确定该网络是一个独立的子网络
            % 需要做如下几件事：
            %   1) 确定网络的类型
            type = getNetworkType(E, outSG);

            if type == 2 % 并联网络
                outSG2 = [];
                
                CE = union(CE, outSG); % 将子网络中的分支全部添加到CE中
                
                % 需要对并联网络进行拆分
                sg = [];
                for ee = outSG
                    sg = [sg ee];
                    % 找到一条"并联分支"
                    if E(ee, 3) == v % 汇点                        
                        type = getNetworkType(E, sg);
                        
                        % 排除长度为1的串联分支或不是大串联的情况
                        if type == 1 & length(sg)<2
                            outSG2 = [outSG2 sg];
                            sg = [];
                            continue;
                        end
                                                
                        ve = size(E,1)+1;
                        E(ve, 1:end)=[-2 u v]; % -2表示该分支是虚拟的源汇分支
                        A{u,1}=[A{u,1} ve];
                        A{v,2}=[A{v,2} ve];
                        
                        networkList = mergeNetworkList(E, networkList, u, v, ve, type, sg);
                        
                        outSG2 = [outSG2 ve];
                        sg = []; % 清空，寻找下一条"并联分支"
                    end
                end
                
                % 如果outSG2为空，表示该并联结构不可拆分
                outSG = outSG2;
                type = 2;
            end
            
            % 在进行并联处理之后
            if isempty(outSG)
                continue;
            end
            
            %   3) 将子网络中的分支添加到CE中，在dfs搜索中将不再搜索这些分支            
            CE = union(CE, outSG);

            %   4) 建立一个虚拟分支，并添加到分支集合E和邻接表A中
            ve = size(E,1)+1;
            E(ve, 1:end)=[-2 u v]; % -2表示该分支是虚拟的源汇分支
            A{u,1}=[A{u,1} ve];
            A{v,2}=[A{v,2} ve];

            %   5) 合并到网络链表中
            networkList = mergeNetworkList(E, networkList, u, v, ve, type, outSG);

        end

        outE = E;
        outA = A;
    end

    function [outSG, outTentacle, inSG, inTentacle] = dfs(E, A, layer, CE, u, v)
        outSG = [];
        inSG = [];
        outTentacle = [];
        inTentacle = [];

        [outSG, outTentacle] = dfs_f(E, A, layer, CE, u, v);
        disp(sprintf('\t正向搜索:\n\t\t子网络分支集合:%s\n\t\t出边触手分支:%s', mat2str(outSG), mat2str(outTentacle)));
        % 表示源汇不能构成子网络
        if isempty(outSG)
            return;
        end

        % 有出边“触手”分支，表示不是一个独立的子网络
        % 将来可能会考虑一种“假网络简化”(!!!)
        if ~isempty(outTentacle)
            return;
        end

        [inSG, inTentacle] = dfs_b(E, A, layer, CE, v, u);
        disp(sprintf('\t反向搜索:\n\t\t子网络分支集合:%s\n\t\t入边触手分支:%s', mat2str(inSG), mat2str(inTentacle)));
        if isempty(inSG)
            return;
        end

        % 有入边“触手”分支，表示不是一个独立的子网络
        % 将来可能会考虑一种“假网络简化”(!!!)
        if ~isempty(inTentacle)
            return;
        end
    end
   
    function outNetworkList = mergeNetworkList(E, networkList, u, v, e, type, sg)
        % 将子网络合并到网络链表中
        % 需要建立子网络和父网络的链接关系
        count = size(networkList, 1) + 1;
        networkList{count, 1} = [u v]; % 子网络的源汇
        networkList{count, 2} = type;     % 子网络的类型
        networkList{count, 3} = 0;     % 子网络的父网络
        networkList{count, 4} = e;     % 子网络对应的虚拟分支
        networkList{count, 5} = sg;    % 子网络的分支集合

        % 对子网络进行遍历，查找是否存在子网络
        for ee = sg
            if E(ee,1) == -2   % -2表示该分支是虚拟的源汇分支
                for i=1:count-1
                    if networkList{i, 4} == ee
                        networkList{i, 3} = count;
                    end
                end
            end
        end

        outNetworkList = networkList;
    end

    function type = getNetworkType(E, sg)
        % 确定子网络的类型
        sv = []; % 子网络的节点集合
        for e = sg
            sv = union(sv, E(e, 2:3));
        end

        cn = length(sg); % 子网络的分支数
        cm = length(sv); % 子网络的节点数

        type = 0; % 不确定网络类型
        if cn == cm-1    % 串联
            type = 1;
        elseif cn == cm  % 并联
            type = 2;
        else             % 角联
            type = 3;
        end
    end

end