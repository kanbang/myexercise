function changedTopology = adjustTopology(A, X, vertexPosInTopology)
% 调整拓扑
count = 1; % 计数器
for x = X
    % 扫描编码串，根据基因位的值对拓扑进行变换
    v = vertexPosInTopology(count); % 获取当前基因位置对应的节点
    outDegree = length(A{v}); % 查询节点的出度

    A{v} = changeOutEdges(A{v}, x);
    %     switch outDegree
    %         case 2
    %             A{v} = changeOutEdgesOfTwo(A{v}, x);
    %         case 3
    %             A{v} = changeOutEdgesOfThree(A{v}, x);
    %     end

    count = count+1;
end

changedTopology = A;

    function result = swap(m, i, j)
        % 交换数组的元素
        temp = m(i);
        m(i) = m(j);
        m(j) = temp;
        result = m;
    end

    function changedOutEdges = changeOutEdges(outEdges, x)
        % 修改出度为2的节点的出边顺序
        outDegree = length(outEdges); % 计算出度

        changedOutEdges = outEdges; % 默认值
        if outDegree == 2
            if x == 2
                changedOutEdges = fliplr(outEdges);
            end
        else
            fn = factorial(outDegree); % 计算阶乘(出度的总变化方案）
            a = fn/outDegree; % 平均每个位置的变化方案
            pos = ceil(x/a);
            child_pos = x-(pos-1)*a;
            changedOutEdges(1) = outEdges(pos);
            outEdges(pos) = [];
            changedOutEdges(2:end) = changeOutEdges(outEdges, child_pos);
        end
    end

end