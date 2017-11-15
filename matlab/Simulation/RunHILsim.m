%% Get Parameters
% Initial Conditions & other sim params
theta0 = 2 * pi/180;    % Initial Theta

tsim = 20;              % Simulation Time (s)
vref_step_point = 500e-3;  % Set point of vref (m/s)
inpdist_step_point = 100e-3;

run('ControllerDesign.m')

%% Simulation
serialPort = 'COM1';
port = serial(serialPort, 'BaudRate', 115200);
set(port, 'DataTerminalReady', 'off');
fopen(port);

if port ~= -1
    fprintf(port, 'ctrl mode HIL\n');
    fprintf(port, 'ctrl set theta %f\n', theta0 + (rand(1)-0.5)*0.002);
    sim('model_hil');
    %fprintf(port, 'ctrl mode OFF\n');
    fprintf('Closed Port\n');
    fclose(port);
end

%%
thetactrl.Data = squeeze(thetactrl.Data);
dthetactrl.Data = squeeze(dthetactrl.Data);
phictrl.Data = squeeze(phictrl.Data);
dphictrl.Data = squeeze(dphictrl.Data);
biasctrl.Data = squeeze(biasctrl.Data);

%% Plot Results
figure;
hold on;
title(['States (IC: \theta = ' num2str(theta0*180/pi) '\circ)']);
subplot(4,1,1);
plot(theta.Time, theta.Data*180/pi, 'b-', thetactrl.Time, thetactrl.Data*180/pi, 'g-');
xlabel('Time (s)', 'FontSize', 11);
ylabel('\theta (\circ)', 'FontSize', 14);
legend('Actual', 'Estimated');
%ylim([-90 90]);
grid on;

subplot(4,1,2);
plot(dtheta.Time, dtheta.Data*180/pi, 'b-', dthetactrl.Time, dthetactrl.Data*180/pi, 'g--', biasctrl.Time, biasctrl.Data*180/pi, 'r--');
xlabel('Time (s)', 'FontSize', 11);
ylabel('d\theta/dt (\circ)', 'FontSize', 14);
legend('Actual', 'Estimated', 'Bias');
%ylim([-500 500]);
grid on;

subplot(4,1,3);
plot(phi.Time, (phi.Data + theta.Data).* r, 'b-', phictrl.Time, (phictrl.Data + thetactrl.Data).* r, 'g--');
xlabel('Time (s)', 'FontSize', 11);
ylabel('x (m)', 'FontSize', 14);
legend('Actual', 'Estimated');
%ylim([-10 10]);
grid on;

subplot(4,1,4);
plot(dphi.Time, (dphi.Data + dtheta.Data) .* r, 'b-', dphictrl.Time, (dphictrl.Data + dthetactrl.Data) .* r, 'g--', vref.Time, vref.Data, 'r--');
xlabel('Time (s)', 'FontSize', 11);
ylabel('dx/dt (m/s)', 'FontSize', 14);
legend('Actual', 'Estimated', 'Demanded');
%ylim([-2 2]);
grid on;

%%
figure;
subplot(3,1,1);
plot(Torque.Time, Torque.Data*10, 'b-', DesiredTorque.Time, DesiredTorque.Data*10, 'r--');
xlabel('Time (s)', 'FontSize', 11);
ylabel('Control Torque (N.cm)', 'FontSize', 14);
legend('Actual', 'Desired');
title(['Control Signals (IC: \theta = ' num2str(theta0*180/pi) '\circ)']);
ylim([-3 5]);
grid on;

subplot(3,1,2);
plot(time, ML_Current*10^3, 'r-', time, MR_Current*10^3, 'b-');
xlabel('Time (s)', 'FontSize', 11);
ylabel('Current (mA)', 'FontSize', 14);
legend('Left', 'Right');
grid on;

subplot(3,1,3);
plot(ML_Voltage.Time, ML_Voltage.Data, 'r-', MR_Voltage.Time, MR_Voltage.Data, 'b-');
legend('Left', 'Right');
ylabel('Control Voltage (V)', 'FontSize', 14);
xlabel('Time (s)', 'FontSize', 11);
ylim([-15, 15]);
grid on;
