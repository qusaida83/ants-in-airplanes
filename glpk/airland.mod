# number of planes
param n;
param freezetime;

# set of planes
set P := {1..n};

param R{i in P};
param E{i in P};
param T{i in P};
param L{i in P};
param g{i in P};
param h{i in P};
param S{i in P, j in P};

# variables
var x{i in P}, >= 0;
var a{i in P}, >= 0;
var b{i in P}, >= 0;
var sigma{i in P, j in P : i != j}, >= 0, <= 1, binary;

# bounds
s.t. window{i in P} :
  E[i] <= x[i] <= L[i];

s.t. sequence{i in P, j in P : i != j} :
  sigma[i,j] + sigma[j,i] == 1;

s.t. separation{i in P, j in P : i != j} :
  x[j] >= x[i] + S[i,j] - (L[i] + S[i,j] - E[j])*sigma[j,i];

s.t. alphatimebound{i in P} :
  a[i] >= T[i] - x[i];

s.t. alphawindow{i in P} :
  0 <= a[i] <= T[i] - E[i];

s.t. betatimebound{i in P} :
  b[i] >= x[i] - T[i];

s.t. betawindow{i in P} :
  0 <= b[i] <= L[i] - T[i];

s.t. landingtime{i in P} :
  x[i] == T[i] - a[i] + b[i];

# objective
minimize cost: sum{i in P} (g[i] * a[i] + h[i] * b[i]);

# lpx_print_sol(airland, "output");

end;
