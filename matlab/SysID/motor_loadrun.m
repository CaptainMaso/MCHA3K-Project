function [Eta, Tm] = motor_loadrun(data, K, m)
    %% Params
    global ADC_GAIN ADC_MAX ADC_VREF ADC_RESISTOR;

    LOWDATA = 100;
    HIGHDATA = 400;
    
    % T0 = F*d = m*g*r
    To = m*9.81*(25.5e-3);
    
    V = zeros(size(data,1), 1);

    CURRENT = zeros(size(data,1), 1);
    
    for i = 1:size(data,1)
        %% Get Data
        [~, ~,v_tmp,~,ADC] = import_motor_load(data{i}, LOWDATA, HIGHDATA);

        %% Format Data
        cur_tmp = abs(ADC*ADC_VREF/ADC_GAIN/ADC_MAX/ADC_RESISTOR/2);
        
        [Eta_lsqr(i), Tm_lsqr(i)] = motor_load_lsqf(cur_tmp, K, To);
    end
    
    Eta = mean(Eta_lsqr);
    Tm = mean(Tm_lsqr);
end

function [Eta, Tm] = motor_load_lsqf(i, K, To)
    global GearRatio;
    % T0 = i + Tm

    Y = ones(size(i))*To;
    Phi = [i, ones(size(i))];
    % Theta = [K*Eta*N; N*Tm]

    Theta = lsqr(Phi, Y);
    Eta = Theta(2)/(K*GearRatio);
    Tm = Theta(1)/(Eta * GearRatio);
end