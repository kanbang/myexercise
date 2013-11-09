function test_qh_ga()
% 测试进化代数对ga的影响

% 加载网络
[A, V, VE, E, vertexPosInTopology, from, to] = loadNetwork();

% 定义适应值函数
MyFitnessFcn = @(X) qh_fitness(X, A, VE, E, vertexPosInTopology, from , to);

% 自定义初始化算子
MyInitialFcn = @(GenomeLength, FitnessFcn, options) qh_initialFcn(GenomeLength, FitnessFcn, options, A);

% 自定义变异算子
MyMutateFcn = @(parents, options, nvars, FitnessFcn, state, thisScore, thisPopulation) qh_mutateFcn(parents, options, nvars, FitnessFcn, state, thisScore, thisPopulation, A, vertexPosInTopology);

% 设置变量个数(我猜测也是代表染色体的长度）
nvars = length(vertexPosInTopology);

%Start with default options
options = gaoptimset;
options = gaoptimset(options,'PopulationType' ,'custom');
% options = gaoptimset(options,'Generations' ,100);
options = gaoptimset(options,'PopulationSize' ,30);

%options = gaoptimset(options,'StallGenLimit', 50); % 群体在第50次迭代的时候没有改进，算法停止
options = gaoptimset(options,'CrossoverFcn' ,@crossoversinglepoint);
options = gaoptimset(options,'MutationFcn' , MyMutateFcn); % custom mutate operator
options = gaoptimset(options, 'CreationFcn', MyInitialFcn); % custom create operator

options = gaoptimset(options,'Display' ,'off');
% options = gaoptimset(options,'PlotFcns' ,{ @gaplotbestf });

% 测试进化代数对ga的影响
gen = 10:1:20;
y = [];
for i = gen
    options = gaoptimset(options,'Generations' ,i);
    [X,FVAL,REASON,OUTPUT,POPULATION,SCORES] = ga(MyFitnessFcn, nvars, options);
    y = [y FVAL]; % 适应值
end

subplot(2,2,1);
plot(gen, y); % 绘制曲线

options = gaoptimset(options,'Generations' ,10); % 进化代数固定为100
% 测试种群个数对ga的影响
ps = 10:1:30;
y = [];
for j = ps
    options = gaoptimset(options,'PopulationSize' ,j);
    [X,FVAL,REASON,OUTPUT,POPULATION,SCORES] = ga(MyFitnessFcn, nvars, options);
    y = [y FVAL]; % 适应值
end

subplot(2,2,2);
plot(ps, y); % 绘制曲线