clc;clear;
%% Get Parameters
% Physical World Parameters
g = 9.81;           % Acceleration due to gravity (m/s/s)
alpha = 0*pi/180;   % Ground slope (rad = deg*pi/180)

% Initial Conditions & other sim params
theta0 = 25 * pi/180;    % Initial Theta

tsim = 40;              % Simulation Time (s)
vref_step_point = 500e-3;  % Set point of vref (m/s)
inpdist_step_point = 0.05;


run('Chassis_ss.m')
run('ControllerDesign.m')

%% Simulation
sim('model_sf');

%% Plot Results
h = gcf;
clf(gcf);
hold on;
subplot(4,2,1);
plot(time, theta*180/pi, 'b-');
xlabel('Time (s)', 'FontSize', 11);
ylabel('\theta (\circ)', 'FontSize', 14);
legend('Actual');
grid on;

subplot(4,2,3);
plot(time, dtheta*180/pi);
xlabel('Time (s)', 'FontSize', 11);
ylabel('d\theta/dt (\circ)', 'FontSize', 14);
legend('Actual');
grid on;

subplot(4,2,5);
plot(time, (phi + theta) * r);
xlabel('Time (s)', 'FontSize', 11);
ylabel('x (m)', 'FontSize', 14);
legend('Actual');
grid on;

subplot(4,2,7);
plot(time, (dphi + dtheta) .* r, 'b-', time, vref, 'r--');
xlabel('Time (s)', 'FontSize', 11);
ylabel('dx/dt (m/s)', 'FontSize', 14);
legend('Actual', 'Demanded');
grid on;

subplot(4,2,2);
plot(Torque.Time, Torque.Data*10, 'b-', DesiredTorque.Time, DesiredTorque.Data*10, 'r--');
xlabel('Time (s)', 'FontSize', 11);
ylabel('Control Torque (N.cm)', 'FontSize', 14);
legend('Actual', 'Desired');
ylim([-1 1]);
grid on;

subplot(4,2,6);
plot(time, ML_Current*10^3, 'r-', time, MR_Current*10^3, 'b-');
xlabel('Time (s)', 'FontSize', 11);
ylabel('Current (mA)', 'FontSize', 14);
legend('Left', 'Right');
grid on;

subplot(4,2,4);
plot(ML_Voltage.Time, ML_Voltage.Data, 'r-', MR_Voltage.Time, MR_Voltage.Data, 'b-');
legend('Left', 'Right');
ylabel('Control Voltage (V)', 'FontSize', 14);
xlabel('Time (s)', 'FontSize', 11);
ylim([-15, 15]);
grid on;

h.NextPlot = 'add';
a = axes;
ht = title(['IC: \theta = ' num2str(theta0*180/pi) '\circ'], 'FontSize', 16);
a.Visible = 'off';
ht.Visible = 'on';
hold off;