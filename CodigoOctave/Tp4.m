clc;
clear;
close all;
pkg load symbolic;
pkg load signal;
pkg load control;
pkg load tisean;

% Filtro Butterworth pasa-bajos Digital 

Rp = 3;
Rs = 50; % Aumentamos hasta conseguir un orden de 4, para no sobrecargar el micro

fp1 = 30;
fs1 = 50;

fs = 110;  % Frecuencia de Nyquist es la maxima*2 y la de muestreo es esta misma + 10, en este caso 
Ts = 1/fs;

wp = fp1*(2/fs);
ws = fs1*(2/fs);

[N,Wn] = buttord(wp, ws, Rp, Rs)
[b,a] = butter(N, Wn, 'low') %low, high o stop, sin la 's' te la da digital y no analog - Al ser digital los valores de Wn deben estar normalizados
fprintf('Filtro Butterworth - Orden: %d \n',N)
filtro_Butter = tf(b, a, Ts)

% ------------------------------------------------------------------------------------------------------

fig1 = figure(1);
hold on
bode(filtro_Butter)
% xlim(2*pi*[fp1-1000 fp2+2000])
title('Diagramas de Bode')
legend('Butterworth')
%saveas(fig1,'Bode','jpg')
hold off

% La caida es mucho mas pronunciada en la parte de la derecha ya que esta la asintota de la frecuecnia m¨¢xima, la cual exige una atenuacion infinita 

% ------------------------------------------------------------------------------------------------------

% y(i) =  0.1237*x(i) + 0.4949*x1 + 0.7423*x2 + 0.4949*x3 + 0.1237*x4 - 0.3566*y1 - 0.5262*y2 - 0.07653*y3 - 0.02015*y4

CoefEntrada = [0.1237 0.4949 0.7423 0.4949 0.1237];
CoefSalida = [1 -0.3566 -0.5262 -0.07653 -0.02015];

ArrayEntrada = [0 0 0 0 0]; 
ArraySalida = [0 0 0 0 0];

for i = 1:300
  n(i) = i*Ts;
  ArrayEntrada(1) = sin(2*pi*50*n(i));
  %ArrayEntrada(1) = 0.245
  Salida = ArrayEntrada(1) * CoefEntrada(1);
  for j = 2:5
    Salida += ArrayEntrada(j) * CoefEntrada(j) + ArraySalida(j) * CoefSalida(j); 
  end
  ArraySalida(1) = Salida;
  SalidaGrafica(i) = Salida;
  for l = 2:5
    k = 7;
    ArrayEntrada(k-l) = ArrayEntrada((k-l)-1);
    ArraySalida(k-l) = ArraySalida((k-l)-1);
  end
endfor

fig2 = figure(2)
%plot((n-Ts), x, "r")
%hold on;
plot(SalidaGrafica)
axis([1, 300, -1, 1])
title("Respuesta")
legend("Salida")
%saveas(fig2,'50Hz','jpg')
grid on;