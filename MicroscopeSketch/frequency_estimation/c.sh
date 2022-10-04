g++ -mcmodel=medium ./CM/MicroSketch-CM.cpp -o ./CM/MicroSketch-CM -g -Wall -O3 -std=c++17 -lm -w
g++ -mcmodel=medium ./CU/MicroSketch-CU.cpp -o ./CU/MicroSketch-CU -g -Wall -O3 -std=c++17 -lm -w
g++ -mcmodel=medium ./ECM/ECM_FrequencyEstimation.cpp -o ./ECM/ECM_FrequencyEstimation -g -Wall -O3 -std=c++17 -lm -w
g++ -mcmodel=medium ./SI/demo/main.cpp -o ./SI/demo/main -g -Wall -O3 -std=c++17 -lm -w
g++ -mcmodel=medium ./ECM/ECM-speed.cpp -o ./ECM/ECM-speed -g -Wall -O3 -std=c++17 -lm -w
g++ -mcmodel=medium ./SI/demo/main_speed.cpp -o ./SI/demo/main_speed -g -Wall -O3 -std=c++17 -lm -w
g++ -mcmodel=medium ./CM/MicroSketch-CM-speed.cpp -o ./CM/MicroSketch-CM-speed -g -Wall -O3 -std=c++17 -lm -w
g++ -mcmodel=medium ./CU/MicroSketch-CU-speed.cpp -o ./CU/MicroSketch-CU-speed -g -Wall -O3 -std=c++17 -lm -w