clc; clear all; close all;

%addpath('/home/adam/Fast_SET/PathBasedSoftError');
%addpath('/home/acwatkins/Fast_SET/PathBasedSoftError')

%inp_data1 = dlmread('OutputRes5_0.out');
%inp_data2 = dlmread('OutputResNot5_0.out');
%inp_data3 = dlmread('OutputRes6_5.out');
%inp_data4 = dlmread('OutputRes6_7.out');
%inp_data5 = dlmread('OutputRes7_11.out');
%res_data2 = dlmread('OutputRes3.out');
cur_data = dlmread('CurOut.out');

%figure;
%hold on
%plot(inp_data1, 'b');
%plot(inp_data2, 'r');
%plot(inp_data3, 'g');
%plot(inp_data4, 'k');
%plot(inp_data5, 'm');
%hold off

%figure;
%hold on
%plot(res_data1, 'b');
%plot(res_data2, 'r');
%hold off

figure;
plot(cur_data);
