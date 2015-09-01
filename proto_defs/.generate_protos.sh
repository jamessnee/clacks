#! /bin/bash

echo "Generating protobuf sources..."
/usr/local/bin/protoc-c --c_out=. Discovery.proto

echo "Moving into place"
mv *.h ../include/
cp *.c ../clacksd/src/transport-server/generated
cp *.c ../lib/src/transport-client/generated
rm *.c

echo "Done"
