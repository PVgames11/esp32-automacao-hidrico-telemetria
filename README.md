# Automatização de Reservatório Hídrico com ESP32 e Telemetria via WhatsApp 

[cite_start]Este projeto foi desenvolvido como trabalho de conclusão da disciplina de **Programação de Microcontroladores** (Projeto de Extensão) na Universidade Estácio de Sá, concluído em novembro de 2025 com **nota máxima (10)**[cite: 86, 91].

##  Sobre o Projeto
[cite_start]O sistema foi concebido para resolver problemas reais de escassez e transbordamento de água no escritório **Vanda Miranda Advocacia**, em Niterói-RJ[cite: 99, 108]. [cite_start]A solução substitui o monitoramento manual por um sistema inteligente capaz de gerenciar o nível do reservatório de 2.000 litros de forma autônoma[cite: 109, 116, 275].

### Minha Contribuição Técnica
Como responsável pelo hardware e firmware, atuei na:
* [cite_start]**Concepção e Implementação:** Planejamento completo do sistema e seleção de componentes[cite: 57, 410].
* [cite_start]**Desenvolvimento de Hardware:** Montagem do circuito eletrônico integrando sensores e atuadores à placa ESP32[cite: 58, 301, 411].
* [cite_start]**Firmware (C/C++):** Desenvolvimento de toda a lógica de controle embarcado utilizando a Arduino IDE[cite: 59, 63, 434].
* [cite_start]**QA e Testes:** Identificação e correção de falhas em campo para garantir a confiabilidade do sistema[cite: 60, 64, 413].

##  Especificações Técnicas
* [cite_start]**Microcontrolador:** ESP32 (Gestão de lógica e conectividade Wi-Fi)[cite: 328, 336].
* [cite_start]**Sensoriamento:** Sensor Ultrassônico HC-SR04 para medição de nível sem contato[cite: 330, 1041].
* [cite_start]**Atuação:** Módulo Relé para acionamento físico da bomba d'água[cite: 334, 1053].
* [cite_start]**Firmware:** Desenvolvido em C++ com bibliotecas para JSON, HTTP e integração de APIs[cite: 863, 864, 866].

##  Funcionalidades Principais
* [cite_start]**Modos de Operação:** * **Automático:** Aciona a bomba ao atingir 15% e desliga ao chegar em 40% (evitando transbordamento)[cite: 75, 466].
    * [cite_start]**Manual (Manutenção):** Permite o controle direto via interface web, ignorando a automação para fins de limpeza[cite: 75, 368, 466].
* [cite_start]**Telemetria em Tempo Real:** Envio de dados via protocolo **HTTP** para um banco de dados MySQL e exibição em Dashboard[cite: 70, 74, 980].
* [cite_start]**Alertas via WhatsApp:** Notificações automáticas via API sobre mudanças de estado e alertas de segurança[cite: 76, 459, 467, 918].

##  Equipe e Orientação
* [cite_start]**Firmware e Eletrônica:** Paulo Vitor Silva Quintanilha[cite: 301, 410].
* [cite_start]**Desenvolvimento Web e DB:** Bryan Souza Santiago[cite: 292, 390].
* [cite_start]**Comunicação e Integração:** João Ricardo de Miranda[cite: 295, 425].
* [cite_start]**Documentação e Gestão:** Débora Cristina Figueiredo e Mariana Toledo Oliveira[cite: 293, 299, 406].
* [cite_start]**Orientação:** Prof. Ralfh Ansuattigui[cite: 55, 90, 432].

---
*Nota: Este repositório foca na camada de firmware e hardware do sistema.*
