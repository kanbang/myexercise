function val = randInt(min, max)
% 随机生成[min,max]之间的整数
val = min + round(rand(1)*(max-min));
end
