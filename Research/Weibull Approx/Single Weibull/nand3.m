% function [vout] = nand3()
% Test Resistance Based Pulsed Approximation Method
clc;
clear all;
close all;
commandwindow;

% addpath('/ugrad/watkins/Desktop/Research/Weibull Approx/HspiceToolbox');
% addpath('/ugrad/watkins/Desktop/Research/Weibull Approx/Single Weibull');
% addpath('/ugrad/watkins/Desktop/Research/Weibull Approx/Single Weibull/delays');

addpath('/home/adam/Fast_SET/Research/Weibull Approx/HspiceToolbox');
addpath('/home/adam/Fast_SET/Research/Weibull Approx/Single Weibull');
addpath('/home/adam/Fast_SET/Research/Weibull Approx/Single Weibull/delays');

% Vdd
vdd = 1.05;

% Signal number of PMOS resistance file
p_fnum = 9;

% Signal number of NMOS resistance file
n_fnum = 9;

% Signal number of PMOS current file
cp_fnum = 9;

% Signal number of NMOS current file
cn_fnum = 9;

% Granularity of Vd
Vd_gran = 0.0125;

% Granularity of Vg
Vg_gran = 0.0125;

% Absolute Value of Minimum Value For Characterization
min_val = 2;

% Maximum value of Vd
max_vd = 2.5;

% Maximum value of Vg
max_vg = 2.5;

% Number of steps
steps = 900;

% Flag == 0 - Our Method
% Flag == 1 - [6] Method
flag = 0;

% Input File Indexes
if flag == 0
    Vg1_in = 9;
else
    Vg1_in = 4;
end
Vg2_in = 10;
Vg3_in = 4;

Res_out = 11;

cn1_in = 6;
cn2_in = 7;

% Initial Node Voltage Node 1
% n_vol_1 = 0.90;
n_vol_1 = 0;

% Initial Node Voltage Node 2
% n_vol_2 = 0.90;
n_vol_2 = 0;

% Load capacitance
c_load = 0.25e-15;

% Shift Commands
shift = 2;
enable_shift = 0;

% LUT input
pres = loadsig('TransVals/32nmTables/pmosres.sw0');
nres = loadsig('TransVals/32nmTables/nmosres.sw0');

% Simulation Data (p11 to p12)
inpsig = loadsig('singleapprox.tr0');
Vg1 = inpsig(Vg1_in).data;
Vg2 = inpsig(Vg2_in).data;
Vg3 = inpsig(Vg3_in).data;

t = 0;
tstep_1 = 0.5e-12;
inp_dim = length(Vg1);
inp_file = fopen('t_input.dat', 'w+');
for i = 1:inp_dim
    fprintf(inp_file, '%e ', t);
    fprintf(inp_file, '%e ', Vg1(i));
    fprintf(inp_file, '%e', Vg2(i));
    fprintf(inp_file, '%e\n', Vg3(i));
    t = t+tstep_1;
end

% Miller Capacitance Data
n_miller = loadsig('TransVals/32nmTables/nmosmiller.tr0');
p_miller = loadsig('TransVals/32nmTables/pmosmiller.tr0');

Cp_miller = p_miller(cp_fnum).data;
[gp_size, p_col] = size(Cp_miller);
% max_gp = 1.5;

Cn_miller = n_miller(cn_fnum).data;
[gn_size, n_col] = size(Cn_miller);

g1_length = length(Vg1);
g2_length = length(Vg2);

% HSpice Calculated Output
result = inpsig(Res_out).data;

t_node_vol = inpsig(5).data;

t_pcur1 = abs(inpsig(2).data);
t_pcur2 = abs(inpsig(3).data);
t_pcur = t_pcur1+t_pcur2;

% Scale down Hspice signal for comparison
inp_l = length(Vg1);
scale = floor(inp_l/steps);
count = 1;
for i = 1:inp_l
    if mod(i,scale) == 0
        red_tpcur(count) = t_pcur(i);
        tn_volarr(count) = t_node_vol(i);
        c_sig(count) = result(i);
        Vg1_down(count) = Vg1(i);
        Vg2_down(count) = Vg2(i);
        Vg3_down(count) = Vg3(i);
        count = count + 1;
    end
end

% disp('Node Voltages');
% lssig(nsig)

disp('PMOS Resistance');
lssig(pres)

disp('NMOS Resistance');
lssig(nres)

disp('Inp Sig');
lssig(inpsig)

disp('PMOS Miller');
lssig(p_miller)

disp('NMOS Miller');
lssig(n_miller)

% ------------------------------------
% Input Type
% 0 - a = 0, b = 0
% 1 - a = 0, b = 1
% 2 - a = 1, b = 0
% 3 - a = 1, b = 1
% ------------------------------------
% init_out = vdd;
init_out = 0;

% ------------------------------------
% Initial Conditions
% ------------------------------------

vout_init = init_out;

g_itr = floor(g2_length/steps);

vout(1) = vout_init;

if flag == 1
    n_vol_arr_1 = inpsig(cn1_in).data;
    n_vol_arr_2 = inpsig(cn2_in).data;
else    
    n_vol_arr_1(1) = n_vol_1;
    n_vol_arr_2(1) = n_vol_2;
end

% figure;
% hold on
% plot(Vg1_down, 'm');
% plot(Vg2_down, 'k');
% plot(Vg3_down, 'g');
% plot(c_sig, 'r');
% legend('VG1', 'VG2', 'VG3', 'Hspice Output');
% title('Comparison of Hspice and the Model');
% hold off

% ------------------------------------
% Start of Simulation
% ------------------------------------

tic
for index = 2:steps
    
    % ------------------------------------
    % Get Values from LUT
    % ------------------------------------
    if index == 1 || index == 2
        vd = vout_init;
        vd_prev = vout_init;
        vg1 = Vg1(1);
        vg2 = Vg2(1);
        vg3 = Vg3(1);
    else
        vd = vout(index-1);
        vd_prev = vout(index-2);
        vg1 = Vg1(index*g_itr - g_itr);
        vg2 = Vg2(index*g_itr - g_itr);
        vg3 = Vg3(index*g_itr - g_itr);
    end
    
    % Convert vd to index
    vd_i = floor((vd+min_val)/Vd_gran)+1;
    
    % Convert vg to index
    vg1_i = floor((vg1+min_val)/Vg_gran)+1;
    
    vg2_i = floor((vg2+min_val)/Vg_gran)+1;
    
    vg3_i = floor((vg3+min_val)/Vg_gran)+1;
    
    % Indexes for Miller Capacitance
    max_gvol = max_vg + min_val;
    
    vg1mp_i = floor((vg1+min_val)*(gp_size/max_gvol))+1;
    vg2mp_i = floor((vg2+min_val)*(gp_size/max_gvol))+1;
    vg3mp_i = floor((vg3+min_val)*(gp_size/max_gvol))+1;
    
    vdp = vd;
    vdp_i = floor((vdp+min_val)/Vd_gran)+1;
    
    % PMOS
    ip1 = pres(cp_fnum).data(vg1_i, vd_i);
    cmp1 = Cp_miller(vg1mp_i, vdp_i);
    
    ip2 = pres(cp_fnum).data(vg2_i, vd_i);
    cmp2 = Cp_miller(vg2mp_i, vdp_i);
    
    ip3 = pres(cp_fnum).data(vg3_i, vd_i);
    cmp3 = Cp_miller(vg3mp_i, vdp_i);
    
    % NMOS
    vn1g = vg1 - n_vol_1;
    
    % Gate Voltage for Miller 1
    vg1mn_i = floor((vn1g+min_val)*(gp_size/max_gvol))+1;
    vn1g_i = floor((vn1g+min_val)/Vg_gran)+1;
    
    vdn1 = vd - n_vol_1;
    vdn1_i = floor((vdn1+min_val)/Vd_gran)+1;
    
    in1 = nres(cn_fnum).data(vn1g_i, vdn1_i);
    cmn1 = Cn_miller(vg1mn_i, vdn1_i);
    
    vdn2 = n_vol_1;
    vdn2_i = floor((vdn2+min_val)/Vd_gran)+1;
    
    vn2g = vg2 - n_vol_2;
    
    % Gate Voltage for Miller 2
    vg2mn_i = floor((vn2g+min_val)*(gp_size/max_gvol))+1;
    vn2g_i = floor((vn2g+min_val)/Vg_gran)+1;
    n_vol1_i = floor((n_vol_1+min_val)/Vg_gran)+1;
    
    in2 = nres(cn_fnum).data(vn2g_i, vdn2_i);
    cmn2 = Cn_miller(vg2mn_i, vdn2_i);
    
    vdn3 = n_vol_2;
    vdn3_i = floor((vdn3+min_val)/Vd_gran)+1;
    n_vol2_i = floor((n_vol_2+min_val)/Vg_gran)+1;
    
    % Gate voltage for Miller 3
    vg3mn_i = floor((vg3+min_val)*(gp_size/max_gvol))+1;
    in3 = nres(cn_fnum).data(vg3_i, vdn3_i);
    cmn3 = Cn_miller(vg3mn_i, vdn3_i);
    
    % ------------------------------------
    % Evaluate Result
    % ------------------------------------
    
    % 2 - Input NAND Gate
    % Node Va
%     rpeq = 1/(1/rp1 + 1/rp2);
    p_cur = ip1 +  ip2 + ip3;
    pcur_arr(index) = p_cur;
    
    t_step = 0.5e-12*g_itr;
    
    del_vga = Vg1(index*g_itr) - Vg1(index*g_itr - g_itr);
    del_vgb = Vg2(index*g_itr) - Vg2(index*g_itr - g_itr);
    del_vgc = Vg3(index*g_itr) - Vg3(index*g_itr - g_itr);
    
    avg_i = (in1+in2+in3)/3;
    n_cur = avg_i;
    ncur_arr(index) = n_cur;
    
    % Node Voltage Calculation
    %     n_vol = inpsig(5).data(index);
    cmn2 = 0.25e-15;
    n_vol_arr_1(index) = (((-in1 + in2)*t_step)/(cmn2)) + n_vol_arr_1(index-1);
    n_vol_1 = n_vol_arr_1(index);

%     n_vol_1 = inpsig(cn1_in).data(index);
    
    cmn3 = 0.25e-15;
    n_vol_arr_2(index) = (((-in2 + in3)*t_step)/(cmn3)) + n_vol_arr_2(index-1);
    n_vol_2 = n_vol_arr_2(index);
    
%     n_vol_2 = inpsig(cn2_in).data(index);
    
    % Pulse Approximatin with Load Capacitance
%   vout(index) = (((p_cur - n_cur)*t_step)/c_load) + vout(index-1);
    
    % Latest Pulse Approximation Algorithm - 3 input NAND
    nom = (p_cur+n_cur)*t_step+(cmp1+cmn1)*del_vga+(cmp2)*del_vgb + (cmp3)*del_vgc;
    vout(index) = (nom/(c_load+cmp1+cmp2+cmp3+cmn1))+vout(index-1);
end
toc

% Shift signal
if enable_shift == 1
    temp_s = vout;
    t_ind = 1;
    for i = 1:steps
        if i <= (shift+1)
            vout(i) = temp_s(1);
        else
            vout(i) = temp_s(t_ind);
            t_ind = t_ind+1;
        end
    end
end

%     vout_t = vout(500:1200);
%     c_sig_t = c_sig(500:1200);
%     n = length(vout_t);
%
%     mse_out = (1/n)*sum((vout_t - c_sig_t).^2)

c_sig_t = c_sig(1:length(vout));
mse_out = (1/steps)*sum((vout - c_sig_t).^2)

if flag == 0
    f_name = ['PlotResults/NAND3/Propout', num2str(steps), '.out'];
else
    f_name = ['PlotResults/NAND3/Propout6', num2str(steps), '.out'];
end
f_hspice = ['PlotResults/NAND3/HPSPICE', num2str(steps), '.out'];

% Send Output Resutls to file
prop_file = fopen(f_name, 'w+');
res_file = fopen(f_hspice, 'w+');

for i = 1:length(vout)
    fprintf(prop_file, '%e\n', vout(i));
    fprintf(res_file, '%e\n', c_sig(i));
end

figure;
hold on
plot(Vg1_down, 'm');
plot(Vg2_down, 'k');
plot(Vg3_down, 'g');
plot(c_sig, 'r');
plot(vout, 'b');
legend('Input a', 'Input b', 'HSpice Output', 'Estimated Output')
title('Plot of Original Data with Inputs');
hold off

figure;
hold on;
plot(c_sig, 'r');
plot(vout, 'b');
legend('Hspice Output', 'Estimated Output');
title('Comparison of Hspice and the Model');
hold off;

% figure;
% hold on;
% plot(pcur_arr, 'r');
% plot(pcur_ideal, 'b');
% legend('Calculated', 'Ideal');
% hold off;

% figure;
% hold on;
% plot(ncur_arr, 'r');
% plot(in1_ideal, 'b');
% plot(in2_ideal, 'k');
% plot(in3_ideal, 'g');
% legend('Calculated', 'Ideal 1','Ideal 2', 'Ideal 3');
% hold off;

% figure;
% hold on;
% plot(n_vol_arr_1, 'r');
% plot(inpsig(cn1_in).data);
% legend('Calculated', 'Hspice');
% hold off;
% % 
% figure;
% hold on;
% plot(n_vol_arr_2, 'r');
% plot(inpsig(cn2_in).data);
% legend('Calculated', 'Hspice');
% hold off;

% figure;
% hold on;
% plot(cmn2_arr);
% plot(cmn3_arr, 'r');
% hold off;

% val = 1:steps;
% length(val)
% length(vout)
% length(cmn2_arr)
% figure;
% plotyy(val, vout, val, cmn1_arr);

% figure;
% hold on;
% plot(n_vol_arr);
% plot(c_sig, 'r');
% legend('Node Voltage', 'Output Voltage');
% hold off;

% figure;
% hold on;
% plot(inpsig(6).data, 'r');
% plot(n_vol_arr);
% legend('Hspice', 'Calculated');
% title('Comparison of Node Voltages');
% hold off;
% end
