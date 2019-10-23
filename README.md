# Szakdolgozat
Az ELTE IK Programtervező Informatikus BSC szakdolgozatom. (Szoftverfejlesztő szakirány)

## A dolgozat címe:	
Háromdimenziós közlekedés szimuláció

## A feldolgozott probléma:
Úthálózatok és a rajta történő közlekedés modellezési lehetőségeinek vizsgálata és egy gyakorlati modell kidolgozása.

## Az eredmények felhasználása:
A modern világban egyre fontosabbá válik az úthálózatok modellezhető számítógépes reprezentációja. Ez a feladat kulcs szerepet játszik a térképszoftverek, helymeghatározó és útvonaltervező rendszerek készítésénel.
Ezen felül egy ilyen modell hatékonyan támogathatja a várostervezést, valamint a napjainkban egyre jobban előtérbe kerülő közlekedésszervezési algoritmusok vizsgálatát az övezető járművek esetében.

## A dolgozat témája: 
Egy szimulációs eszköz elkészítése, mellyel útszakaszok forgalomáteresztő képessége vizsgálható.
A program beolvassa az útszakaszt és a forgalom paramétereit leíró fájlt, majd ezt háromdimenzióban megjeleníti.
A szimuláció során a program a felhasználó által definiált paramétereknek megfelelő forgalmat enged át a modellezett útszakaszon.

A járművek megközelítőleg a közúti közlekedés szabályainak megfelelően haladnak a belépési pontjuktól az előre definiált célállomásuk felé a saját útkereső algoritmusuk által kijelölt útvonal alapján.

Ha minden jármű célba ért, a szimuláció véget ér. Ekkor a program a megjeleníti a szimuláció során gyűjtött statisztikai adatokat. Így megállapíthatóvá válik, hogy a futtatott szimulációs és a megvalósított forgalmi szabályok milyen módon befolyásolják a forgalmat és a forgalmi fennakadásokat.

A program C++ nyelven kerül megvalósításra. A háromdimenziós megjelenítés OpenGL-el történik, aminek implementálásához az  SDL-t (Simple DirectMedia Layer) hívom segítségül.
A program fejlesztése és futtatása Windows platformon történik, de a fejlesztés során szemelőtt tartom, hogy a lehető leginkább platformfüggetlen megoldásokat alkalmazzak.

## Felhasznált eszközök:
### Könyvtárak:
- SDL2
- SDL2_image
- GLEW
- MICROGUI for SDL
- GOOGLE TEST
### IDE:
- Eclipse
### Felhasznált eszközök:
- CMAKE
- MAKE
- MINGW
- DOXYGEN
### LATEX:
- A szakdolgozat szövege LATEX-ban készült mejet a következő sablonra alapoztam: https://github.com/mcserep/elteikthesis.git