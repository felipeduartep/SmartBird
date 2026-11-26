#ifndef ALIMENTACAO_H
#define ALIMENTACAO_H
#include <Arduino.h>
#include "Estado_SB.h"

bool inicializarAlimentacao(const ConfiguracaoAlimentacao& configuracao);
bool iniciarAlimentacao(DadosSistema& dados);
void atualizarAlimentacao(DadosSistema& dados);
bool zerarBalanca();
bool calibrarBalanca(float pesoConhecidoGramas, DadosSistema& dados);
bool cancelarAlimentacao(DadosSistema& dados);

#endif
