function [R, L, K, Eta, J, Tm] = minimiseMotorCost(data, R0, L0, K0, Eta0, J0, Tm0)
    %% Params
    global T GearRatio CPR ADC_GAIN ADC_MAX ADC_VREF ADC_RESISTOR Vmax;
    
    N = 17;

    LOWDATA = 100;
    HIGHDATA = 4000;

    t = zeros(HIGHDATA - LOWDATA - 2*N + 1, size(data,1));
    
    V = zeros(HIGHDATA - LOWDATA - 2*N + 1, size(data,1));
    
    THETA = zeros(HIGHDATA - LOWDATA - 2*N + 1, size(data,1));
    DTHETA = zeros(HIGHDATA - LOWDATA - 2*N + 1, size(data,1));
    DDTHETA = zeros(HIGHDATA - LOWDATA - 2*N + 1, size(data,1));

    CURRENT = zeros(HIGHDATA - LOWDATA - 2*N + 1, size(data,1));
    DCURRENT = zeros(HIGHDATA - LOWDATA - 2*N + 1, size(data,1));
    
    for i = 1:size(data,1)
        %% Get Data
        [t_tmp,v_tmp,ENC,ADC] = import_motor_free(data{i}, LOWDATA, HIGHDATA);

        %% Format Data
        theta_tmp = ENC * 2 * pi/CPR;
        cur_tmp = ADC*ADC_VREF/ADC_GAIN/ADC_MAX/ADC_RESISTOR/2;
        v_tmp = Vmax.*sqrt(abs(v_tmp/Vmax)).*sign(v_tmp);

        %% Get Derivatives
        [theta_tmp, dtheta_tmp, ddtheta_tmp] = quadraticSavitzkyGolay(theta_tmp',T,N);  
        V(:, i) = v_tmp(N+1:end-N);
        THETA(:, i) = theta_tmp(N+1:end-N)';
        DTHETA(:, i) = dtheta_tmp(N+1:end-N)';
        DDTHETA(:, i) = ddtheta_tmp(N+1:end-N)';

        [cur_tmp,dcur_tmp, ~] = quadraticSavitzkyGolay(cur_tmp',T,N);  
        CURRENT(:, i) = cur_tmp(N+1:end-N)';
        DCURRENT(:, i) = dcur_tmp(N+1:end-N)';

        t(:, i) = t_tmp(N+1:end-N);
        t(:, i) = t(:, i) - t(1, i);
    end
    
    %% Minimising Error Function
    minfunc = @(x) motor_cost(t, T, CURRENT, DTHETA, V, GearRatio, x(1), x(2), x(3), x(4), x(5), Tm0);
    [res, cost] = fminunc(minfunc, [R0, L0, K0, Eta0, J0]);
    
    R = R0;%res(1);
    L = L0;%res(2);
    K = K0;%res(3);
    Eta = Eta0;%res(4);
    J = J0;%res(5);
    Tm = Tm0;%res(6);
    
    [tsim, omegasim, currentsim] = motor_sim(T, max(t), V(:,1), DTHETA(1,1), CURRENT(1,1), GearRatio, R, L, K, Eta, J, Tm);

    plot(t, DTHETA(:,1), 'b', tsim, omegasim, 'r');
    figure;
    plot(t, CURRENT(:,1), 'b', tsim, currentsim, 'r');
end

function cost = motor_cost(t, T, Current, Omega, V, N, R, L, K, Eta, J, Tm)    
    for i = 1:size(t,2)    
        [~,omegasim, currentsim] = motor_sim(T, max(t), V, Omega(1), Current(1), N, R, L, K, Eta, J, Tm);
        cost(i) = meansqr([Omega(:,i) - omegasim, Current(:,i) - currentsim]);
    end
    cost = mean(cost);
end
