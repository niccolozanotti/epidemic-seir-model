if [ -d "../sim_build" ]
then
rm -r ../sim_build
fi
mkdir ../sim_build
cmake -S .. -B ../sim_build -DCMAKE_BUILD_TYPE=Release
cd ../sim_build || exit
make sim
echo "Simulation target successfully built"
./apps/sim #add cmd line args for default