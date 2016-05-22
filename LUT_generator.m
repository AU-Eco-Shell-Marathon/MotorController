%% indtastet værdier 
clear
close all
clc
H_d=0.478;
gear=1/20;
MAX_Watt = 200;
MAX_Watt_peak = 300;
MAX_speed = 8.34;


FILE_NAME='MotorController.cydsn\LUT_data.h';

array_size_watt_shift=2;
array_size_watt=2^array_size_watt_shift
array_size=round(MAX_Watt_peak/array_size_watt);

ramp = [200 0 0.5; 250 250 0.6; 250 2250 0.8; 450 3300 0.85; 600 7000 0.9];
const = [0 0 0.5; 50 100 0.6; 110 500 0.8; 110 900 0.85; 220 2250 0.9; 250 3600 0.92; 275 4800 0.93; 295 5600 0.935];

% udregning

MAX_RPM=MAX_speed*(60/(pi*H_d))*gear^-1

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

power_ramp_LUT=ones(1, array_size)*power_ramp(size(power_ramp,1),2);
for i=1:size(power_ramp,1)-1
    
    watt_start=floor(power_ramp(i,1)/array_size_watt);
    watt_stop=floor(power_ramp(i+1,1)/array_size_watt);
    power_ramp_LUT(watt_start+1:watt_stop+1)=linspace(power_ramp(i,2),power_ramp(i+1,2),watt_stop-watt_start+1);
    %
end
power_ramp_LUT=power_ramp_LUT(1:array_size).*gear

power_const_LUT=ones(1, array_size)*power_const(size(power_const,1),2);
for i=1:size(power_const,1)-1
    
    watt_start=floor(power_const(i,1)/array_size_watt);
    watt_stop=floor(power_const(i+1,1)/array_size_watt);
    power_const_LUT(watt_start+1:watt_stop+1)=linspace(power_const(i,2),power_const(i+1,2),watt_stop-watt_start+1);
    %
end
power_const_LUT=power_const_LUT(1:array_size).*gear


N=length(power_ramp_LUT);
subplot(2,2,[3,4])
plot([0:N-1]*2^array_size_watt_shift,power_ramp_LUT)
hold on
grid on
plot([0:N-1]*2^array_size_watt_shift,power_const_LUT)
title('LUT')
xlabel('watt')
ylabel('rpm(wheel)')

%% LUT to header file

fileID=fopen(FILE_NAME,'w');

fprintf(fileID,'#include <project.h>\n#define LUT_SIZE %d\n#define LUT_LSB %d\n#define LUT_SHIFT %d\n\n',round(array_size), round(array_size_watt),round(array_size_watt_shift));

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



