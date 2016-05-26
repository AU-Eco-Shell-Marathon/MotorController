%% indtastet værdier 
clear
close all
clc
H_d=0.478;
gear=1/20;
MAX_Watt = 200;
MAX_Watt_peak = 300;
MAX_speed = 8.34;

% Convert 
OmegaToRpm = 60/(2*pi);
SpeedToOmega = (2*pi)/(pi*H_d);
SpeedToRpm = SpeedToOmega * OmegaToRpm;

MAX_RPM = MAX_speed * SpeedToRpm

FILE_NAME='src\MotorController.cydsn\LUT_data.h';

array_size_RPM_shift=2;
array_size_RPM=2^array_size_RPM_shift
array_size=round(MAX_RPM/array_size_RPM);

ramp = [200 0 0.5; 250 250 0.6; 250 2250 0.8; 450 3300 0.85; 600 7000 0.9];
const = [0 0 0.5; 50 100 0.6; 110 500 0.8; 110 900 0.85; 220 2250 0.9; 250 3600 0.92; 275 4800 0.93; 295 5600 0.935];

% udregning

Maxon_MAX_RPM=MAX_speed*(60/(pi*H_d))*gear^-1

power_ramp=zeros(size(ramp,1),3);
for i=1:size(ramp,1)
    power_ramp(i,3) = ramp(i, 1)*(10^-3)*ramp(i, 2)*(2*pi/60);
    power_ramp(i,2) = ramp(i, 2);
    power_ramp(i,1) = power_ramp(i,3)*(2-ramp(i, 3));
end

figure
subplot(221)
plot(power_ramp(:,2), power_ramp(:,1));
hold on
grid on
plot(power_ramp(:,2), power_ramp(:,3));
title('Ramp')
xlabel('rpm(motor)')
ylabel('watt')


power_const=zeros(size(const,1),3);
for i=1:size(const,1)
    power_const(i,3) = const(i, 1)*(10^-3)*const(i, 2)*(2*pi/60);
    power_const(i,2) = const(i, 2);
    power_const(i,1) = power_const(i,3)*(2-const(i, 3));
end

subplot(222)
plot(power_const(:,2), power_const(:,1));
hold on
grid on
plot(power_const(:,2), power_const(:,3));
title('Const')
xlabel('rpm(motor)')
ylabel('watt')

%power_ramp_LUT=ones(1, array_size)*power_ramp(size(power_ramp,1),2)*gear;
power_ramp_LUT=zeros(1, array_size)*MAX_Watt_peak;
for i=1:size(power_ramp,1)-1
    
    RPM_start=floor((power_ramp(i,2)*gear)/array_size_RPM)
    RPM_stop=floor((power_ramp(i+1,2)*gear)/array_size_RPM)
    power_ramp_LUT(RPM_start+1:RPM_stop+1)=linspace(power_ramp(i,1),power_ramp(i+1,1),RPM_stop-RPM_start+1)
    %
end
power_ramp_LUT=power_ramp_LUT(1:array_size)

%power_const_LUT=ones(1, array_size)*power_const(size(power_const,1),2)*gear
power_const_LUT=zeros(1, array_size);
for i=1:size(power_const,1)-1
    
    RPM_start=floor((power_const(i,2)*gear)/array_size_RPM)
    RPM_stop=floor((power_const(i+1,2)*gear)/array_size_RPM)
    power_const_LUT(RPM_start+1:RPM_stop+1)=linspace(power_const(i,1),power_const(i+1,1),RPM_stop-RPM_start+1);
    
end
%power_const_LUT=power_const_LUT(1:array_size)

for i=1:length(power_const_LUT)
    if(power_const_LUT(i) > MAX_Watt)
        power_const_LUT(i) = MAX_Watt;
    end
    if(power_ramp_LUT(i) > MAX_Watt_peak)
        power_ramp_LUT(i) = MAX_Watt_peak;
    end
    
end

N=length(power_ramp_LUT);
subplot(2,2,[3,4])
plot([0:N-1]*2^array_size_RPM_shift,power_ramp_LUT)
hold on
grid on
plot([0:N-1]*2^array_size_RPM_shift,power_const_LUT)
title('LUT')
xlabel('rpm(wheel)')
ylabel('watt')

%% LUT to header file

fileID=fopen(FILE_NAME,'w');

fprintf(fileID,'#include <project.h>\n#define LUT_SIZE %d\n#define LUT_LSB %d\n#define LUT_SHIFT %d\n\n',round(array_size), round(array_size_RPM),round(array_size_RPM_shift));

fprintf(fileID,'static const uint8 RAMP_LUT[LUT_SIZE] = {\n');

fprintf(fileID,'%d', uint8(power_ramp_LUT(1)));
for i=2:length(power_ramp_LUT)
    fprintf(fileID,', %d', uint8(power_ramp_LUT(i)));
end

fprintf(fileID,'\n};\n\n');

fprintf(fileID,'static const uint8 CONST_LUT[LUT_SIZE] = {\n');

fprintf(fileID,'%d', uint8(power_const_LUT(1)));
for i=2:length(power_const_LUT)
    fprintf(fileID,', %d', uint8(power_const_LUT(i)));
end

fprintf(fileID,'\n};');

fclose(fileID);



