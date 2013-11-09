function resultA = runGA(A, VE, from , to)
% 测试ga

popsize = 5; % 种群规模
generation = 10; % 进化代数
crossRate  = 0.8; % 交叉率
mutateRate = 0.015; % 变异率
stallGenLimit = floor(generation/2);

% 记录拓扑编码中每个位置对应的节点
vertexPosInTopology = recordVertexPosInTopology(A);

disp(sprintf('\n可编码节点个数:%d\n', length(vertexPosInTopology)));
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%遗传算法参数设置%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% 定义适应值函数
MyFitnessFcn = @(X) qh_fitness(X, A, VE, vertexPosInTopology, from , to);

% 自定义初始化算子
MyInitialFcn = @(GenomeLength, FitnessFcn, options) qh_initialFcn(GenomeLength, FitnessFcn, options, A);

% 自定义变异算子
MyMutateFcn = @(parents, options, nvars, FitnessFcn, state, thisScore, thisPopulation, mutationRate) qh_mutateFcn(parents, options, nvars, FitnessFcn, state, thisScore, thisPopulation, mutationRate, A, vertexPosInTopology);

% 设置变量个数(我猜测也是代表染色体的长度）
nvars = length(vertexPosInTopology);

%Start with default options
options = gaoptimset;
options = gaoptimset(options,'PopulationType' ,'custom');
options = gaoptimset(options,'Generations' , generation);
options = gaoptimset(options,'PopulationSize' , popsize);

options = gaoptimset(options,'StallGenLimit', stallGenLimit); % 群体在第50次迭代的时候没有改进，算法停止
options = gaoptimset(options,'CrossoverFcn' ,@crossoversinglepoint);
gaoptimset(options,'CrossoverFraction' , crossRate); % 设置交叉率
options = gaoptimset(options,'MutationFcn' , {MyMutateFcn, mutateRate}); % 自定义变异算子以及设置变异率
options = gaoptimset(options, 'CreationFcn', MyInitialFcn); % custom create operator

options = gaoptimset(options,'Display' ,'off');
options = gaoptimset(options,'PlotFcns' ,{ @gaplotbestf });

% 输出遗传算法参数
msg = [sprintf('【遗传算法参数】\n')];
msg = [msg sprintf('\t编码方案:\t%s\n ', '自定义变上限整数编码')];
msg = [msg sprintf('\t种群规模:\t%d\n', popsize)];
msg = [msg sprintf('\t进化代数:\t%d\n', generation)];
msg = [msg sprintf('\t初始化算子:\t%s\n', '自定义随机初始化')];
msg = [msg sprintf('\t交叉算子：\t%s\t交叉率:\t%.3f\n', '单点交叉', crossRate)];
msg = [msg sprintf('\t变异算子:\t%s\t变异率:\t%.3f\n', '自定义单点变异', mutateRate)];
disp(msg);

disp('开始执行ga')
[X,FVAL,REASON,OUTPUT,POPULATION,SCORES] = ga(MyFitnessFcn, nvars, options);

disp('ga运行完毕')
% X = ga(MyFitnessFcn, nvars, options);                   % 运行一次遗传算法
resultA = adjustTopology(A, X, vertexPosInTopology);   % 根据计算结果调整拓扑


    function vertexPosInTopology = recordVertexPosInTopology(A)
        % 记录拓扑编码中每个位置对应的节点
        vertexPosInTopology = [];        
        m = length(A);
        for i=1:m
            if length(A{i})>1
                vertexPosInTopology = [vertexPosInTopology i];  % 记录第i个位置对应的节点                
            end
        end
    end

end
