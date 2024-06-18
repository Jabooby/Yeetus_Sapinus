function [t,VAR,Output] = test
%===========================================================================
% File: test.m created Jun 18 2024 by MotionGenesis 6.3.
% Portions copyright (c) 2009-2023 Motion Genesis LLC.  Rights reserved.
% MotionGenesis Student Licensee: William Forcier. (until March 2028).
% Paid-up MotionGenesis Student licensees are granted the right
% to distribute this code for legal student-academic (non-professional) purposes only,
% provided this copyright notice appears in all copies and distributions.
%===========================================================================
% The software is provided "as is", without warranty of any kind, express or    
% implied, including but not limited to the warranties of merchantability or    
% fitness for a particular purpose. In no event shall the authors, contributors,
% or copyright holders be liable for any claim, damages or other liability,     
% whether in an action of contract, tort, or otherwise, arising from, out of, or
% in connection with the software or the use or other dealings in the software. 
%===========================================================================
eventDetectedByIntegratorTerminate1OrContinue0 = [];
fd=0; fg=0; FRcx=0; FRcy=0; FRdy=0; FRx_b_a=0; FRx_c_a=0; FRx_d_b=0; FRx_p_a=0; FRy_b_a=0; FRy_c_a=0; FRy_d_b=0; FRy_p_a=0; Nd=0;
Ng=0; qB=0; TA_B=0; x=0; qdDt=0; xDt=0; qDDt=0; qgDDt=0; XaiDDt=0; YaiDDt=0; Tau=0; Bx=0; By=0; Ccmx=0; Ccmy=0; Ccorner2x=0; Ccorner2y=0;
Ccorner3x=0; Ccorner3y=0; WdContactx=0; WdContacty=0; WgContacty=0;


%-------------------------------+--------------------------+-------------------+-----------------
% Quantity                      | Value                    | Units             | Description
%-------------------------------|--------------------------|-------------------|-----------------
g                               =  9.8;                    % N/kg                Constant
IAzz                            =  403.03;                 % kg*mm^2             Constant
IBzz                            =  403.03;                 % kg*mm^2             Constant
ICzz                            =  7270.62;                % g*mm^2              Constant
IPzz                            =  0.1;                    % kg*m^2              Constant
mA                              =  0.5;                    % kg                  Constant
mB                              =  0.2;                    % kg                  Constant
mC                              =  0.020;                  % kg                  Constant
mD                              =  0.020;                  % kg                  Constant
mP                              =  0.2;                    % kg                  Constant
mPpre                           =  0.1;                    % kg                  Constant
Rc                              =  30;                     % mm                  Constant
Rd                              =  30;                     % mm                  Constant
Td                              =  0.0;                    % N*m                 Constant
Tp                              =  0.0;                    % N*m                 Constant
X_Ao_To_Acm                     =  76.2;                   % mm                  Constant
X_Ao_To_Ccm                     =  152.40;                 % mm                  Constant
X_Bo_To_Bcm                     =  76.2;                   % mm                  Constant
X_Bo_To_Dcm                     =  152.40;                 % mm                  Constant
X_Po_To_Pcm                     =  100;                    % mm                  Constant
X_Po_To_Ppre                    =  200;                    % mm                  Constant
Y_Ao_To_Acm                     =  0;                      % mm                  Constant
Y_Ao_To_Ccm                     =  0;                      % mm                  Constant
Y_Bo_To_Bcm                     =  0;                      % mm                  Constant
Y_Bo_To_Dcm                     =  0;                      % mm                  Constant
Y_Po_To_Pcm                     =  0;                      % mm                  Constant
Y_Po_To_Ppre                    =  0;                      % mm                  Constant

q                               =  0;                      % deg                 Initial Value
qd                              =  0;                      % deg                 Initial Value
qg                              =  0;                      % deg                 Initial Value
Xai                             =  0;                      % m                   Initial Value
Yai                             =  0;                      % m                   Initial Value
qDt                             =  0;                      % rad/sec             Initial Value
qgDt                            =  0;                      % rad/sec             Initial Value
XaiDt                           =  0;                      % m/s                 Initial Value
YaiDt                           =  0;                      % m/s                 Initial Value

tInitial                        =  0.0;                    % second              Initial Time
tFinal                          =  2;                     % sec                 Final Time
tStep                           =  0.02;                   % sec                 Integration Step
printIntScreen                  =  1;                      % 0 or +integer       0 is NO screen output
printIntFile                    =  1;                      % 0 or +integer       0 is NO file   output
absError                        =  1.0E-07;                %                     Absolute Error
relError                        =  1.0E-08;                %                     Relative Error
%-------------------------------+--------------------------+-------------------+-----------------

% Unit conversions.  UnitSystem: kg, meter, second.
DEGtoRAD = pi / 180.0;
RADtoDEG = 180.0 / pi;
IAzz = IAzz * 1.0E-06;                                     %  Converted from kg*mm^2 
IBzz = IBzz * 1.0E-06;                                     %  Converted from kg*mm^2 
ICzz = ICzz * 1.0E-09;                                     %  Converted from g*mm^2 
Rc = Rc * 0.001;                                           %  Converted from mm 
Rd = Rd * 0.001;                                           %  Converted from mm 
X_Ao_To_Acm = X_Ao_To_Acm * 0.001;                         %  Converted from mm 
X_Ao_To_Ccm = X_Ao_To_Ccm * 0.001;                         %  Converted from mm 
X_Bo_To_Bcm = X_Bo_To_Bcm * 0.001;                         %  Converted from mm 
X_Bo_To_Dcm = X_Bo_To_Dcm * 0.001;                         %  Converted from mm 
X_Po_To_Pcm = X_Po_To_Pcm * 0.001;                         %  Converted from mm 
X_Po_To_Ppre = X_Po_To_Ppre * 0.001;                       %  Converted from mm 
Y_Ao_To_Acm = Y_Ao_To_Acm * 0.001;                         %  Converted from mm 
Y_Ao_To_Ccm = Y_Ao_To_Ccm * 0.001;                         %  Converted from mm 
Y_Bo_To_Bcm = Y_Bo_To_Bcm * 0.001;                         %  Converted from mm 
Y_Bo_To_Dcm = Y_Bo_To_Dcm * 0.001;                         %  Converted from mm 
Y_Po_To_Pcm = Y_Po_To_Pcm * 0.001;                         %  Converted from mm 
Y_Po_To_Ppre = Y_Po_To_Ppre * 0.001;                       %  Converted from mm 
q = q * DEGtoRAD;                                          %  Converted from deg 
qd = qd * DEGtoRAD;                                        %  Converted from deg 
qg = qg * DEGtoRAD;                                        %  Converted from deg 

% Evaluate constants
qdDt = 0;
fd = 0;


VAR = SetMatrixFromNamedQuantities;
[t,VAR,Output] = IntegrateForwardOrBackward( tInitial, tFinal, tStep, absError, relError, VAR, printIntScreen, printIntFile );
OutputToScreenOrFile( [], 0, 0 );   % Close output files.


%===========================================================================
function sys = mdlDerivatives( t, VAR, uSimulink )
%===========================================================================
SetNamedQuantitiesFromMatrix( VAR );
qB = sin(4*t);

% Quantities previously specified in MotionGenesis.
Tau = 0;

COEF = zeros( 16, 16 );
COEF(1,1) = -1;
COEF(1,3) = -1;
COEF(1,5) = 1;
COEF(1,13) = mA;
COEF(2,2) = -1;
COEF(2,4) = -1;
COEF(2,6) = 1;
COEF(2,14) = mA;
COEF(3,1) = 1;
COEF(3,7) = -1;
COEF(3,13) = mB;
COEF(4,2) = 1;
COEF(4,8) = -1;
COEF(4,14) = mB;
COEF(5,3) = 1;
COEF(5,9) = -1;
COEF(5,13) = mC;
COEF(6,4) = 1;
COEF(6,10) = -1;
COEF(6,14) = mC;
COEF(7,7) = 1;
COEF(7,13) = mD;
COEF(8,8) = 1;
COEF(8,11) = -1;
COEF(8,14) = mD;
COEF(9,5) = -1;
COEF(9,12) = (mP*X_Po_To_Pcm+mPpre*X_Po_To_Ppre)*cos(q) - (mP*Y_Po_To_Pcm+mPpre*Y_Po_To_Ppre)*sin(q);
COEF(9,13) = mP + mPpre;
COEF(10,6) = -1;
COEF(10,12) = (mP*X_Po_To_Pcm+mPpre*X_Po_To_Ppre)*sin(q) + (mP*Y_Po_To_Pcm+mPpre*Y_Po_To_Ppre)*cos(q);
COEF(10,14) = mP + mPpre;
COEF(11,1) = X_Ao_To_Acm*sin(0.5*qB) - Y_Ao_To_Acm*cos(0.5*qB);
COEF(11,2) = -X_Ao_To_Acm*cos(0.5*qB) - Y_Ao_To_Acm*sin(0.5*qB);
COEF(11,3) = (X_Ao_To_Acm-X_Ao_To_Ccm)*sin(0.5*qB) - (Y_Ao_To_Acm+Y_Ao_To_Ccm)*cos(0.5*qB);
COEF(11,4) = -(Y_Ao_To_Acm+Y_Ao_To_Ccm)*sin(0.5*qB) - (X_Ao_To_Acm-X_Ao_To_Ccm)*cos(0.5*qB);
COEF(11,5) = Y_Ao_To_Acm*cos(0.5*qB) - X_Ao_To_Acm*sin(0.5*qB);
COEF(11,6) = X_Ao_To_Acm*cos(0.5*qB) + Y_Ao_To_Acm*sin(0.5*qB);
COEF(11,15) = -1;
COEF(12,1) = Y_Bo_To_Bcm*cos(0.5*qB) - X_Bo_To_Bcm*sin(0.5*qB);
COEF(12,2) = -X_Bo_To_Bcm*cos(0.5*qB) - Y_Bo_To_Bcm*sin(0.5*qB);
COEF(12,7) = (X_Bo_To_Bcm-X_Bo_To_Dcm)*sin(0.5*qB) - (Y_Bo_To_Bcm-Y_Bo_To_Dcm)*cos(0.5*qB);
COEF(12,8) = (X_Bo_To_Bcm-X_Bo_To_Dcm)*cos(0.5*qB) + (Y_Bo_To_Bcm-Y_Bo_To_Dcm)*sin(0.5*qB);
COEF(12,15) = 1;
COEF(13,9) = -Rc;
COEF(13,16) = -ICzz;
COEF(14,5) = X_Po_To_Pcm*cos(q) - Y_Po_To_Pcm*sin(q);
COEF(14,6) = X_Po_To_Pcm*sin(q) + Y_Po_To_Pcm*cos(q);
COEF(14,12) = IPzz - mPpre*X_Po_To_Ppre*(X_Po_To_Pcm-X_Po_To_Ppre) - mPpre*Y_Po_To_Ppre*(Y_Po_To_Pcm-Y_Po_To_Ppre);
COEF(14,13) = -mPpre*((X_Po_To_Pcm-X_Po_To_Ppre)*cos(q)-(Y_Po_To_Pcm-Y_Po_To_Ppre)*sin(q));
COEF(14,14) = -mPpre*((X_Po_To_Pcm-X_Po_To_Ppre)*sin(q)+(Y_Po_To_Pcm-Y_Po_To_Ppre)*cos(q));
COEF(15,13) = -1;
COEF(15,16) = Rc;
COEF(16,14) = 1;
RHS = zeros( 1, 16 );
RHS(1) = -0.25*mA*((2*Y_Ao_To_Acm*sin(t)+2*Y_Ao_To_Ccm*sin(t)+X_Ao_To_Acm*cos(t)^2-X_Ao_To_Ccm*cos(t)^2)*cos(0.5*qB)-(2*  ...
X_Ao_To_Acm*sin(t)-2*X_Ao_To_Ccm*sin(t)-Y_Ao_To_Acm*cos(t)^2-Y_Ao_To_Ccm*cos(t)^2)*sin(0.5*qB));
RHS(2) = -0.25*mA*(4*g+(2*Y_Ao_To_Acm*sin(t)+2*Y_Ao_To_Ccm*sin(t)+X_Ao_To_Acm*cos(t)^2-X_Ao_To_Ccm*cos(t)^2)*sin(0.5*qB)+(2*  ...
X_Ao_To_Acm*sin(t)-2*X_Ao_To_Ccm*sin(t)-Y_Ao_To_Acm*cos(t)^2-Y_Ao_To_Ccm*cos(t)^2)*cos(0.5*qB));
RHS(3) = 0.25*mB*((2*Y_Bo_To_Bcm*sin(t)+X_Bo_To_Bcm*cos(t)^2)*cos(0.5*qB)-(2*X_Ao_To_Ccm*sin(t)+Y_Ao_To_Ccm*cos(t)^2)*sin(0.5*qB)-(  ...
2*X_Bo_To_Bcm*sin(t)-Y_Bo_To_Bcm*cos(t)^2)*sin(0.5*qB)-(2*Y_Ao_To_Ccm*sin(t)-X_Ao_To_Ccm*cos(t)^2)*cos(0.5*qB));
RHS(4) = 0.25*mB*((2*X_Ao_To_Ccm*sin(t)+Y_Ao_To_Ccm*cos(t)^2)*cos(0.5*qB)-4*g-(2*Y_Bo_To_Bcm*sin(t)+X_Bo_To_Bcm*cos(t)^2)*sin(0.5*  ...
qB)-(2*X_Bo_To_Bcm*sin(t)-Y_Bo_To_Bcm*cos(t)^2)*cos(0.5*qB)-(2*Y_Ao_To_Ccm*sin(t)-X_Ao_To_Ccm*cos(t)^2)*sin(0.5*qB));
RHS(6) = -g*mC;
RHS(7) = 0.25*mD*((2*Y_Bo_To_Dcm*sin(t)+X_Bo_To_Dcm*cos(t)^2)*cos(0.5*qB)-(2*X_Ao_To_Ccm*sin(t)+Y_Ao_To_Ccm*cos(t)^2)*sin(0.5*qB)-(  ...
2*X_Bo_To_Dcm*sin(t)-Y_Bo_To_Dcm*cos(t)^2)*sin(0.5*qB)-(2*Y_Ao_To_Ccm*sin(t)-X_Ao_To_Ccm*cos(t)^2)*cos(0.5*qB));
RHS(8) = 0.25*mD*((2*X_Ao_To_Ccm*sin(t)+Y_Ao_To_Ccm*cos(t)^2)*cos(0.5*qB)-4*g-(2*Y_Bo_To_Dcm*sin(t)+X_Bo_To_Dcm*cos(t)^2)*sin(0.5*  ...
qB)-(2*X_Bo_To_Dcm*sin(t)-Y_Bo_To_Dcm*cos(t)^2)*cos(0.5*qB)-(2*Y_Ao_To_Ccm*sin(t)-X_Ao_To_Ccm*cos(t)^2)*sin(0.5*qB));
RHS(9) = (mP*X_Po_To_Pcm+mPpre*X_Po_To_Ppre)*sin(q)*qDt^2 + (mP*Y_Po_To_Pcm+mPpre*Y_Po_To_Ppre)*cos(q)*qDt^2 - 0.25*(mP+mPpre)*(2*  ...
X_Ao_To_Ccm*sin(t)+Y_Ao_To_Ccm*cos(t)^2)*sin(0.5*qB) - 0.25*(mP+mPpre)*(2*Y_Ao_To_Ccm*sin(t)-X_Ao_To_Ccm*cos(t)^2)*cos(0.5*qB);
RHS(10) = 0.25*(mP+mPpre)*(2*X_Ao_To_Ccm*sin(t)+Y_Ao_To_Ccm*cos(t)^2)*cos(0.5*qB) + (mP*Y_Po_To_Pcm+mPpre*Y_Po_To_Ppre)*sin(q)*qDt^2  ...
- g*mP - g*mPpre - 0.25*(mP+mPpre)*(2*Y_Ao_To_Ccm*sin(t)-X_Ao_To_Ccm*cos(t)^2)*sin(0.5*qB) - (mP*X_Po_To_Pcm+mPpre*X_Po_To_Ppre)*  ...
cos(q)*qDt^2;
RHS(11) = 0.5*IAzz*sin(t) + Tau - Tp;
RHS(12) = -Td - 0.5*IBzz*sin(t);
RHS(13) = 0.5*ICzz*sin(t) - Tau;
RHS(14) = Tp + g*mPpre*(X_Po_To_Pcm-X_Po_To_Ppre)*sin(q) + g*mPpre*(Y_Po_To_Pcm-Y_Po_To_Ppre)*cos(q) + 0.25*mPpre*((X_Po_To_Pcm-  ...
X_Po_To_Ppre)*(2*Y_Ao_To_Ccm*sin(t)-X_Ao_To_Ccm*cos(t)^2)*cos(q-0.5*qB)-(X_Po_To_Pcm-X_Po_To_Ppre)*(2*X_Ao_To_Ccm*sin(t)+  ...
Y_Ao_To_Ccm*cos(t)^2)*sin(q-0.5*qB)-(Y_Po_To_Pcm-Y_Po_To_Ppre)*(2*X_Ao_To_Ccm*sin(t)+Y_Ao_To_Ccm*cos(t)^2)*cos(q-0.5*qB)-(  ...
Y_Po_To_Pcm-Y_Po_To_Ppre)*(2*Y_Ao_To_Ccm*sin(t)-X_Ao_To_Ccm*cos(t)^2)*sin(q-0.5*qB)) + mPpre*X_Po_To_Ppre*Y_Po_To_Pcm*qDt^2 -   ...
mPpre*X_Po_To_Pcm*Y_Po_To_Ppre*qDt^2;
SolutionToAlgebraicEquations = COEF \ transpose(RHS);

% Update variables after uncoupling equations
FRx_b_a = SolutionToAlgebraicEquations(1);
FRy_b_a = SolutionToAlgebraicEquations(2);
FRx_c_a = SolutionToAlgebraicEquations(3);
FRy_c_a = SolutionToAlgebraicEquations(4);
FRx_p_a = SolutionToAlgebraicEquations(5);
FRy_p_a = SolutionToAlgebraicEquations(6);
FRx_d_b = SolutionToAlgebraicEquations(7);
FRy_d_b = SolutionToAlgebraicEquations(8);
FRcx = SolutionToAlgebraicEquations(9);
FRcy = SolutionToAlgebraicEquations(10);
FRdy = SolutionToAlgebraicEquations(11);
qDDt = SolutionToAlgebraicEquations(12);
XaiDDt = SolutionToAlgebraicEquations(13);
YaiDDt = SolutionToAlgebraicEquations(14);
TA_B = SolutionToAlgebraicEquations(15);
qgDDt = SolutionToAlgebraicEquations(16);

sys = transpose( SetMatrixOfDerivativesPriorToIntegrationStep );
end



%===========================================================================
function VAR = SetMatrixFromNamedQuantities
%===========================================================================
VAR = zeros( 1, 9 );
VAR(1) = q;
VAR(2) = qd;
VAR(3) = qg;
VAR(4) = Xai;
VAR(5) = Yai;
VAR(6) = qDt;
VAR(7) = qgDt;
VAR(8) = XaiDt;
VAR(9) = YaiDt;
end


%===========================================================================
function SetNamedQuantitiesFromMatrix( VAR )
%===========================================================================
q = VAR(1);
qd = VAR(2);
qg = VAR(3);
Xai = VAR(4);
Yai = VAR(5);
qDt = VAR(6);
qgDt = VAR(7);
XaiDt = VAR(8);
YaiDt = VAR(9);
end


%===========================================================================
function VARp = SetMatrixOfDerivativesPriorToIntegrationStep
%===========================================================================
VARp = zeros( 1, 9 );
VARp(1) = qDt;
VARp(2) = qdDt;
VARp(3) = qgDt;
VARp(4) = XaiDt;
VARp(5) = YaiDt;
VARp(6) = qDDt;
VARp(7) = qgDDt;
VARp(8) = XaiDDt;
VARp(9) = YaiDDt;
end



%===========================================================================
function Output = mdlOutputs( t, VAR, uSimulink )
%===========================================================================
x = Xai + X_Ao_To_Ccm*cos(0.5*qB) - Y_Ao_To_Ccm*sin(0.5*qB);
xDt = XaiDt - 0.5*X_Ao_To_Ccm*cos(t)*sin(0.5*qB) - 0.5*Y_Ao_To_Ccm*cos(t)*cos(0.5*qB);
fg = FRcx;
Ng = FRcy;
Nd = FRdy;
Ccorner2x = Xai + X_Ao_To_Ccm*cos(0.5*qB) + X_Bo_To_Dcm*cos(0.5*qB) + Y_Bo_To_Dcm*sin(0.5*qB) - Y_Ao_To_Ccm*sin(0.5*qB);
Ccorner2y = Yai + X_Ao_To_Ccm*sin(0.5*qB) + Y_Ao_To_Ccm*cos(0.5*qB) + Y_Bo_To_Dcm*cos(0.5*qB) - X_Bo_To_Dcm*sin(0.5*qB);
Ccorner3x = Xai + X_Ao_To_Ccm*cos(0.5*qB) + X_Bo_To_Dcm*cos(0.5*qB) + Y_Bo_To_Dcm*sin(0.5*qB) - Y_Ao_To_Ccm*sin(0.5*qB);
Ccorner3y = Yai + X_Ao_To_Ccm*sin(0.5*qB) + Y_Ao_To_Ccm*cos(0.5*qB) + Y_Bo_To_Dcm*cos(0.5*qB) - X_Bo_To_Dcm*sin(0.5*qB);
Ccmx = Xai + X_Ao_To_Ccm*cos(0.5*qB) - Y_Ao_To_Ccm*sin(0.5*qB);
Ccmy = Yai + X_Ao_To_Ccm*sin(0.5*qB) + Y_Ao_To_Ccm*cos(0.5*qB);
Bx = Xai + X_Po_To_Ppre*sin(q) + Y_Po_To_Ppre*cos(q) + X_Ao_To_Ccm*cos(0.5*qB) - Y_Ao_To_Ccm*sin(0.5*qB);
By = Yai + Y_Po_To_Ppre*sin(q) + X_Ao_To_Ccm*sin(0.5*qB) + Y_Ao_To_Ccm*cos(0.5*qB) - X_Po_To_Ppre*cos(q);
WgContacty = Yai - Rc;
WdContactx = Xai + X_Ao_To_Ccm*cos(0.5*qB) + X_Bo_To_Dcm*cos(0.5*qB) + Y_Bo_To_Dcm*sin(0.5*qB) - Y_Ao_To_Ccm*sin(0.5*qB);
WdContacty = Yai + X_Ao_To_Ccm*sin(0.5*qB) + Y_Ao_To_Ccm*cos(0.5*qB) + Y_Bo_To_Dcm*cos(0.5*qB) - Rd - X_Bo_To_Dcm*sin(0.5*qB);

Output = zeros( 1, 28 );
Output(1) = t;
Output(2) = x;
Output(3) = xDt;
Output(4) = q*RADtoDEG;                               % Converted to deg
Output(5) = qDt*RADtoDEG;                             % Converted to deg/s
Output(6) = qgDt*RADtoDEG;                            % Converted to deg/sec
Output(7) = qdDt*RADtoDEG;                            % Converted to deg/sec
Output(8) = fg;
Output(9) = fd;
Output(10) = Ng;
Output(11) = Nd;
Output(12) = Tau;

Output(13) = Xai;
Output(14) = Yai;
Output(15) = Ccorner2x;
Output(16) = Ccorner2y;
Output(17) = Ccorner3x;
Output(18) = Ccorner3y;
Output(19) = Xai;
Output(20) = Yai;
Output(21) = Ccmx;
Output(22) = Ccmy;
Output(23) = Bx;
Output(24) = By;
Output(25) = Xai;
Output(26) = WgContacty;
Output(27) = WdContactx;
Output(28) = WdContacty;
end


%===========================================================================
function OutputToScreenOrFile( Output, shouldPrintToScreen, shouldPrintToFile )
%===========================================================================
persistent FileIdentifier hasHeaderInformationBeenWritten;

if( isempty(Output) ),
   if( ~isempty(FileIdentifier) ),
      for( i = 1 : 2 ),  fclose( FileIdentifier(i) );  end
      clear FileIdentifier;
      fprintf( 1, '\n Output is in the files test.i  (i=1,2)\n' );
      fprintf( 1, '\n Note: To automate plotting, issue the command OutputPlot in MotionGenesis.\n' );
      fprintf( 1, '\n To load and plot columns 1 and 2 with a solid line and columns 1 and 3 with a dashed line, enter:\n' );
      fprintf( 1, '    someName = load( ''test.1'' );\n' );
      fprintf( 1, '    plot( someName(:,1), someName(:,2), ''-'', someName(:,1), someName(:,3), ''--'' )\n\n' );
   end
   clear hasHeaderInformationBeenWritten;
   return;
end

if( isempty(hasHeaderInformationBeenWritten) ),
   if( shouldPrintToScreen ),
      fprintf( 1,                '%%       t              x             x''              q             q''             qg''            qd''            fg             fd             Ng             Nd             tau\n' );
      fprintf( 1,                '%%     (sec)           (m)          (m/sec)         (deg)         (deg/s)       (deg/sec)      (deg/sec)         (N)            (N)            (N)            (N)            (N)\n\n' );
   end
   if( shouldPrintToFile && isempty(FileIdentifier) ),
      FileIdentifier = zeros( 1, 2 );
      FileIdentifier(1) = fopen('test.1', 'wt');   if( FileIdentifier(1) == -1 ), error('Error: unable to open file test.1'); end
      fprintf(FileIdentifier(1), '%% FILE: test.1\n%%\n' );
      fprintf(FileIdentifier(1), '%%       t              x             x''              q             q''             qg''            qd''            fg             fd             Ng             Nd             tau\n' );
      fprintf(FileIdentifier(1), '%%     (sec)           (m)          (m/sec)         (deg)         (deg/s)       (deg/sec)      (deg/sec)         (N)            (N)            (N)            (N)            (N)\n\n' );
      FileIdentifier(2) = fopen('test.2', 'wt');   if( FileIdentifier(2) == -1 ), error('Error: unable to open file test.2'); end
      fprintf(FileIdentifier(2), '%% FILE: test.2\n%%\n' );
      fprintf(FileIdentifier(2), '%%   Ccorner1x      Ccorner1y      Ccorner2x      Ccorner2y      Ccorner3x      Ccorner3y      Ccorner4x      Ccorner4y        Ccmx           Ccmy            Bx             By         WgContactx     WgContacty     WdContactx     WdContacty\n' );
      fprintf(FileIdentifier(2), '%%    (UNITS)        (UNITS)        (UNITS)        (UNITS)        (UNITS)        (UNITS)        (UNITS)        (UNITS)        (UNITS)        (UNITS)        (UNITS)        (UNITS)        (UNITS)        (UNITS)        (UNITS)        (UNITS)\n\n' );
   end
   hasHeaderInformationBeenWritten = 1;
end

if( shouldPrintToScreen ), WriteNumericalData( 1,                 Output(1:12) );  end
if( shouldPrintToFile ),   WriteNumericalData( FileIdentifier(1), Output(1:12) );  end
if( shouldPrintToFile ),   WriteNumericalData( FileIdentifier(2), Output(13:28) );  end
end


%===========================================================================
function WriteNumericalData( fileIdentifier, Output )
%===========================================================================
numberOfOutputQuantities = length( Output );
if( numberOfOutputQuantities > 0 ),
   for( i = 1 : numberOfOutputQuantities ),
      fprintf( fileIdentifier, ' %- 14.6E', Output(i) );
   end
   fprintf( fileIdentifier, '\n' );
end
end



%===========================================================================
function [functionsToEvaluateForEvent, eventTerminatesIntegration1Otherwise0ToContinue, eventDirection_AscendingIs1_CrossingIs0_DescendingIsNegative1] = EventDetection( t, VAR, uSimulink )
%===========================================================================
% Detects when designated functions are zero or cross zero with positive or negative slope.
% Step 1: Uncomment call to mdlDerivatives and mdlOutputs.
% Step 2: Change functionsToEvaluateForEvent,                      e.g., change  []  to  [t - 5.67]  to stop at t = 5.67.
% Step 3: Change eventTerminatesIntegration1Otherwise0ToContinue,  e.g., change  []  to  [1]  to stop integrating.
% Step 4: Change eventDirection_AscendingIs1_CrossingIs0_DescendingIsNegative1,  e.g., change  []  to  [1].
% Step 5: Possibly modify function EventDetectedByIntegrator (if eventTerminatesIntegration1Otherwise0ToContinue is 0).
%---------------------------------------------------------------------------
% mdlDerivatives( t, VAR, uSimulink );        % UNCOMMENT FOR EVENT HANDLING
% mdlOutputs(     t, VAR, uSimulink );        % UNCOMMENT FOR EVENT HANDLING
functionsToEvaluateForEvent = [];
eventTerminatesIntegration1Otherwise0ToContinue = [];
eventDirection_AscendingIs1_CrossingIs0_DescendingIsNegative1 = [];
eventDetectedByIntegratorTerminate1OrContinue0 = eventTerminatesIntegration1Otherwise0ToContinue;
end


%===========================================================================
function [isIntegrationFinished, VAR] = EventDetectedByIntegrator( t, VAR, nIndexOfEvents )
%===========================================================================
isIntegrationFinished = eventDetectedByIntegratorTerminate1OrContinue0( nIndexOfEvents );
if( ~isIntegrationFinished ),
   SetNamedQuantitiesFromMatrix( VAR );
%  Put code here to modify how integration continues.
   VAR = SetMatrixFromNamedQuantities;
end
end



%===========================================================================
function [t,VAR,Output] = IntegrateForwardOrBackward( tInitial, tFinal, tStep, absError, relError, VAR, printIntScreen, printIntFile )
%===========================================================================
OdeMatlabOptions = odeset( 'RelTol',relError, 'AbsTol',absError, 'MaxStep',tStep, 'Events',@EventDetection );
t = tInitial;                 epsilonT = 0.001*tStep;                   tFinalMinusEpsilonT = tFinal - epsilonT;
printCounterScreen = 0;       integrateForward = tFinal >= tInitial;    tAtEndOfIntegrationStep = t + tStep;
printCounterFile   = 0;       isIntegrationFinished = 0;
mdlDerivatives( t, VAR, 0 );
while 1,
   if( (integrateForward && t >= tFinalMinusEpsilonT) || (~integrateForward && t <= tFinalMinusEpsilonT) ), isIntegrationFinished = 1;  end
   shouldPrintToScreen = printIntScreen && ( isIntegrationFinished || printCounterScreen <= 0.01 );
   shouldPrintToFile   = printIntFile   && ( isIntegrationFinished || printCounterFile   <= 0.01 );
   if( isIntegrationFinished || shouldPrintToScreen || shouldPrintToFile ),
      Output = mdlOutputs( t, VAR, 0 );
      OutputToScreenOrFile( Output, shouldPrintToScreen, shouldPrintToFile );
      if( isIntegrationFinished ), break;  end
      if( shouldPrintToScreen ), printCounterScreen = printIntScreen;  end
      if( shouldPrintToFile ),   printCounterFile   = printIntFile;    end
   end
   [TimeOdeArray, VarOdeArray, timeEventOccurredInIntegrationStep, nStatesArraysAtEvent, nIndexOfEvents] = ode45( @mdlDerivatives, [t tAtEndOfIntegrationStep], VAR, OdeMatlabOptions, 0 );
   if( isempty(timeEventOccurredInIntegrationStep) ),
      lastIndex = length( TimeOdeArray );
      t = TimeOdeArray( lastIndex );
      VAR = VarOdeArray( lastIndex, : );
      printCounterScreen = printCounterScreen - 1;
      printCounterFile   = printCounterFile   - 1;
      if( abs(tAtEndOfIntegrationStep - t) >= abs(epsilonT) ), warning('numerical integration failed'); break;  end
      tAtEndOfIntegrationStep = t + tStep;
      if( (integrateForward && tAtEndOfIntegrationStep > tFinal) || (~integrateForward && tAtEndOfIntegrationStep < tFinal) ) tAtEndOfIntegrationStep = tFinal;  end
   else
      t = timeEventOccurredInIntegrationStep( 1 );    % time  at firstEvent = 1 during this integration step.
      VAR = nStatesArraysAtEvent( 1, : );             % state at firstEvent = 1 during this integration step.
      printCounterScreen = 0;
      printCounterFile   = 0;
      [isIntegrationFinished, VAR] = EventDetectedByIntegrator( t, VAR, nIndexOfEvents(1) );
   end
end
end


%=============================
end    % End of function test
%=============================
