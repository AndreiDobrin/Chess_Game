# Joc de Șah (Chess Game)

Acesta este un joc de șah dezvoltat în C++ folosind interfața grafică SDL3 și SQLite pentru baza de date (istoricul meciurilor). 
Proiectul respectă conceptele de Programare Orientată pe Obiecte (POO) și este compilat folosind CMake.

## Cerințe preliminare (Prerequisites)

Pentru a compila și rula acest proiect, vei avea nevoie de următoarele instalate pe sistemul tău:

1. **Git**: [Descarcă Git](https://git-scm.com/downloads)
2. **CMake**: Versiunea 3.10 sau mai nouă. [Descarcă CMake](https://cmake.org/download/)
3. **Compilator C++**: 
   - *Pe Windows*: Recomandat MinGW-w64 (ex. instalat prin MSYS2) sau Visual Studio Build Tools (MSVC).
   - *Pe Linux*: `gcc` sau `clang`.
   - *Pe macOS*: Clang (instalat prin Xcode command line tools).

---

## Pasul 1: Obținerea Codului Sursă

Alege una dintre cele două modalități de mai jos pentru a obține proiectul:

### Varianta A: Clonare de pe GitHub (Recomandat)

Deschide un terminal și rulează comanda de mai jos. Este **critic** să folosești parametrul `--recursive` pentru a descărca și bibliotecile externe (SDL3, SQLite etc.) și să specifici branch-ul `gui`:

```bash
git clone -b gui --recursive https://github.com/AndreiDobrin/Chess_Game.git
cd Chess_Game
```

### Varianta B: Folosind Arhiva ZIP (Predare proiect)

1. Dezarhivează fișierul `Chess_Game.zip`.
2. Intră în folderul rezultat (`cd Chess_Game`).
3. **Atenție:** Dacă arhiva a fost descărcată folosind butonul "Download ZIP" de pe GitHub, folderul `vendor/` va fi gol. În acest caz, trebuie să inițializezi manual bibliotecile externe rulând următoarea comandă în terminal:
   ```bash
   git submodule update --init --recursive
   ```

---

## Pasul 2: Configurarea proiectului cu CMake

```bash
# Pentru configurarea standard:

    cmake -E make_directory build                                                                                                                                                                                                   
    cmake -E chdir build cmake ..

```

*Notă pentru utilizatorii de Windows (MinGW):* 
```bash
cmake -G "MinGW Makefiles" ..
```

---

## Pasul 3: Compilarea

Rulează următoarea comandă în folderul `build`:

```bash
cmake --build .
```

*Notă:* Prima compilare poate dura câteva minute deoarece se compilează bibliotecile externe din `vendor/`.

---

## Pasul 4: Rularea jocului

După compilare, executabilul va fi generat în folderul `build/src`.

### Pe Windows:
```bash
.\src\Chess_Game.exe
```

### Pe Linux / macOS:
```bash
./src/Chess_Game
```

---

### Depanare (Erori frecvente):

1. **Eroare la `cmake ..` (Missing SDL3 / include errors)**: Înseamnă că bibliotecile din `vendor/` nu au fost descărcate. Rulează `git submodule update --init --recursive`.
2. **Ecran Negru / Piese lipsă**: Asigură-te că rulezi jocul având folderul rădăcină ca "Working Directory". Executabilul caută imaginile în `./src/chess_pieces/`.
3. **Lipsește `arial.ttf`**: Asigură-te că fișierul font se află în folderul din care rulezi jocul. working directory setat ca Chess_Game/
