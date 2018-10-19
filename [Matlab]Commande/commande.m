function[K]=commande()
mp=0.2; %masse du pondule
mc=0.5; %masse du chariot
lp=0.3; %demi longueur du pondule
g=9.81; %accélération de pesenteur

%%%%%%%%%%%termes des matrice Q et r%%%%%%%
q11=5;
q22=0.2;
q33=10;
q44=5;
r=0.05;
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

A=[0 0 1 0 ; 0 0 0 1; 0 -mp*g/mc 0 0; 0 (mc+mp)*g/(mc*lp) 0 0];
B=[0 0 1/mc -1/(mc*lp)]' ;
q=[q11 q22 q33 q44];
Q=diag(q);

K=lqr(A,B,Q,r);
end