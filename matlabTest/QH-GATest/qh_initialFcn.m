function pop = qh_initialFcn(GenomeLength, FitnessFcn, options, A, vertexPosInTopology)
% 自定义的初始化种群函数
% 额外输入参数：
%   A -- 邻接表

% disp('种群初始化开始')
totalPopulationSize = sum(options.PopulationSize);
% pop = cell(totalPopulationSize,1);
pop = [];

for i=1:totalPopulationSize
    pop = [pop; genChromosome(A)]; % 初始化种群(向种群中添加染色体)
end

%disp('打印初始种群')
%pop

% disp('种群初始化结束')

    function chromosome = genChromosome(A)
        % 随机生成一个编码串
        chromosome=[];
        m = length(A);
        for k=1:m
            outDegree = length(A{k});
%             disp(sprintf('节点v%d的出度为%d\n', k, outDegree));
            if outDegree < 2
                continue;
            end

            chromosome(end+1) = randInt(1, factorial(outDegree));;
        end
    end

end
