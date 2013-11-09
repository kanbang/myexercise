function genDotFile(graphFile)
% ����dot�ļ�����չ��Ϊ.gv)
% ������Ҫ����ѡ���Ƿ���Ҫ��֧���
E = load(graphFile);

n = size(E,1);

fid=fopen('gd.gv', 'w');  % �½��ļ���׼��д��
fprintf(fid, 'digraph G {\n');
fprintf(fid, 'graph[rankdir=BT];\n');

for i=1:n
    % �з�֧���
    fprintf(fid,'\tv%d -> v%d [label="e%d"];\n', E(i,2), E(i,3), E(i,1));
    
    % �޷�֧���
    %fprintf(fid,'\tv%d -> v%d;\n', E(i,2), E(i,3));
end

fprintf(fid, '}');
fclose(fid); % �ر��ļ�

end