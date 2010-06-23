param n := 10;
param freezetime := 10;


# set of planes
set P := {1..n};



param R {i in P}:=
1 54
2 120
3 14
4 21
5 35
6 45
7 49
8 51
9 60
10 85
;

param E {i in P}:=
1 129
2 195
3 89
4 96
5 110
6 120
7 124
8 126
9 135
10 160
;

param T {i in P}:=
1 155
2 258
3 98
4 106
5 123
6 135
7 138
8 140
9 150
10 180
;

param L {i in P}:=
1 559
2 744
3 510
4 521
5 555
6 576
7 577
8 573
9 591
10 657
;

param g {i in P}:=
1 10.00
2 10.00
3 30.00
4 30.00
5 30.00
6 30.00
7 30.00
8 30.00
9 30.00
10 30.00
;

param h {i in P}:=
1 10.00
2 10.00
3 30.00
4 30.00
5 30.00
6 30.00
7 30.00
8 30.00
9 30.00
10 30.00
;

param S {i in P, j in P}:=
1 1 99999
1 2 3
1 3 15
1 4 15
1 5 15
1 6 15
1 7 15
1 8 15
1 9 15
1 10 15
2 1 3
2 2 99999
2 3 15
2 4 15
2 5 15
2 6 15
2 7 15
2 8 15
2 9 15
2 10 15
3 1 15
3 2 15
3 3 99999
3 4 8
3 5 8
3 6 8
3 7 8
3 8 8
3 9 8
3 10 8
4 1 15
4 2 15
4 3 8
4 4 99999
4 5 8
4 6 8
4 7 8
4 8 8
4 9 8
4 10 8
5 1 15
5 2 15
5 3 8
5 4 8
5 5 99999
5 6 8
5 7 8
5 8 8
5 9 8
5 10 8
6 1 15
6 2 15
6 3 8
6 4 8
6 5 8
6 6 99999
6 7 8
6 8 8
6 9 8
6 10 8
7 1 15
7 2 15
7 3 8
7 4 8
7 5 8
7 6 8
7 7 99999
7 8 8
7 9 8
7 10 8
8 1 15
8 2 15
8 3 8
8 4 8
8 5 8
8 6 8
8 7 8
8 8 99999
8 9 8
8 10 8
9 1 15
9 2 15
9 3 8
9 4 8
9 5 8
9 6 8
9 7 8
9 8 8
9 9 99999
9 10 8
10 1 15
10 2 15
10 3 8
10 4 8
10 5 8
10 6 8
10 7 8
10 8 8
10 9 8
10 10 99999
;


# variables
var x{i in P}, >= 0;
var a{i in P}, >= 0;
var b{i in P}, >= 0;
var sigma{i in P, j in P : i != j}, >= 0, <= 1, binary;


# objective
minimize cost: sum{i in P} (g[i] * a[i] + h[i] * b[i]);



# bounds
subject to window{i in P} :
  E[i] <= x[i] <= L[i];

subject to sequence{i in P, j in P : i != j} :
  sigma[i,j] + sigma[j,i] == 1;

subject to separation{i in P, j in P : i != j} :
  x[j] >= x[i] + S[i,j] - (L[i] + S[i,j] - E[j])*sigma[j,i];

subject to alphatimebound{i in P} :
  a[i] >= T[i] - x[i];

subject to alphawindow{i in P} :
  0 <= a[i] <= T[i] - E[i];

subject to betatimebound{i in P} :
  b[i] >= x[i] - T[i];

subject to betawindow{i in P} :
  0 <= b[i] <= L[i] - T[i];

subject to landingtime{i in P} :
  x[i] == T[i] - a[i] + b[i];

end;
