clear all;
clc;
addpath('/ugrad/watkins/Desktop/Research/Weibull Approx/HspiceToolbox');
addpath('/ugrad/watkins/Desktop/Research/Weibull Approx/Single Weibull');
addpath('/ugrad/watkins/Desktop/Research/Weibull Approx/Single Weibull/Model\ Compare');

x = loadsig('singleapprox.tr0');

testdata = dlmread('result.res', ' ');
t_size = size(testdata);

% Variables 
step = 5e-12;
f_end = (step*t_size(1)) - step;

% Print Signals in x
for p = 1:size(x)
	printf("%d. ", p);
	printf("Plot: %s\n", x(p).name);
	if strcmp(x(p).name, "vdd")
		vddval = x(p).data(1)
	endif
endfor

% Prompt user to choose signal
val = input("Enter number to plot data: ");

% Initial Dimensions
init_size = size(x(val).data);
init_step = .5e-12;
init_end = (init_step*init_size(1)) - init_step;

t1 = 0:step:f_end;
t2 = 0:init_step:init_end;

figure(1);
clf;
newplot;
hold on;
plot(t1, testdata);
plot(t2, x(val).data, 'r');
hold off;
