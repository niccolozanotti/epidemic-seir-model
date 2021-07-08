if [ -d "../sim_graph_build" ]
then
rm -r ../sim_graph_build
fi
mkdir ../sim_graph_build
cmake -S .. -B ../sim_graph_build -DCMAKE_BUILD_TYPE=Release
cd ../sim_graph_build || exit
make sim-graphics
echo "Graphics simulation target successfully built"
./apps/sim-graphics #add cmd line args for default