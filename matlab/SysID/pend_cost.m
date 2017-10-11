function cost = pend_cost(I, c, t, theta, dtheta)
    tcost = zeros(size(t));
    pend_odet = @(t,y) pend_ode(t,y,I,c);
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

function dydt = pend_ode(t, y, I, c)
    global M l g;
    theta = y(1);
    dtheta = y(2);
    dydt = [dtheta; (-c/I * dtheta) - M*l*g*sin(theta)/I];
end