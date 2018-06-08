set title "various 16N(d,3He)15C OP"
set logscale y
set grid
set key autotitle columnheader
set xlabel "thetaCM"
set ylabel "dxsec/dOmega"
plot "16N_d3He.out.Xsec.txt" u 1:2 w l linecolor rgb "#FF0000",\
                         ""  u 1:3 w l linecolor rgb "#FF5500",\
                         ""  u 1:4 w l linecolor rgb "#FFAA00",\
                         ""  u 1:5 w l linecolor rgb "#0000FF",\
                         ""  u 1:6 w l linecolor rgb "#0055FF",\
                         ""  u 1:7 w l linecolor rgb "#00AAFF"
     
     
#pause -1 "Hit any key to continous"
