function resultA = runGA(A, VE, from , to)
% ����ga

popsize = 5; % ��Ⱥ��ģ
generation = 10; % ��������
crossRate  = 0.8; % ������
mutateRate = 0.015; % ������
stallGenLimit = floor(generation/2);

% ��¼���˱�����ÿ��λ�ö�Ӧ�Ľڵ�
vertexPosInTopology = recordVertexPosInTopology(A);

disp(sprintf('\n�ɱ���ڵ����:%d\n', length(vertexPosInTopology)));
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%�Ŵ��㷨��������%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% ������Ӧֵ����
MyFitnessFcn = @(X) qh_fitness(X, A, VE, vertexPosInTopology, from , to);

% �Զ����ʼ������
MyInitialFcn = @(GenomeLength, FitnessFcn, options) qh_initialFcn(GenomeLength, FitnessFcn, options, A);

% �Զ����������
MyMutateFcn = @(parents, options, nvars, FitnessFcn, state, thisScore, thisPopulation, mutationRate) qh_mutateFcn(parents, options, nvars, FitnessFcn, state, thisScore, thisPopulation, mutationRate, A, vertexPosInTopology);

% ���ñ�������(�Ҳ²�Ҳ�Ǵ���Ⱦɫ��ĳ��ȣ�
nvars = length(vertexPosInTopology);

%Start with default options
options = gaoptimset;
options = gaoptimset(options,'PopulationType' ,'custom');
options = gaoptimset(options,'Generations' , generation);
options = gaoptimset(options,'PopulationSize' , popsize);

options = gaoptimset(options,'StallGenLimit', stallGenLimit); % Ⱥ���ڵ�50�ε�����ʱ��û�иĽ����㷨ֹͣ
options = gaoptimset(options,'CrossoverFcn' ,@crossoversinglepoint);
gaoptimset(options,'CrossoverFraction' , crossRate); % ���ý�����
options = gaoptimset(options,'MutationFcn' , {MyMutateFcn, mutateRate}); % �Զ�����������Լ����ñ�����
options = gaoptimset(options, 'CreationFcn', MyInitialFcn); % custom create operator

options = gaoptimset(options,'Display' ,'off');
options = gaoptimset(options,'PlotFcns' ,{ @gaplotbestf });

% ����Ŵ��㷨����
msg = [sprintf('���Ŵ��㷨������\n')];
msg = [msg sprintf('\t���뷽��:\t%s\n ', '�Զ����������������')];
msg = [msg sprintf('\t��Ⱥ��ģ:\t%d\n', popsize)];
msg = [msg sprintf('\t��������:\t%d\n', generation)];
msg = [msg sprintf('\t��ʼ������:\t%s\n', '�Զ��������ʼ��')];
msg = [msg sprintf('\t�������ӣ�\t%s\t������:\t%.3f\n', '���㽻��', crossRate)];
msg = [msg sprintf('\t��������:\t%s\t������:\t%.3f\n', '�Զ��嵥�����', mutateRate)];
disp(msg);

disp('��ʼִ��ga')
[X,FVAL,REASON,OUTPUT,POPULATION,SCORES] = ga(MyFitnessFcn, nvars, options);

disp('ga�������')
% X = ga(MyFitnessFcn, nvars, options);                   % ����һ���Ŵ��㷨
resultA = adjustTopology(A, X, vertexPosInTopology);   % ���ݼ�������������


    function vertexPosInTopology = recordVertexPosInTopology(A)
        % ��¼���˱�����ÿ��λ�ö�Ӧ�Ľڵ�
        vertexPosInTopology = [];        
        m = length(A);
        for i=1:m
            if length(A{i})>1
                vertexPosInTopology = [vertexPosInTopology i];  % ��¼��i��λ�ö�Ӧ�Ľڵ�                
            end
        end
    end

end
