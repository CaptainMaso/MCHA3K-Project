function pend_plot(I,c, t, theta, dtheta)

global M l g T N

%% Sim
pend_odet = @(t,y)pend_ode(t,y,I,c);
    
[tsim, y] = ode23(pend_odet, t, [theta(1), dtheta(1)]);
thetasim = y(:,1);
dthetasim = y(:,2);

%% Plot Results

figure;
hold on;
subplot(2,1,1);
plot(t, theta, 'b-', tsim, thetasim(:, 1), 'r--');
%plot(tsim, thetasim(:, 1), 'r--');
legend('Data', 'Sim');
xlim([min([t; tsim]) max([t; tsim])]);
grid on;
subplot(2,1,2);
plot(t, dtheta, 'b-', tsim, dthetasim(:, 1), 'r--');
%plot(tsim, dthetasim(:, 1), 'r--');
legend('Data', 'Sim');
xlim([min([t; tsim]) max([t; tsim])]);
grid on;
hold off;

end

function dydt = pend_ode(t, y, I, c)
    global M l g;
    theta = y(1);
    dtheta = y(2);
    dydt = [dtheta; ((-c * dtheta) - M*l*g*sin(theta))/I];
end