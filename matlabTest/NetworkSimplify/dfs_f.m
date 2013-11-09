function [sg, tentacle] = dfs_f(E, A, layer, CE, u, v)
% �����������(��������-���ҳ���)

n = size(E, 1);
color = zeros(1, n);
color(CE) = 1; % �������������������е����з�֧��ɫ
[sg, tentacle] = dfs1(E, A, layer, u, v, [], [], [], color, false);
tentacle = removeErrorTentacleEdges1(E, A, sg, tentacle, u);

    function [outSG, outTentacle, outTempTentacle, outColor, outGoBack] = dfs1(E, A, layer, u, v, sg, tentacle, tempTentacle, color, goBack)
        % �����������(��������-���ҳ���)
        if u == v
            % ��ʾ�ѵ�����
            outSG = sg;
            outTentacle = tentacle;
            outTempTentacle = tempTentacle;
            outColor = color;
            outGoBack = goBack;
            return;
        end

        if layer(u) >= layer(v)
            outSG = sg;
            outTentacle = tentacle;
            outTempTentacle = tempTentacle;
            outColor = color;
            outGoBack = true;  % �Ѿ��������·������!
            return;
        end

        % ��������
        for e=A{u,1}
            if color(e)
                continue;
            end

            % �����ҵ���·
            goBack = false;
            color(e) = 1;

            if ~isempty(tempTentacle)
                tentacle = [tentacle tempTentacle(end)];
                tempTentacle = [];
            end

            sg = [sg e]; % ��ӵ�����ķ�֧������
            [outSG, outTentacle, outTempTentacle, outColor, outGoBack] = dfs1(E, A, layer, E(e,3), v, sg, tentacle, tempTentacle, color, goBack);
            sg = outSG;
            tentacle = outTentacle;
            tempTentacle = outTempTentacle;
            color = outColor;
            goBack = outGoBack;

            if goBack & sg(end) == e % ��ʾ��֧û�гɹ�����ǰ��
                % ��Ҫ���ˣ�����֧����
                tempTentacle = [tempTentacle e]; % �����"����"��֧
                sg = sg(1:end-1);
            end
        end
        
        if ~isempty(tempTentacle)
            tentacle = [tentacle tempTentacle(end)];
            tempTentacle = [];
        end

        outSG = sg;
        outTentacle = tentacle;
        outTempTentacle = tempTentacle;
        outColor = color;
        outGoBack = goBack;
    end

    function outTentacle = removeErrorTentacleEdges1(E, A, sg, tentacle, from)
        % ɾ������ĳ���"����"��֧
        % ��2�ࣺ1) ʼ�ڵ㲻���������ϵķ�֧
        %       2) ��Դ����Ϊʼ�ڵ�ĳ���"����"��֧
        color = zeros(1, size(A, 1));
        for e = sg
            color(E(e, 2)) = 1;
            color(E(e, 3)) = 1;
        end

        outTentacle = [];
        for e = tentacle
            u = E(e, 2);
            if  color(u) & u ~= from   % ��֧��ʼ�ڵ��Ƿ�����������
                outTentacle = [outTentacle e];
            end
        end
    end

end