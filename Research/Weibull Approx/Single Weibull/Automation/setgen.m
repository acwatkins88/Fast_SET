% Generate 2 PWL pulses
clear;
close all;
clc;
% Edit These Values------------------------------------------
% Technology Dependent Constant
tau = 90e-12;

% 65nm NAND Falling Input
% st_charge = 2.5;
% end_charge = 5.5;
% itr = 0.5;
% max_sep = 150e-12;

% 65nm NAND Rising Input
% st_charge = 8;
% end_charge = 16;
% itr = 2;
% max_sep = 70e-15;

% 65nm NOR Falling Input
% st_charge = 3.5;
% end_charge = 6.5;
% itr = 0.5;
% max_sep = 150e-12;

% 65nm NOR Rising Input
st_charge = 8;
end_charge = 12;
itr = 1;
max_sep =150e-15;

% Start Time of First Pulse
starttime = 250e-12;
% Start Time of Second Pulse Relative to First
% separation = 95e-12;
% Output file Resolution
tstep = 0.5e-12;
% End Time of Output File
endtime = 1200e-12;
% -----------------------------------------------------------
%Q1 = input("Enter Pulse 1 Charge Value: ");
%Q2 = input("Enter Pulse 2 Charge Value: ");

f_count = 1;
for Q1 = st_charge:itr:end_charge
    Q1 = Q1*1e-15;
    for Q2 = st_charge:itr:end_charge
        separation = max_sep*rand(1)+30e-12;
        Q2 = Q2*1e-15;
        index1 = 1;
        index2 = 1;
        current = zeros(1, 601);
        current2 = zeros(1, 601);
        qtime1 = 0;
        qtime2 = 0;
        filename = ['pwlpulse', num2str(f_count), '.dat'];
        outfile = fopen(filename, 'w+'); 

        for t = 0:tstep:endtime
            if t<starttime && t < starttime + separation
                fprintf(outfile, '%e ', t);
                fprintf(outfile, '%d ', 0);
                fprintf(outfile, '%d\n', 0);
            elseif t<starttime && t > starttime+separation
                fprintf(outfile, '%e ', t);
                fprintf(outfile, '%d ', 0);
                I2 = ((2*Q2)/(tau*(pi)^(.5)))*((qtime2/tau)^(.5))*exp(-qtime2/tau);
                qtime2 = qtime2 + tstep;
                current2(index2) = I2;
                index2 = index2 + 1;
                fprintf(outfile, '%e\n', I2);
            elseif t>starttime && t<starttime+separation
                I1 = ((2*Q1)/(tau*(pi)^(.5)))*((qtime1/tau)^(.5))*exp(-qtime1/tau);
                qtime1 = qtime1 + tstep;
                current(index1) = I1;
                index1 = index1 + 1;
                fprintf(outfile, '%e ', t);
                fprintf(outfile, '%e ', I1);
                fprintf(outfile, '%d\n', 0);
            elseif t>starttime && t>starttime+separation
                I1 = ((2*Q1)/(tau*(pi)^(.5)))*((qtime1/tau)^(.5))*exp(-qtime1/tau);
                I2 = ((2*Q2)/(tau*(pi)^(.5)))*((qtime2/tau)^(.5))*exp(-qtime2/tau);
                qtime1 = qtime1 + tstep;
                qtime2 = qtime2 + tstep;
                current(index1) = I1;
                index1 = index1 + 1;
                current2(index2) = I1;
                index2 = index2 + 1;
                fprintf(outfile, '%e ', t);
                fprintf(outfile, '%e ', I1);
                fprintf(outfile, '%e\n', I2);
            end
        end
        time1 = 0:tstep:endtime-starttime-2*tstep;
        time2 = 0:tstep:endtime-starttime-separation-tstep;
        fclose(outfile);
        f_count = f_count + 1;
    end
end  

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
