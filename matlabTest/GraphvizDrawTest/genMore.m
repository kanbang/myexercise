function genMore(dotExe, graphDir, graphFormat, pngDir)
% ���������ȡĳ��Ŀ¼�µ����������ļ���Ȼ��ʹ��graphviz�������pngͼ��
% �������:
%   graphDir -- ����������ݵ�Ŀ¼
%   pngDir    -- pngͼ�εı���Ŀ¼

txtFiles = dir(fullfile(graphDir,'*.txt'));
n = length(txtFiles);

for i = 1:n
    graphTopofile = fullfile(graphDir, txtFiles(i).name);
   genDotFile(graphTopofile); % ����gd.gv�ļ�
    
    % �ָ��ļ�·��
    [pathstr,name,ext,versn] = fileparts(graphTopofile);  
    pngFile = fullfile(pngDir, [name '.' graphFormat]);
    
    cmd2 = ['!' dotExe ' -T' graphFormat ' gd.gv -o ' pngFile];        
    eval(cmd2);
end


% ɾ��gd.gv�ļ�
delete('gd.gv');

end