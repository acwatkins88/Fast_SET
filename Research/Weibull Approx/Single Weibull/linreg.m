function [a, b] = linreg(shiftparam, inptime, voltage, selpeak, peakval, rf, transtype, numpoints, minval)
% rf == 0 => rising
% rf == 1 => falling
% transtype == 0 => low-high-low
% transtype == 1 => high-low-high
if transtype == 0 && rf == 0
    starval = 1;
    endval = selpeak-1;
elseif transtype == 0 && rf == 1 
    starval = selpeak;
    endval = size(voltage, 2);
elseif transtype == 1 && rf == 0
    starval = selpeak;
    endval = size(voltage, 2);
elseif transtype == 1 && rf == 1
    starval = 1;
    endval = selpeak-1;
endif

xsum = 0;
xsq = 0;
ysum = 0;
ysq = 0;
xysum = 0;
iternum = 0;
order = 10;
coef = polyfit(inptime, voltage, order);
for t = 1:size(inptime, 2)
	coeftest(t) = polyval(coef, inptime(t));
endfor
figure(3);
newplot;
plot(inptime, coeftest); 
vderiv = polyderiv(coef);

for t = starval:endval
	vd = polyval(vderiv, inptime(t));
    v = voltage(t);
    z = inptime(t);
    if peakval != v
    	if (rf == 0) && (transtype == 0) && (z != shiftparam) && (peakval != v)
			x = log(log(abs(peakval/(peakval-v))));	
			y = log(abs(z-shiftparam));
			iternum++;
			xsum = xsum + x;
			ysum = ysum + y;
		elseif (rf == 1) && (transtype == 1) && (z!= shiftparam) && (peakval != v)
			x = log(log(peakval/(v)));
			y = log(abs(z-shiftparam));
			iternum++;
			xsum = xsum + x;
			ysum = ysum + y;
		elseif (rf == 0) && (transtype == 1) && (z!= shiftparam) && (peakval != v)
			x = log(log(abs(peakval/(peakval-v))));	
			y = log(abs(z-shiftparam));
			iternum++;
			xsum = xsum + x;
			ysum = ysum + y;
		elseif (rf == 1) && (transtype == 0) && (z!= shiftparam) && (peakval != v)
			x = log(log(peakval/(v)));
			y = log(abs(z-shiftparam));
			iternum++;
			xsum = xsum + x;
			ysum = ysum + y;
		endif
    endif
endfor

ymean = ysum/iternum;
xmean = xsum/iternum;
sxy = 0;
sxx = 0;

for t = starval:endval
	vd = polyval(vderiv, inptime(t));
    v = voltage(t);
    z = inptime(t);
    if peakval != v
		if (rf == 0) && (transtype == 0) && (shiftparam != z) && (peakval != v)
			x = log(log(abs(peakval/(peakval-v))));		
			y = log(abs(z-shiftparam));
		elseif (rf == 1) && (transtype == 1) && (shiftparam != z) && (peakval != v)
			x = log(log(peakval/(v)));
			y = log(abs(z-shiftparam));
		elseif (rf == 0) && (transtype == 1) && (z!= shiftparam) && (peakval != v)
			x = log(log(abs(peakval/(peakval-v))));	
			y = log(abs(z-shiftparam));
		elseif (rf == 1) && (transtype == 0) && (z!= shiftparam) && (peakval != v)
			x = log(log(peakval/(v)));
			y = log(abs(z-shiftparam));
		endif
		
		diffmult = (x-xmean)*(y-ymean);
		sxy = sxy + diffmult;
		xdiffsq = (x-xmean)^2;
		sxx = sxx + xdiffsq;
    endif
endfor

m = sxy/sxx;
a = 1/m;
inter = ymean - (xmean)*m;
b = exp(inter); 
end
