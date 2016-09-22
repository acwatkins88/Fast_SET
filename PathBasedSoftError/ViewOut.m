clc; clear all; close all;

%addpath('/home/adam/Fast_SET/PathBasedSoftError');
addpath('/home/acwatkins88/Fast_SET/PathBasedSoftError')

inp_data = dlmread('OutputRes.out');
cur_data = dlmread('CurOut.out');

figure;
plot(inp_data);

figure;
plot(cur_data);
