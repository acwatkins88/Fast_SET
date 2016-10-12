clc; clear all; close all;

%addpath('/home/adam/Fast_SET/PathBasedSoftError');
%addpath('/home/acwatkins/Fast_SET/PathBasedSoftError')

inp_data1 = dlmread('OutputRes6_0_5.out');
%inp_data2 = dlmread('OutputRes8_1.out');
%inp_data3 = dlmread('OutputRes8_2.out');
%inp_data4 = dlmread('OutputRes8_3.out');
%inp_data5 = dlmread('OutputRes8_17.out');
%inp_data6 = dlmread('OutputRes8_5.out');
%res_data2 = dlmread('OutputRes3.out');
%cur_data = dlmread('CurOut.out');

figure;
hold on
plot(inp_data1, 'b');
%plot(inp_data2, 'r');
%plot(inp_data3, 'g');
%plot(inp_data4, 'k');
%plot(inp_data5, 'm');
%plot(inp_data6, 'y');
hold off

%figure;
%hold on
%plot(res_data1, 'b');
%plot(res_data2, 'r');
%hold off

%figure;
%plot(cur_data);
