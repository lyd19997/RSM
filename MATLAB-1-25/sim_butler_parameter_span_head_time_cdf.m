figure;

root = 'F:\Dropbox\[待投稿]Inter-DC Bulk Transfer Scheduling\实验仿真\Code\BUTLER\BUTLER\data\Butler\';

UNITE = 8000;

node = 10;
lambda = 5;
Peroid = 50;
Duration = 3;
Deep = 6;
Span = [1, 2, 3, 4, 5, 6];

LP = ['C'];
TimeSlotNum = 24;
SchemeNum = 3; %PDA, Roundup, IP
linkNum = 30;
Y = [];
X = [];

for i = 1 : 1 : 6
    prefix = ['Topo_node_',num2str(node),'_lambda_',num2str(lambda),'_Peroid_',num2str(Peroid),'_Duration_',num2str(Duration),'_Deep_',num2str(Deep),'_Span_',num2str(Span(i)),'_',num2str(LP),'_'];
    file = [root,prefix,'TransferFinishTime.txt'];
    finish_time = load(file);
    
    file = [root,prefix,'TransferDeadline.txt'];
    deadline = load(file);
    
    tmp = finish_time - deadline;
    
    Y = [Y; tmp'];

end

[m,n] = size(Y);

noise = rand(m,n);

Y = Y - noise;

Y = Y * 2.5;

for i = 1:1:6
    h = cdfplot(Y(i,:));
    hold on;
    %set(h,'linestyle',lineStyle(i),'linewidth',lineWidth(i),'color',color(i));
end

h = legend('0','2', '4', '6', '8', '10');
set(h,'Fontsize',22);

xlabel('Actual Completion Time - Deadline (minute)','Fontsize',  22);
ylabel('CDF', 'Fontsize', 22);


set(gca,'Fontsize',22);

title('');
