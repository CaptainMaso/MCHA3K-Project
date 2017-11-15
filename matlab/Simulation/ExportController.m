function ExportController(T_main, T_kfcorr, K_BITSHIFT, Chassis_ctrl, ML_ctrl, ML_PARAM, MR_ctrl, MR_PARAM)

    ExportMainCtrl(T_main, T_kfcorr, K_BITSHIFT, Chassis_ctrl, ML_ctrl);
    ExportChassisCtrl(Chassis_ctrl, K_BITSHIFT);
    ExportMotorCtrl(K_BITSHIFT, ML_ctrl, ML_PARAM, MR_ctrl, MR_PARAM);
end

function ExportMainCtrl(T_main, T_kfcorr, K_BITSHIFT, Chassis_ctrl, ML_ctrl)
    FID = fopen('C:\Users\masom\OneDrive\Documents\Uni\MCHA3000\Project\src\ctrl_param.def', 'w+');
    format long;
    if FID ~= -1
        [~,~,~,~,T_pend] = ssdata(Chassis_ctrl);
        [~,~,~,~,T_motors] = ssdata(ML_ctrl);
        
        K_FP_GAIN = 2^K_BITSHIFT;
        
        fprintf(FID, '\nstatic const float T_main = %.6f;\n', T_main);
        fprintf(FID, 'static const uint32_t Main_loop_reset = %d;\n', 1/T_main);
        
        pend_interval = T_pend/T_main;
        fprintf(FID, '\nstatic const uint32_t pend_interval = %d;\n', pend_interval); 
        fprintf(FID, 'static const float T_pend = %.6f;\n', T_pend);   
        
        motor_interval = T_motors/T_main;
        fprintf(FID, '\nstatic const uint32_t motor_interval = %d;\n', motor_interval); 
        fprintf(FID, 'static const float T_motor = %.6f;\n', T_motors); 
        
        fprintf(FID, 'static const uint16_t K_FP_GAIN = %d;\n', K_FP_GAIN);
        fprintf(FID, 'static const int8_t K_FP_BITSHIFT = %d;\n', K_BITSHIFT);
        
        fprintf(FID, 'static const float T_KF = %.6f;\n', T_kfcorr);
        
        fclose(FID);
    else
        fprintf('ERROR: Could not open file\n');
    end
end

function ExportChassisCtrl(Chassis_ctrl, K_BITSHIFT)
    FID = fopen('C:\Users\masom\OneDrive\Documents\Uni\MCHA3000\Project\src\pend_ctrl_param.def', 'w+');
    format long;
    if FID ~= -1
        %fprintf(FID, '#ifndef CHASSIS_CTRL_DEF\n#define CHASSIS_CTRL_DEF\n');
        [A,B,C,D,~] = ssdata(Chassis_ctrl);
        
        %fprintf(FID, 'const float T_pend = %.6f;\n', T_ctrl);
        %interval = T_ctrl/T_main;
        %fprintf(FID, 'const uint32_t pend_interval = %d;\n', interval);
        
        Kerr_int = A;
        Kerr_dTheta = B(2);
        Kerr_dPhi = B(3);
        Kerr_Vref = B(4);
        
        K_int = C;
        K_Theta = D(1);
        K_dTheta = D(2);
        K_dPhi = D(3);
        
        K_FP_GAIN = 2^K_BITSHIFT;
                
        fprintf(FID, '\n#ifdef FIXEDPOINT_PENDCTRL');
        fprintf(FID, '\nstatic const int32_t Kerr_int = %d;\n', round(Kerr_int*K_FP_GAIN));
        fprintf(FID, 'static const int32_t Kerr_dTheta = %d;\n', round(Kerr_dTheta*K_FP_GAIN));
        fprintf(FID, 'static const int32_t Kerr_dPhi = %d;\n', round(Kerr_dPhi*K_FP_GAIN));
        fprintf(FID, 'static const int32_t Kerr_Vref = %d;\n\n', round(Kerr_Vref*K_FP_GAIN));
        
        fprintf(FID, 'static const int32_t K_int = %d;\n', round(K_int*K_FP_GAIN));
        fprintf(FID, 'static const int32_t K_Theta = %d;\n', round(K_Theta*K_FP_GAIN));
        fprintf(FID, 'static const int32_t K_dTheta = %d;\n', round(K_dTheta*K_FP_GAIN));
        fprintf(FID, 'static const int32_t K_dPhi = %d;\n', round(K_dPhi*K_FP_GAIN));
        fprintf(FID, '\n#else');
        fprintf(FID, '\nstatic const float Kerr_int = %f;\n', round(Kerr_int, 8));
        fprintf(FID, 'static const float Kerr_dTheta = %f;\n', round(Kerr_dTheta, 8));
        fprintf(FID, 'static const float Kerr_dPhi = %f;\n', round(Kerr_dPhi, 8));
        fprintf(FID, 'static const float Kerr_Vref = %f;\n\n', round(Kerr_Vref, 8));
        
        fprintf(FID, 'static const float K_int = %f;\n', round(K_int, 8));
        fprintf(FID, 'static const float K_Theta = %f;\n', round(K_Theta, 8));
        fprintf(FID, 'static const float K_dTheta = %f;\n', round(K_dTheta, 8));
        fprintf(FID, 'static const float K_dPhi = %f;\n', round(K_dPhi, 8));
        fprintf(FID, '\n#endif');
        fclose(FID);
    else
        fprintf('ERROR: Could not open file\n');
    end
end


function ExportMotorCtrl(K_BITSHIFT, ML_ctrl, ML_PARAM, MR_ctrl, MR_PARAM)
    FID = fopen('C:\Users\masom\OneDrive\Documents\Uni\MCHA3000\Project\src\motor_ctrl_param.def', 'w+');
    
    if FID ~= -1
        %fprintf(FID, '#ifndef MOTOR_CTRL_DEF\n#define MOTOR_CTRL_DEF\n');
        [K_ML_int,K_ML_V,K_ML_OUT, ~, T_motors_L] = ssdata(ML_ctrl);
        [K_MR_int,K_MR_V,K_MR_OUT, ~, T_motors_R] = ssdata(MR_ctrl);
        
        K_FP_GAIN = 2^K_BITSHIFT;
        
        if T_motors_L == T_motors_R 
            fprintf(FID, '\n#ifdef FIXEDPOINT_MOTORCTRL');
            
            % MOTOR LEFT
            fprintf(FID, '\n/*\n *\tMOTOR LEFT DATA\n*/\n');

            fprintf(FID, 'static const int32_t K_ML_int = %d;\n', round(K_ML_int*K_FP_GAIN));
            fprintf(FID, 'static const int32_t K_ML_V = %d;\n', round(K_ML_V*K_FP_GAIN));
            fprintf(FID, 'static const int32_t K_ML_OUT = %d;\n', round(K_ML_OUT*K_FP_GAIN));
            fprintf(FID, 'static const int32_t ML_HFG = %d;\n', round(ML_PARAM.HFG*K_FP_GAIN));
            fprintf(FID, 'static const int32_t ML_T2C = %d;\n', round(ML_PARAM.T2C*K_FP_GAIN));
            
            fprintf(FID, 'static const int32_t ML_OFFS_POS = %d;\n', round(ML_PARAM.OFFS_POS*K_FP_GAIN));
            fprintf(FID, 'static const int32_t ML_OFFS_NEG = %d;\n', round(abs(ML_PARAM.OFFS_NEG*K_FP_GAIN)));
            
            % MOTOR LEFT
            fprintf(FID, '\n\n/*\n *\tMOTOR RIGHT DATA\n*/\n');

            fprintf(FID, 'static const int32_t K_MR_int = %d;\n', round(K_MR_int*K_FP_GAIN));
            fprintf(FID, 'static const int32_t K_MR_V = %d;\n', round(K_MR_V*K_FP_GAIN));
            fprintf(FID, 'static const int32_t K_MR_OUT = %d;\n', round(K_MR_OUT*K_FP_GAIN));
            fprintf(FID, 'static const int32_t MR_HFG = %d;\n', round(MR_PARAM.HFG*K_FP_GAIN));
            fprintf(FID, 'static const int32_t MR_T2C = %d;\n', round(MR_PARAM.T2C*K_FP_GAIN));
            
            fprintf(FID, 'static const int32_t MR_OFFS_POS = %d;\n', round(MR_PARAM.OFFS_POS*K_FP_GAIN));
            fprintf(FID, 'static const int32_t MR_OFFS_NEG = %d;\n', round(abs(MR_PARAM.OFFS_NEG*K_FP_GAIN)));
            
            
            fprintf(FID, '\n#else');
            % MOTOR LEFT
            fprintf(FID, '\n/*\n *\tMOTOR LEFT DATA\n*/\n');

            fprintf(FID, 'static const float K_ML_int = %f;\n', K_ML_int);
            fprintf(FID, 'static const float K_ML_V = %.6f;\n', K_ML_V);
            fprintf(FID, 'static const float K_ML_OUT = %.6f;\n', K_ML_OUT);
            fprintf(FID, 'static const float ML_HFG = %.6f;\n', ML_PARAM.HFG);
            fprintf(FID, 'static const float ML_T2C = %.6f;\n', ML_PARAM.T2C);
            
            fprintf(FID, 'static const float ML_OFFS_POS = %.6f;\n', ML_PARAM.OFFS_POS);
            fprintf(FID, 'static const float ML_OFFS_NEG = %.6f;\n', abs(ML_PARAM.OFFS_NEG));
            fprintf(FID, 'static const float ML_KN = %.6f;\n', ML_PARAM.K*ML_PARAM.N);
            fprintf(FID, 'static const float ML_R = %.6f;\n', ML_PARAM.R);
            
            % MOTOR LEFT
            fprintf(FID, '\n\n/*\n *\tMOTOR RIGHT DATA\n*/\n');

            fprintf(FID, 'static const float K_MR_int = %f;\n', K_MR_int);
            fprintf(FID, 'static const float K_MR_V = %.6f;\n', K_MR_V);
            fprintf(FID, 'static const float K_MR_OUT = %.6f;\n', K_MR_OUT);
            fprintf(FID, 'static const float MR_HFG = %.6f;\n', MR_PARAM.HFG);
            fprintf(FID, 'static const float MR_T2C = %.6f;\n', MR_PARAM.T2C);
            
            fprintf(FID, 'static const float MR_OFFS_POS = %.6f;\n', MR_PARAM.OFFS_POS);
            fprintf(FID, 'static const float MR_OFFS_NEG = %.6f;\n', abs(MR_PARAM.OFFS_NEG));
            fprintf(FID, 'static const float MR_KN = %.6f;\n', MR_PARAM.K*MR_PARAM.N);
            fprintf(FID, 'static const float MR_R = %.6f;\n', MR_PARAM.R);
            
            fprintf(FID, '\n#endif');
        else
            fprintf('ERROR: T_motors are not equal');
        end
        
        

        %fprintf(FID, '\n#endif');
        fclose(FID);
    else
        fprintf('ERROR: Could not open file\n');
    end
end