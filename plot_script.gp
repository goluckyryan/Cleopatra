filename = "208Pb_dp.out.Xsec.txt"
titlelStr = "208Pb(d,p)209Pb Ed = 16 MeV"

########################################################
set title titlelStr
set logscale y
set grid
set key autotitle columnheader
set xlabel "thetaCM"
set ylabel "dxsec/dOmega"
set terminal x11 font "Helvetica,20"

#rainbow
#plot filename u 1:2 w l linecolor rgb "#FF0000",\
#          ""  u 1:3 w l linecolor rgb "#FF7F00",\
#          ""  u 1:4 w l linecolor rgb "#00DD00",\
#          ""  u 1:5 w l linecolor rgb "#00FFFF",\
#          ""  u 1:6 w l linecolor rgb "#0000FF",\
#          ""  u 1:7 w l linecolor rgb "#FF00FF"

plot filename u 1:2 w l linecolor rgb "#FF0000",\
          ""  u 1:3 w l linecolor rgb "#00DD00",\
          ""  u 1:4 w l linecolor rgb "#0000FF"
#                            ""  u 1:5 w l linecolor rgb "#0000FF",\
#                            ""  u 1:6 w l linecolor rgb "#0055FF",\
#                            ""  u 1:7 w l linecolor rgb "#00AAFF"
                         
#plot filename u 1:($2/$5) w l linecolor rgb "#FF0000",\
#                         ""  u 1:($3/$6) w l linecolor rgb "#005500",\
#                         ""  u 1:($4/$7) w l linecolor rgb "#0000FF"
     
     
#pause -1 "Hit any key to continous"
