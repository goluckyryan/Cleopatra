filename = "208Pb_dp_0MeV.out.Xsec.txt"
titlelStr = "208Pb(d,p)208Pb Ed = 12.3 MeV"

########################################################
set title titlelStr
set logscale y
set grid
set key autotitle columnheader
set key right bottom
set xlabel "thetaCM"
set ylabel "dxsec/dOmega"
set yrange [0.05:3]
set terminal x11 font "Helvetica,20"
set terminal x11 size 800, 800

#rainbow, ground state, different Potential
plot filename u 1:2 w l linecolor rgb "#FF0000",\
          ""  u 1:3 w l linecolor rgb "#FF7F00",\
          ""  u 1:4 w l linecolor rgb "#00DD00",\
          ""  u 1:5 w l linecolor rgb "#00FFFF",\
          ""  u 1:6 w l linecolor rgb "#0000FF",\
          ""  u 1:7 w l linecolor rgb "#FF00FF" 

#plot filename u 1:2 w l linecolor rgb "#FF0000",\
#          ""  u 1:3 w l linecolor rgb "#FF7F00",\
#          ""  u 1:4 w l linecolor rgb "#00DD00",\
#          ""  u 1:5 w l linecolor rgb "#00DD11",\
#          ""  u 1:6 w l linecolor rgb "#00FFFF",\
#          ""  u 1:7 w l linecolor rgb "#0000FF",\
#          ""  u 1:8 w l linecolor rgb "#FF00FF"

#plot filename u 1:2 w l linecolor rgb "#FF0000",\
#          ""  u 1:3 w l linecolor rgb "#00DD00",\
#          ""  u 1:4 w l linecolor rgb "#0000FF"
#                            ""  u 1:5 w l linecolor rgb "#0000FF",\
#                            ""  u 1:6 w l linecolor rgb "#0055FF",\
#                            ""  u 1:7 w l linecolor rgb "#00AAFF"
                         
#plot filename u 1:($2/$5) w l linecolor rgb "#FF0000",\
#                         ""  u 1:($3/$6) w l linecolor rgb "#005500",\
#                         ""  u 1:($4/$7) w l linecolor rgb "#0000FF"
     
     
#pause -1 "Hit any key to continous"
