% Generate 2 PWL pulses
clear;
close all;
% Edit These Values------------------------------------------
% Technology Dependent Constant
% tau = 90e-12;
tau = 32e-12;

% 32nm Test

% NAND 2, 3 - Falling 1
% Q1 = 5.75e-15;
% Q2 = 6.75e-15;

% NAND 2, 3 - Falling 2
% Q1 = 6.75e-15;
% Q2 = 6.35e-15;

% NAND 2, 3 - Falling 3
Q1 =5e-15;
Q2 = 5e-15;

% NAND 2, 3 - Falling 4
% Q1 = 7.00e-15;
% Q2 = 5.95e-15;

% NAND 2, 3 - Rising 1
% Q1 = 16.50e-15;
% Q2 = 14.50e-15;

% NAND 2, 3 - Rising 2
% Q1 = 14.45e-15;
% Q2 = 16.55e-15;

% NAND 2, 3 - Rising 3
% Q1 = 17.50e-15;
% Q2 = 13.50e-15;

% NAND 2, 3 - Rising 4
% Q1 = 16.75e-15;
% Q2 = 15.50e-15;

% NOR2 - Rising
% Q1 = 10.50e-15;
% Q2 = 10.75e-15;

% NOR2 - Falling
% Q1 = 6.75e-15;
% Q2 = 7.5e-15;

% Falling Results
% Q1 = 14.5e-15;
% Q2 = 13.5e-15;

% Start Time of First Pulse
% starttime = 50e-12;
starttime = 550e-12;
starttime2 = starttime+1000e-12;

% Start Time of Second Pulse Relative to First
separation = 50e-12;
% separation = 0;

% Output file Resolution
tstep = 0.5e-12;
% tstep = 200e-12;
% tstep = 100e-12;

% End Time of Output File
endtime = 1200e-12;
% endtime = 3000e-12;
% endtime = 200e-9;
% -----------------------------------------------------------
%Q1 = input("Enter Pulse 1 Charge Value: ");
%Q2 = input("Enter Pulse 2 Charge Value: ");
index1 = 1;
index2 = 1;
current = zeros(1, 601);
current2 = zeros(1, 601);
qtime1 = 0;
qtime2 = 0;
qtime3 = 0;
qtime4 = 0;
outfile = fopen('pwlpulse.dat', 'w+'); 

err_min = 1e-25;
 	
for t = 0:tstep:endtime
    if t<starttime && t < starttime + separation
        fprintf(outfile, '%e ', t);
        fprintf(outfile, '%d ', 0);
        fprintf(outfile, '%d ', 0);
        fprintf(outfile, '%d ', 0);
        fprintf(outfile, '%d\n', 0);
        
    elseif t<starttime && t > starttime+separation
        fprintf(outfile, '%e ', t);
        fprintf(outfile, '%d ', 0);
        I2 = ((2*Q2)/(tau*(pi)^(.5)))*((qtime2/tau)^(.5))*exp(-qtime2/tau);
        qtime2 = qtime2 + tstep;
        current2(index2) = I2;
        index2 = index2 + 1;
        
        if I2 < err_min
            I2 = err_min;     
        end
        
        fprintf(outfile, '%e ', I2);
        fprintf(outfile, '%d ', 0);
        fprintf(outfile, '%d\n', 0);
        
    elseif t>starttime && t<starttime+separation
        fprintf(outfile, '%e ', t);
        I1 = ((2*Q1)/(tau*(pi)^(.5)))*((qtime1/tau)^(.5))*exp(-qtime1/tau);
        qtime1 = qtime1 + tstep;
        current(index1) = I1;
        index1 = index1 + 1;
        
        if I1 < err_min
            I1 = err_min;
        end
        
        fprintf(outfile, '%e ', I1);
        fprintf(outfile, '%d ', 0);
        fprintf(outfile, '%d ', 0);
        fprintf(outfile, '%d\n', 0);
        
    elseif t>starttime && t>starttime+separation && t<starttime2
        I1 = ((2*Q1)/(tau*(pi)^(.5)))*((qtime1/tau)^(.5))*exp(-qtime1/tau);
        I2 = ((2*Q2)/(tau*(pi)^(.5)))*((qtime2/tau)^(.5))*exp(-qtime2/tau);
        qtime1 = qtime1 + tstep;
        qtime2 = qtime2 + tstep;
        current(index1) = I1;
        index1 = index1 + 1;
        current2(index2) = I1;
        index2 = index2 + 1;
        fprintf(outfile, '%e ', t);
        
        if I1 < err_min
            I1 = err_min;
        end
        
        if I2 < err_min
            I2 = err_min;
        end
        
        fprintf(outfile, '%e ', I1);
        fprintf(outfile, '%e ', I2);
        fprintf(outfile, '%d ', 0);
        fprintf(outfile, '%d\n', 0);
        
    elseif t<starttime2 && t > starttime2+separation
        fprintf(outfile, '%e ', t);
        fprintf(outfile, '%d ', 0);
        fprintf(outfile, '%d ', 0);
        fprintf(outfile, '%d ', 0);
        I2 = ((2*Q2)/(tau*(pi)^(.5)))*((qtime4/tau)^(.5))*exp(-qtime4/tau);
        qtime4 = qtime4 + tstep;
        current2(index2) = I2;
        index2 = index2 + 1;
        
        if I2 < err_min
            I2 = err_min;     
        end
        
        fprintf(outfile, '%e\n', I2);
        
    elseif t>starttime2 && t<starttime2+separation
        fprintf(outfile, '%e ', t);
        fprintf(outfile, '%d ', 0);
        fprintf(outfile, '%d ', 0);
        I1 = ((2*Q1)/(tau*(pi)^(.5)))*((qtime3/tau)^(.5))*exp(-qtime3/tau);
        qtime3 = qtime3 + tstep;
        current(index1) = I1;
        index1 = index1 + 1;
        
        if I1 < err_min
            I1 = err_min;
        end
        
        fprintf(outfile, '%e ', I1);
        fprintf(outfile, '%d\n', 0);
        
    elseif t>starttime2 && t>starttime2+separation
        I1 = ((2*Q1)/(tau*(pi)^(.5)))*((qtime3/tau)^(.5))*exp(-qtime3/tau);
        I2 = ((2*Q2)/(tau*(pi)^(.5)))*((qtime4/tau)^(.5))*exp(-qtime4/tau);
        qtime3 = qtime3 + tstep;
        qtime4 = qtime4 + tstep;
        current(index1) = I1;
        index1 = index1 + 1;
        current2(index2) = I1;
        index2 = index2 + 1;
        fprintf(outfile, '%e ', t);
        fprintf(outfile, '%d ', 0);
        fprintf(outfile, '%d ', 0);
        
        if I1 < err_min
            I1 = err_min;
        end
        
        if I2 < err_min
            I2 = err_min;
        end
        
        fprintf(outfile, '%e ', I1);
        fprintf(outfile, '%e\n', I2);
    end
end
time1 = 0:tstep:endtime-starttime-2*tstep;
time2 = 0:tstep:endtime-starttime-separation-tstep;
fclose(outfile);

% figure(1)
% subplot(1, 2, 1);
% plot(time1, current);
% xlabel 'Time';
% ylabel 'Current';
% title 'Charge vs Current Model';
% subplot(1, 2, 2);
% plot(time2, current2);
% xlabel 'Time';
% ylabel 'Current';
