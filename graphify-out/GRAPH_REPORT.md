# Graph Report - SmartBird  (2026-11-27)

## Corpus Check
- Corpus is ~46,872 words - fits in a single context window. You may not need a graph.

## Summary
- 502 nodes · 940 edges · 20 communities (18 shown, 2 thin omitted)
- Extraction: 93% EXTRACTED · 7% INFERRED · 0% AMBIGUOUS · INFERRED: 66 edges (avg confidence: 0.86)
- Token cost: unavailable for the six Codex extraction chunks. Structural extraction used 0 model tokens.

## Community Hubs (Navigation)
- Dashboard Interactions
- System Architecture and Protocol
- Device Interface and State
- Feeding and Hydration Control
- Arduino Setup and Configuration
- EEPROM Configuration Persistence
- Schedule and Execution Tracking
- JSON Protocol Handlers
- ESP8266 Serial Bridge
- Web Dashboard and Documentation
- Hydration State Model
- Feeding State Model
- Shared Web Logo
- Local Menu Modes
- Button State Tracking
- LCD Screen Views
- Dashboard Logo Artwork
- Canary Photo
- Hydration Configuration Status

## God Nodes (most connected - your core abstractions)
1. `DadosSistema` - 64 edges
2. `ConfiguracoesPersistidas` - 21 edges
3. `processarMensagem()` - 20 edges
4. `render()` - 19 edges
5. `AgendaDiaria` - 18 edges
6. `inicializarControle()` - 15 edges
7. `send()` - 13 edges
8. `salvarDadosPersistidos()` - 12 edges
9. `obterDataHora()` - 11 edges
10. `mount()` - 11 edges

## Surprising Connections (you probably didn't know these)
- `SmartBird — Protocolo de Comunicação v0.2.3` --conceptually_related_to--> `processarMensagem()`  [INFERRED]
  documentação/protocolo_comunicacao_v1.md → Sistema_SmartBird_Arduino/Protocolo.cpp
- `Sincronização do RTC em horário local via sync_time` --conceptually_related_to--> `parseDateTimeLocal()`  [INFERRED]
  Sistema_SmartBird_Arduino/README.md → Sistema_SmartBird_Arduino/Protocolo.cpp
- `feed_manual` --conceptually_related_to--> `feedingSafetyError()`  [INFERRED]
  documentação/protocolo_comunicacao_v1.md → Sistema_Web/app.js
- `Ponte ESP32 descrita no README principal` --semantically_similar_to--> `Ponte ESP8266 em UART Serial1 a 9600 baud`  [INFERRED] [semantically similar]
  README.md → Sistema_SmartBird_Arduino/README.md
- `Broker HiveMQ Cloud descrito no README principal` --semantically_similar_to--> `Acesso ao broker EMQX por MQTT sobre WSS`  [INFERRED] [semantically similar]
  README.md → Sistema_Web/index.html

## Import Cycles
- None detected.

## Hyperedges (group relationships)
- **get_schedule da Web à fila de ACK e response** — documentação_documento_mestre_smartbird2_0_painel_web, documentação_documentacao_esp8266_emqx, documentação_documentacao_esp8266_encaminhamento_de_comandos_mqtt, documentação_documento_mestre_smartbird2_0_arduino_mega_2560, documentação_documentacao_esp8266_fila_circular, documentação_protocolo_comunicacao_v1_catalogo_de_agenda [EXTRACTED 1.00]
- **Intertravamento da bomba com reservatório e boia do bebedouro** — documentação_protocolo_comunicacao_v1_water_manual, documentação_documento_mestre_smartbird2_0_modulo_de_hidratacao, documentação_documento_mestre_smartbird2_0_reservatorios_de_origem, documentação_protocolo_comunicacao_v1_estado_binario_do_bebedouro [EXTRACTED 1.00]
- **Diagnóstico de bancada no painel web** — sistema_web_index_simulador_arduino, sistema_web_index_comando_json_manual, sistema_web_index_painel_da_gaiola [EXTRACTED 1.00]

## Communities (20 total, 2 thin omitted)

### Community 0 - "Dashboard Interactions"
Cohesion: 0.05
Nodes (68): feed_manual, active(), activeAlert(), after(), apply(), available(), buttonText(), canActuate() (+60 more)

### Community 1 - "System Architecture and Protocol"
Cohesion: 0.06
Nodes (43): Broker EMQX, Encaminhamento de comandos MQTT à UART, ESP8266 como ponte MQTT/UART, Gerenciamento Wi-Fi do ESP8266, Guia do firmware SmartBird ESP8266 v2.4, Priorização da recepção UART, Publicação MQTT da fila, Reconexão MQTT do ESP8266 (+35 more)

### Community 2 - "Device Interface and State"
Cohesion: 0.09
Nodes (42): DadosSistema, agendaAlimentacao, agendaHidratacao, alimentacao, conectividadeRecebida, configuracaoAlimentacao, configuracaoHidratacao, configuracaoManutencao (+34 more)

### Community 3 - "Feeding and Hydration Control"
Cohesion: 0.07
Nodes (32): atualizarAlimentacao(), calibrarBalanca(), cancelarAlimentacao(), fecharComporta(), inicializarAlimentacao(), enviarMensagem(), houveOverflowComunicacao(), inicializarComunicacao() (+24 more)

### Community 4 - "Arduino Setup and Configuration"
Cohesion: 0.07
Nodes (28): ConfiguracaoHidratacao, tempoMaximoBombaMs, ConfiguracaoManutencao, intervaloLimpezaSegundos, ConfiguracaoReservatorio, distanciaNivelCheioCm, distanciaNivelVazioCm, nivelMinimoPercentual (+20 more)

### Community 5 - "EEPROM Configuration Persistence"
Cohesion: 0.07
Nodes (40): aplicarSnapshot(), calcularCRC32(), carregarDadosPersistidos(), ConfiguracoesPersistidas, agendaAlimentacao, agendaHidratacao, aguaDistanciaCheioCm, aguaDistanciaVazioCm (+32 more)

### Community 6 - "Schedule and Execution Tracking"
Cohesion: 0.07
Nodes (38): iniciarAlimentacao(), concluirAgendamento(), processarAgendamentos(), AgendaDiaria, horarios, quantidadeHorarios, EstadoExecucao, AGUARDANDO (+30 more)

### Community 7 - "JSON Protocol Handlers"
Cohesion: 0.17
Nodes (21): adicionarAgenda(), adicionarRecipientes(), adicionarReservatorio(), anoBissexto(), diasNoMes(), enviarAck(), enviarDocumento(), enviarErro() (+13 more)

### Community 8 - "ESP8266 Serial Bridge"
Cohesion: 0.13
Nodes (20): bufferUART · montagem de uma linha, Recepção e reenquadramento UART, Verificação estrutural com jsonIntegro(), adicionarNaFila(), enviarEstadoConectividade(), gerenciarWiFi(), jsonIntegro(), lerDadosDoMega() (+12 more)

### Community 9 - "Web Dashboard and Documentation"
Cohesion: 0.10
Nodes (14): Demonstração de alimentação programada, Demonstração de avisos de manutenção, Demonstração de hidratação automática, Broker HiveMQ Cloud descrito no README principal, Ponte ESP32 descrita no README principal, SmartBird (projeto acadêmico), Ponte ESP8266 em UART Serial1 a 9600 baud, Agenda diária de até quatro horários por módulo (+6 more)

### Community 10 - "Hydration State Model"
Cohesion: 0.13
Nodes (15): DadosHidratacao, bombaLigada, estado, estadoRecipiente, ultimaExecucaoMs, EstadoHidratacao, ABASTECENDO, AGUARDANDO (+7 more)

### Community 11 - "Feeding State Model"
Cohesion: 0.17
Nodes (12): DadosAlimentacao, comportaAberta, estado, leituraValida, pesoAtualGramas, ultimaExecucaoMs, EstadoAlimentacao, AGUARDANDO (+4 more)

### Community 12 - "Shared Web Logo"
Cohesion: 0.18
Nodes (11): Cream Background, Three-Lobed Green Crest, Stylized Green Parrot, Rounded Green Wing, Curved Orange Beak, Circular Orange Eye, SmartBird Logo, Pale beige background (+3 more)

### Community 13 - "Local Menu Modes"
Cohesion: 0.25
Nodes (8): ModoMenu, CONFIRMAR_HORARIO, CONFIRMAR_LIMPEZA, EDITAR_HORARIO, LISTA_HORARIOS, MENSAGEM, MODULO_AGENDA, NENHUM

### Community 14 - "Button State Tracking"
Cohesion: 0.29
Nodes (6): Botao, estadoEstavel, leituraAnterior, pino, pressionado, ultimaMudanca

### Community 15 - "LCD Screen Views"
Cohesion: 0.29
Nodes (7): Tela, CONECTIVIDADE, HOME, MANUTENCAO, POTINHOS, PROXIMA_ACAO, RESERVATORIOS

### Community 16 - "Dashboard Logo Artwork"
Cohesion: 0.29
Nodes (7): Stylized green parrot, Layered green wing, Orange circular eye, Orange hooked beak, Pale cream background, Rounded green crest, SmartBird logo

### Community 17 - "Canary Photo"
Cohesion: 0.60
Nodes (5): Blurred green foliage, Canary portrait, Dark open space on the left, Diagonal perch branch, Yellow canary

## Knowledge Gaps
- **146 isolated node(s):** `INICIALIZANDO`, `OPERACIONAL`, `ALERTA`, `ERRO`, `AGUARDANDO` (+141 more)
  These have ≤1 connection - possible missing edges or undocumented components. (Counts symbols only; 179 node(s) total have ≤1 connection when file, concept and rationale nodes are included.)
- **2 thin communities (<3 nodes) omitted from report** — run `graphify query` to explore isolated nodes.

## Suggested Questions
_Questions this graph is uniquely positioned to answer:_

- **Why does `DadosSistema` connect `Device Interface and State` to `Feeding and Hydration Control`, `Arduino Setup and Configuration`, `EEPROM Configuration Persistence`, `Schedule and Execution Tracking`, `JSON Protocol Handlers`, `Hydration State Model`, `Feeding State Model`?**
  _High betweenness centrality (0.271) - this node is a cross-community bridge._
- **Are the 8 inferred relationships involving `processarMensagem()` (e.g. with `SmartBird — Protocolo de Comunicação v0.2.3` and `atualizarControle()`) actually correct?**
  _`processarMensagem()` has 8 INFERRED edges - model-reasoned connections that need verification._
- **What connects `INICIALIZANDO`, `OPERACIONAL`, `ALERTA` to the rest of the system?**
  _146 weakly-connected nodes found - possible documentation gaps or missing edges._
- **Should `Dashboard Interactions` be split into smaller, more focused modules?**
  _Cohesion score 0.05225576111652061 - nodes in this community are weakly interconnected._
- **Why does `processarMensagem()` connect `JSON Protocol Handlers` to `System Architecture and Protocol`, `Device Interface and State`, `Feeding and Hydration Control`, `EEPROM Configuration Persistence`, `Schedule and Execution Tracking`?**
  _High betweenness centrality (0.262) - this node is a cross-community bridge._
- **Should `System Architecture and Protocol` be split into smaller, more focused modules?**
  _Cohesion score 0.061224489795918366 - nodes in this community are weakly interconnected._
- **Why does `SmartBird — Protocolo de Comunicação v0.2.3` connect `System Architecture and Protocol` to `JSON Protocol Handlers`?**
  _High betweenness centrality (0.245) - this node is a cross-community bridge._

## Build Notes

- The 37-file corpus includes 26 code files, seven documents, and four images. Both Arduino sketches and the Arduino headers were parsed as C++ using per-run language mappings. The two CSS stylesheets are outside Graphify's default file types.
- Graphify flagged partial parsing in Sistema_SmartBird_Arduino/Comunicacao.cpp near line 34. Its structural pass found no modeled symbols in 12 files, primarily headers containing declarations.
- The raw extraction diagnostic found 39 edges with an endpoint absent from the raw node list, 4 self-loops, and 14 edges that share an endpoint pair in the undirected graph. Some relationships may be incomplete.
- Git reports graphify-out/ as untracked in this working tree. [Graphify team workflows](https://docs.graphify.com/guides/team-workflows)
