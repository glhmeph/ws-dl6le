# WS – DL6LE Anpassungen

Diese Version basiert auf der WS Digital Mode Suite von Uwe Risse (DG2YCB)
und weiteren Autoren, hervorgegangen aus WSJT-X.

Originalprojekt: https://sourceforge.net/projects/wsjt-x-improved/
Quellcode-Downloads: https://sourceforge.net/projects/wsjt-x-improved/files/WS_v3.2.1/Source%20code/

Die vorhandenen Lizenz- und Autorenhinweise gelten weiterhin; siehe `COPYING`.
Dieses Repository dokumentiert eine angepasste Version und ist kein offizielles Release.

## Änderungen gegenüber dem lokalen Basisarchiv

- Der Fenstertitel kennzeichnet diese Ausgabe mit **DL6LE-Version (Basis: WS 3.2.1 / 260926)**.

- Auto CQ für normalen FT8-/FT4-Betrieb: Rückkehr zu CQ nach Abschluss eines QSOs.
  Über das Häkchen **Auto CQ** abschaltbar; standardmäßig aus.
  Details und manuelle Prüfungen: `doc/development/auto-cq.md`.
- JTTY-Standardmakros unterstützen vollständige Rufzeichen mit Präfix/Suffix,
  beispielsweise `TK/DL6LE` und `DL6LE/P`, über die bestehende Textcodierung.
  Standardrufzeichen behalten die kompakte Codierung; ungültige Austauschdaten
  werden weiterhin abgewiesen.
- Regressionstests für Auto CQ und JTTY-Makros einschließlich Codec-Rundlauf.

## Herkunft und Versionsgeschichte

Der lokale Import stammt aus `src/ws.tgz` des bisherigen Arbeitsordners,
Version 3.2.1 mit angegebener Revision 260926. Seine MD5-Prüfsumme stimmt mit
`src/ws.tgz.md5sum` überein. Das Archiv enthält bereits frühere Anpassungen;
es ist **nicht unabhängig als unverändertes SourceForge-Original verifiziert**.
Es darf deshalb zunächst nur als lokaler Basisstand bezeichnet werden.
Prüfsummen und Details stehen in `SOURCE_ORIGIN.md`.

Branches:

- `imported-source`: unveränderte Quelldateien aus dem lokalen Basisarchiv,
  soweit von dessen vorhandener `.gitignore` erfasst.
- `main`: darauf aufbauende aktuelle Anpassungen und diese Dokumentation.
- `upstream`: erst nach Prüfung eines offiziellen SourceForge-Archivs anlegen.

## Bauen unter Linux

Benötigt werden CMake, ein C/C++-Compiler mit C++17-Unterstützung, gfortran,
Qt 5 einschließlich der verwendeten Zusatzmodule, Boost, FFTW, Hamlib und libusb.
Weitere Hinweise enthält `INSTALL`.

Der Repository-Stamm entspricht dem bisherigen Unterordner `ws`:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DWSJT_GENERATE_DOCS=OFF -DWSJT_SKIP_MANPAGES=ON
cmake --build build --parallel 2
QT_QPA_PLATFORM=offscreen ctest --test-dir build --output-on-failure
./build/ws
```

Der UDP-Test `test_tx_inhibit_gate` benötigt lokal verfügbare UDP-Sockets.
Er bestand im bisherigen Arbeitsordner außerhalb der eingeschränkten Sandbox.
Die aktuelle Anwendung wurde dort erfolgreich gebaut; die übrigen vier Tests
(einschließlich `test_jtty_macros`) bestanden ebenfalls. Dieser separate
Repository-Ordner wurde noch nicht eigenständig vollständig neu gebaut.

## Veröffentlichung

Ein leeres GitHub-Repository anlegen.
Die Remote-Adresse wird anschließend als `origin` eingetragen und die Branches
sowie Tags werden hochgeladen. Build-Ausgaben und persönliche Betriebsdaten
bleiben außerhalb der Versionsverwaltung.

Commit-Autor: Rene Morgenstern. Die Git-Identität ist nur für dieses lokale Repository konfiguriert.

## Debian-Paket

Die Vorabversion `v3.2.1-dl6le.1` wird als `.deb` für amd64 bereitgestellt.
Gebaut unter Linux Mint 22.3 mit Bibliotheken der Ubuntu-24.04-Paketbasis.
Andere Distributionen sind nicht getestet; die Paketabhängigkeiten müssen
auf dem Zielsystem verfügbar sein.

Installation des heruntergeladenen Pakets:

```bash
sudo apt install ./ws-dl6le_3.2.1+dl6le.1_amd64.deb
```

Start anschließend über den Menüeintrag WS oder mit `ws` im Terminal.
Das Paket installiert auch WS-MAP (`wsmap`) und EME65 (`eme65`).
Es kann wegen gemeinsam verwendeter Programmpfade nicht gleichzeitig mit
den Paketen `ws` oder `wsjtx` installiert sein. Prüfe den von apt angezeigten
Installationsplan vor der Bestätigung.

Erstellung aus einem fertig gebauten Quellstand:

```bash
WS_DL6LE_BUILD_DIR="$PWD/build" cpack -G DEB --config packaging/DL6LECPackConfig.cmake -B release-assets
```

Die Hamlib-Hilfsprogramme `rigctl`, `rigctld` und `rigctlcom` werden nur dann
mitgeliefert, wenn sie auf dem Build-System gefunden wurden. Sie sind in diesem
ersten Paket nicht enthalten. Die CAT-Steuerung über die Hamlib-Bibliothek
bleibt enthalten.
