function [Theta, dTheta, Bias] = IMUKalmanFilter(Gyro, Ap, Ar)
global T pb pw;
persistent P_last Theta_last dTheta_last Bias_last;

if isempty(Theta_last)
   Theta_last = 0;
   dTheta_last = 0;
   Bias_last = 0.001;
   P_last = [T*pw, 0.5*T^2*pw, 0; 0.5*T^2*pw, T^3*pw/3, 0; 0, 0, T*pb];
end

TimeUpdate(Theta_last, dTheta_last, Bias_last);
end

function [Theta, dTheta, Bias, P] = TimeUpdate(Theta_prior, dTheta_prior, Bias_prior, P_prior)
    global T qw qb;
    
    % State Update
    dTheta = dTheta_prior;
    Theta  = T*dTheta_prior + Theta_prior;
    Bias = Bias_prior;
    
    % Covariance Update
    Pbb_prior = P_prior(3,3);
    Ptb_prior = P_prior(2,3);
    Ptt_prior = P_prior(2,2);
    Pwb_prior = P_prior(1,3);
    Pwt_prior = P_prior(1,2);
    Pww_prior = P_prior(1,1);
    
    Pbb = Pbb_prior + T*qb;
    Ptb = T*Pwb_prior+Ptb_prior;
    Ptt = Ptt_prior + T^2*Pww_prior + 2*T*Pwt_prior + T^3*qw/3;
    Pwb = Pwb_prior;
    Pwt = T*Pww_prior + Pwt_prior + 1/2*T^2*qw;
    Pww = Pww_prior + T*qw;
    
    P = [Pww, Pwt, Pwb; Pwt, Ptt, Ptb; Pwb, Ptb, Pbb];
end

function GyroCorrection(Theta_p, dTheta_p, Bias_p, P_p
