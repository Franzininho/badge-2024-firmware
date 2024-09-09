# Teste E-PAPER
Este teste mostra o uso do periférico SPI do CH32V003 com o módulo e-paper de 1.54 polegadas do WeAct Studio.

O teste foi desenvolvido usando a infraestrutura [ch32v003fun](https://github.com/cnlohr/ch32v003fun)

## Detalhes da Implementação
Como o CH32V003 não tem memória para para armazenar a imagem gráfica completa, é usada uma tela alfanumérica com caracteres semi-gráficos.

Uma tela de 25 linhas por 25 caracteres é armazenada na RAM e a imagem gráfica é gerada dinamicamente através de um fonte 8x8 armazenado na Flash

## Conexões

* PC2 - RST
* PC3 - CS
* PC4 - DC
* PC5 - SCK
* PC6 - MOSI
* PD2 - BUSY

