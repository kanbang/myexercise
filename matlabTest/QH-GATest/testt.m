function testt()

[filename, pathname] = uigetfile('*.txt','请选择一个拓扑数据txt文件');
if isequal(filename,0)
   return;
end

E = load(fullfile(pathname, filename)); % 加载网络

disp(E);

end