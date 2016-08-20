filterdeep = 120;

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%1000采样率
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
fp = fopen('D:\filter_h_1000.txt','wt');
format long g
fprintf(fp, '%05d', filterdeep);
for i =1 : 125/2-1
    fprintf(fp, 'Frq=%05dHz@', i);
    fprintf(fp, '%d$', fir1(filterdeep,i/125*2,'high'));
    fprintf(fp, '!\r');
end
fclose(fp);

fp = fopen('D:\filter_l_1000.txt','wt');
fprintf(fp, '%05d', filterdeep);
format long g
for i =1 : 125/2-1
    fprintf(fp, 'Frq=%05dHz@', i);
    fprintf(fp, '%d$', fir1(filterdeep,i/125*2));
    fprintf(fp, '!\r');
end
fclose(fp);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%10000采样率
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
fp = fopen('D:\filter_h_10000.txt','wt');
fprintf(fp, '%05d', filterdeep);
format long g
for i =1 : 1250/2-1
    fprintf(fp, 'Frq=%05dHz@', i);
    fprintf(fp, '%d$', fir1(filterdeep,i/1250*2,'high'));
    fprintf(fp, '!\r');
end
fclose(fp);

fp = fopen('D:\filter_l_10000.txt','wt');
fprintf(fp, '%05d', filterdeep);
format long g
for i =1 : 1250/2-1
    fprintf(fp, 'Frq=%05dHz@', i);
    fprintf(fp, '%d$', fir1(filterdeep,i/1250*2));
    fprintf(fp, '!\r');
end
fclose(fp);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%20000采样率
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
fp = fopen('D:\filter_h_20000.txt','wt');
fprintf(fp, '%05d', filterdeep);
format long g
for i =1 : 2500/2-1
    fprintf(fp, 'Frq=%05dHz@', i);
    fprintf(fp, '%d$', fir1(filterdeep,i/2500*2,'high'));
    fprintf(fp, '!\r');
end
fclose(fp);

fp = fopen('D:\filter_l_20000.txt','wt');
fprintf(fp, '%05d', filterdeep);
format long g
for i =1 : 2500/2-1
    fprintf(fp, 'Frq=%05dHz@', i);
    fprintf(fp, '%d$', fir1(filterdeep,i/2500*2));
    fprintf(fp, '!\r');
end
fclose(fp);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%25000采样率
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
fp = fopen('D:\filter_h_25000.txt','wt');
fprintf(fp, '%05d', filterdeep);
format long g
for i =1 : 3125/2-1
    fprintf(fp, 'Frq=%05dHz@', i);
    fprintf(fp, '%d$', fir1(filterdeep,i/3125*2,'high'));
    fprintf(fp, '!\r');
end
fclose(fp);

fp = fopen('D:\filter_l_25000.txt','wt');
fprintf(fp, '%05d', filterdeep);
format long g
for i =1 : 3125/2-1
    fprintf(fp, 'Frq=%05dHz@', i);
    fprintf(fp, '%d$', fir1(filterdeep,i/3125*2));
    fprintf(fp, '!\r');
end
fclose(fp);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%40000采样率
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
fp = fopen('D:\filter_h_40000.txt','wt');
fprintf(fp, '%05d', filterdeep);
format long g
for i =1 : 5000/2-1
    fprintf(fp, 'Frq=%05dHz@', i);
    fprintf(fp, '%d$', fir1(filterdeep,i/5000*2,'high'));
    fprintf(fp, '!\r');
end
fclose(fp);

fp = fopen('D:\filter_l_40000.txt','wt');
fprintf(fp, '%05d', filterdeep);
format long g
for i =1 : 5000/2-1
    fprintf(fp, 'Frq=%05dHz@', i);
    fprintf(fp, '%d$', fir1(filterdeep,i/5000*2));
    fprintf(fp, '!\r');
end
fclose(fp);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%50000采样率
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
fp = fopen('D:\filter_h_50000.txt','wt');
fprintf(fp, '%05d', filterdeep);
format long g
for i =1 : 6250/2-1
    fprintf(fp, 'Frq=%05dHz@', i);
    fprintf(fp, '%d$', fir1(filterdeep,i/6250*2,'high'));
    fprintf(fp, '!\r');
end
fclose(fp);

fp = fopen('D:\filter_l_50000.txt','wt');
fprintf(fp, '%05d', filterdeep);
format long g
for i =1 : 6250/2-1
    fprintf(fp, 'Frq=%05dHz@', i);
    fprintf(fp, '%d$', fir1(filterdeep,i/6250*2));
    fprintf(fp, '!\r');
end
fclose(fp);


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%100000采样率
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
fp = fopen('D:\filter_h_100000.txt','wt');
fprintf(fp, '%05d', filterdeep);
format long g
for i =1 : 12500/2-1
    fprintf(fp, 'Frq=%05dHz@', i);
    fprintf(fp, '%d$', fir1(filterdeep,i/12500*2,'high'));
    fprintf(fp, '!\r');
end
fclose(fp);

fp = fopen('D:\filter_l_100000.txt','wt');
fprintf(fp, '%05d', filterdeep);
format long g
for i =1 : 12500/2-1
    fprintf(fp, 'Frq=%05dHz@', i);
    fprintf(fp, '%d$', fir1(filterdeep,i/12500*2));
    fprintf(fp, '!\r');
end
fclose(fp);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%200000采样率
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
fp = fopen('D:\filter_h_200000.txt','wt');
fprintf(fp, '%05d', filterdeep);
format long g
for i =1 : 25000/2-1
    fprintf(fp, 'Frq=%05dHz@', i);
    fprintf(fp, '%d$', fir1(filterdeep,i/25000*2,'high'));
    fprintf(fp, '!\r');
end
fclose(fp);

fp = fopen('D:\filter_l_200000.txt','wt');
fprintf(fp, '%05d', filterdeep);
format long g
for i =1 : 25000/2-1
    fprintf(fp, 'Frq=%05dHz@', i);
    fprintf(fp, '%d$', fir1(filterdeep,i/25000*2));
    fprintf(fp, '!\r');
end
fclose(fp);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%250000采样率
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
fp = fopen('D:\filter_h_250000.txt','wt');
fprintf(fp, '%05d', filterdeep);
format long g
for i =1 : 31250/2-1
    fprintf(fp, 'Frq=%05dHz@', i);
    fprintf(fp, '%d$', fir1(filterdeep,i/31250*2,'high'));
    fprintf(fp, '!\r');
end
fclose(fp);

fp = fopen('D:\filter_l_250000.txt','wt');
fprintf(fp, '%05d', filterdeep);
format long g
for i =1 : 31250/2-1
    fprintf(fp, 'Frq=%05dHz@', i);
    fprintf(fp, '%d$', fir1(filterdeep,i/31250*2));
    fprintf(fp, '!\r');
end
fclose(fp);

x= 1;