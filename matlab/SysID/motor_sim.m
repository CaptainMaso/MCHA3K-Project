function [t, omega, current] = motor_sim(T, t_end, V, omega0, current0, N, R, L, K, Eta, J, Tm)

    A = [-R/L, -K*N/L; K*N*Eta/J, 0];
    B = [1/L; 0];
    C = [K*N*Eta, 0];
    D = 0;
    t_tmp = 0:T:t_end;
    U_tmp = V;
    x0_tmp = [omega0, current0];
    DC_sym = ss(A,B,C,D,0, 'StateName', {'Velocity', 'Current'}, 'InputName', 'Voltage', 'OutputName', 'Torque');
    [~, t, xsim] = lsim(DC_sym, U_tmp, t_tmp, x0_tmp);
    omega = xsim(:,1);
    current = xsim(:,2);