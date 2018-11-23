//golbal varibale for Optical Potential
double v, r0, a;
double vi, ri0, ai;
double vsi, rsi0, asi;
double vso, rso0, aso;
double vsoi, rsoi0, asoi, rc0;

string potentialRef(string name){
   
   if( name == "A" ){
      return "An and Cai (2006) | E < 183 | 12 < A < 238 | http://dx.doi.org/10.1103/PhysRevC.73.054605";
   }
   
   if( name == "K" ){
      return "Koning and Delaroche (2009) | 0.001 < E < 200 | 24 < A < 209 | Iso. Dep. | http://dx.doi.org/10.1016/S0375-9474(02)01321-0";
   }
   
   return "";
}

//======================== deuteron 
bool AnCaiPotential(int A, int Z, double E){
  // d + A(Z)
  // E < 183 or 81.5 MeV/u
  // 12 < A < 238
  // http://dx.doi.org/10.1103/PhysRevC.73.054605
  
  if( !(12 <= A &&  A <= 238 ) ) return false;
  if( E > 183 ) return false;

  double A3 = pow(A, 1./3.);

  v  = 91.85 - 0.249*E + 0.000116*pow(E,2) + 0.642 * Z / A3;
  r0 = 1.152 - 0.00776 / A3;
  a  = 0.719 + 0.0126 * A3;

  vi  = 1.104 + 0.0622 * E;
  ri0 = 1.305 + 0.0997 / A3;
  ai  = 0.855 - 0.1 * A3;

  vsi  = 10.83 - 0.0306 * E;
  rsi0 = 1.334 + 0.152 / A3;
  asi  = 0.531 + 0.062 * A3;

  vso  = 3.557;
  rso0 = 0.972;
  aso  = 1.011;

  vsoi  = 0.0;
  rsoi0 = 0.0;
  asoi  = 0.0;

  rc0 = 1.303;

  return true;
}


//======================== proton 
bool KoningPotential(int A, int Z, double E){
  // p + A(Z)
  // E < 200 or 200 MeV/u
  // 24 < A < 209
  // http://dx.doi.org/10.1016/S0375-9474(02)01321-0
  
  if( !(24 <= A &&  A <= 209 ) ) return false;
  if( E > 200 ) return false;

  int N   = A-Z;
  double A3 = pow(A, 1./3.);
  
  double vp1 = 59.3 + 21*(N-Z)/A - 0.024*A;
  double vn1 = 59.3 - 21*(N-Z)/A - 0.024*A;
  
  double vp2 = 0.007067 + 0.00000423*A;
  double vn2 = 0.007228 - 0.00000148*A;
  
  double vp3 = 0.00001729 + 0.00000001136 * A;
  double vn3 = 0.00001994 - 0.00000002 * A;
  
  double vp4 = 7e-9; // = vn4
  
  double wp1 = 14.667 + 0.009629*A;
  double wn1 = 12.195 + 0.0167*A;
  
  double wp2 = 73.55 + 0.0795*A; // = wn2
  
  double dp1 = 16 + 16*(N-Z)/A;
  double dn2 = 16 - 16*(N-Z)/A;
  
  double dp2 = 0.018 + 0.003802/(1 + exp((A-156)/8)); // = dn2
  
  double dp3 = 11.5 ; // = dn3
  
  double vso1 = 5.922 + 0.003 * A;
  double vso2 = 0.004;
  
  double wso1 = -3.1;
  double wso2 = 160;
  
  double epf = -8.4075 + 0.01378 *A;
  double enf = -11.2814 + 0.02646 *A;
  
  double rc = 1.198 + 0.697/pow(A3,2) + 12.995/pow(A3,5);
  double vc = 1.73/rc * Z / A3;
  
  v  = vp1*(1 - vp2*(E-epf)) + vp3*pow(E-epf,2) + vp4*pow(E-epf,3) + vc * vp1 * (vp2 - 2*vp3*(E-epf)) + 3*vp4*pow(E-epf,2);
  r0 = 1.3039 - 0.4054 / A3;
  a  = 0.6778 - 0.000148 * A3;

  vi  = wp1 * pow(E-epf,2)/(pow(E-epf,2) + pow(wp2,2));
  ri0 = 1.3039 - 0.4054 / A3;
  ai  = 0.6778 - 0.000148 * A3;

  vsi  = dp1 * pow(E-epf,2)/(pow(E-epf,2)+pow(dp3,2)) * exp(-dp2*(E-epf));
  rsi0 = 1.3424 - 0.01585 * A3;
  asi  = 0.5187 + 0.0005205 * A;

  vso  = vso1 * exp(-vso2 * (E-epf));
  rso0 = 1.1854 - 0.647/A3;
  aso  = 0.59;

  vsoi  = wso1 * pow(E-epf,2)/(pow(E-epf,2)+pow(wso2,2));
  rsoi0 = 1.1854 - 0.647/A3;
  asoi  = 0.59;

  rc0 = rc;

  return true;
}



bool CallPotential(string potName, int A, int Z, double E){
  bool okFlag = false;

  if( potName == "A") AnCaiPotential(A, Z, E);
  if( potName == "K") KoningPotential(A, Z, E);
  
  return okFlag;
}
