# Virtual_Machine
32-bit instruction  
## first byte  
```
0. 1 if arg1 is an immediate, else 0
1. 1 if arg2 is an immediate, else 0  
2-3.  10 if arg1 is to be read from RAM  
      01 if arg2 is to be read from RAM  
      11 if destination is RAM  
      00 if neither is RAM     
4. 1 if condition, 0 if ALU  
5-6-7. operation code
```
## second byte
argument 1
## third byte
argument 2
## fourth byte
destination
   
