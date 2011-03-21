mmm: mmm.o
	g++ -o mmm mmm.o
	
trialBR: trialBR.o Region.o
	g++ -g -o trialBR trialBR.o Region.o

trialBR.o: trialBR.cpp Region.h
	g++ -g -c trialBR.cpp

Region.o: Region.cpp Region.h util.h
	g++ -g -c Region.cpp

.PHONY: reset
reset:
	rm file.dat
	touch file.dat
	cp ../samples/world/region/r.0.0.mcr .
