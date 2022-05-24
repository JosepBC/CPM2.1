#!/bin/bash

mkdir -p out

procs_per_node=(1 2)
n_nodes=(2 4 8)

for proc in ${procs_per_node[@]}; do
    for node in ${n_nodes[@]}; do      
        let total_procs=${proc}*${node}
        echo ${proc} processos per node, ${node} nodes, total proc ${total_procs}--------------
        salloc -p pop -N 8 -n 32 time mpirun -n ${total_procs} -npernode ${proc} ./build/par.out 2>> out/${proc}_${node}_${total_procs}.out
    done
done

procs_per_node=(3)
n_nodes=(4 8)
for proc in ${procs_per_node[@]}; do
    for node in ${n_nodes[@]}; do      
        let total_procs=${proc}*${node}
        echo ${proc} processos per node, ${node} nodes, total proc ${total_procs}--------------
        salloc -p pop -N 8 -n 32 time mpirun -n ${total_procs} -npernode ${proc} ./build/par.out 2>> out/${proc}_${node}_${total_procs}.out
    done
done

procs_per_node=(4)
n_nodes=(8)
for proc in ${procs_per_node[@]}; do
    for node in ${n_nodes[@]}; do      
        let total_procs=${proc}*${node}
        echo ${proc} processos per node, ${node} nodes, total proc ${total_procs}--------------
        salloc -p pop -N 8 -n 32 time mpirun -n ${total_procs} -npernode ${proc} ./build/par.out 2>> out/${proc}_${node}_${total_procs}.out
    done
done

salloc -p pop -N 8 -n 32 time mpirun -n 64 --mca mpi_yield_when_idle true --oversubscribe ./build/par.out 2>> out/4_16_64.out
salloc -p pop -N 8 -n 32 time mpirun -n 128 --mca mpi_yield_when_idle true --oversubscribe ./build/par.out 2>> out/4_32_128.out
exit 0