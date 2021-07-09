if [ ! -d "../test_build" ]
then
  mkdir ../test_build
fi
cd ../test_build || exit
cmake  ..  -DCMAKE_BUILD_TYPE=Debug
make test1 test2 test3