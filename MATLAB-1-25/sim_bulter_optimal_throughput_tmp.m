sort(Y1,2);

counter1 = 0;
counter2 = 0;

for i = 1:1:247
   if Y1(1,i) >= Y1(2,i)
       counter1 =  counter1 + 1;
   end
   if Y1(1,i) >= Y1(3,i)
       counter2 =  counter2 + 1;
   end
end