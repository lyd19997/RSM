figure;

X = [1,2,3,4,5,6];
Y = [458.3333333
427.5
390.5833333
353.25
353.25
353.25
];
U = [31.66666667
40.5
63.41666667
15.75
15.75
15.75
];

L = [51.83333333
72
36.58333333
20.25
20.25
20.25
];

errorbar(X,Y,L,U,'.k');
hold on;
bar(Y);
xlabel('\psi','Fontsize', 22);
ylabel('Bandwidth Cost (Units)', 'Fontsize', 22);

% set(gca,'XLim',[0 24]);%X轴的数据显示范围
set(gca,'YLim',[400 550]);%Y轴的数据显示范围

%set(gca,'xtick', [1:1:6]);
%set(gca,'ytick', [0:100:500]);
%set(gca,'ytick',[400 500 600]);

ylim([400 550]);
xlim([0 7]);
set(gca,'ytick',[400,450,500,550]);
set(gca,'xtick',[1,2,3,4,5,6]);
set(gca,'yticklabel',{'0','450','500','550'});
set(gca,'xticklabel',{'0','2','4','6','8','10'});

set(gca,'Fontsize',22);