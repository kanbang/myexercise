function pop = qh_initialFcn(GenomeLength, FitnessFcn, options, A, vertexPosInTopology)
% �Զ���ĳ�ʼ����Ⱥ����
% �������������
%   A -- �ڽӱ�

% disp('��Ⱥ��ʼ����ʼ')
totalPopulationSize = sum(options.PopulationSize);
% pop = cell(totalPopulationSize,1);
pop = [];

for i=1:totalPopulationSize
    pop = [pop; genChromosome(A)]; % ��ʼ����Ⱥ(����Ⱥ�����Ⱦɫ��)
end

%disp('��ӡ��ʼ��Ⱥ')
%pop

% disp('��Ⱥ��ʼ������')

    function chromosome = genChromosome(A)
        % �������һ�����봮
        chromosome=[];
        m = length(A);
        for k=1:m
            outDegree = length(A{k});
%             disp(sprintf('�ڵ�v%d�ĳ���Ϊ%d\n', k, outDegree));
            if outDegree < 2
                continue;
            end

            chromosome(end+1) = randInt(1, factorial(outDegree));;
        end
    end

end
