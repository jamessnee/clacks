all:
	protoc-c --c_out=. TraceMessage.proto

install:
	mv *.h ../../include/
	cp *.c ../../clacksd/src/transport-server/generated
	cp *.c ../../lib/src/transport-client/generated
	rm *.c
