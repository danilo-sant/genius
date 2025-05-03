🎮 Projeto: Jogo Genius com ESP32 e Display OLED

Este repositório apresenta o desenvolvimento de uma versão embarcada do clássico jogo Genius (Simon Says), utilizando o microcontrolador ESP32, botões coloridos com LEDs, um display OLED SSD1306 e um buzzer. O objetivo do jogo é testar a memória do jogador, que deve repetir corretamente as sequências geradas pela CPU.

🔧 Componentes utilizados

- ESP32 DevKit V1

- Display OLED 0.96” I2C (SSD1306)

- 4 botões com LEDs (vermelho, verde, amarelo e azul)

- Buzzer (ativo ou passivo)

- Resistores (para LEDs e botões)

- Jumpers e Protoboard

🧠 Funcionalidades implementadas

- Interface gráfica no display OLED otimizada para facilitar a leitura das instruções e pontuação.

- Geração automática de sequência de cores com som e luz.

- Leitura dos botões com resposta visual e sonora.

- Sistema de níveis com aumento progressivo de velocidade a cada 5 acertos, iniciando com 1,2 Hz (uma piscada a cada 0,83 s) e podendo atingir até 3,8 Hz.

- Indicação de acertos e mensagem de "Game Over".

- LED de inicialização no GPIO 26 aceso ao ligar o sistema.

- Organização do código para facilitar leitura e manutenção.

🎯 Limitações

O protótipo desenvolvido implementa exclusivamente a habilidade 3 no tipo de jogo 1, que é a sequência tradicional do Genius, onde o jogador deve repetir a ordem exata dos sinais. A escolha entre habilidades e tipos de jogo não foi configurada nesta versão, focando-se em garantir estabilidade e desempenho do jogo principal.

No entanto, foi elaborado um diagrama de sequência de estados considerando o jogo completo, o que permite futuras expansões do projeto com outros modos de jogo e mais funcionalidades.

📈 Possibilidades futuras

- Inclusão de seleção de habilidades (memória reversa, repetição de maior sequência, etc.)

- Integração com Wi-Fi para salvar pontuação

- Interface Web para monitoramento ou controle

- Montagem em caixa física para tornar o jogo portátil

👨‍💻 Autor

Desenvolvido por Danilo Santos

Projeto acadêmico com foco em sistemas embarcados e automação.
