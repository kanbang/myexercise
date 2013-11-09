function P = findAllPaths_NoCycle_ByMatrix(A, E, from, to)
% �����޵����·�����ȫ��ͨ·�����þ�����㷽��
% ֻ�������޵����·�����磬���������е����·������
% �ο����ף����������硷��2��--2.5С�ڣ�24-28
% ���������
%   A -- �ڵ��ڽӾ���
% ���������
%   P -- ͨ·����
% celldisp(P)   

SE = {}; % ��֧��������
for e = E
    SE{end+1}=sprintf('e%d',e);
end

m = size(A, 1); % �ڵ���
U = sym(zeros(m, m));
for i=1:m
    for j=1:m
        if A(i,j)==1
            U(i,j) = sprintf('e%d', getEdgeNum(E, i, j));
        elseif i==j
            U(i,j)='1';
        end
    end
end

disp('���ž���:')
disp(U)

% ��Դ��from->���to��ͨ·
% ֻ���ǵ�Դ��������

% ȡ��to�У�from������ʽ����
U_from_to = form(U, to, from);
u = caclMatrixDet(U_from_to);
u = expand(u);
disp(u)

% �����ʽת����ͨ·����
 % ������ת�����ַ���
paths = char(u);

% ����ͨ·
P = splitAllPaths(paths);

 function edgeNum = getEdgeNum(E, u, v)
        % ��ȡ�ڵ�u->v�ķ�֧���
        edgeNum = -1;
        for ee=1:size(E,1)
            if u == E(ee,2) & v == E(ee,3)
                edgeNum = ee;
                break;
            end
        end
    end

    function back_matrix = form(matrix, i, j)
        % ����ž��������ʽ
        back_matrix_hang=0;
        back_matrix=sym([]);
        a=size(matrix);
        for hang=1:a(1)
            if hang~=i
                back_matrix_hang=back_matrix_hang+1;
                back_matrix_lie = 0;
                for lie=1:a(2)
                    if lie~=j
                        back_matrix_lie=back_matrix_lie+1;
                        back_matrix(back_matrix_hang, back_matrix_lie)=matrix(hang,lie);
                    end
                end
            end
        end
    end

    function u = caclMatrixDet(U)
        % չ�����������ʽ������ʽ�������㣨���Ǵ�������ʽ��
        if size(U,1)==2
            u = det(U); % ֱ�Ӽ���
            return;
        end
        
        u = sym('0');
        % ʼ��ѡ���1�н�������ʽ�ֽ�
        for i=1:size(U,1)
            LU = form(U, 1, i); % ����ʽ
%             disp(sprintf('��1��,%d�е�����ʽ:',i));
%             disp(LU)
            u = u + U(1, i)*caclMatrixDet(LU);
        end
    end

    function path = splitPath(strPath)
        % ��e1*e23*e30֮����ַ����н���ͨ·
        path=[];
        rem = strPath;        
        while true
            [tok, rem] = strtok(rem, 'e*');
            if isempty(tok)
                break;
            end
            
            path = [path str2num(tok)];
        end
    end

    function P = splitAllPaths(strPaths)
        % ��e1*e23*e30+e2*e24*e32+...֮����ַ����н���ͨ·����
        P ={};
        rem = strPaths;
        while true
            [tok, rem]=strtok(rem, '+');
            if isempty(tok)
                break;
            end
            
            P{end+1} = splitPath(tok); % ����һ��ͨ·
        end
    end


end