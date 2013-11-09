function OutPaths = findAllPaths(A, E, from, to)
% ���þ�����㷽����������ͨ·

% 1��A=A+I
% �ڵ��ڽӾ���A�ĶԽ���Ԫ��ȫ����1
m = size(A,1);
for i = 1:m
    A(i,i) = A(i,i)+1;
end

disp(sprintf('Դ��:%d\t���:%d', from ,to));
disp('�ڵ��ڽӾ���')
disp(A)

% ͨ��������㷽������Դ��from->���to֮���ȫ��ͨ·
% OutPaths -- ��һ��Ԫ������(cell array)
OutPaths = caclMatrixDet(E, form(A, to, from), [to from]);

% ��ӡ���е�ͨ·
celldisp(OutPaths);

	% �Ӻ���
	% �������е�ͨ·
	% ���������
	% 	E -- ��֧��Ϣ����
	% 	U -- Ҫ���������ʽ����
	% 	pos -- ���ֽ⵽��ǰ����Uʱ������¼�Ļ���Ԫ�ص�"λ��"��Ϣ
	% 		���磺һ��3*3����M
	% 			a) ���Ȼ��־���M��Ԫ��(3,1)������ʽM1 -> pos = [3 1];
	% 			b) M1��2*2�ľ���
	%			c) ���־���M1��Ԫ��(1,1)������ʽM11 -> pos = [1 1; 3 1];
	%		ͨ��pos��Ϣ�����Խ���ǰ������Ԫ�ص�"����λ��"
	%		��ԭ��ԭʼ�����"ʵ������λ��"
    function Paths = caclMatrixDet(E, U, pos)
        disp('��ǰ����ľ���')
        disp(U); % ��ǰ����

        Paths = {};
        if size(U,1)==2
            Paths = findInTwoMatrix(E, U, pos);
            return;
        end

        % ʼ��ѡ���1�н�������ʽ�ֽ�
        for i=1:size(U,1)
            if U(1,i)==1
                [u v] = updateRC(1, i, pos); % ȡ���ĵ�ǰ����
                disp(sprintf('��ǰԪ��λ��:(%d, %d)',u ,v)); % ���ϲ�����������
                LU = form(U, 1, i); % ��ȡ����ʽ
                pp = caclMatrixDet(E, LU, [1 i; pos]);

                disp('������')
                celldisp(pp)
                nn = length(pp);

                e = findEdge(E, u, v);
                disp(sprintf('��֧���: %s', mat2str(e)));
                for i=1:nn
                    Paths{end+1} = [e pp{i}];
                end
            end
        end
    end


	% �Ӻ���������2*2�����ͨ·
    function ChildPaths = findInTwoMatrix(E, U, pos)
        ChildPaths = {};
                
        if U(2,2) ~= 0 & U(1,1)~=0
            res = [];
            res = [res findChildPath(E, 1, 1, pos)];
            res = [res findChildPath(E, 2, 2, pos)];
            ChildPaths{end+1} = res;
        end

        if U(1,2)~=0 & U(2,1)~=0
            res = [];
            res = [res findChildPath(E, 1, 2, pos)];
            res = [res findChildPath(E, 2, 1, pos)];
            ChildPaths{end+1} = res;
        end        
    end

    function path = findChildPath(E, a, b, pos)
         path = [];
        [u v] = updateRC(a, b, pos); % ����Ԫ���ھ����е�ԭʼλ��
        disp(sprintf('u:%d\tv:%d', u, v));
        if u ~= v % �ǶԽ���Ԫ��
            path = [path findEdge(E, u, v)];
        end
    end
	
	% �Ӻ���������(uu, vv)��ԭʼ�����е�ʵ��λ��(ou, ov)
    function [ou ov] = updateRC(uu, vv, pos)
        ou = uu;
        ov = vv;

        for ii=1:size(pos,1)
            if ou >= pos(ii,1)
                ou = ou+1;
            end

            if ov >= pos(ii,2)
                ov = ov+1;
            end
        end
    end

	% �Ӻ��������ҷ�֧���
    function e = findEdge(E, i, j)
        e= [];
        if i == j
            return;
        end

        n = size(E,1);
        for x=1:n
            if E(x,2) == i & E(x, 3) == j
                e = x;
                break;
            end
        end
    end

	% �Ӻ�������ȡ����ʽ
    function back_matrix = form(matrix, i, j)
        back_matrix_hang=0;
        back_matrix=[];
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
end
