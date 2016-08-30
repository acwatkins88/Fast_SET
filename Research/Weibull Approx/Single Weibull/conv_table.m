% Test Resistance Based Pulsed Approximation Method
clc;
clear all;
close all;
commandwindow;

addpath('/ugrad/watkins/Desktop/Research/Weibull Approx/HspiceToolbox');
addpath('/ugrad/watkins/Desktop/Research/Weibull Approx/Single Weibull');
addpath('/ugrad/watkins/Desktop/Research/Weibull Approx/Single Weibull/delays');

% MOS Current File Index
f_cnum = 9;

% Miller File Index
f_mnum = 9;

% LUT input
pres = loadsig('TransVals/32nmTables/pmosres.sw0');
nres = loadsig('TransVals/32nmTables/nmosres.sw0');

% Miller Capacitance Data
n_miller = loadsig('TransVals/32nmTables/nmosmiller.tr0');
p_miller = loadsig('TransVals/32nmTables/pmosmiller.tr0');

% Open File Handles
% Files For Transistor Current
pres_file = fopen('TransVals/32nmTables/pmoscur.out', 'w');
nres_file = fopen('TransVals/32nmTables/nmoscur.out', 'w');

% Files for Miller Capacitance
nmiller_file = fopen('TransVals/32nmTables/nmosmiller.out', 'w');
pmiller_file = fopen('TransVals/32nmTables/pmosmiller.out', 'w');

% Assume the PMOS and NMOS current files are the same dimensions
[row col] = size(pres(f_cnum).data);
for r = 1:row
    for c = 1:col
        fprintf(pres_file, '%d ', pres(f_cnum).data(r, c));
        fprintf(nres_file, '%d ', nres(f_cnum).data(r, c));
    end
    if r ~= row
        fprintf(pres_file, '\n\r');
        fprintf(nres_file, '\n\r');
    end
end

% Assume the PMOS and NMOS Miller files are the same dimensions
[row col] = size(n_miller(f_mnum).data);
for r = 1:row
    for c = 1:col
        fprintf(nmiller_file, '%d ', n_miller(f_mnum).data(r, c));
        fprintf(pmiller_file, '%d ', p_miller(f_mnum).data(r, c));
    end
    if r ~= row
        fprintf(nmiller_file, '\n\r');
        fprintf(pmiller_file, '\n\r');
    end
end

disp('Conversion Completed');
