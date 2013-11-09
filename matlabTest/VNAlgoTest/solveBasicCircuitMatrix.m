function C = solveBasicCircuitMatrix(B, T)
% 通过完全关联矩阵B求解基本回路矩阵

% 去掉第1行，构成一个基本关联矩阵
B = B(2:end, 1:end);

% 1、根据树T将矩阵B，分割成BL、BT
BL=[]; % 余枝
BT=[]; % 树枝
color=zeros(size(B,2),1);
for e=T
    color(e)=1; % 标记为树边分支
end

for e=1:size(B,2)
    if color(e)
        BT = [BT B(1:end, e)]; % 余枝矩阵
    else
        BL = [BL B(1:end, e)]; % 树枝矩阵
    end
end

C = [];  % 初始化回路矩阵
if det(BT)~=0
    I_BT=inv(BT); % BT的逆矩阵
    M = -1*BL'*I_BT';
    C = [eye(size(M,1)) M];
end

% 此时，C的列按照余枝BL和树枝BT的顺序排列，
% 如果要求回路，则还需要进一步的处理
end