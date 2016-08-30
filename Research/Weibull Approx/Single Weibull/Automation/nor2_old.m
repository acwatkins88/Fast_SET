% function [vout] = nor2()
    % Test Resistance Based Pulsed Approximation Method
    clc;
    clear all;
    close all;
    commandwindow;

    addpath('/ugrad/watkins/Desktop/Research/Weibull Approx/HspiceToolbox');
    addpath('/ugrad/watkins/Desktop/Research/Weibull Approx/Single Weibull');
    addpath('/ugrad/watkins/Desktop/Research/Weibull Approx/Single Weibull/delays');
    
    % VDD
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
    
    % Modified values for PMOS
%     min_val_p = 1.6;
%     
%     max_vd_p = 1.6;
%     
%     max_vg_p = 1.6;
    
    % Absolute Value of Minimum Value For Characterization
    min_val = 2;

    % Maximum value of Vd
    max_vd = 2.5;

    % Maximum value of Vg
    max_vg = 2.5;

    % Number of steps
    steps = 2399;
    
    % Flag to determine simulation type
    % flag = 0 - Our Method
    % flag = 1 - [6] Method
    flag = 0;
    
    % Input File Indexes
    Vg1_in = 9;
    if flag == 0
        Vg2_in = 8;
    else
        Vg2_in = 4;
    end
    Res_out = 10;

    cn1_in = 6;

    % Initial Node Voltage Node 1
%     p_vol_1 = 0.90;
    p_vol_1 = 0;

    % Load capacitance
    c_load = 0;
    
    % Shift Commands
    shift = 2;
    enable_shift = 0;

    % LUT input
    pres = loadsig('TransVals/65nmTables/pmosres.sw0');
    nres = loadsig('TransVals/65nmTables/nmosres.sw0');

    % Simulation Data (p11 to p12)
    inpsig = loadsig('norapprox.tr24');
    Vg1 = inpsig(Vg1_in).data;
    Vg2 = inpsig(Vg2_in).data;

    % Miller Capacitance Data
    n_miller = loadsig('TransVals/65nmTables/nmosmiller.tr0');
    p_miller = loadsig('TransVals/65nmTables/pmosmiller.tr0');

    Cp_miller = p_miller(cp_fnum).data;
    [gp_size, p_col] = size(Cp_miller);

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
    init_out = 1.05;

    % ------------------------------------
    % Initial Conditions
    % ------------------------------------

    vout_init = init_out;

    g_itr = floor(g2_length/steps);

    vout(1) = vout_init;

    % Current Arrays
    % in2_arr(1) = 0;
    % in3_arr(1) = 0;

    % Resistance Arrays
    % rn2_arr(1) = 0;
    % rn3_arr(1) = 0;

    p_vol_arr_1(1) = p_vol_1;

    % ------------------------------------
    % Start of Simulation
    % ------------------------------------
    dec = 0;
    
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
        else
            vd = vout(index-1);
            vd_prev = vout(index-2);
            vg1 = Vg1(index*g_itr - g_itr);
            vg2 = Vg2(index*g_itr - g_itr);
        end

        % Convert vd to index
        vd_i = floor((vd+min_val)/Vd_gran)+1;

        % Convert vg to index
        vg1_i = floor((vg1+min_val)/Vg_gran)+1;

        vg2_i = floor((vg2+min_val)/Vg_gran)+1;

        % Indexes for Miller Capacitance
        max_gvol = max_vg + min_val;
        
        vdp1 = p_vol_1;
        vdp_i = floor((vdp1+min_val)/Vd_gran)+1;

        vg1mp_i = floor((vg1+min_val)*(gp_size/max_gvol))+1;
        
%         rp1 = pres(p_fnum).data(vg1_i, vdp_i);
        ip1 = pres(p_fnum).data(vg1_i, vdp_i);
        cmp1 = Cp_miller(vg1mp_i, vdp_i);
        cmp1_arr(index) = cmp1;
        
        vn_ref = vdd - p_vol_1;
        
        vdp2 = vd + vn_ref;
        vdp2_i = floor((vdp2+min_val)/Vd_gran)+1;
        
        vgp2 = vg2 + vn_ref;
        vg2mp_i = floor((vgp2+min_val)*(gp_size/max_gvol))+1;
        vgp2_i = floor((vgp2+min_val)/Vd_gran)+1;
        
%         rp2 = pres(p_fnum).data(vgp2_i, vdp2_i);
        ip2 = pres(p_fnum).data(vgp2_i, vdp2_i);
        cmp2 = Cp_miller(vg2mp_i, vdp2_i);
        
        % NMOS

        % Gate Voltage for Miller 1
        vg1mn_i = floor((vg1+min_val)*(gp_size/max_gvol))+1;

%         rn1 = nres(n_fnum).data(vg1_i, vd_i);
        in1 = nres(n_fnum).data(vg1_i, vd_i);
        cmn1 = Cn_miller(vg1mn_i, vd_i);

        % Gate Voltage for Miller 2
        vg2mn_i = floor((vg2+min_val)*(gp_size/max_gvol))+1;

%         rn2 = nres(n_fnum).data(vg2_i, vd_i);
        in2 = nres(n_fnum).data(vg2_i, vd_i);
        cmn2 = Cn_miller(vg2mn_i, vd_i);

        % ------------------------------------
        % Evaluate Result
        % ------------------------------------

        % 2 - Input NAND Gate
        % Node Va
        p_cur = (ip1 +  ip2)/2;
        pcur_arr(index) = p_cur;

        t_step = 0.5e-12*g_itr;

        del_vga = Vg1(index*g_itr) - Vg1(index*g_itr - g_itr);
        del_vgb = Vg2(index*g_itr) - Vg2(index*g_itr - g_itr);
        
        n_cur = in1 + in2;
        ncur_arr(index) = n_cur;

        % Node Voltage Calculation
        cmp1 = 0.05e-15;
        if flag == 0
%             p_vol_arr_1(index) = inpsig(6).data(index);
%             p_vol_arr_1(index) = (((ip1 + -ip2)*t_step)/(cmp1)) + p_vol_arr_1(index-1) + del_vga;
            p_vol_arr_1(index) = (((ip1 + -ip2)*t_step)/(cmp1)) + p_vol_arr_1(index-1);
        else
            p_vol_arr_1(index) = inpsig(6).data(index);
        end
        p_vol_1 = p_vol_arr_1(index);

        % Pulse Approximatin with Load Capacitance
    %     vout(index) = (((p_cur - n_cur)*t_step)/c_load) + vout(index-1);
        
        cur_diff(index) = p_cur+n_cur;
        
        % Latest Pulse Approximation Algorithm - 2 input NAND
        nom = (p_cur + n_cur)*t_step+(cmp2+cmn2)*del_vgb+cmn1*del_vga;
        vout(index) = (nom/(c_load+cmp2+cmn1+cmn2))+vout(index-1);    
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
%     vout_t = vout;
%     c_sig_t = c_sig;
%     n = length(vout_t);
%     
% %     mse_out = (1/n)*sum((vout - c_sig).^2)
%     mse_out = (1/n)*sum((vout_t - c_sig_t))
    c_sig_t = c_sig(1:length(vout));
    mse_out = (1/steps)*sum((vout - c_sig_t).^2)

% Plot 1
    
t = 0:0.5e-12:1.2e-9;
t = t(1:length(Vg1_down));

figure;
hold on
plot(t, Vg1_down, 'b', 'LineWidth', 2);
plot(t, Vg2_down, 'r', 'LineWidth', 2);
set(gca, 'FontSize', 15);
legend('Input A', 'Input B', 'FontSize', 15);
title('Plot of Gate Inputs', 'FontSize', 15);
xlabel('Time (ns)', 'FontSize', 15);
ylabel('Voltage', 'FontSize', 15);
hold off

figure;
hold on;
plot(c_sig, 'b', 'LineWidth', 2);
plot(vout, 'r', 'LineWidth', 2);
set(gca, 'FontSize', 15);
legend('Hspice Output', 'Estimated Output');
title('Comparison Between HSPICE and the Model', 'FontSize', 15);
xlabel('Time (ns)', 'FontSize', 15);
ylabel('Voltage', 'FontSize', 15);
hold off;

% figure;
% hold on;
% plot(p_vol_arr_1, 'r');
% plot(inpsig(cn1_in).data);
% legend('Calculated', 'Hspice');
% hold off;

% Plot 2

% t = 0:0.5e-12:1.2e-9;
% t = t(1:1000);
% 
% figure;
% hold on
% plot(Vg1_down, 'b', 'LineWidth', 2);
% plot(Vg2_down, 'r', 'LineWidth', 2);
% set(gca, 'FontSize', 15);
% legend('Input A', 'Input B', 'FontSize', 15);
% title('Plot of Gate Inputs', 'FontSize', 15);
% xlabel('Time (ns)', 'FontSize', 15);
% ylabel('Voltage', 'FontSize', 15);
% hold off
% 
% figure;
% hold on;
% plot(c_sig, 'b', 'LineWidth', 2);
% plot(vout, 'r', 'LineWidth', 2);
% set(gca, 'FontSize', 15);
% legend('Hspice Output', 'Estimated Output');
% title('Comparison Between HSPICE and the Model', 'FontSize', 15);
% xlabel('Time (ns)', 'FontSize', 15);
% ylabel('Voltage', 'FontSize', 15);
% hold off;

