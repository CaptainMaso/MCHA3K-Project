function J = motor_freeinertia(data, K, Neta, Tm)
    %% Params
    global T CPR ADC_GAIN ADC_MAX ADC_VREF ADC_RESISTOR;
    
    N = 17;

    LOWDATA = 100;
    HIGHDATA = 800;
    
    DDTHETA = zeros(HIGHDATA - LOWDATA - 2*N + 1, size(data,1));

    CURRENT = zeros(HIGHDATA - LOWDATA - 2*N + 1, size(data,1));
    
    for i = 1:size(data,1)
        %% Get Data
        [~,~,ENC,ADC] = import_motor_free(data(i, :), LOWDATA, HIGHDATA);

        %% Format Data
        theta_tmp = ENC * 2 * pi/CPR;
        cur_tmp = ADC*ADC_VREF/ADC_GAIN/ADC_MAX/ADC_RESISTOR/2;

        %% Get Derivatives
        [~, ~, ddtheta_tmp] = quadraticSavitzkyGolay(theta_tmp',T,N);  
        DDTHETA(:, i) = ddtheta_tmp(N+1:end-N)';
        CURRENT(:, i) = cur_tmp(N+1:end-N);

        J_lsqf(i) = motor_freeinertia_lsqf(DDTHETA(:,i), CURRENT(:,i), K, Neta, Tm);
    end
    
    J = mean(J_lsqf);
end

function J = motor_freeinertia_lsqf(i, dw, K, Neta, Tm)
    global GearRatio;
    %% Sum of Torques
    % J*dw = K*N*Neta*i - N*Neta*Tm

    Y = dw;
    Phi = [K*GearRatio*Neta*i + GearRatio*Neta*Tm];
    % Theta = [1/J]

    Theta = lsqr(Phi, Y);
    J = Theta;
end