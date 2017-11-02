function res = atan2lookup(y,x)
    global table table_length;
    table_length = 10;
    table_input = linspace(0, 1, table_length);
    table = atan(table_input);
    r = 1;
    x = linspace(1, -1, 100);
    y = sign(x).*sqrt(r.^2-x.^2);
    for i = 1:size(x,1);
        ang(i) = atan2fcn(x(i),y(i));
    end
    plot(x, y, 'xb', cos(ang), sin(ang), 'or');
    xlim([-1.2, 1.2]);
    ylim([-1.2, 1.2]);
    grid on;
end
function res = atan2fcn(y, x)
    global table table_length;
    if (abs(y) > abs(x))
        if (abs(x) == 0)
            res = pi/2;
        else
            low = floor(abs(x)/abs(y)*table_length);
            high = ceil(abs(x)/abs(y)*table_length);
            res = pi/2 - (table(low)+table(high))/2;
        end
    else
        if (abs(y) == 0);
            res = 0;
        else
            low = floor(abs(y)/abs(x)*table_length);
            high = ceil(abs(y)/abs(x)*table_length);
            res = (table(low)+table(high))/2;
        end
    end

    if (y < 0)
        res = - res;
    end
    
    if (x < 0)
        res = pi - res;
    end
end