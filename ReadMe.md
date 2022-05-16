# Toepassingopdracht Machines en berekenbaarheid.
## Introductie
Ons project voor machines en Berekenbaarheid gaat over een gestructureerde taal om Turing machines te ontwerpen. Dit taaltje laat ons toe om snel Turing machines te designen, te testen en die visueel voor te stellen om toekomstige Informatica studenten te helpen de leestrof beter te doen begrijpen. Om dit project te realiseren hebben we een context vrije taal opgesteld die een LALR parser en AST zal gebruiken om te controleren of een inputbestand een correcte beschrijving van een Turing machine bevat. Vervolgens kan onze code deze Turing machine opstellen aan de hand van dit correcte inputbestand. Dit project laat zowel een statische als dynamische visualisatie van Turing Machines toe. Op deze manier zorgen we ervoor dat toekomstige informatica studenten op een correcte, snelle en handige manier kunnen bijleren over de werking van Turing machines. Ons project is gedeeltelijk gebaseerd op twee reeds bestaande projecten [1][2].

## Dependencies
CMAKE  
QT5

## Run
```
git clone https://github.com/DriesDeMaeseneer/TO_Group_MB.git
cd TO_Group_MB  
mkdir build  
cd build  
cmake ..  
make
```

## References
[1] ”Alan”(2021) https://github.com/kelvindecosta/alan, accessed 28-11-2021  
[2] ”turingmachine.io”(2021) https://turingmachine.io/, accessed 28-11-2021
