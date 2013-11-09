function P = dfs_visit(A, E, u, v, color)

    function w = caclPathFlowWidth(path, E, theta)
        % ����ͨ·����С�������
        % theta -- ���ɸ���ʵ������趨������0.1��
        q = [];
        for e = path
            q = [q E(e,3)]; % ��ȡ��֧������
        end
%         q = q(find(q>theta));
        w = min(q); % ����С������
    end

    function outE = removeFlow(E, path, q)
        % ͨ·path�ķ�֧��ȥ����q
        for e = path
            E(e,3) = E(e,3) - q;
        end
        outE = E;
    end

path=[];
P={};
color= zeros(1, size(E, 1));  % �ڵ���ɫ����(��ʼ��Ϊ0��Ĭ�Ͻڵ㶼δ��ɫ)
theta = 10^-3; % ����ƫ��

while true
    outEdges = findOutEdges(A, E, color, u, 0); % Ѱ�ҽڵ�u��δ��ɫ���߷�֧
    if isempty(outEdges)
        if isempty(path)
            return;
        else
            outEdges = findOutEdges(A, E, color, u, 1); % Ѱ�ҽڵ�u����ɫ��֧����
            if isempty(outEdges)               
                continue;
            end
            
%             color(outEdges(1))=0; % ��ѡһ����֧��ɫ������ѡ������˳��ĵ�һ����ɫ��֧��
            u = E(path(end), 1);  % ͨ·�����һ����֧��ʼ�ڵ���ΪѰ��ʼ�ڵ�
            path = path(1:end-1);
        end
    else
        for e = outEdges
            if color(e)
                continue;
            end

            color(e) = 1;
            path = [path, e];       % ����֧��ӵ�·����
            u = E(e, 2);            % ��ѯ��֧��ĩ�ڵ�

            if u == v
                minQ = caclPathFlowWidth(path, E, theta);
                if minQ > theta % ��������������0�ķ�֧
                    pathNum = size(P,1)+1;
                    P{pathNum, 1} = path;

                    % �޸ģ�����������������������
                    % ����ͨ·����С�������

                    P{pathNum, 2} = minQ;        % ��¼ͨ·����С�������
                    E = removeFlow(E, path, minQ);   % ͨ·��ȥ����minQ
                end
                
                u = E(path(end), 1);             % ͨ·�����һ����֧��ʼ�ڵ���ΪѰ��ʼ�ڵ�
                path = path(1:end-1);
            end
            break;
        end
    end
end
end