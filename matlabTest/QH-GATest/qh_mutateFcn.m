function mutationChildren = qh_mutateFcn(parents, options, nvars, FitnessFcn, state, thisScore, thisPopulation, mutationRate, A, vertexPosInTopology)
% 自定义变异操作
% 额外输入参数
%   A -- 邻接表
%   vertexPosInTopology -- 记录染色体的基因位置所对应的节点

% disp('种群变异操作开始')

mutationChildren = [];
% mutationChildren = cell(length(parents),1);
chromosomeNum = length(parents); % 子个体的个数

for i=1:chromosomeNum
    chromosome = thisPopulation(parents(i), :); % 获取个体    
    mutationChildren = [mutationChildren; doMuate(chromosome, selectMutatePos(length(chromosome)), A, vertexPosInTopology)];
end

    function pos = selectMutatePos(chromosomeLen)
        % 随机选择变异的位置(采用单点变异)
        pos = 1 + round(rand(1)*(chromosomeLen-1));
    end

    function outChromosome = doMuate(inChromosome, pos, A, vertexPosInTopology)
        % 在染色体的第pos个位置上变异
        v = vertexPosInTopology(pos); % 第pos个位置对应的节点
        outDegree = length(A{v}); % 节点的出度
        
        inChromosome(pos) = randInt(1, factorial(outDegree));         
        
        outChromosome = inChromosome;
    end

% disp('种群变异操作结束')
end