# E-PAPER demonstration
This example code demonstrates the use of the CH32V003 SPI peripheral with the WeAct Studio 1.54 inch e-paper module.

Version for the Franzininho Badge

## Implementation Details
As the CH32V003 does not have enough RAM to store a full bitmap image, a semigraphic characters approach is used.

A 12 line x 16 character screen is stored in the RAM, the graphic image is generated going through a 12x16 font stored in Flash.


