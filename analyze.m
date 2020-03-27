function void = analyze()
	waitingTime = importdata("waitLog");
	queueSize 	= importdata("queueLog");
	waitMean = mean(waitingTime)
	waitStd = std(waitingTime)
	queueMean = mean(queueSize)
	hist(waitingTime,20);
	title("Waiting time");
	xlabel("Time in microseconds");
	ylabel("Number of workFunctions");
	print("waiting","-dsvg");
	
	grid on;
	plot(queueSize);
	title("Length of queue");
	xlabel("Time");
	ylabel("workFunctions in queue");
	print("queue","-dsvg");
end
