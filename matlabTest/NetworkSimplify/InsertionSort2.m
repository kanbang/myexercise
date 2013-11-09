function Answer = InsertionSort2(x, data)
% 插入排序 -- 2维数组

ArrayOfRandomNumbers=x;
Sequence=ArrayOfRandomNumbers;
Sequence1=Sequence;

len = size(Sequence,1);
for counter=1:1:len-1
    for counter1=counter:-1:1
        s2 = data(Sequence(counter1+1, 1:end));
        s1 = data(Sequence(counter1, 1:end));
        gap2 = s2(2)-s2(1);
        gap1 = s1(2)-s1(1);
        if gap2 < gap1 % 按间距排序
            Sequence1(counter1, 1:end)=Sequence(counter1+1, 1:end);
            Sequence1(counter1+1, 1:end)=Sequence(counter1, 1:end);
            Sequence=Sequence1;
        elseif gap2 == gap1 & sum(s2) < sum(s1)
            Sequence1(counter1, 1:end)=Sequence(counter1+1, 1:end);
            Sequence1(counter1+1, 1:end)=Sequence(counter1, 1:end);
            Sequence=Sequence1;
        end
    end
end

Answer=Sequence1;

end