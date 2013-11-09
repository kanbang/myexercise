function OutPaths = findAllPaths(A, E, from, to)
% 采用矩阵计算方法查找所有通路

% 1、A=A+I
% 节点邻接矩阵A的对角线元素全部加1
m = size(A,1);
for i = 1:m
    A(i,i) = A(i,i)+1;
end

disp(sprintf('源点:%d\t汇点:%d', from ,to));
disp('节点邻接矩阵')
disp(A)

% 通过矩阵计算方法搜索源点from->汇点to之间的全部通路
% OutPaths -- 是一个元胞数组(cell array)
OutPaths = caclMatrixDet(E, form(A, to, from), [to from]);

% 打印所有的通路
celldisp(OutPaths);

	% 子函数
	% 查找所有的通路
	% 输入参数：
	% 	E -- 分支信息集合
	% 	U -- 要计算的行列式矩阵
	% 	pos -- 当分解到当前矩阵U时，所记录的划分元素的"位置"信息
	% 		例如：一个3*3矩阵M
	% 			a) 首先划分矩阵M中元素(3,1)的余子式M1 -> pos = [3 1];
	% 			b) M1是2*2的矩阵
	%			c) 划分矩阵M1中元素(1,1)的余子式M11 -> pos = [1 1; 3 1];
	%		通过pos信息，可以将当前矩阵中元素的"坐标位置"
	%		还原成原始矩阵的"实际坐标位置"
    function Paths = caclMatrixDet(E, U, pos)
        disp('当前计算的矩阵')
        disp(U); % 当前矩阵

        Paths = {};
        if size(U,1)==2
            Paths = findInTwoMatrix(E, U, pos);
            return;
        end

        % 始终选择第1行进行余子式分界
        for i=1:size(U,1)
            if U(1,i)==1
                [u v] = updateRC(1, i, pos); % 取最后的当前坐标
                disp(sprintf('当前元素位置:(%d, %d)',u ,v)); % 最上层的坐标在最后
                LU = form(U, 1, i); % 提取余子式
                pp = caclMatrixDet(E, LU, [1 i; pos]);

                disp('计算结果')
                celldisp(pp)
                nn = length(pp);

                e = findEdge(E, u, v);
                disp(sprintf('分支编号: %s', mat2str(e)));
                for i=1:nn
                    Paths{end+1} = [e pp{i}];
                end
            end
        end
    end


	% 子函数：查找2*2矩阵的通路
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
        [u v] = updateRC(a, b, pos); % 计算元素在矩阵中的原始位置
        disp(sprintf('u:%d\tv:%d', u, v));
        if u ~= v % 非对角线元素
            path = [path findEdge(E, u, v)];
        end
    end
	
	% 子函数：计算(uu, vv)在原始矩阵中的实际位置(ou, ov)
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

	% 子函数：查找分支编号
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

	% 子函数：提取余子式
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
