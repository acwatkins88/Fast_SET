function [transtype] = dectrans(peakval, dataarr, val)
	avsum = 0;
	%decide transtype
	for t = 1:5
	   avsum = avsum + dataarr(val).data(t);
	endfor

	average = avsum/5;
	if average <= .05
		transtype = 0;
	else
		transtype = 1;
	endif
end
