% Beginning of Pulse Approximation Method
clear all;
clc;
addpath('/ugrad/watkins/Desktop/Research/Weibull Approx/HspiceToolbox');
addpath('/ugrad/watkins/Desktop/Research/Weibull Approx/Single Weibull');
addpath('/ugrad/watkins/Desktop/Research/Weibull Approx/Single Weibull/delays');
x = loadsig('singleapprox.tr0');
%drdelay = dlmread('delays/doublerisinga.csv', ',');
%dfdelay = dlmread('delays/doublefallinga.csv', ',');
tstep = .5e-12;
endtime = 1199.5e-12;
numpoints = 30;
for p = 1:size(x)
	printf('%d. ', p);
	printf('Plot: %s\n', x(p).name);
	if strcmp(x(p).name, 'vdd')
		vddval = x(p).data(1)
	endif
endfor

val = input("Enter number to plot data: ");
%appmethod = input("Enter approximation type: 0 for proposed, 1 for published: ")
appmethod = 0;
index = 1;
%time1=0:tstep:endtime-2*tstep;
time1=0:tstep:endtime;
ftime = endtime-2*tstep;
inptime = zeros(1, 1);
volt = zeros(1, 1);

indconv = endtime/tstep;
[c, peakpos] = max(x(val).data)
[minval, minpos] = min(x(val).data)
if minval < 0
	minval = 0;
endif

% rf == 0 => rising
% rf == 1 => falling
% transtype == 0 => low-high-low
% transtype == 1 => high-low-high
[transtype] = dectrans(c, x, val);

errorval = 0;
errcount = 0;

%if appmethod == 0
	% Low-High-Low
	if transtype == 0 
		[inptime volt selpeak shiftparam maxvolt1 maxvolt2] = selectpointsnew(tstep, endtime, x, val, peakpos, c, inptime, volt, numpoints, transtype, c);
		effparam = peakpos/indconv;
		cdf = effparam*endtime;
		cdf 
		c
		rf = 0;
		% Rising Transition
		[a1, b1] = linreg(shiftparam, inptime, volt, selpeak, c, rf, transtype, numpoints, minval)
		%a1 = 1.6554
		%b1 = 3.10991e-11
		
		rf = 1;
		% Falling Transition
		[a2, b2] = linreg(cdf, inptime, volt, selpeak, c, rf, transtype, numpoints, minval)
		%a2 = 1.7254
		%b2 = 1.7287e-11
		%shiftparam = 4.722e-10
		%cdf = 6.079e-10
		
		if c > vddval
			c = vddval;
		endif	
		
		for t2 = time1
		    if t2 < cdf && t2 >= shiftparam
		        voltage(index) = c*(1-exp(-((t2-shiftparam)/b1)^a1));
		        if(x(val).data(index) > .2)
					temperr = abs((voltage(index) - x(val).data(index))/x(val).data(index));
					errorval = errorval + temperr;
					errcount++;
			   	endif
		    elseif t2 < shiftparam
		        voltage(index) = 0;    
		    elseif t2 == cdf
		    	voltage(index) = voltage(index-1);
		    elseif t2 > cdf
		        voltage(index) = (c)*(exp(-((t2-(cdf))/b2)^(a2))); 
		        if(x(val).data(index) > .2)
					temperr = abs((voltage(index) - x(val).data(index))/x(val).data(index));
					errorval = errorval + temperr;
					errcount++;
			   	endif
		    endif
		    index = index + 1;
		endfor
		errout = errorval/errcount
	endif

	% High-Low-High
	if transtype == 1
		[inptime volt selpeak shiftparam maxvolt1 maxvolt2] = selectpointsnew(tstep, endtime, x, val, minpos, minval, inptime, volt, numpoints, transtype, c);
		effparam = minpos/indconv;
		cdf = effparam*endtime;
		cdf
		c = vddval - minval;
		minval
		transtype = 0;
		rf = 0;
		% Falling Transition
		[a1, b1] = linreg(shiftparam, inptime, volt, selpeak, c, rf, transtype, numpoints, minval)
		a1 = 1.273
		% Middleq
		%b1 = 3.39e-11
		% Out
		b1 = 2.09386e-11
		rf = 1;
		% Rising Transition
		[a2, b2] = linreg(cdf, inptime, volt, selpeak, maxvolt2, rf, transtype, numpoints, minval)
		a2 = 2.3709
		% Middle
		%b2 = 6.1446e-11
		% Out
		b2 = 2.9e-11
		%shiftparam = 3.12884e-10
		%cdf = 5.10e-10
		minval = .26083;
		c = vddval - minval;
		
		for t2 = time1
		    if t2 < cdf && t2 >= shiftparam
		        voltage(index) = (c*(exp(-((t2-shiftparam)/b1)^a1)))+minval;
		        if(x(val).data(index) < .8) && (x(val).data(index)>= ((.2*vddval) + minval));
				    temperr = abs((voltage(index) - x(val).data(index))/x(val).data(index));
				    errorval = errorval + temperr;
				    errcount++;
				endif
		    elseif t2 < shiftparam
		        voltage(index) = vddval;
		    elseif t2 == cdf
		    	voltage(index) = voltage(index-1);
		    elseif t2 > cdf
		        voltage(index) = ((c)*(1-exp(-((t2-cdf)/b2)^a2)))+minval;
		        if(x(val).data(index) < .8) && (x(val).data(index)>= ((.2*vddval) + minval));
				   	temperr = abs((voltage(index) - x(val).data(index))/x(val).data(index));
				   	errorval = errorval + temperr;
					errcount++;
			   	endif
		    endif
		    index = index + 1;
		endfor
		errout = errorval/errcount
	endif

% Published Approximation Method
%elseif appmethod == 1

%	if transtype == 0
%		scval = .02*c;
%	elseif transtype == 1
%		scval = .95*vddval;
%	endif

%	%decide shiftparam
%	for t = 1:size(x(val).data)
%		if transtype == 0
%			if scval <= x(val).data(t)
%				startpos = t;
%				break;
%			endif
%		elseif transtype == 1
%			if scval >= x(val).data(t)
%				startpos = t;
%				break;
%			endif
%		endif	
%	endfor
%	
%	effparam = startpos/indconv;
%	shiftparam = effparam*endtime
%	
%	if transtype == 0
%		for tsw = 1:size(x(val).data)
%			inpvolt(tsw) = x(val).data(tsw);
%		endfor
%		
%		for t1 = 1:peakpos
%			pinpvolt(t1) = inpvolt(t1);
%			ptime1(t1) = time1(t1);
%		endfor
%		
%		
%		vddval = 2
%		% Trapezoidal
%		if c >= vddval
%			errout = 1;
%			c = 2*c;
%			for a = 2:.5:6
%				for b = 20e-12:5e-12:200e-12
%					errcount = 0;
%					errorval = 0;
%					index = 1;
%					for t1 = time1
%						if t1 >= shiftparam
%							b1 = ((a-1)/a)^((1-a)/a);
%							b2 = ((t1-shiftparam)/b)^(a-1);
%							b3 = exp(-((((t1-shiftparam)/b)^a) - ((a-1)/a)));
%							voltage = c*b1*b2*b3;
%							if voltage > 1
%								voltage = 1;
%							endif
%							if((x(val).data(index) > .2))
%							   temperr = abs((voltage - x(val).data(index))/x(val).data(index));
%							   errorval = errorval + temperr;
%							   errcount++;
%							endif
%						endif
%						index++;
%					endfor
%					errcalc = errorval/errcount;
%					if errcalc < errout
%						alpha = a;
%						beta = b;
%						errout = errcalc;
%					endif
%				endfor
%			endfor
%			
%			a = alpha;
%			b = beta;
%			
%		% Normal Pulse
%		else
%			% Calculate Areas
%			areaf = trapz(time1, inpvolt)
%			areap = trapz(ptime1, pinpvolt)
%	
%			% Calculate a parameter (shape)
%			P = areap/areaf;
%			
%			a = 1/(1+log(1-P))
%			
%			% Calculate b parameter (width)
%			b = ((areaf*a)/c)*((a-1)/a)^((a-1)/a)*exp((1-a)/a)
%				
%		endif
%		
%		%shiftparam = 245e-12;
%		index = 1;
%		errorval = 0;
%		errcount = 0;
%		% Calculate voltage vector
%		for t1 = time1
%			if t1 < shiftparam
%				voltage(index) = 0;
%			else
%				b1 = ((a-1)/a)^((1-a)/a);
%				b2 = ((t1-shiftparam)/b)^(a-1);
%				b3 = exp(-((((t1-shiftparam)/b)^a) - ((a-1)/a)));
%				voltage(index) = c*b1*b2*b3;
%%				if voltage(index) > 1
%%					voltage(index) = 1;
%%				endif
%				if((x(val).data(index) > .2) && (x(val).data(index) <= .9))
%				   temperr = abs((voltage(index) - x(val).data(index))/x(val).data(index));
%				   errorval = errorval + temperr;
%				   errcount++;
%				endif
%			endif
%			index++;
%		endfor
%		errout = errorval/errcount
%		
%	elseif transtype == 1
%%		for tsw = 1:size(x(val).data)
%%			tempvoltar(tsw) = vddval-x(val).data(tsw);
%%		endfor
%%		
%%		iv = size(tempvoltar,2);
%%		for tsw = 1:size(tempvoltar,2)
%%			inpvolt(tsw) = tempvoltar(iv);
%%			iv--;
%%		endfor
%		
%		for tsw = 1:size(x(val).data)
%			inpvolt(tsw) = vddval-x(val).data(tsw);
%		endfor
%		
%		for t1 = 1:size(x(val).data) - minpos
%			pinpvolt(t1) = inpvolt(t1);
%			ptime1(t1) = time1(t1);
%		endfor
%		
%		[c, peakpos] = max(inpvolt);
%		c
%		
%		% Trapezoidal
%		if minval <= .5
%%			b = input("Enter Beta Value: ");
%%			
%%			areaf = trapz(time1, inpvolt)
%%			areap = trapz(ptime1, pinpvolt)
%%		
%%			P = areap/areaf;
%%			a = -1/(1+log(1-P))
%%			c = 2*c
%			
%			errout = 1;
%			c = 2*(vddval-minval);
%			for a = 2.5:.25:3.5
%				for b = 20e-12:5e-12:200e-12
%					errcount = 0;
%					errorval = 0;
%					index = 1;
%					for t1 = time1
%						if t1 >= shiftparam
%							b1 = ((a-1)/a)^((1-a)/a);
%							b2 = ((t1-shiftparam)/b)^(a-1);
%							b3 = exp(-((((t1-shiftparam)/b)^a) - ((a-1)/a)));
%							voltage = c*b1*b2*b3;
%							if voltage < 0
%								voltage = 0;
%							endif
%							if((x(val).data(index) <= .8) && (x(val).data(index) >= .2))
%							   temperr = abs((voltage - inpvolt(index))/inpvolt(index));
%							   errorval = errorval + temperr;
%							   errcount++;
%							endif
%						endif
%						index++;
%					endfor
%					errcalc = errorval/errcount;
%					if errcalc < errout
%						alpha = a;
%						beta = b;
%						errout = errcalc;
%					endif
%				endfor
%			endfor
%			
%			a = alpha
%			b = beta
%			
%		% Normal Pulse
%		else
%			% Calculate Areas
%			areaf = trapz(time1, inpvolt)
%			areap = trapz(ptime1, pinpvolt)
%	
%			% Calculate a parameter (shape)
%			P = areap/areaf
%			%if P > .5
%			%	P = .45
%			%endif 
%			a = 1/(1+log(1-P)) 
%			
%			% Calculate b parameter (width)
%			b = ((areaf*a)/c)*((a-1)/a)^((a-1)/a)*exp((1-a)/a)
%	
%		endif
%		
%		errorval = 0;
%		errcount = 0;
%		index = 1;
%		% Calculate voltage vector
%		for t1 = time1
%			if t1 < shiftparam
%				voltage(index) = 1;
%			else
%				b1 = ((a-1)/a)^((1-a)/a);
%				b2 = ((t1-shiftparam)/b)^(a-1);
%				b3 = ((t1-shiftparam)/b)^a;
%				b4 = (a-1)/a;
%				b5 = exp(-(b3 - b4));
%				volt = -c*(b1*b2*b5) + vddval;
%				if volt <= 0
%					voltage(index) = 0;
%				else
%					voltage(index) = volt;
%				endif
%				if((x(val).data(index) <= .8) && (x(val).data(index) >= .2))
%				   temperr = abs((voltage(index) - x(val).data(index))/x(val).data(index));
%				   errorval = errorval + temperr;
%				   errcount++;
%				endif
%			endif
%			index++;
%		endfor
%		errout = errorval/errcount
%	endif
%elseif appmethod > 1
%	printf("Invalid Method Number\n");
%endif

figure(1);
% clf;
% newplot;
hold on
%plot(time1, inpvolt, 'm');
plot(time1, voltage, '-');
plot(time1, x(val).data, 'r');
xlabel 'Time';
ylabel 'Voltage';
title 'Voltage vs Time';
xlim([250e-12 700e-12]);
%xlim([400e-12 700e-12]);
legend('Model', 'Original');
print('figout.png'); 
print(2, 'figout2.png');
print(3, 'figout3.png');  
hold off
