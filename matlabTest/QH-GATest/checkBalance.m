function isBalanced = checkBalance(V, E)
% ���ڵ�����Ƿ�ƽ��

theta = 0.99;
unBalanceNodes = [];
for i=1:length(V)
    % ������������
    inSum = caclFlow(E, findInEdges(E, i));
    % ������������
    outSum = caclFlow(E, findOutEdges(E, i));
    
%     disp(inSum~=outSum);
%     disp(abs(inSum-outSum));

    if (inSum>0) & (outSum>0) & (abs(inSum-outSum)>theta)
        unBalanceNodes = [unBalanceNodes V(i)];
        disp(sprintf('�ڵ�%d��������%.4f, ��������:%.4f\n', V(i), inSum, outSum));
    end
end

isBalanced = isempty(unBalanceNodes);
% disp('������ƽ��Ľڵ�:');
% disp(unBalanceNodes);

    function inEdges = findInEdges(E, u)
        inV = E(1:end, 3);
        inEdges = find(inV==u);
        inEdges = inEdges';
    end

    function outEdges = findOutEdges(E, u)
        outV = E(1:end, 2);
        outEdges = find(outV==u);
        outEdges = outEdges';
    end

    function sumFlow = caclFlow(E, edges)
        sumFlow = 0;
        for e=edges
            sumFlow = sumFlow+E(e,4);
        end
    end
end