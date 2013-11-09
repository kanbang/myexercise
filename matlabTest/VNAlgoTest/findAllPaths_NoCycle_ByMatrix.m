function P = findAllPaths_NoCycle_ByMatrix(A, E, from, to)
% 计算无单向回路网络的全部通路，采用矩阵计算方法
% 只适用于无单向回路的网络，不能用于有单向回路的网络
% 参考文献：《流体网络》第2章--2.5小节：24-28
% 输入参数：
%   A -- 节点邻接矩阵
% 输出参数：
%   P -- 通路集合
% celldisp(P)   

SE = {}; % 分支符号数组
for e = E
    SE{end+1}=sprintf('e%d',e);
end

m = size(A, 1); % 节点数
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

disp('符号矩阵:')
disp(U)

% 求源点from->汇点to的通路
% 只考虑单源单汇网络

% 取第to行，from列余子式矩阵
U_from_to = form(U, to, from);
u = caclMatrixDet(U_from_to);
u = expand(u);
disp(u)

% 将表达式转换成通路集合
 % 将符号转换成字符串
paths = char(u);

% 解析通路
P = splitAllPaths(paths);

 function edgeNum = getEdgeNum(E, u, v)
        % 获取节点u->v的分支编号
        edgeNum = -1;
        for ee=1:size(E,1)
            if u == E(ee,2) & v == E(ee,3)
                edgeNum = ee;
                break;
            end
        end
    end

    function back_matrix = form(matrix, i, j)
        % 求符号矩阵的余子式
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
        % 展开矩阵的行列式中余子式，并计算（不是代数余子式）
        if size(U,1)==2
            u = det(U); % 直接计算
            return;
        end
        
        u = sym('0');
        % 始终选择第1行进行余子式分界
        for i=1:size(U,1)
            LU = form(U, 1, i); % 余子式
%             disp(sprintf('第1行,%d列的余子式:',i));
%             disp(LU)
            u = u + U(1, i)*caclMatrixDet(LU);
        end
    end

    function path = splitPath(strPath)
        % 从e1*e23*e30之类的字符串中解析通路
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
        % 从e1*e23*e30+e2*e24*e32+...之类的字符串中解析通路集合
        P ={};
        rem = strPaths;
        while true
            [tok, rem]=strtok(rem, '+');
            if isempty(tok)
                break;
            end
            
            P{end+1} = splitPath(tok); % 解析一条通路
        end
    end


end