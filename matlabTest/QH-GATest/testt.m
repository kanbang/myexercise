function testt()

[filename, pathname] = uigetfile('*.txt','��ѡ��һ����������txt�ļ�');
if isequal(filename,0)
   return;
end

E = load(fullfile(pathname, filename)); % ��������

disp(E);

end