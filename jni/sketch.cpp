//Archivo de cabecera de SimDSP
#include <simdsp.h>


//Frecuencia de muestreo
#define F_S 44100

//Carga los coeficientes del filtro y la longitud de la respuesta al impulso (Nh)
#include "../matlab/coefs.h"

//Tamaño del bloque de datos
#define N_DATA 512

//Señal de salida
short y[N_DATA];

//Bloque temporal  donde se almacenan las muestras del bloque anterior y las muestras nuevas
short xt[N_DATA+Nh]; 

//Rutina que simulará la ISR que se invoca una vez ha finalizado
//la transferencia por DMA, audio apunta a los datos que se procesarán
void process_data(short *x){

  //Para los ciclos for
  int n, k;

  //Invoca captura por DMA nuevamente
  captureBlock(process_data );

  //Realiza el movimiento de datos del bloque de entrada al bloque temporal
  //usado en el cálculo de la convolución
  for(n=0; n<N_DATA; n++) {
    xt[n+Nh-1] = x[n];
  }

  //Inicia procesamiento
  for(n=0;n<N_DATA;n++){
    y[n] = 0;
    for(k=0; k<Nh; k++) {
      y[n] += h[k] * xt[n-k+Nh-1]; 
    }
  }

  //Reproduce señal
  playBlock(y);

  //Prepara el bloque temporal para procesar el siguiente bloque
  for(n=0; n<Nh; n++) {
    xt[n] = xt[n+N_DATA];
  }
}


//Funcion de inicializacion
void dsp_setup()
{
  //Habilita sistema de audio del teléfono
  enableAudio(N_DATA,F_S);

  //Imprime tasa de muestreo actual
  println("Sampling frequency is %d Hz and buffer size is %d samples",F_S,N_DATA);

  //Inicia captura por DMA
  captureBlock(process_data );
}
