clear
clc

%User Defined Properties 
serialPort = 'COM1';            % define COM port #
plotTitle = 'Serial Data Log';  % plot title
xLabel1 = 'Elapsed Time (s)';    % x-axis label
yLabel1 = 'Theta';                % y-axis label
xLabel2 = 'Elapsed Time (s)';    % x-axis label
yLabel2 = 'dTheta';                % y-axis label
xLabel3 = 'Elapsed Time (s)';    % x-axis label
yLabel3 = 'Bias';                % y-axis label
plotGrid = 'on';                % 'off' to turn off grid
min = -180;                     % set y-min
max = 180;                      % set y-max
scrollWidth = 10;               % display period in plot, plot entire data log if <= 0
delay = .0000001;                    % make sure sample faster than resolution

%Define Function Variables
time = 0;
data = zeros(8,1);
count = 0;

%Set up Plot
figure;
subplot(3,3,1);
hold on;
plotGraph_THETA = plot(time,data(1,:),'-r',...
            'LineWidth',2,...
            'MarkerFaceColor','w',...
            'MarkerSize',2);      
title(plotTitle,'FontSize',25);
xlabel(xLabel1,'FontSize',15);
ylabel(yLabel1,'FontSize',15);
axis([0 10 min max]);
grid(plotGrid);
hold off;

subplot(3,3,2);
hold on;
plotGraph_THETAM = plot(time,data(2,:),'-r',...
            'LineWidth',2,...
            'MarkerFaceColor','w',...
            'MarkerSize',2);        
title(plotTitle,'FontSize',25);
xlabel(xLabel1,'FontSize',15);
ylabel('Meas. Theta','FontSize',15);
axis([0 10 min max]);
grid(plotGrid);
hold off;

subplot(3,3,4);
hold on
plotGraph_DTHETA = plot(time,data(3,:),'-m',...
            'LineWidth',1,...
            'MarkerFaceColor','w',...
            'MarkerSize',2);
xlabel(xLabel2,'FontSize',15);
ylabel(yLabel2,'FontSize',15);
axis([0 10 min max]);
grid(plotGrid);

subplot(3,3,5);
hold on
plotGraph_DTHETAM = plot(time,data(4,:),'-m',...
    'LineWidth',1,...
    'MarkerFaceColor','w',...
    'MarkerSize',2);
xlabel(xLabel2,'FontSize',15);
ylabel('Meas. dTheta','FontSize',15);
axis([0 10 min max]);
grid(plotGrid);

hold on
subplot(3,3,7);
plotGraph_BIAS = plot(time,data(5,:),'-m',...
            'LineWidth',1,...
            'MarkerFaceColor','w',...
            'MarkerSize',2);
xlabel(xLabel3,'FontSize',15);
ylabel(yLabel3,'FontSize',15);
axis([0 10 min max]);
grid(plotGrid);

hold on;
subplot(3,3,3);
plotGraph_PWW = plot(time,data(6,:),'-r',...
            'LineWidth',2,...
            'MarkerFaceColor','w',...
            'MarkerSize',2);
title(plotTitle,'FontSize',25);
xlabel('Time (s)','FontSize',15);
ylabel('P_{WW}','FontSize',15);
axis([0 10 -1 1]);
grid(plotGrid);

hold on
subplot(3,3,6);
plotGraph_PTT = plot(time,data(7,:),'-m',...
            'LineWidth',1,...
            'MarkerFaceColor','w',...
            'MarkerSize',2);
xlabel('Time (s)','FontSize',15);
ylabel('P_{TT}','FontSize',15);
axis([0 10 -1 1]);
grid(plotGrid);

hold on
subplot(3,3,9);
plotGraph_PBB = plot(time,data(8,:),'-m',...
            'LineWidth',1,...
            'MarkerFaceColor','w',...
            'MarkerSize',2);
xlabel('Time (s)','FontSize',15);
ylabel('P_{BB}','FontSize',15);
axis([0 10 -1 1]);
grid(plotGrid);

%Open Serial COM Port
s = serial(serialPort, 'BaudRate', 115200)
disp('Close Plot to End Session');
fopen(s);

tic

while ishandle(plotGraph_BIAS)  %Loop when Plot is Active

dat = fscanf(s,'T:%f, mT:%f, dT:%f, mdT:%f, B:%f, WW: %f, TT: %f, BB: %f\n'); %Read Data from Serial as Float

if(~isempty(dat) && isfloat(dat)) %Make sure Data Type is Correct        
    count = count + 1;    
    time(count) = toc;    %Extract Elapsed Time in seconds
    data(:,count) = dat(:,1); %Extract 1st Data Element         

    %Set Axis according to Scroll Width
    if(scrollWidth > 0)
        set(plotGraph_THETA,'XData',time(time > time(count)-scrollWidth),...
            'YData', data(1,time > time(count)-scrollWidth));
        set(plotGraph_THETAM,'XData',time(time > time(count)-scrollWidth),...
            'YData', data(2,time > time(count)-scrollWidth));
        set(plotGraph_DTHETA,'XData',time(time > time(count)-scrollWidth),...
            'YData', data(3,time > time(count)-scrollWidth));
        set(plotGraph_DTHETAM,'XData',time(time > time(count)-scrollWidth),...
            'YData', data(4,time > time(count)-scrollWidth));
        set(plotGraph_BIAS,'XData',time(time > time(count)-scrollWidth),...
            'YData', data(5,time > time(count)-scrollWidth));
        set(plotGraph_PWW,'XData',time(time > time(count)-scrollWidth),...
            'YData', data(6,time > time(count)-scrollWidth));
        set(plotGraph_PTT,'XData',time(time > time(count)-scrollWidth),...
            'YData', data(7,time > time(count)-scrollWidth));
        set(plotGraph_PBB,'XData',time(time > time(count)-scrollWidth),...
            'YData', data(8,time > time(count)-scrollWidth));
        
        subplot(3,3,1);
        axis([time(count)-scrollWidth time(count) min max]);
        subplot(3,3,2);
        axis([time(count)-scrollWidth time(count) min max]);
        subplot(3,3,4);
        axis([time(count)-scrollWidth time(count) min max]);
        subplot(3,3,5);
        axis([time(count)-scrollWidth time(count) min max]);
        subplot(3,3,7);
        axis([time(count)-scrollWidth time(count) min max]);
        
        subplot(3,3,3);
        axis([time(count)-scrollWidth time(count) 0 5]);
        subplot(3,3,6);
        axis([time(count)-scrollWidth time(count) 0 5]);
        subplot(3,3,9);
        axis([time(count)-scrollWidth time(count) 0 50]);
    else
        set(plotGraph_THETA,'XData',time,'YData',data(1,:));
        set(plotGraph_DTHETA,'XData',time,'YData',data(2,:));
        set(plotGraph_BIAS,'XData',time,'YData',data(3,:));
        set(plotGraph_PWW,'XData',time,'YData',data(4,:));
        set(plotGraph_PTT,'XData',time,'YData',data(5,:));
        set(plotGraph_PBB,'XData',time,'YData',data(6,:));
end

    %Allow MATLAB to Update Plot
    pause(delay);
end
end

%Close Serial COM Port and Delete useless Variables
fclose(s);

clear count dat delay max min plotGraph plotGraph1 plotGraph2 plotGrid...
    plotTitle s scrollWidth serialPort xLabel yLabel;

disp('Session Terminated');

% prompt = 'Export Data? [Y/N]: ';
% str = input(prompt,'s');
% if str == 'Y' || strcmp(str, ' Y') || str == 'y' || strcmp(str, ' y')
%     %export data
%     csvwrite('accelData.txt',data);
%     type accelData.txt;
% else
% end
% 
% clear str prompt;