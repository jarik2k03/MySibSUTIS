#!/bin/sh


# size
 m=15000
# n=15000

# threads
threads=2

# compiling
gcc -fopenmp -Wall main.c -o ./bench

 echo 'm = 15000'
 for (( i=threads;i<=6;i+=1 ))
 do
     ./bench 15000 15000 $i
 done

# echo 'm = 20000'
# for (( i=threads;i<=6;i+=1 ))
# do
    # ./bench 20000 20000 $i
# done

#echo 'm = 25000'
#for (( i=threads;i<=6;i+=1 ))
#do
 #   ./bench 25000 25000 $i
#done
