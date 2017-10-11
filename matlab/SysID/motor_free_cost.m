function cost = motor_free_cost(L, R, K, t, theta, dtheta)
    tcost = zeros(size(t));
    pend_odet = @(t,y) pend_ode(t,y, L, R, K);
    for i = 1:size(t,2)
        ic = [theta(1, i), dtheta(1, i)];
        t_tmp = t(:, i);
        [tsim, y] = ode45(pend_odet, t_tmp, ic);
        thetasim = y(:, 1);
        tcost(:, i) = theta(:, i) - thetasim;
        meansqr(
    end
    cost = meansqr(tcost);
end

function didt = motor_free_ode(t, y, L, R, K)
    global GearRatio;
    
    didt = (V - R*i -K*N*w)/L
end