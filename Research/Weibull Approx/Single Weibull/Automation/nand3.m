clc;
clear all;
close all;
commandwindow;

addpath('/ugrad/watkins/Desktop/Research/Weibull Approx/HspiceToolbox');
addpath('/ugrad/watkins/Desktop/Research/Weibull Approx/Single Weibull');
addpath('/ugrad/watkins/Desktop/Research/Weibull Approx/Single Weibull/delays');

% Vdd
vdd = 1.05;

% Signal number of PMOS resistance file
p_fnum = 9;

% Signal number of NMOS resistance file
n_fnum = 9;

% Signal number of PMOS Miller Capacitance
cp_fnum = 9;

% Signal number of NMOS Miller Capacitance
cn_fnum = 9;

% Granularity of Vd
Vd_gran = 0.0125;

% Granularity of Vg
Vg_gran = 0.0125;

% Absolute Value of Minimum Value for Characterizaton
min_val = 2;

% Maximum value of Vd
max_vd = 2.5;

% Maximum value of Vg
max_vg = 2.5;

% Number of steps
steps = 2399;

% Initial Node Voltage Node 
n_vol_1 = 0;
n_vol_2 = 0;

% Load capacitance
% No Load
c_load = 0;

% Shift Commands
shift = 2;
enable_shift = 0;

% LUT input
nres = loadsig('TransVals/65nmTables/pmosres.sw0');
pres = loadsig('TransVals/65nmTables/nmosres.sw0');

% pres = loadsig('TransVals/32nmTables/pmosres.sw0');
% nres = loadsig('TransVals/32nmTables/nmosres.sw0');

% Miller Capacitance Data
n_miller = loadsig('TransVals/65nmTables/nmosmiller.tr0');
p_miller = loadsig('TransVals/65nmTables/pmosmiller.tr0');

% n_miller = loadsig('TransVals/32nmTables/nmosmiller.tr0');
% p_miller = loadsig('TransVals/32nmTables/pmosmiller.tr0');

% Cp_miller = p_miller(cp_fnum).data;
Cp_miller = n_miller(cp_fnum).data;
[gp_size, p_col] = size(Cp_miller);
% max_gp = 3;

% Cn_miller = n_miller(cn_fnum).data;
Cn_miller = p_miller(cn_fnum).data;
[gn_size, n_col] = size(Cn_miller);

disp('PMOS Resistance');
lssig(pres)

disp('NMOS Resistance');
lssig(nres)

disp('PMOS Miller');
lssig(p_miller)

disp('NMOS Miller');
lssig(n_miller)

% Flag == 0 - Our Method
% Flag == 1 - [6] Method
flag = 0;

% Input File Indexes
Vg1_in = 10;
if flag == 0
    Vg2_in = 9;
else
    Vg2_in = 4;
end
Vg3_in = 4;
Res_out = 11;

cn1_in = 6;
cn2_in = 7;

% MSE index
m_ind = 1;

list = dir('*.tr*');
for file = list'    
    % Simulation Data (p11 to p12)
    inpsig = loadsig(file.name);
    Vg1 = inpsig(Vg1_in).data;
    Vg2 = inpsig(Vg2_in).data;
    Vg3 = inpsig(Vg3_in).data;

    % HSpice Calculated Output
    result = inpsig(Res_out).data;

    % Scale down Hspice signal for comparison
    inp_l = length(Vg1);
    scale = floor(inp_l/steps);
    count = 1;
    for i = 1:inp_l
        if mod(i,scale) == 0
            c_sig(count) = result(i);
            Vg1_down(count) = Vg1(i);
            Vg2_down(count) = Vg2(i);
            count = count + 1;
        end
    end

    g1_length = length(Vg1);
    g2_length = length(Vg2);

    % ------------------------------------
    % Input Type
    % 0 - a = 0, b = 0
    % 1 - a = 0, b = 1
    % 2 - a = 1, b = 0
    % 3 - a = 1, b = 1
    % ------------------------------------
    init_out = 1;

    % ------------------------------------
    % Initial Conditions
    % ------------------------------------

    vout_init = init_out;

    g_itr = floor(g2_length/steps);

    vout(1) = vout_init;

    % Current Arrays
    % in2_arr(1) = 0;

    % Resistance Arrays
    % rn2_arr(1) = 0;

    n_vol_arr_1(1) = n_vol_1;
    n_vol_arr_2(1) = n_vol_2;

    % ------------------------------------
    % Start of Simulation
    % ------------------------------------

    q_t = 0;

    if flag == 1
        n_vol_arr_1 = inpsig(cn1_in).data;
    end

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
    %     rp1 = pres(p_fnum).data(vg1_i, vd_i);
        ip1 = pres(cp_fnum).data(vg1_i, vd_i);
        cmp1 = Cp_miller(vg1mp_i, vdp_i);

    %     rp2 = pres(p_fnum).data(vg2_i, vd_i);
        ip2 = pres(cp_fnum).data(vg2_i, vd_i);
        cmp2 = Cp_miller(vg2mp_i, vdp_i);

    %     rp3 = pres(p_fnum).data(vg3_i, vd_i);
        ip3 = pres(cp_fnum).data(vg3_i, vd_i);
        cmp3 = Cp_miller(vg3mp_i, vdp_i);

        % NMOS
        vn1g = vg1 - n_vol_1;

        % Gate Voltage for Miller 1
        vg1mn_i = floor((vn1g+min_val)*(gp_size/max_gvol))+1;
        vn1g_i = floor((vn1g+min_val)/Vg_gran)+1;

        vdn1 = vd - n_vol_1;
        vdn1_i = floor((vdn1+min_val)/Vd_gran)+1;

    %     rn1 = nres(n_fnum).data(vn1g_i, vdn1_i);
        in1 = nres(cn_fnum).data(vn1g_i, vdn1_i);
        cmn1 = Cn_miller(vg1mn_i, vdn1_i);

        vdn2 = n_vol_1;
        %     vdn2 = n_vol_1 - n_vol_2;
        vdn2_i = floor((vdn2+min_val)/Vd_gran)+1;

        vn2g = vg2 - n_vol_2;

        % Gate Voltage for Miller 2
        vg2mn_i = floor((vn2g+min_val)*(gp_size/max_gvol))+1;
        vn2g_i = floor((vn2g+min_val)/Vg_gran)+1;
        n_vol1_i = floor((n_vol_1+min_val)/Vg_gran)+1;

    %     rn2 = nres(n_fnum).data(vn2g_i, vdn2_i);
        in2 = nres(cn_fnum).data(vn2g_i, vdn2_i);
        cmn2 = Cn_miller(vg2mn_i, vdn2_i);

        vdn3 = n_vol_2;
        vdn3_i = floor((vdn3+min_val)/Vd_gran)+1;
        n_vol2_i = floor((n_vol_2+min_val)/Vg_gran)+1;

        % Gate voltage for Miller 3
        vg3mn_i = floor((vg3+min_val)*(gp_size/max_gvol))+1;
    %     rn3 = nres(n_fnum).data(vg3_i, vdn3_i);
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

        avg_i = -(in1+in2+in3)/3;
        n_cur = avg_i;
        ncur_arr(index) = n_cur;

        % Node Voltage Calculation
        %     n_vol = inpsig(5).data(index);
        cmn2 = 0.2e-15;
        cmn3 = 0.5e-15;

        if flag == 0
            n_vol_arr_1(index) = (((-in1 + in2)*t_step)/(cmn2)) + n_vol_arr_1(index-1) + del_vgb;
            n_vol_arr_1(index) = (((-in1 + in2)*t_step)/(cmn2)) + n_vol_arr_1(index-1);
            n_vol_1 = n_vol_arr_1(index);

            n_vol_arr_2(index) = (((-in2 + in3)*t_step)/(cmn3)) + n_vol_arr_2(index-1) + del_vgc;
            n_vol_arr_2(index) = (((-in2 + in3)*t_step)/(cmn3)) + n_vol_arr_2(index-1);
            n_vol_2 = n_vol_arr_2(index);
        else
            n_vol_1 = inpsig(cn1_in).data(index);
            n_vol_arr_1(index) = inpsig(cn1_in).data(index);

            n_vol_2 = inpsig(cn2_in).data(index);
            n_vol_arr_2(index) = inpsig(cn2_in).data(index);
        end

        % Latest Pulse Approximation Algorithm - 3 input NAND
        nom = (p_cur-n_cur)*t_step+(cmp1+cmn1)*del_vga+(cmp2)*del_vgb + (cmp3)*del_vgc;
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

    c_sig_t = c_sig(1:length(vout));
    mse_out(m_ind) = (1/steps)*sum((vout - c_sig_t).^2);
    s_time(m_ind) = toc;
    
    figure;
    hold on;
    plot(c_sig, 'r');
    plot(vout, 'b');
    legend('Hspice Output', 'Estimated Output');
    title('Comparison of Hspice and the Model');
    hold off;
end

s_time_avg = mean(s_time)
mse_avg = mean(mse_out)

% figure;
% hold on
% plot(Vg1_down, 'm');
% plot(Vg2_down, 'k');
% plot(c_sig, 'r');
% plot(vout, 'b');
% legend('Input a', 'Input b', 'HSpice Output', 'Estimated Output')
% title('Plot of Original Data with Inputs');
% hold off
% 
% figure;
% hold on;
% plot(c_sig, 'r');
% plot(vout, 'b');
% legend('Hspice Output', 'Estimated Output');
% title('Comparison of Hspice and the Model');
% hold off;
% 
% figure;
% hold on;
% plot(n_vol_arr_1, 'r');
% plot(inpsig(cn1_in).data);
% legend('Calculated', 'Hspice');
% hold off;