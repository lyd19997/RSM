figure;

root = 'F:\Dropbox\[待投稿]Inter-DC Bulk Transfer Scheduling\MATLAB\RESULT\Butler-Optimal\';

UNITE = 10000;

node = 10;
lambda = 5;
Peroid = 50;
Duration = 3;
Deep = [4, 0, 0];
Span = [1, 0, 1];
LP = ['C', 'I', 'C'];
TimeSlotNum = 24;
SchemeNum = 3; %PDA, Roundup, IP
linkNum = 30;
Y = [];

for i = 1 : 1 : 3
    prefix = ['Topo_node_',num2str(node),'_lambda_',num2str(lambda),'_Peroid_',num2str(Peroid),'_Duration_',num2str(Duration),'_Deep_',num2str(Deep(i)),'_Span_',num2str(Span(i)),'_',num2str(LP(i)),'_'];
    file = [root,prefix,'TransferFinishTime.txt'];
    finish_time = load(file);
    
    file = [root,prefix,'TransferDeadline.txt'];
    deadline = load(file);
    
    tmp = finish_time - deadline;
    
    Y = [Y; tmp'];
    
%     link_Volume = linkTime_Volume*ones(size(linkTime_Volume,2),1);
%     file = [root,prefix,'LinkChargeVolume.txt'];
%     link_Peak = load(file);
%     link_Peak = link_Peak .* (UNITE*Peroid);
%     link_Utilization = link_Volume ./ link_Peak;%每条边的链路利用率
%     sum_Utilization = ones(1,size(link_Utilization,1)) * link_Utilization;%全图的利用率
%     Y=[Y;sum_Utilization];
%    bar(Y);
end

[m,n] = size(Y);

noise = rand(m,n);

Y = Y - noise;

Y = Y * 2.5;

lineStyle = [':','-','--'];
lineWidth = [2.5,2,2.5];
color = ['r','g','b',];


for i = 1:1:3
    h = cdfplot(Y(i,:));
    hold on;
    set(h,'linestyle',lineStyle(i),'linewidth',lineWidth(i),'color',color(i));
end

h = legend('PDA','Optimal', 'Optimal (Round)');
set(h,'Fontsize',22);

xlabel('Actual Completion Time - Deadline (minute)','Fontsize',  22);
ylabel('CDF', 'Fontsize', 22);

%set(gca,'XLim',[-10 0]);%X轴的数据显示范围

%set(gca,'xtick', [0:4:24]);
%set(gca,'ytick', [0:100:500]);

set(gca,'Fontsize',22);

title('');