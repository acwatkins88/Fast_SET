function externalfig(X1, Y1, Y2, Y3, Y4)
%CREATEFIGURE(X1,Y1,Y2,Y3,Y4)
%  X1:  vector of x data
%  Y1:  vector of y data
%  Y2:  vector of y data
%  Y3:  vector of y data
%  Y4:  vector of y data

%  Auto-generated by MATLAB on 28-Apr-2016 21:50:48

% Create figure
figure1 = figure('InvertHardcopy','off','Color',[1 1 1], 'Position', [200 200 656 660]);

% Create subplot
subplot1 = subplot(4,1,1,'Parent',figure1,'FontWeight','bold',...
    'FontName','times');
%% Uncomment the following line to preserve the X-limits of the axes
xlim(subplot1,[0 3e-09]);
%% Uncomment the following line to preserve the Y-limits of the axes
ylim(subplot1,[-0.5 2]);
%% Uncomment the following line to preserve the Z-limits of the axes
zlim(subplot1,[-1 1]);
box(subplot1,'on');
grid(subplot1,'on');
hold(subplot1,'all');

% Create plot
plot(X1,Y1,'Parent',subplot1,'LineWidth',4);

% Create title
title('CLK','FontWeight','bold','FontSize',14,'FontName','times');

% Create ylabel
ylabel('Voltage','FontWeight','bold','FontSize',14,'FontName','times');

% Create subplot
subplot2 = subplot(4,1,2,'Parent',figure1,'FontWeight','bold',...
    'FontName','times');
%% Uncomment the following line to preserve the X-limits of the axes
xlim(subplot2,[0 3e-09]);
%% Uncomment the following line to preserve the Y-limits of the axes
ylim(subplot2,[-0.5 2]);
%% Uncomment the following line to preserve the Z-limits of the axes
zlim(subplot2,[-1 1]);
box(subplot2,'on');
grid(subplot2,'on');
hold(subplot2,'all');

% Create plot
plot(X1,Y2,'Parent',subplot2,'LineWidth',4);

% Create title
title('D','FontWeight','bold','FontSize',14,'FontName','times');

% Create ylabel
ylabel('Voltage','FontWeight','bold','FontSize',14,'FontName','times');

% Create subplot
subplot3 = subplot(4,1,3,'Parent',figure1,'FontWeight','bold',...
    'FontName','times');
%% Uncomment the following line to preserve the X-limits of the axes
xlim(subplot3,[0 3e-09]);
%% Uncomment the following line to preserve the Y-limits of the axes
ylim(subplot3,[-1.5 1.5]);
%% Uncomment the following line to preserve the Z-limits of the axes
zlim(subplot3,[-1 1]);
box(subplot3,'on');
grid(subplot3,'on');
hold(subplot3,'all');

% Create plot
plot(X1,Y3,'Parent',subplot3,'LineWidth',4);

% Create title
title('N5','FontWeight','bold','FontSize',14,'FontName','times');

% Create ylabel
ylabel('Voltage','FontWeight','bold','FontSize',14,'FontName','times');

% Create subplot
subplot4 = subplot(4,1,4,'Parent',figure1,'FontWeight','bold',...
    'FontName','times');
%% Uncomment the following line to preserve the X-limits of the axes
xlim(subplot4,[0 3e-09]);
%% Uncomment the following line to preserve the Y-limits of the axes
ylim(subplot4,[-0.5 2]);
%% Uncomment the following line to preserve the Z-limits of the axes
zlim(subplot4,[-1 1]);
box(subplot4,'on');
grid(subplot4,'on');
hold(subplot4,'all');

% Create plot
plot(X1,Y4,'Parent',subplot4,'LineWidth',4);

% Create title
title('OUT','FontWeight','bold','FontSize',14,'FontName','times');

% Create ylabel
ylabel('Voltage','FontWeight','bold','FontSize',14,'FontName','times');

% Create xlabel
xlabel('Time (s)','FontWeight','bold','FontSize',14,'FontName','times');

% Create textarrow
% annotation(figure1,'textarrow',[0.542682926829268 0.481890243902439],...
%     [0.21092564491654 0.193960546282246],'TextEdgeColor','none',...
%     'FontWeight','bold',...
%     'FontSize',14,...
%     'FontName','times',...
%     'String',{'SEU'});
% 
% % Create textarrow
% annotation(figure1,'textarrow',[0.541158536585366 0.483231707317074],...
%     [0.39453717754173 0.418816388467375],'TextEdgeColor','none',...
%     'FontWeight','bold',...
%     'FontSize',14,...
%     'FontName','times',...
%     'String',{'SEU'});

