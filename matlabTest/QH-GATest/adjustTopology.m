function changedTopology = adjustTopology(A, X, vertexPosInTopology)
% ��������
count = 1; % ������
for x = X
    % ɨ����봮�����ݻ���λ��ֵ�����˽��б任
    v = vertexPosInTopology(count); % ��ȡ��ǰ����λ�ö�Ӧ�Ľڵ�
    outDegree = length(A{v}); % ��ѯ�ڵ�ĳ���

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
        % ���������Ԫ��
        temp = m(i);
        m(i) = m(j);
        m(j) = temp;
        result = m;
    end

    function changedOutEdges = changeOutEdges(outEdges, x)
        % �޸ĳ���Ϊ2�Ľڵ�ĳ���˳��
        outDegree = length(outEdges); % �������

        changedOutEdges = outEdges; % Ĭ��ֵ
        if outDegree == 2
            if x == 2
                changedOutEdges = fliplr(outEdges);
            end
        else
            fn = factorial(outDegree); % ����׳�(���ȵ��ܱ仯������
            a = fn/outDegree; % ƽ��ÿ��λ�õı仯����
            pos = ceil(x/a);
            child_pos = x-(pos-1)*a;
            changedOutEdges(1) = outEdges(pos);
            outEdges(pos) = [];
            changedOutEdges(2:end) = changeOutEdges(outEdges, child_pos);
        end
    end

end