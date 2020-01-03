# Sistemes Operatius 2 @ UB (tardor 2019-20)

## Organització

### Pràctiques

#### 1

- Nota: 5 (entregada tard).
- Comentaris
  - Primera pràctica, sense més.
- Dificultat: Fàcil (2-3 hores).
- [Enunciat](https://github.com/tarasyarema/SO2/blob/master/1/practica1.pdf)
- [Codi](https://github.com/tarasyarema/SO2/blob/master/1/sol.md)

#### 2

- Nota: 6.5
- Comentaris: 
  - Implemetació del *core* que s'anirà utilitzant en les següents pràctiques.
- Defectes: 
  - La implementació de la cerca del millor node era molt poc òptima. Aquesta s'ha millorat en les següents entregues.
  - Es va intentar fer una implementació personal de `strcasecmp` pero no funcionava bé. Es va modificar de manera que s'utilitza posteriorment.
- Dificultat: Fàcil (6-8 hores).
- [Enunciat](https://github.com/tarasyarema/SO2/blob/master/2/practica2.pdf)
- [Codi](https://github.com/tarasyarema/SO2/tree/master/2/src)

#### 3

- Nota: 10
- Comentaris: 
  - Crear el menú. Un cop tens les funcions per a guardar i llegir l'arbre en el format binàri donat, és trivial.
- Dificultat: Fàcil (6-8 hores).
- [Enunciat](https://github.com/tarasyarema/SO2/blob/master/3/practica3.pdf)
- [Codi](https://github.com/tarasyarema/SO2/tree/master/3/src)

#### 4

- Nota: ?
- Comentaris: 
  - Programació multi proccés.
  - Memòria compartida (`mmap`).
- Defectes:
  - A la funció `process_list_files_sem` 
    ([enllaç](https://github.com/tarasyarema/SO2/blob/afd4407ed072b434e0b281331c744e420e8d661c/4/src/src/process-mmap.c#L292))
    es serialitza l'arbre per a cada procés. 
    Resulta que no fa falta, es pot accedir a aquest directament. 
    Aquest canvi pot comportar un *boost* notable.
- Dificultat: Difícil (12-16 hores).
- [Enunciat](https://github.com/tarasyarema/SO2/blob/master/4/practica4.pdf)
- [Codi](https://github.com/tarasyarema/SO2/tree/master/4/src/src)

#### 5

- Nota: ?
- Comentaris: 
  - Programació multi fils.
  - Si es revisa la teoria, es bastant directe. 
    En comparació amb la pràctica anterior, un no s'ha de preocupar de la memòria compartida, cosa que simplifica molt la pràctica.
- Dificultat: Mitjana (6-8 hores).
- [Enunciat](https://github.com/tarasyarema/SO2/blob/master/5/practica5.pdf)
- [Codi](https://github.com/tarasyarema/SO2/tree/master/5/src)

### Teoría

Recopilatori de les transparències sense efectes de tota l'assignatura. 
Consultar la carpeta [teoria](https://github.com/tarasyarema/SO2/tree/master/teoria).

### Exàmens

Recopilatori dels exàmens penjats al campus. Tant d'altres anys com de l'actual.
Consultar la carpeta [exams](https://github.com/tarasyarema/SO2/tree/master/exams).

## Referències
- [Manual de Linux](https://linux.die.net/man/3)
