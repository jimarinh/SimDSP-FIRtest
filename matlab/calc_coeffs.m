% Este programa calcula la respuesta al impulso de un filtro pasa-bajo de 
% FIR de frecuencia de 5kHz para ser usado con SimDSP. La respuesta al impulso
% se exporta en un archivo de cabecera .h para ser usado por el 
% en la implementación del filtro en SimDSP.
% =============================================================================================

close all;
clear all;

% Calcula la respuesta al impulso del filtro deseado hd
fs = 44100; %Frecuencia de muestreo (Hz)
fc = 5000;  %Frecuencia de corte (Hz)
Oc = 2*pi*fc/fs; %Frecuencia de corte normalizada
n=-30:30;
hd = (Oc/pi)*sinc((Oc/pi)*n); 

%Enventana la secuencia
ha = hd .* hamming(length(n))';

%Calcula y grafica la respuesta en frecuencia
freqz(ha);

%Genera el archivo de cabecera .h para el programa en C
fid = fopen('coefs.h', 'wt');
fprintf(fid, '#define Nh %d\n', length(ha));
fprintf(fid, 'float h[] = {\n');
fprintf(fid, '%f,\n', ha);
fprintf(fid, '};');
fclose(fid);