# Szakdolgozat
Az ELTE IK Programtervező Informatikus BSC szakdolgozatom. (Szoftverfejlesztő szakirány)

## A dolgozat címe:	
Háromdimenziós közlekedés szimuláció

## A feldolgozott probléma:
Úthálózatok és a rajta történő közlekedés modellezési lehetőségeinek vizsgálata és egy gyakorlati modell kidolgozása.

## Az eredmények felhasználása:
A modern világban egyre fontosabbá válik az úthálózatok modellezhető számítógépes reprezentációja. Ez a feladat kulcs szerepet játszik a térképszoftverek, helymeghatározó és útvonaltervező rendszerek készítésénél.
Ezen felül egy ilyen modell hatékonyan támogathatja a várostervezést, valamint a napjainkban egyre jobban előtérbe kerülő közlekedésszervezési algoritmusok vizsgálatát az önvezető járművek esetében.

## A dolgozat témája: 
Egy szimulációs eszköz elkészítése, mellyel útszakaszok terepasztal szerűen modellezhetőek és az úthálózatok forgalomáteresztő képessége vizsgálható.

A programban terepasztal szerűen szerkeszthetők és vizualizálhatók az úthálózatok, települések, ezek azután fájlba menthetőek.

A program beolvassa az útszakaszt és a forgalom paramétereit leíró fájlt, majd ezt háromdimenzióban megjeleníti.

A szimuláció során a program a felhasználó által definiált paramétereknek megfelelő forgalmat enged át a modellezett útszakaszon.

A járművek megközelítőleg a közúti közlekedés szabályainak megfelelően haladnak a belépési pontjuktól az előre definiált célállomásuk felé a saját útkereső algoritmusuk által kijelölt útvonal alapján.

Ha minden jármű célba ért, a szimuláció véget ér. Ekkor a program a megjeleníti a szimuláció során gyűjtött statisztikai adatokat. Így megállapíthatóvá válik, hogy a futtatott szimulációs és a megvalósított forgalmi szabályok milyen módon befolyásolják a forgalmat és a forgalmi fennakadásokat.

A program C++ nyelven kerül megvalósításra.

A program, a forráskód és a megjegyzések Angol nyelvűek, ezen ismertető és a dokumentáció Magyar nyelvű.

A háromdimenziós megjelenítés OpenGL-el történik, aminek implementálásához az  SDL-t (Simple DirectMedia Layer) hívom segítségül.

A program fejlesztése és futtatása Windows platformon történik, de a fejlesztés során szemelőtt tartom, hogy a lehető leginkább platformfüggetlen megoldásokat alkalmazzak.

## A program Windows X64 rendszeren futtaható állománya a Binary mappában található.

## A forráskód DoxyGen dokumentációja az alábbi webcímen érhető el:
### https://sandorbalazshu.github.io/elte-ik-bsc-thesis-doc/

## Felhasznált eszközök:

### Könyvtárak:
- SDL2
- SDL2_image
- GLEW
- ImGui for SDL
- GOOGLE TEST

### IDE:
- Visual Studio

### Dokumentáció:
- DOXYGEN

### Dolgozat:
- A szakdolgozat szövege Microsoft Word-ben készült.