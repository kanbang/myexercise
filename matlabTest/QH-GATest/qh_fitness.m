function score = qh_fitness(X, A, VE, vertexPosInTopology, from , to)
% ���ۺ��� -- ���㽻��Ŀ���
% ���������
%   A -- ԭʼ������
%   E -- ��֧������Ϣ
%   vertexPosInToplogy -- �ڵ��ڱ��봮x�е�λ��

% disp('��Ⱥ��Ӧֵ���㿪ʼ')

A = adjustTopology(A, X, vertexPosInTopology); % ��������

% disp('�����������!!!')
P = standalone_dfs(A, VE, from, to); % ��������ͨ·

% disp('��������ͨ·���!!!')
% score = 1/caclBlockCrossNum(P); % ����齻����
score = 1500 - caclBlockCrossNum(P); % ����齻����

% disp(score)
    function blockCrossNum = caclBlockCrossNum(P)
        % ����齻����
        len = length(P);

        blockCrossNum = length(P{1});

        if len>1
            for i=2:len
                cp = intersect(P{i}, P{i-1});
                blockCrossNum = blockCrossNum + length(P{i}) - length(cp);
            end
        end
    end

% disp('��Ⱥ��Ӧֵ�������')
end