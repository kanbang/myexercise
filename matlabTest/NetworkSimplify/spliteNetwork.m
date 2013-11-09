function networkList = simplifyNetwork(V, E, A, layer, from, to)
% �ָ�����
% ���������
%   E -- ��֧����
%   A -- ˫���ڽӱ�
%   from -- Դ��
%   to -- ���
%   layer -- �ڵ�Ĳ������
%  ���������
%   networkList -- �򻯺����������
%   ����cell��ʾ��������2ά����
%   ��1�� -- Դ�㡢���
%   ��2�� -- ��������(����-1; ����-2; ����-3)
%   ��3�� -- ������ı��(0��ʾû�и����磬Ҳ��ʾ�����紦�����λ��)
%   ��4�� -- �����֧���
%   ��5�� -- ����Ķ�����֧����

SS = buildSSArray(E, A, layer); % ����Դ��ڵ��
SS(end+1, 1:end) = [from to];   % ��ԭʼ�����Դ����ӵ�SS��

disp('��μ��Ͳ��֮�ͽ�С����������');
disp('Դ�������:');
msg = sprintf('\tԴ��\t���\t��μ��\t���֮��\n');
for i=1:size(SS,1)
    u = SS(i,1);
    v = SS(i,2);
    lu = layer(u);
    lv = layer(v);
    msg = [msg sprintf('\t%d\t\t%d\t\t%d\t\t%d\n', u, v, lv-lu, lu+lv)];
end
disp(msg);

networkList = findChildNetwork(E, A, SS); % ���������磬�����浽����������

% �Ӻ���
    function networkList = findChildNetwork(E, A, SS)
        % ���������磬�����浽����������
        networkList = {};
        CE = []; % ��ȷ����������ķ�֧����

        pairCount = size(SS, 1);
        for i=1:pairCount
            % ��������
            u = SS(i, 1);
            v = SS(i, 2);
            disp(sprintf('==>����Դ��%d->%d\n', u, v));

            % �����������Լ����ߴ��ַ�֧����ߴ��ַ�֧�������㷨��
            [outSG, outTentacle, inSG, inTentacle] = dfs(E, A, layer, CE, u, v);

            % Դ��u->v�޷����ɶ�����������
            if isempty(outSG) | isempty(inSG) | ~isempty(outTentacle) | ~isempty(inTentacle)
                continue;
            end

            % �ų�ֻ��һ����֧��������
            if length(outSG)<2
                continue;
            end

            % ͨ������ͷ�����������ȷ����������һ��������������
            % ��Ҫ�����¼����£�
            %   1) ȷ�����������
            type = getNetworkType(E, outSG);

            if type == 2 % ��������
                outSG2 = [];
                
                CE = union(CE, outSG); % ���������еķ�֧ȫ����ӵ�CE��
                
                % ��Ҫ�Բ���������в��
                sg = [];
                for ee = outSG
                    sg = [sg ee];
                    % �ҵ�һ��"������֧"
                    if E(ee, 3) == v % ���                        
                        type = getNetworkType(E, sg);
                        
                        % �ų�����Ϊ1�Ĵ�����֧���Ǵ��������
                        if type == 1 & length(sg)<2
                            outSG2 = [outSG2 sg];
                            sg = [];
                            continue;
                        end
                                                
                        ve = size(E,1)+1;
                        E(ve, 1:end)=[-2 u v]; % -2��ʾ�÷�֧�������Դ���֧
                        A{u,1}=[A{u,1} ve];
                        A{v,2}=[A{v,2} ve];
                        
                        networkList = mergeNetworkList(E, networkList, u, v, ve, type, sg);
                        
                        outSG2 = [outSG2 ve];
                        sg = []; % ��գ�Ѱ����һ��"������֧"
                    end
                end
                
                % ���outSG2Ϊ�գ���ʾ�ò����ṹ���ɲ��
                outSG = outSG2;
                type = 2;
            end
            
            % �ڽ��в�������֮��
            if isempty(outSG)
                continue;
            end
            
            %   3) ���������еķ�֧��ӵ�CE�У���dfs�����н�����������Щ��֧            
            CE = union(CE, outSG);

            %   4) ����һ�������֧������ӵ���֧����E���ڽӱ�A��
            ve = size(E,1)+1;
            E(ve, 1:end)=[-2 u v]; % -2��ʾ�÷�֧�������Դ���֧
            A{u,1}=[A{u,1} ve];
            A{v,2}=[A{v,2} ve];

            %   5) �ϲ�������������
            networkList = mergeNetworkList(E, networkList, u, v, ve, type, outSG);

        end

        outE = E;
        outA = A;
    end

    function [outSG, outTentacle, inSG, inTentacle] = dfs(E, A, layer, CE, u, v)
        outSG = [];
        inSG = [];
        outTentacle = [];
        inTentacle = [];

        [outSG, outTentacle] = dfs_f(E, A, layer, CE, u, v);
        disp(sprintf('\t��������:\n\t\t�������֧����:%s\n\t\t���ߴ��ַ�֧:%s', mat2str(outSG), mat2str(outTentacle)));
        % ��ʾԴ�㲻�ܹ���������
        if isempty(outSG)
            return;
        end

        % �г��ߡ����֡���֧����ʾ����һ��������������
        % �������ܻῼ��һ�֡�������򻯡�(!!!)
        if ~isempty(outTentacle)
            return;
        end

        [inSG, inTentacle] = dfs_b(E, A, layer, CE, v, u);
        disp(sprintf('\t��������:\n\t\t�������֧����:%s\n\t\t��ߴ��ַ�֧:%s', mat2str(inSG), mat2str(inTentacle)));
        if isempty(inSG)
            return;
        end

        % ����ߡ����֡���֧����ʾ����һ��������������
        % �������ܻῼ��һ�֡�������򻯡�(!!!)
        if ~isempty(inTentacle)
            return;
        end
    end
   
    function outNetworkList = mergeNetworkList(E, networkList, u, v, e, type, sg)
        % ��������ϲ�������������
        % ��Ҫ����������͸���������ӹ�ϵ
        count = size(networkList, 1) + 1;
        networkList{count, 1} = [u v]; % �������Դ��
        networkList{count, 2} = type;     % �����������
        networkList{count, 3} = 0;     % ������ĸ�����
        networkList{count, 4} = e;     % �������Ӧ�������֧
        networkList{count, 5} = sg;    % ������ķ�֧����

        % ����������б����������Ƿ����������
        for ee = sg
            if E(ee,1) == -2   % -2��ʾ�÷�֧�������Դ���֧
                for i=1:count-1
                    if networkList{i, 4} == ee
                        networkList{i, 3} = count;
                    end
                end
            end
        end

        outNetworkList = networkList;
    end

    function type = getNetworkType(E, sg)
        % ȷ�������������
        sv = []; % ������Ľڵ㼯��
        for e = sg
            sv = union(sv, E(e, 2:3));
        end

        cn = length(sg); % ������ķ�֧��
        cm = length(sv); % ������Ľڵ���

        type = 0; % ��ȷ����������
        if cn == cm-1    % ����
            type = 1;
        elseif cn == cm  % ����
            type = 2;
        else             % ����
            type = 3;
        end
    end

end