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
    file = [root,prefix,'Cost.txt'];
    cost = load(file);
    
    X = [X, i];
    Y = [Y;cost];

end
bar(X,Y);
hold on;

xlabel('SPAN','Fontsize', 22);
ylabel('Bandwidth Cost (Units)', 'Fontsize', 22);

% set(gca,'XLim',[0 24]);%X轴的数据显示范围
set(gca,'YLim',[400 550]);%Y轴的数据显示范围

%set(gca,'xtick', [1:1:6]);
%set(gca,'ytick', [0:100:500]);
%set(gca,'ytick',[400 500 600]);

ylim([400 550]);
xlim([0 5]);
set(gca,'ytick',[400,450,500,550]);
%set(gca,'xtick',[1,2,3,4]);
set(gca,'yticklabel',{'0','450','500','550'});
%set(gca,'xticklabel',{'0','2','4','6','8','10'});

set(gca,'Fontsize',22);

