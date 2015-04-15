  CFLAGS = -I ../../include/
    HEADPATH =../../include/
    MYSQL=-L /usr/include/mysql

    helloIDS: init.o sqlToCache.o dataCache.o process.o detect_acvl.o pcapToCache.o
	g++  -o helloworld  init.o sqlToCache.o pcapToCache.o dataCache.o process.o detect_acvl.o -lpthread -L/usr/lib/mysql -lmysqlclient -lz `pkg-config --cflags --libs gtk+-2.0`


    init.o: init.cpp $(HEADPATH)sniffer_sqlToCache.h $(HEADPATH)decoder_dataCache.h $(HEADPATH)preprocessor_process.h $(HEADPATH)detect_acvl.h
	g++ -c $(CFLAGS) init.cpp -lpthread `pkg-config --cflags --libs gtk+-2.0`

    pcapToCache.o: ../Sniffer/pcapToCache.cpp $(HEADPATH)decoder_dataCache.h
	g++ -c  $(CFLAGS) -I/usr/include/mysql -L/usr/lib/mysql -lmysqlclient -lz $(mysql_config --cflags) ../Sniffer/sqlToCache.cpp $(mysql_config --libs) -lpthread -lpcap

    sqlToCache.o: ../Sniffer/sqlToCache.cpp $(HEADPATH)decoder_dataCache.h
	g++ -c  $(CFLAGS) -I/usr/include/mysql -L/usr/lib/mysql -lmysqlclient -lz $(mysql_config --cflags) ../Sniffer/sqlToCache.cpp $(mysql_config --libs) -lpthread 


    dataCache.o: ../Decoder/dataCache.cpp $(HEADPATH)decoder_dataCache.h
	g++ -c $(CFLAGS) ../Decoder/dataCache.cpp


    process.o: ../Preprocessor/process.cpp $(HEADPATH)preprocessor_process.h $(HEADPATH)detect_acvl.h
	g++ -c $(CFLAGS) ../Preprocessor/process.cpp


    detect_acvl.o: ../DetectionEngine/detect_acvl.cpp $(HEADPATH)detect_acvl.h
	g++ -c $(CFLAGS) -I/usr/include/mysql -L/usr/lib/mysql -lmysqlclient  -lz  $(mysql_config --cflags) ../DetectionEngine/detect_acvl.cpp $(mysql_config --libs) 


    clean :
	rm *.o
