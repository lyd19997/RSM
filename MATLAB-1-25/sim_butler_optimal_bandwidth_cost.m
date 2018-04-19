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
Y = zeros(TimeSlotNum, SchemeNum);
X = zeros(TimeSlotNum, 1);

for i = 1 : 1 : TimeSlotNum
   X(i) = i; 
end

for i = 1 : 1 : 3
    prefix = ['Topo_node_',num2str(node),'_lambda_',num2str(lambda),'_Peroid_',num2str(Peroid),'_Duration_',num2str(Duration),'_Deep_',num2str(Deep(i)),'_Span_',num2str(Span(i)),'_',num2str(LP(i)),'_'];
    file = [root,prefix,'LinkCost.txt'];
    price = load(file);
    
    file = [root,prefix,'LinkTime_Charge.txt'];
    LinkTime_Charge = load(file);
    
    for j = 1 : 1 : TimeSlotNum
        tmp = 0;
        for k = 1 : 1 : linkNum
            tmp = tmp + price(k,1) * LinkTime_Charge(k,j);
        end
        Y(j,i) = tmp;
    end
    
%     link_Volume = linkTime_Volume*ones(size(linkTime_Volume,2),1);
%     file = [root,prefix,'LinkChargeVolume.txt'];
%     link_Peak = load(file);
%     link_Peak = link_Peak .* (UNITE*Peroid);
%     link_Utilization = link_Volume ./ link_Peak;%每条边的链路利用率
%     sum_Utilization = ones(1,size(link_Utilization,1)) * link_Utilization;%全图的利用率
%     Y=[Y;sum_Utilization];
%    bar(Y);
end

lineStyle = ['+','*','s'];
lineWidth = [2,2,2];
color = ['r','g','b',]

for i = 1:1:3
    plot(X, Y(:,i), lineStyle(i), 'MarkerEdgeColor', color(i),  'MarkerSize',8, 'LineWidth',2);
    hold on;
end

h = legend('PDA','Optimal', 'Optimal (Round)');
set(h,'Fontsize',22);

xlabel('Timeslot','Fontsize', 22);
ylabel('Bandwidth Cost (Units)', 'Fontsize', 22);

set(gca,'XLim',[0 24]);%X轴的数据显示范围
set(gca,'YLim',[0 500]);%Y轴的数据显示范围

set(gca,'xtick', [0:4:24]);
set(gca,'ytick', [0:100:500]);

set(gca,'Fontsize',22);
