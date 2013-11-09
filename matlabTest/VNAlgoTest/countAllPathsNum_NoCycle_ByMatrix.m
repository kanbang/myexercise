function pathNum = countAllPathsNum_NoCycle_ByMatrix(A, from, to)
% 计算无单向回路的网络的通路总数，采用矩阵算法
% 参考文献：[1] 《流体网络》第2章--2.5.1小节
%          [2] 《通风网络中通路总数确定方法的改进》--贾进章，郑丹，刘剑
% 这里采用文献2的行向量法，不使用文献1的矩阵方法
% 输入参数：
%   A -- 节点邻接矩阵
%   from, to -- 源汇节点
% 输出参数：
%   pathNum -- 通路总数

a = A(from,1:end);
m = size(A,1);
pathNum = 0;
for i=1:m-1    
    pathNum = pathNum + a(to);
    a = a*A;
end

end