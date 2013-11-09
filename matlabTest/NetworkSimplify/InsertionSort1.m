function Answer = InsertionSort1(x, data)
% ²åÈëÅÅĞò -- 1Î¬Êı×é

ArrayOfRandomNumbers=x;
Sequence=ArrayOfRandomNumbers;
Sequence1=Sequence;

len = size(Sequence,1);
for counter=1:1:len-1
    for counter1=counter:-1:1
        % if (Sequence(counter1+1)<Sequence(counter1))
        if (data(Sequence(counter1+1)) < data(Sequence(counter1)))
            Sequence1(counter1)=Sequence(counter1+1);
            Sequence1(counter1+1)=Sequence(counter1);
            Sequence=Sequence1;
        end
    end
end

Answer=Sequence1;

end