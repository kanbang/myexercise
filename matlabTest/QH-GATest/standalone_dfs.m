function [P, lastPath, lastNode, lastE] = standalone_dfs(A, E, start, target)
% ����ͨ·��
% �����������¼���ǽڵ�ĳ���
%   A         -- �ڽӱ��û��豣֤�����ǵ�Դ����ģ������ڽӱ������ű��봮�ı仯���仯��
%   V         -- ͼ�Ľڵ㼯��
%   E         -- ͼ�ķ�֧����
%   start     -- �����Դ��
%   target    -- ����Ļ��
% ���������
%   P -- ����ͨ·����(ʹ�÷�֧��ʾͨ·)

[P, lastPath, lastNode, lastE] = dfs_visit(A, E, start, target); % �������ͨ·����

    function [P, lastPath, lastNode, lastE] = dfs_visit(A, E, u, v)
        lastPath = [];
        lastNode = [];
        lastE = [];
        path=[];
        P={};
        theta = 10^-3; % ����ƫ��

        w = u;
        we=[];
        % ��ɫ��֧(��ǰ���ڴ���ķ�֧��Ҳ����path�еķ�֧)
        color = zeros(1, size(E, 1));

        while true
            % Ѱ�ҽڵ�u����������0�����������ٵķ�֧
            outEdges = findOutEdges(A, E, color, w, theta);
            if isempty(outEdges)
                if isempty(path)
                    lastPath = path;
                    lastNode = w;
                    lastE = E;
                    return;
                else
                    %                     disp(sprintf('����1...\n'));
                    w = E(path(end), 1); % ����
                    path = path(1:end-1);
                    continue;
                end
            end

            for e=outEdges
%             e = outEdges;
            color(e) = 1;
            path = [path e];       % ����֧��ӵ�·����
            w = E(e, 2);            % ��ѯ��֧��ĩ�ڵ�

            if w == v
                pathNum = size(P,1)+1;
                disp(sprintf('�ҵ�һ��ͨ·:%s\n', mat2str(path)));
                P{pathNum, 1} = path; % ��¼ͨ·

                [minQ we]= caclPathFlowWidth(path, E, theta); % ����ͨ·�е���С�������
                P{pathNum, 2} = minQ;                     % ��¼ͨ·����С�������
                P{pathNum, 3} = path(we);                 % ��¼��ȷ�֧
                E = removeFlow(E, path, minQ);            % ͨ·��ȥ����minQ
                w = u;
                path = [];
                color = zeros(1, size(E, 1));
            end
            break;
        end
        end

    end

    function outEdges = findOutEdges(A, E, color, v, theta)
        % �������ܣ�
        % ���ݵ�ǰ�����˽ṹ�����ҵ�ǰ�ڵ�v�ĳ��߷�֧
        % 1���ų���0������֧
        % ���������
        %   A -- �ڵ��ڽӾ���
        %   v -- Ҫ���ҳ��ߵĽڵ�
        % ��������
        %   outEdges -- �ڵ�v�ĳ��߷�֧

                outEdges = A{v}; % ��ȡ����A�ĵ�v�У��ȼ��ڻ�ȡ�ڵ�v�ĳ�����Ϣ��

        % �ų���ɫ��֧(��ǰ���ڴ���ķ�֧��Ҳ����path�еķ�֧)
                outEdges = outEdges(find(color(outEdges)==0));

        % �ų�0������֧
                outEdges = outEdges(find(E(outEdges, 3)>theta));

%         outEdges = [];
%         for e=A{v}
%             if color(e) == 1 | E(e,3) <= theta
%                 continue;
%             end
% 
%             if isempty(outEdges)
%                 outEdges = e;
%             elseif E(outEdges, 3) > E(e,3)
%                 outEdges = e;
%             end
%         end
    end

    function [width ei] = caclPathFlowWidth(path, E, theta)
        % ����ͨ·����С�������
        % theta -- ���ɸ���ʵ������趨������0.1��
        q = [];
        for e = path
            q = [q E(e,3)]; % ��ȡ��֧������
        end
        q = q(find(q>theta));
        [width ei] = min(q); % ����С�������Ѿ����ڵ�λ��index
    end

    function outE = removeFlow(E, path, q)
        % ͨ·path�ķ�֧��ȥ����q
        for e = path
            E(e,3) = E(e,3) - q;
        end
        outE = E;
    end

end
