function mutationChildren = qh_mutateFcn(parents, options, nvars, FitnessFcn, state, thisScore, thisPopulation, mutationRate, A, vertexPosInTopology)
% �Զ���������
% �����������
%   A -- �ڽӱ�
%   vertexPosInTopology -- ��¼Ⱦɫ��Ļ���λ������Ӧ�Ľڵ�

% disp('��Ⱥ���������ʼ')

mutationChildren = [];
% mutationChildren = cell(length(parents),1);
chromosomeNum = length(parents); % �Ӹ���ĸ���

for i=1:chromosomeNum
    chromosome = thisPopulation(parents(i), :); % ��ȡ����    
    mutationChildren = [mutationChildren; doMuate(chromosome, selectMutatePos(length(chromosome)), A, vertexPosInTopology)];
end

    function pos = selectMutatePos(chromosomeLen)
        % ���ѡ������λ��(���õ������)
        pos = 1 + round(rand(1)*(chromosomeLen-1));
    end

    function outChromosome = doMuate(inChromosome, pos, A, vertexPosInTopology)
        % ��Ⱦɫ��ĵ�pos��λ���ϱ���
        v = vertexPosInTopology(pos); % ��pos��λ�ö�Ӧ�Ľڵ�
        outDegree = length(A{v}); % �ڵ�ĳ���
        
        inChromosome(pos) = randInt(1, factorial(outDegree));         
        
        outChromosome = inChromosome;
    end

% disp('��Ⱥ�����������')
end