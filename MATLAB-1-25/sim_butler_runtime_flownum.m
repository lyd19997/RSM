label = {'PDA','IP Solver'};
LP = [5.469,11.975,11.517,14.396]';
ILP = [37.594,107.8,172.8,185]';
Y = [LP,ILP];%semilogy(Y);
%C = categorical({''});
bar(Y);
set(gca,'xticklabel',10:10:40)
set(gca,'yscale','log');

%grid on;

legend(label,'fontsize',24);
xlabel('Flow Arrival Rate','fontsize',24);
ylabel('Time (s)','fontsize',24);
title('');
set(gca,'fontsize',20);