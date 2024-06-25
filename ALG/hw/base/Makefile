HW=main
ZIP=zip
UNZIP = unzip
DATAPUB = datapub

$(HW): main.cpp 
	$(CXX) main.cpp -o $(HW)

zip:
	$(ZIP) $(HW)-brute.zip main.cpp

unzip_datapub:
	mkdir $(DATAPUB)
	$(UNZIP) $(DATAPUB).zip -d $(DATAPUB)

clean:
	$(RM) -f *.o
	$(RM) -f $(HW)
	$(RM) -f $(HW)-brute.zip
	$(RM) -rf datapub_output

end:
	$(RM) -f *.o
	$(RM) -f $(HW)
	$(RM) -f $(HW)-brute.zip
	$(RM) -rf datapub_output
	$(RM) -rf datapub
	$(RM) start.sh
	$(RM) prepare.sh
	$(RM) test.sh
	$(RM) Makefile

.PHONY: clean zip
