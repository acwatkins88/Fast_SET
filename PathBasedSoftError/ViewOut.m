clc; clear all; close all;

addpath('/home/adam/Fast_SET/PathBasedSoftError');
% addpath('/home/acwatkins88/Fast_SET/PathBasedSoftError')

inp_data1 = dlmread('OutputRes0.out');
inp_data2 = dlmread('OutputRes1.out');
res_data1 = dlmread('OutputRes2.out');
res_data2 = dlmread('OutputRes3.out');
%cur_data = dlmread('CurOut.out');

figure;
hold on
plot(inp_data1, 'b');
plot(inp_data2, 'r');
hold off

figure;
hold on
plot(res_data1, 'b');
plot(res_data2, 'r');
hold off

%figure;
%plot(cur_data);
