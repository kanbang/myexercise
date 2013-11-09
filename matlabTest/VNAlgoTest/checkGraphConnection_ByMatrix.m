function isConnectedGraph = checkGraphConnection_ByMatrix(V, E)
% 判断图是否连通，采用矩阵计算的方法
% 参见文献:《基于邻接矩阵图的连通性判定准则》 -- 贾进章，刘剑，宋寿森
% 输入参数：
%   UA -- 无向图的节点邻接矩阵
% 输出参数：
%   isConnectedGraph -- 是否连通

UA = createUA(V, E);
m = size(UA,1);

TA = UA; % 求积累乘
S = UA; % 求和累加
for i=2:m-1
    TA = TA*UA;
    S = S + TA;
end

% 检查S，是否存在不为0的元素
isConnectedGraph = true; % 默认图是连通的
for i = 1:m
    for j = 1:m
        if S(i,j) == 0
            isConnectedGraph = false;
            break;
        end
    end
    if ~isConnectedGraph
        break
    end
end


    function UA = createUA(V, E)
        % 计算真实的节点数
        m = length(find(V~=-1));

        % 构造邻接矩阵A和关联矩阵B
        A=zeros(m, m);
        for i=1:size(E,1)
            if E(i,1) ~= -1
                u = E(i,2); % 分支的始节点
                v = E(i,3); % 分支的末节点
                A(u,v)=1;
            end
        end

        % 构造无向图节点邻接矩阵(对称矩阵)
        UA = A;
        for i=1:m
            for j = 1:i
                if UA(i,j) == UA(j,i) % 同时等于0或同时等于1
                    continue;
                end
                UA(i,j)=1;
                UA(j,i)=1;
            end
        end
    end
end