function pathNum = countAllPathsNum_NoCycle_ByMatrix(A, from, to)
% �����޵����·�������ͨ·���������þ����㷨
% �ο����ף�[1] ���������硷��2��--2.5.1С��
%          [2] ��ͨ��������ͨ·����ȷ�������ĸĽ���--�ֽ��£�֣��������
% �����������2��������������ʹ������1�ľ��󷽷�
% ���������
%   A -- �ڵ��ڽӾ���
%   from, to -- Դ��ڵ�
% ���������
%   pathNum -- ͨ·����

a = A(from,1:end);
m = size(A,1);
pathNum = 0;
for i=1:m-1    
    pathNum = pathNum + a(to);
    a = a*A;
end

end