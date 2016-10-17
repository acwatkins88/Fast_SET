clc; close all; clear all; 

addpath('C:\Users\bokya\Fast_SET\Research\Weibull Approx\HspiceToolbox');
addpath('C:\Users\bokya\Fast_SET\Research\Weibull Approx\Single Weibull');

data = loadsig('singleapprox.tr0');

set(0, 'DefaultAxesFontSize', 18);
set(0, 'DefaultAxesFontName', 'Times');
set(0, 'DefaultAxesFontWeight', 'Normal');

figure;
plot(data(12).data, 'LineWidth', 3);
axis([1000 1500 0 1.75])
% Create title
title('Example of Transient Pulse','FontWeight', 'Normal', 'FontSize',18,'FontName','Times');

% Create ylabel
ylabel('Voltage','FontSize',18,'FontName','Times');