#ifndef PERSISTENCIA_H
#define PERSISTENCIA_H

#include <Arduino.h>
#include "Estado_SB.h"

// Carrega somente configuracoes persistentes. Os estados de execucao,
// leituras e atuadores permanecem nos valores seguros definidos na RAM.
bool carregarDadosPersistidos(DadosSistema& dados);

// Grava um snapshot das configuracoes em um dos dois slots da EEPROM.
bool salvarDadosPersistidos(const DadosSistema& dados);

// Invalida os registros e permite que o sistema volte aos padroes definidos
// durante a inicializacao. Usar apenas em procedimento explicito de reset.
bool limparDadosPersistidos();

#endif
