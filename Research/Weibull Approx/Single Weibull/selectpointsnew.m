function [inptime voltage selpeak shiftparam maxvolt1 maxvolt2] = selectpointsnew(tstep, endtime, dataarr, val, peakpos, peakval, inptime, voltage, numpoints, transtype, vddval)
% transtype == 0 => low-high-low
% transtype == 1 => high-low-high
if transtype == 0
	scval = .05*peakval;
elseif transtype == 1
	scval = .95*vddval;
endif

%decide shiftparam
for t = 1:size(dataarr(val).data)
	if transtype == 0
		if scval <= dataarr(val).data(t)
			startpos = t;
			break;
		endif
	elseif transtype == 1
		if scval >= dataarr(val).data(t)
			startpos = t;
			break;
		endif
	endif
endfor

% convert position to time
[shiftparam] = postotime(startpos, tstep, endtime);
printf("Shiftparam = %e\n", shiftparam);

testval = .632*peakval
testval2 = peakval/2;
for t = 1:size(dataarr(val).data)-1
	if (dataarr(val).data(t) < testval) && (dataarr(val).data(t+1) > testval) || (dataarr(val).data(t) == testval)
		[beta1] = postotime(t, tstep, endtime);
		beta = beta1 - shiftparam;
	endif
	if (dataarr(val).data(t) < testval2) && (dataarr(val).data(t+1) > testval2) || (dataarr(val).data(t) == testval2)
		[timeval1] = postotime(t, tstep, endtime);
		timeval = timeval1 - shiftparam;
		voltval = dataarr(val).data(t);
	endif
endfor

%alpha = (log(log(peakval/(peakval-voltval))))/(log(timeval)-log(beta));
%printf("Calculated Alpha: %e\n", alpha);

% generate a set of points, must be updated for multiple pulses
index = 1;
if transtype == 0
	p1 = .2*peakval;
	p2 = .9*peakval;
elseif transtype == 1
	p1 = .8*vddval;
	diffval = abs(vddval - peakval);
	multval = diffval*.8;
	p2 =  vddval - multval;
endif

for i = 1:(size(dataarr(val).data)-1)
	if (p1 > dataarr(val).data(i)) && (p1 < dataarr(val).data(i+1)) || (p1 == dataarr(val).data(i))
		ps(index) = i;
		index++;
	endif
	if (p1 < dataarr(val).data(i)) && (p1 > dataarr(val).data(i+1)) || (p1 == dataarr(val).data(i))
	   	ps(index) = i;
	   	index++;
	endif
	if (p2 > dataarr(val).data(i)) && (p2 < dataarr(val).data(i+1)) || (p2 == dataarr(val).data(i))
	    ps(index) = i;
	    index++;
	endif
	if (p2 < dataarr(val).data(i)) && (p2 > dataarr(val).data(i+1)) || (p2 == dataarr(val).data(i))
    	ps(index) = i;
    	index++;
    endif
    if peakpos == i
    	ps(index) = i;
    	index++;
	endif
endfor

for i = 1:size(ps, 2)-1
	rangear(i) = abs(ps(i) - ps(i+1));
endfor  

num3 = numpoints/6;
num6 = numpoints/3;

for i = 1:size(rangear, 2)
	if i == 1
		ppos(i) = ceil(rangear(i)/num6);
	elseif i == 2
		ppos(i) = ceil(rangear(i)/num3);
	elseif i == 3
		ppos(i) = ceil(rangear(i)/num3);
	elseif i == 4
		ppos(i) = ceil(rangear(i)/num6);
	endif
endfor

pcount = 0;
vindex = 1;
section = 1;
boundcount = 2;
tindex = 1;
tparam = 0;
limitval = .9*peakval;
limitval2 = .1*vddval + peakval;
for i = 1:numpoints+2
	pindex = ps(section) + pcount;
	if ((pindex < peakpos) && (pindex + ppos(section) > peakpos))||(pindex == peakpos) 
		selpeak = vindex;
	else
		if transtype == 0
			if dataarr(val).data(pindex) < limitval
				voltage(vindex) = dataarr(val).data(pindex);
				tparam = 1;
			endif
		elseif transtype == 1
			voltage(vindex) = vddval - dataarr(val).data(pindex);
			tparam = 1;
		endif
		if tparam == 1
			[timeparam] = postotime(pindex, tstep, endtime);  
			inptime(vindex) = timeparam;
			vindex++;
			tparam = 0;
		endif
	endif
	
	if pindex > ps(boundcount)
		pcount = 0;
		psec(tindex) = vindex;
		tindex++;
		boundcount++;
		section++;
		if section > size(ppos, 2)
			break;
		endif		
	endif
	pcount = pcount + ppos(section);
endfor    

figure(2);
for t = 1:selpeak-1
	testtime(t) = inptime(t);
	testvolt(t) = voltage(t);
endfor
index = 1;
for t = selpeak:size(voltage, 2)
	testtime2(index) = inptime(t);
	testvolt2(index) = voltage(t);
	index++;
endfor
maxvolt1 = max(testvolt)
maxvolt2 = max(testvolt2)
newplot;
hold on;
plot(testtime, testvolt, "+", "linewidth", 5);
plot(testtime2, testvolt2, "+r", "linewidth", 5); 
hold off;
