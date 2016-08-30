% Generate Result Figures
close all;
clear all;
addpath('/ugrad/watkins/Desktop/Research/Weibull Approx/Single Weibull/PlotResults');

list = dir('*300.out*');
f_count = 1;
list

for file = list'  
    f_handle = fopen(file.name);
    data = fscanf(f_handle, '%e');
    
    for i = 1:length(data)
        p_data(f_count, i) = data(i);
    end
    f_count = f_count + 1;
end

% 2399
% t = 0:0.5e-12:1.2e-9;

% 960
% t = 0:1.25e-12:1.5e-9;

% 300
t = 0:4e-12:1.2e-9;

t1 = t(1:length(p_data(1,:)));
t2 = t(1:length(p_data(2,:)));
t3 = t(1:length(p_data(3,:)));

figure;

hold on;
plot(t2, p_data(2,:), 'b+', 'LineWidth', 2);
plot(t3, p_data(3,:), 'k--', 'LineWidth', 2);
plot(t1, p_data(1,:), 'r', 'LineWidth', 2);
xlim([0.05e-9 0.40e-9]);
set(gca, 'FontSize', 18);
legend('Proposed', '[1]', 'HSPICE', 'FontSize', 18);
title('Plot of Gate Outputs 300 Points', 'FontSize', 18);
xlabel('Time', 'FontSize', 18);
ylabel('Voltage', 'FontSize', 18);
hold off;
