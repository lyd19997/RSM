label = {'PDA','IP Solver'};
LP = [0.467,1.156,5.469,29]';
ILP = [0.87,6.288,37.594,1200]';
Y = [LP,ILP];%semilogy(Y);
%C = categorical({''});
bar(Y);
set(gca,'xticklabel',10:10:40)
set(gca,'yscale','log');

%grid on;

legend(label,'fontsize',24);
xlabel('Node Number','fontsize',24);
ylabel('Time (s)','fontsize',24);
title('');
set(gca,'fontsize',20);