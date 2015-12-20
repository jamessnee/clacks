all:
	protoc-c --c_out=. TraceMessage.proto

install:
	mv *.h ../../include/
	mkdir ../../clacksd/src/transport-server/generated/
	cp *.c ../../clacksd/src/transport-server/generated/
	mkdir ../../lib/src/transport-client/generated/
	cp *.c ../../lib/src/transport-client/generated/
	rm *.c
