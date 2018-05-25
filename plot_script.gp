set title "various 16N(d,3He)15C OP"
set logscale y
set grid
set key autotitle columnheader
set xlabel "thetaCM"
set ylabel "dxsec/dOmega"
plot "16N_d3He.out.Xsec.txt" u 1:2 w l,\
     ""  u 1:3 w l,\
     ""  u 1:4 w l,\
     ""  u 1:5 w l,\
     ""  u 1:6 w l,\
     ""  u 1:7 w l,\
     ""  u 1:8 w l,\
     ""  u 1:9 w l,\
     ""  u 1:10 w l
     
     
#pause -1 "Hit any key to continous"
