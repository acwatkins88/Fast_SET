clc; clear all; close all;

addpath('/home/adam/Fast_SET/PathBasedSoftError');

inp_data = dlmread('OutputRes.out');

figure;
plot(inp_data);