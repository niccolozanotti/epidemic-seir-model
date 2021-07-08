if [ -d "../test_build" ]
then
  rm -r ../test_build
fi
mkdir ../test_build
cmake -S .. -B ../test_build -DCMAKE_BUILD_TYPE=Debug
cd ../test_build || exit
make seir.t
echo "SEIR test target successfully built"
./tests/seir.t