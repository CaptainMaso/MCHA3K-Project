function KalmanExport(T_kf, T_ts, K_BITSHIFT)
FID = fopen('C:\Users\masom\OneDrive\Documents\Uni\MCHA3000\Project\src\kalman_param.def', 'w+');
format long;
if FID ~= -1
    pw = 100.0;
    pb = 10000.0;
    qw = 300.0;
    qb = 0.0005;
    rt = 0.001;
    rw = 0.0001;

    ps = 1000.0;
    qs = 5000.0;
    rp = 0.00001;
    
    max_gy = 500*pi/180;
    
    K_FP_GAIN = 2^K_BITSHIFT;

    table_len = 10;
    table_input = linspace(0, 1, table_len);
    table = atan(table_input);
    
    fprintf(FID, '\n#ifdef FIXEDPOINT_KALMAN\n');
    
    fprintf(FID, '\nstatic const uint32_t GY2RAD = %d;\n', round(K_FP_GAIN*(max_gy/2^15)));
    
    fprintf(FID, '\nstatic const uint32_t K_pw = %d;\n', round(K_FP_GAIN*pw));
    fprintf(FID, '\nstatic const uint32_t K_pb = %d;\n', round(K_FP_GAIN*pb));
    fprintf(FID, '\nstatic const uint32_t K_qw = %d;\n', round(K_FP_GAIN*qw));
    fprintf(FID, '\nstatic const uint32_t K_qb = %d;\n', round(K_FP_GAIN*qb));
    fprintf(FID, '\nstatic const uint32_t K_RT_T = %d;\n', round(K_FP_GAIN*(rt/T_kf)));
    fprintf(FID, '\nstatic const uint32_t K_RW_T = %d;\n', round(K_FP_GAIN*(rw/T_kf)));
    
    fprintf(FID, '\nstatic const uint32_t K_ps = %d;\n', round(K_FP_GAIN*ps));
    fprintf(FID, '\nstatic const uint32_t K_qs = %d;\n', round(K_FP_GAIN*qs));
    fprintf(FID, '\nstatic const uint32_t K_RP_T = %d;\n', round(K_FP_GAIN*(rp/T_kf)));
    
    fprintf(FID, '\nstatic const uint32_t T_KF_FP = %d;\n', round(K_FP_GAIN*T_kf));
    
    fprintf(FID, '\nstatic const uint32_t T_TS = %d;\n', round(K_FP_GAIN*T_ts));
    
    fprintf(FID, '\nstatic const uint8_t atan_table_len = %d;', table_len);
    fprintf(FID, '\nstatic const uint32_t atan_table[] = {%d', round(table(1)*K_FP_GAIN));
    fprintf(FID, ', %d', round(table(2:end)*K_FP_GAIN));
    fprintf(FID, '};\n'); 
    
    fprintf(FID, '#else\n');
    
    fprintf(FID, '\nstatic const float GY2RAD = %.6f;\n', (max_gy/(2^15)));
    
    fprintf(FID, '\nstatic const float K_pw = %.6f;\n', pw);
    fprintf(FID, '\nstatic const float K_pb = %.6f;\n', pb);
    fprintf(FID, '\nstatic const float K_qw = %.6f;\n', qw);
    fprintf(FID, '\nstatic const float K_qb = %.6f;\n', qb);
    fprintf(FID, '\nstatic const float K_RT_T = %.6f;\n', rt/T_kf);
    fprintf(FID, '\nstatic const float K_RW_T = %.6f;\n', rw/T_kf);
    
    fprintf(FID, '\nstatic const float K_ps = %.6f;\n', ps);
    fprintf(FID, '\nstatic const float K_qs = %.6f;\n', qs);
    fprintf(FID, '\nstatic const float K_RP_T = %.6f;\n', rp/T_kf);
    
    fprintf(FID, '\nstatic const float T_TS = %.6f;\n', T_ts);

    fprintf(FID, '\nstatic const uint8_t atan_table_len = %d;', table_len);
    fprintf(FID, '\nstatic const float atan_table[] = {%.6f', table(1));
    fprintf(FID, ', %.6f', table(2:end));
    fprintf(FID, '};\n'); 
    
    fprintf(FID, '#endif\n\n');
    
    fclose(FID);
else
    fprintf('ERROR: Could not open file\n');
end