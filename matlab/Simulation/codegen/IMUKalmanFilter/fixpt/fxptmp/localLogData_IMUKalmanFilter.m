function localLogData(out_Theta, out_dTheta, out_Bias, out_P, in_Theta, in_dTheta, in_Bias, in_P, in_Gyro, in_AccTheta, in_qw, in_qb, in_rw, in_rt)
global gEMLSimLogVal_out_Theta_IMUKalmanFilter;
global gEMLSimLogVal_out_dTheta_IMUKalmanFilter;
global gEMLSimLogVal_out_Bias_IMUKalmanFilter;
global gEMLSimLogVal_out_P_IMUKalmanFilter;
global gEMLSimLogVal_in_Theta_IMUKalmanFilter;
global gEMLSimLogVal_in_dTheta_IMUKalmanFilter;
global gEMLSimLogVal_in_Bias_IMUKalmanFilter;
global gEMLSimLogVal_in_P_IMUKalmanFilter;
global gEMLSimLogVal_in_Gyro_IMUKalmanFilter;
global gEMLSimLogVal_in_AccTheta_IMUKalmanFilter;
global gEMLSimLogVal_in_qw_IMUKalmanFilter;
global gEMLSimLogVal_in_qb_IMUKalmanFilter;
global gEMLSimLogVal_in_rw_IMUKalmanFilter;
global gEMLSimLogVal_in_rt_IMUKalmanFilter;
global gEMLSimLogRunIdx_IMUKalmanFilter;
persistent maxIdx;

if isempty(gEMLSimLogRunIdx_IMUKalmanFilter)
	gEMLSimLogRunIdx_IMUKalmanFilter = 1;
	maxIdx = 1;
		gEMLSimLogVal_out_Theta_IMUKalmanFilter = {out_Theta};
		gEMLSimLogVal_out_dTheta_IMUKalmanFilter = {out_dTheta};
		gEMLSimLogVal_out_Bias_IMUKalmanFilter = {out_Bias};
		gEMLSimLogVal_out_P_IMUKalmanFilter = {out_P};
		gEMLSimLogVal_in_Theta_IMUKalmanFilter = {in_Theta};
		gEMLSimLogVal_in_dTheta_IMUKalmanFilter = {in_dTheta};
		gEMLSimLogVal_in_Bias_IMUKalmanFilter = {in_Bias};
		gEMLSimLogVal_in_P_IMUKalmanFilter = {in_P};
		gEMLSimLogVal_in_Gyro_IMUKalmanFilter = {in_Gyro};
		gEMLSimLogVal_in_AccTheta_IMUKalmanFilter = {in_AccTheta};
		gEMLSimLogVal_in_qw_IMUKalmanFilter = {in_qw};
		gEMLSimLogVal_in_qb_IMUKalmanFilter = {in_qb};
		gEMLSimLogVal_in_rw_IMUKalmanFilter = {in_rw};
		gEMLSimLogVal_in_rt_IMUKalmanFilter = {in_rt};
	gEMLSimLogRunIdx_IMUKalmanFilter = gEMLSimLogRunIdx_IMUKalmanFilter+1;
	return

end

if gEMLSimLogRunIdx_IMUKalmanFilter > maxIdx
	maxIdx = 2 * maxIdx;
		gEMLSimLogVal_out_Theta_IMUKalmanFilter(maxIdx, :) = {gEMLSimLogVal_out_Theta_IMUKalmanFilter{1}};
		gEMLSimLogVal_out_dTheta_IMUKalmanFilter(maxIdx, :) = {gEMLSimLogVal_out_dTheta_IMUKalmanFilter{1}};
		gEMLSimLogVal_out_Bias_IMUKalmanFilter(maxIdx, :) = {gEMLSimLogVal_out_Bias_IMUKalmanFilter{1}};
		gEMLSimLogVal_out_P_IMUKalmanFilter(maxIdx, :) = {gEMLSimLogVal_out_P_IMUKalmanFilter{1}};
		gEMLSimLogVal_in_Theta_IMUKalmanFilter(maxIdx, :) = {gEMLSimLogVal_in_Theta_IMUKalmanFilter{1}};
		gEMLSimLogVal_in_dTheta_IMUKalmanFilter(maxIdx, :) = {gEMLSimLogVal_in_dTheta_IMUKalmanFilter{1}};
		gEMLSimLogVal_in_Bias_IMUKalmanFilter(maxIdx, :) = {gEMLSimLogVal_in_Bias_IMUKalmanFilter{1}};
		gEMLSimLogVal_in_P_IMUKalmanFilter(maxIdx, :) = {gEMLSimLogVal_in_P_IMUKalmanFilter{1}};
		gEMLSimLogVal_in_Gyro_IMUKalmanFilter(maxIdx, :) = {gEMLSimLogVal_in_Gyro_IMUKalmanFilter{1}};
		gEMLSimLogVal_in_AccTheta_IMUKalmanFilter(maxIdx, :) = {gEMLSimLogVal_in_AccTheta_IMUKalmanFilter{1}};
		gEMLSimLogVal_in_qw_IMUKalmanFilter(maxIdx, :) = {gEMLSimLogVal_in_qw_IMUKalmanFilter{1}};
		gEMLSimLogVal_in_qb_IMUKalmanFilter(maxIdx, :) = {gEMLSimLogVal_in_qb_IMUKalmanFilter{1}};
		gEMLSimLogVal_in_rw_IMUKalmanFilter(maxIdx, :) = {gEMLSimLogVal_in_rw_IMUKalmanFilter{1}};
		gEMLSimLogVal_in_rt_IMUKalmanFilter(maxIdx, :) = {gEMLSimLogVal_in_rt_IMUKalmanFilter{1}};
end

	gEMLSimLogVal_out_Theta_IMUKalmanFilter(gEMLSimLogRunIdx_IMUKalmanFilter, :) = {out_Theta};
	gEMLSimLogVal_out_dTheta_IMUKalmanFilter(gEMLSimLogRunIdx_IMUKalmanFilter, :) = {out_dTheta};
	gEMLSimLogVal_out_Bias_IMUKalmanFilter(gEMLSimLogRunIdx_IMUKalmanFilter, :) = {out_Bias};
	gEMLSimLogVal_out_P_IMUKalmanFilter(gEMLSimLogRunIdx_IMUKalmanFilter, :) = {out_P};
	gEMLSimLogVal_in_Theta_IMUKalmanFilter(gEMLSimLogRunIdx_IMUKalmanFilter, :) = {in_Theta};
	gEMLSimLogVal_in_dTheta_IMUKalmanFilter(gEMLSimLogRunIdx_IMUKalmanFilter, :) = {in_dTheta};
	gEMLSimLogVal_in_Bias_IMUKalmanFilter(gEMLSimLogRunIdx_IMUKalmanFilter, :) = {in_Bias};
	gEMLSimLogVal_in_P_IMUKalmanFilter(gEMLSimLogRunIdx_IMUKalmanFilter, :) = {in_P};
	gEMLSimLogVal_in_Gyro_IMUKalmanFilter(gEMLSimLogRunIdx_IMUKalmanFilter, :) = {in_Gyro};
	gEMLSimLogVal_in_AccTheta_IMUKalmanFilter(gEMLSimLogRunIdx_IMUKalmanFilter, :) = {in_AccTheta};
	gEMLSimLogVal_in_qw_IMUKalmanFilter(gEMLSimLogRunIdx_IMUKalmanFilter, :) = {in_qw};
	gEMLSimLogVal_in_qb_IMUKalmanFilter(gEMLSimLogRunIdx_IMUKalmanFilter, :) = {in_qb};
	gEMLSimLogVal_in_rw_IMUKalmanFilter(gEMLSimLogRunIdx_IMUKalmanFilter, :) = {in_rw};
	gEMLSimLogVal_in_rt_IMUKalmanFilter(gEMLSimLogRunIdx_IMUKalmanFilter, :) = {in_rt};
gEMLSimLogRunIdx_IMUKalmanFilter = gEMLSimLogRunIdx_IMUKalmanFilter+1;

