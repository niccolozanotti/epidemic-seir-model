if [ ! -d "../sim_build" ]
then
mkdir ../sim_build
fi
cd ../sim_build || exit
cmake .. -DCMAKE_BUILD_TYPE=Release
make