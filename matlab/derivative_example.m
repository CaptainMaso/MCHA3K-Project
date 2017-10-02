% Example demonstrating derivative estimation

T = 0.01;
t = 0:T:5;

x = sin(t.^2) + 0.01*randn(size(t));
dx = 2*t.*cos(t.^2);
ddx = 2*cos(t.^2)-4*t.^2.*sin(t.^2);

N = 3; % 2*N+1 points are used as local neighbourhood for quad fit

% Central difference derivatives
dx_diff = nan(size(t));
ddx_diff = nan(size(t));
for k = 1+N:length(t)-N
    dx_diff(k) = (x(k+1) - x(k-1))/(2*T);
    ddx_diff(k) = (x(k+1) - 2*x(k) + x(k-1))/T^2;
end

% Filtered derivatives
[~,dx_quad,ddx_quad] = quadraticSavitzkyGolay(x,T,N);

figure(1);clf
subplot(1,3,1)
plot(t,x,'LineWidth',2)
xlim([0 t(end)]);
xlabel('Time (sec)')
title('Original signal')
grid on

subplot(1,3,2)
plot(t,dx_diff,'b',t,dx_quad,'g',t,dx,'r:','LineWidth',2)
xlim([0 t(end)]);
xlabel('Time (sec)')
title('First derivative')
legend('Diff','Quad','True','Location','Best')
grid on

subplot(1,3,3)
plot(t,ddx_diff,'b',t,ddx_quad,'g',t,ddx,'r:','LineWidth',2)
xlim([0 t(end)]);
xlabel('Time (sec)')
title('Second derivative')
legend('Diff','Quad','True','Location','Best')
grid on