% File to plot latch waveforms
clc; clear all; close all;

addpath('/home/ugrad/watkins/Desktop/Research/Weibull Approx/HspiceToolbox');

x = loadsig('/home/ugrad/watkins/Desktop/Research/Weibull Approx/Single Weibull/Latchtest.tr0');

for i = 1:length(x)
    
    if strcmp(x(i).name, 'ckh');
        clk = x(i).data;
    elseif x(i).name == 'd'
        d = x(i).data;
    elseif strcmp(x(i).name, 'out');
        out = x(i).data;
    elseif strcmp(x(i).name, 'co1');
        co1 = x(i).data;
    elseif strcmp(x(i).name, 'co2');
        co2 = x(i).data;
    elseif strcmp(x(i).name, 'co3');
        co3 = x(i).data;
    elseif strcmp(x(i).name, 'n2');
        n2 = x(i).data;
    elseif strcmp(x(i).name, 'n4');
        n4 = x(i).data;
    elseif strcmp(x(i).name, 'n5');
        n5 = x(i).data;
    end
    
end

t = linspace(0, 3.2e-9, length(x(i).data));

% defaultmodeplot(t, clk, d, out)
% internalfig(t, clk, d, co1, co3, out)
% externalfig(t, clk, d, n4, out)

% g = figure('Position', [50 50 936 893]);

set(0, 'DefaultAxesFontSize', 22);
set(0, 'DefaultAxesFontName', 'Times');
set(0, 'DefaultAxesFontWeight', 'Bold');

g = DNUFig(t, n4, out);
saveas(g, 'Latch Plots/NewPlots/n5out.eps')

% CLK D
% subplot(3, 1, 1)
% plot(t, clk, 'k', 'LineWidth', 4)
% title('CLK')
% ylabel('Voltage')
% axis([0 2e-9 -0.5 2]);
% grid on
% 
% subplot(3, 1, 2)
% plot(t, d, 'k', 'LineWidth', 4)
% title('D')
% ylabel('Voltage')
% axis([0 2e-9 -0.5 2]);
% grid on
% 
% subplot(3, 1, 3)
% plot(t, out, 'k', 'LineWidth', 4)
% title('OUT')
% ylabel('Voltage')
% axis([0 2e-9 -0.5 2]);
% grid on

% Create textbox
% annotation(g,'textbox',...
%     [0.128176253176254 0.83986562150056 0.219045969045969 0.0361001284955697],...
%     'String',{'Transparent Mode'},...
%     'FontWeight','bold',...
%     'FontSize',22,...
%     'FitBoxToText','off',...
%     'EdgeColor','none', 'FontName', 'Times');
% 
% % Create textbox
% annotation(g,'textbox',...
%     [0.363988218988222 0.766114221724524 0.141353661353659 0.0361001284955697],...
%     'String',{'Hold Mode'},...
%     'FontWeight','bold',...
%     'FontSize',22,...
%     'FitBoxToText','off',...
%     'EdgeColor','none', 'Fontname', 'Times');

% saveas(g, 'Latch Plots/defaultoperation.eps')

% subplot(2, 1, 1)
% plot(t, n4, 'LineWidth', 4)
% title('N5')
% ylabel('Voltage')
% % axis([0 3e-9 -0.5 2]);
% grid on
% 
% subplot(2, 1, 2)
% plot(t, out, 'LineWidth', 4)
% title('OUT')
% ylabel('Voltage')
% % axis([0 3e-9 -0.5 2]);
% grid on
% 
% annotation(g,'textarrow',[0.380357142857143 0.294642857142857],...
%     [0.852380952380954 0.823809523809524],'TextEdgeColor','none','FontSize',22,...
%     'FontName','Times',...
%     'String',{'SEU'}, 'FontWeight', 'bold');
% 
% % Create arrow
% annotation(g,'arrow',[0.425 0.483928571428571],...
%     [0.847619047619048 0.830952380952381]);
% 
% % Create textarrow
% annotation(g,'textarrow',[0.387857142857142 0.302142857142857],...
%     [0.38476190476191 0.356190476190479],'TextEdgeColor','none','FontSize',22,...
%     'FontName','Times',...
%     'String',{'SEU'}, 'FontWeight', 'bold');
% 
% % Create arrow
% annotation(g,'arrow',[0.432499999999999 0.491428571428571],...
%     [0.380000000000004 0.363333333333337]);
