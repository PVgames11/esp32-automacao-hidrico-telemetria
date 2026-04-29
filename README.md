# Automatização de Reservatório Hídrico com ESP32 e Telemetria via WhatsApp 💧

Este projeto foi desenvolvido como trabalho de conclusão da disciplina de **Programação de Microcontroladores** (Projeto de Extensão) na Universidade Estácio de Sá, concluído em novembro de 2025 com **nota máxima (10)**.

## 📋 Sobre o Projeto
O sistema foi concebido para resolver problemas reais de escassez e transbordamento de água no escritório **Vanda Miranda Advocacia**, em Niterói-RJ. A solução substitui o monitoramento manual por um sistema inteligente capaz de gerenciar o nível do reservatório de 2.000 litros de forma autônoma.

### Minha Contribuição Técnica
Como responsável pelo hardware e firmware, atuei na:
* **Concepção e Implementação:** Planejamento completo do sistema e seleção de componentes.
* **Desenvolvimento de Hardware:** Montagem do circuito eletrônico integrando sensores e atuadores à placa ESP32.
* **Firmware (C/C++):** Desenvolvimento de toda a lógica de controle embarcado utilizando a Arduino IDE.
* **QA e Testes:** Identificação e correção de falhas em campo para garantir a confiabilidade do sistema.

## 🛠️ Especificações Técnicas
* **Microcontrolador:** ESP32 (Gestão de lógica e conectividade Wi-Fi).
* **Sensoriamento:** Sensor Ultrassônico HC-SR04 para medição de nível sem contato.
* **Atuação:** Módulo Relé para acionamento físico da bomba d'água.
* **Firmware:** Desenvolvido em C++ com bibliotecas para JSON, HTTP e integração de APIs.

## 🚀 Funcionalidades Principais
* **Modos de Operação:** * **Automático:** Aciona a bomba ao atingir 15% e desliga ao chegar em 40% (evitando transbordamento).
* **Manual (Manutenção):** Permite o controle direto via interface web, ignorando a automação para fins de limpeza.
* **Telemetria em Tempo Real:** Envio de dados via protocolo **HTTP** para um banco de dados MySQL e exibição em Dashboard.
* **Alertas via WhatsApp:** Notificações automáticas via API sobre mudanças de estado e alertas de segurança.

## 👥 Equipe e Orientação
* **Firmware e Eletrônica:** Paulo Vitor Silva Quintanilha.
* **Desenvolvimento Web e DB:** Bryan Souza Santiago.
* **Comunicação e Integração:** João Ricardo de Miranda.
* **Documentação e Gestão:** Débora Cristina Figueiredo e Mariana Toledo Oliveira.
* **Orientação:** Prof. Ralfh Ansuattigui.

---
*Nota: Este repositório foca na camada de firmware e hardware do sistema.*
