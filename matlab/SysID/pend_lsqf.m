function [I,c] = pend_lsqf(theta, dtheta, ddtheta)
    %% Estimating parameters
    global M l g N;
    Phi = [ddtheta, dtheta];
    Y = -M*l*g*sin(theta);

    Res = lsqr(Phi, Y);
    I = Res(1);
    c = Res(2);
end