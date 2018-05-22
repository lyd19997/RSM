cmake_minimum_required(VERSION 3.9)
project(RSM_4_17)

set(CMAKE_CXX_STANDARD 11)

add_executable(RSM_4_17 ILP.cpp ILP.h amoeba.cpp Amoeba.h result.h result.cpp RSM_greedy.h RSM_greedy.cpp const.h BL_RSM.h BL_RSM.cpp FlowBase.cpp FlowBase.h graph.cpp graph.h main.cpp request.cpp request.h requestList.cpp requestList.h RL_RSM.cpp RL_RSM.h MinCost.cpp MinCost.h ValueFirst.cpp ValueFirst.h SRM.cpp SRM.h SRMNEW.cpp SRMNEW.h MAAOPT.cpp MAAOPT.h)

include_directories(/usr/local/gurobi752/linux64/include)
target_link_libraries(RSM_4_17 /usr/local/gurobi752/linux64/lib/libgurobi_c++.a)
target_link_libraries(RSM_4_17 /usr/local/gurobi752/linux64/lib/libgurobi75.so)
#target_link_libraries(RSM_4_17 /usr/local/gurobi752/linux64/lib/libaes75.so)
#target_link_libraries(RSM_4_17 /usr/local/gurobi752/linux64/lib/libGurobiJni75.so)