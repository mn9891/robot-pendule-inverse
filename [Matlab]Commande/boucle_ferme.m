function[]=boucle_ferme(N)
mp=0.2; %masse du pondule
mc=0.5; %masse du chariot
lp=0.3; %demi longueur du pondule
g=9.81; %accélération de pesenteur
%%%%%%%%%%%constantes du moteurs%%%%%%%%%%%
km=1;
kg=1;
R=300;
%%%%%%%%%%%termes des matrice Q et r%%%%%%%
q11=2;
q22=12;
q33=0.5;
q44=2.5;
r=0.05;
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

A=[0 0 1 0 ; 0 0 0 1; 0 -mp*g/mc -(km*kg/R)^2*1/mc 0; 0 (mc+mp)*g/(mc*lp) (km*kg/R)^2*1/(mc*lp) 0];
B=[0 0 km*kg/(R*mc) -km*kg/(R*mc*lp)]' ;
q=[q11 q22 q33 q44];
Q=diag(q);

K=lqr(A,B,Q,r);
x=[0 0.3 0 0]';
for i=1:N
    x=(A-B*K)*x;
    position(i)=x(1);
    angle(i)=x(2);
end
subplot(2,1,1) , plot(position);
title('position');
subplot(2,1,2) , plot(angle);
title('angle');
end