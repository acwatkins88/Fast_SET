function [timeout] = postotime(position, tstep, endtime)
	indconv = endtime/tstep;
	effparam = position/indconv;
	timeout = effparam*endtime;
end
