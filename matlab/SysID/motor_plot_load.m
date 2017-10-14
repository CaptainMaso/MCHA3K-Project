function motor_plot_load(data, Title)
    global T CPR ADC_GAIN ADC_MAX ADC_VREF ADC_RESISTOR Vmax GearRatio;

    N = 17;

    LOWDATA = 200;
    HIGHDATA = 300;

    t = zeros(HIGHDATA - LOWDATA - 2*N + 1, size(data,1));
    
    V = zeros(HIGHDATA - LOWDATA - 2*N + 1, size(data,1));
    
    THETA = zeros(HIGHDATA - LOWDATA - 2*N + 1, size(data,1));
    DTHETA = zeros(HIGHDATA - LOWDATA - 2*N + 1, size(data,1));
    DDTHETA = zeros(HIGHDATA - LOWDATA - 2*N + 1, size(data,1));

    CURRENT = zeros(HIGHDATA - LOWDATA - 2*N + 1, size(data,1));
    DCURRENT = zeros(HIGHDATA - LOWDATA - 2*N + 1, size(data,1));

    for i = 1:size(data,1)
        %% Get Data
        [t_tmp, h_tmp,v_tmp,ENC,ADC] = import_motor_load(data(i, :), LOWDATA, HIGHDATA);

        %% Format Data
        theta_tmp = ENC * 2 * pi/CPR;
        cur_tmp = ADC*ADC_VREF/ADC_GAIN/ADC_MAX/ADC_RESISTOR/2;
        v_tmp = Vmax.*sqrt(abs(v_tmp/Vmax)).*sign(v_tmp);

        %% Get Derivatives
        [theta_tmp, dtheta_tmp, ddtheta_tmp] = quadraticSavitzkyGolay(theta_tmp',T,N);  
        V(:, i) = v_tmp(N+1:end-N);
        THETA(:, i) = theta_tmp(N+1:end-N)';
        DTHETA(:, i) = dtheta_tmp(N+1:end-N)';
        DDTHETA(:, i) = ddtheta_tmp(N+1:end-N)';

        [cur_tmp,dcur_tmp, ~] = quadraticSavitzkyGolay(cur_tmp',T,N);  
        CURRENT(:, i) = cur_tmp(N+1:end-N)';
        DCURRENT(:, i) = dcur_tmp(N+1:end-N)';

        t(:, i) = t_tmp(N+1:end-N);
        t(:, i) = t(:, i) - t(1, i);
    end

    figure;
    subplot(6,1,1);
    plot(t, V);
    title([Title, ': V_{RMS}']);
    ylabel('Voltage (V)');
    xlim([0 max(t(:))]);
    grid on;

    subplot(6, 1,2);
    plot(t, THETA);
    title([Title, ': Position']);
    ylabel('Position (rads)');
    xlim([0 max(t(:))]);
    grid on;

    subplot(5,1,2);
    plot(t, DTHETA);
    title([Title, ': Velocity']);
    ylabel('Velocity (rads/s)');
    xlim([0 max(t(:))]);
    grid on;
    
    subplot(5,1,3);
    plot(t, DDTHETA);
    title([Title, ': Acceleration']);
    ylabel('Acceleration (rads/s^2)');
    xlim([0 max(t(:))]);
    grid on;

    subplot(5,1,4);
    plot(t, CURRENT*10^3);
    title([Title, ': Current']);
    ylabel('Current (mA)');
    xlim([0 max(t(:))]);
    grid on;

    subplot(5,1,5);
    plot(t, DCURRENT);
    title([Title, ': Current/s']);
    ylabel('Current (A/s)');
    xlim([0 max(t(:))]);
    grid on;
end