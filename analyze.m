function void = analyze(ms)
	A = importdata('producerTimeLog.txt').data;
	B = importdata('consumerTimeLog.txt').data;
	C = importdata(['drift' num2str(ms) 'msLog.txt']).data(2:end);
	metrics(A);
	metrics(B);
	metrics(C);
end

function nothing = metrics(data)
	min(data)
	max(data)
	mean(data)
	median(data)
	std(data)
end


