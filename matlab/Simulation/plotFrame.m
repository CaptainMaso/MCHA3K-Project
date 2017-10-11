function plotFrame(theta, phi, t)
r = 0.045;
alpha = pi/16;

if (length(theta) == length(phi))
    x = r*(theta + phi);
    xmin = min(x);
    xmax = max(x);
    for i = 1:length(theta)
        figure;
        hold on;
        daspect([1 1 1]);
        % Plots slope
        plot([xmin xmax], [xmin xmax]*tan(alpha), 'b')
        
        %Plots Wheel
        viscircles([x(i), r + x(i)*tan(alpha)], r);
        
        xlim([xmin, xmax]);
        grid on;
        hold off;
    end
end