function test_qh_ga()
% ���Խ���������ga��Ӱ��

% ��������
[A, V, VE, E, vertexPosInTopology, from, to] = loadNetwork();

% ������Ӧֵ����
MyFitnessFcn = @(X) qh_fitness(X, A, VE, E, vertexPosInTopology, from , to);

% �Զ����ʼ������
MyInitialFcn = @(GenomeLength, FitnessFcn, options) qh_initialFcn(GenomeLength, FitnessFcn, options, A);

% �Զ����������
MyMutateFcn = @(parents, options, nvars, FitnessFcn, state, thisScore, thisPopulation) qh_mutateFcn(parents, options, nvars, FitnessFcn, state, thisScore, thisPopulation, A, vertexPosInTopology);

% ���ñ�������(�Ҳ²�Ҳ�Ǵ���Ⱦɫ��ĳ��ȣ�
nvars = length(vertexPosInTopology);

%Start with default options
options = gaoptimset;
options = gaoptimset(options,'PopulationType' ,'custom');
% options = gaoptimset(options,'Generations' ,100);
options = gaoptimset(options,'PopulationSize' ,30);

%options = gaoptimset(options,'StallGenLimit', 50); % Ⱥ���ڵ�50�ε�����ʱ��û�иĽ����㷨ֹͣ
options = gaoptimset(options,'CrossoverFcn' ,@crossoversinglepoint);
options = gaoptimset(options,'MutationFcn' , MyMutateFcn); % custom mutate operator
options = gaoptimset(options, 'CreationFcn', MyInitialFcn); % custom create operator

options = gaoptimset(options,'Display' ,'off');
% options = gaoptimset(options,'PlotFcns' ,{ @gaplotbestf });

% ���Խ���������ga��Ӱ��
gen = 10:1:20;
y = [];
for i = gen
    options = gaoptimset(options,'Generations' ,i);
    [X,FVAL,REASON,OUTPUT,POPULATION,SCORES] = ga(MyFitnessFcn, nvars, options);
    y = [y FVAL]; % ��Ӧֵ
end

subplot(2,2,1);
plot(gen, y); % ��������

options = gaoptimset(options,'Generations' ,10); % ���������̶�Ϊ100
% ������Ⱥ������ga��Ӱ��
ps = 10:1:30;
y = [];
for j = ps
    options = gaoptimset(options,'PopulationSize' ,j);
    [X,FVAL,REASON,OUTPUT,POPULATION,SCORES] = ga(MyFitnessFcn, nvars, options);
    y = [y FVAL]; % ��Ӧֵ
end

subplot(2,2,2);
plot(ps, y); % ��������