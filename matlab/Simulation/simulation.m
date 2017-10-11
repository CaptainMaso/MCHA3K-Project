clear;
%% Model Parameters
M = 1;              % Mass of cart (kg)
m = 0.5;          	% Mass of pendulum (kg)
l = 0.5;            % Length of pendulum (m)
b = 0.005;           % Damping of cart (N.s/m)
c = 0.005;           % Damping of pendulum (N.m.s/rad)
J = 3.22*10^-6;     % Wheel Moment of Inertia
I = 78.125*10^-6;   % Moment of Inertia
r = 45*10^-3;       % Wheel Radius

% Other parameters
g = 9.82;               % Acceleration due to gravity (m/s/s)
alpha = 0*pi/180;       % Ground slope (rad = deg*pi/180)
theta0 = 5 * pi/180;   % Initial Theta

%% Controller Parameters
%

%% Simulation

sim('model');

%% Plot Results
h = gcf;
clf(gcf);
hold on;
subplot(3,2,1);
plot(time, theta*180/pi, 'b-', time, thetaref, 'r--');
xlabel('Time (s)', 'FontSize', 11);
ylabel('\theta (\circ)', 'FontSize', 14);
legend('Actual', 'Demanded');
grid on;
%ylim([-360, 360])
%set(gca,'ytick', -360:90:360);

subplot(3,2,2);
plot(time, thetadot*180/pi);
xlabel('Time (s)', 'FontSize', 11);
ylabel('d\theta/dt (\circ)', 'FontSize', 14);
grid on;

subplot(3,2,3);
plot(time, (phi + theta) * r);
xlabel('Time (s)', 'FontSize', 11);
ylabel('x (m)', 'FontSize', 14);
grid on;

subplot(3,2,4);
plot(time, (phidot + thetadot) .* r, 'b-', time, vref, 'r--');
xlabel('Time (s)', 'FontSize', 11);
ylabel('dx/dt (m/s)', 'FontSize', 14);
legend('Actual', 'Demanded');
grid on;

subplot(3,2,5:6);
plot(time, u);
xlabel('Time (s)', 'FontSize', 11);
ylabel('Control Torque (Nm)', 'FontSize', 14);
grid on;

h.NextPlot = 'add';
a = axes;
ht = title(['IC: \theta = ' num2str(theta0*180/pi) '\circ'], 'FontSize', 16);
a.Visible = 'off';
ht.Visible = 'on';
hold off;