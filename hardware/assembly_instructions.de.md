# Spectral-Explorer Hardware-Bauanleitung

> **Sprache:** [English](assembly_instructions.md) | Deutsch

Diese Anleitung beschreibt den 3D-Druck, die Vorbereitung und den Zusammenbau des **Spectral-Explorer**-Gehäuses mit einer **Logitech C270**-Webcam und einem **DVD-Beugungsgitter**.

Alle 3D-druckbaren `.stl`-Dateien, FreeCAD-Quelldateien (`.FCStd`) und Slic3r-Druckprofile (`.ini`) befinden sich in [`hardware/case/`](case/).

---

## Teile & Materialien

### 3D-gedruckte Teile (`hardware/case/`)

| Teil | STL-Datei | Hinweise |
| --- | --- | --- |
| Hauptgehäuse | [`case.stl`](case/case.stl) | In mattem, lichtdichtem dunklem Filament (z. B. Schwarz) drucken, um Streulicht zu minimieren |
| Deckel | [`cap.stl`](case/cap.stl) | Wird nach Montage und Spaltausrichtung aufgeklebt |
| Frontplatte | [`plate_front.stl`](case/plate_front.stl) | Vordere Abdeckplatte |
| Rechte Platte | [`plate_right.stl`](case/plate_right.stl) | Seitliche Abdeckplatte |
| Mutternhalter | [`nut_holder.stl`](case/nut_holder.stl) | Hält die Befestigungsmuttern im Gehäuse |
| Stativmutternkappe | [`tripod_nut_cover.stl`](case/tripod_nut_cover.stl) | Deckt die 1/4″-Stativmutter im Gehäuse ab |
| Spalt — linke Hälfte (fest) | [`aperture.stl`](case/aperture.stl) | Geschraubte linke Rasierklingen-Spaltblende |
| Spalt — rechte Hälfte (verstellbar) | [`aperture_magnet.stl`](case/aperture_magnet.stl) | Magnetisch verstellbare rechte Spaltblende mit Rändelmuttern ([`knurled_nut.FCStd`](case/knurled_nut.FCStd)) |

### Optionaler Küvettenhalter (`hardware/cuvette_holder/`)

* **Küvettenhalter:** [`cuvette_holder.stl`](cuvette_holder/cuvette_holder.stl) (`.FCStd` + `.ini` enthalten) — wird vorne für Absorptions-/Transmissionsmessungen aufgesteckt
* **Küvettenkappe:** [`cuvette_cap.stl`](cuvette_holder/cuvette_cap.stl) — lichtdichte Kappe für den Küvettenhalter
* **Küvettenständer:** [`cuvette_store.stl`](cuvette_holder/cuvette_store.stl) — Ablage für Küvetten

### Weitere Komponenten & Werkzeuge

* **Kamera:** Logitech C270 USB-Webcam
* **Optisches Gitter:** 1× Standard-DVD-R / DVD+R (als Beugungsgitter)
* **Spaltklingen:** Doppelseitige Rasierklingen (2 Klingen bzw. 1 geteilte Klinge)
* **Schrauben, Muttern & Magnete:**
  * Muttern für `nut_holder` und Schrauben für den Spalt
  * 1× 1/4″-Stativmutter (plus passende 1/4″-Schraube zum Einziehen)
  * Kleine Magnete (für die rechte Magnetblende) + 2× Rändelmuttern & Federn
* **Kleber & Verbrauchsmaterial:**
  * Zweikomponentenkleber (und/oder dünnes doppelseitiges Klebeband für Schritt 4.1)
  * Heißkleber (für die Stativmutternkappe)
  * Brennspiritus / Ethanol und fusselfreie Tücher
  * Schleifpapier (300er und 1000er Körnung)
  * Seitenschneider, Bastelmesser, kleiner Schraubendreher, Lötkolben (oder lichtdichtes Klebeband)

> [!CAUTION]
> **Rasierklingen** sind extrem scharf — schneiden und handhaben Sie sie vorsichtig (z. B. auf einer festen Unterlage oder mit einer Zange). In **Schritt 4.2** wird zudem ein Schraubenkopf erhitzt; verwenden Sie Zangen oder hitzebeständige Werkzeuge, um Verbrennungen zu vermeiden.

---

## Schritt 1: 3D-gedrucktes Gehäuse vorbereiten

1. **Optional — Brim entfernen:**
   * Brim nach unten wegbrechen.
   * Größere Zacken mit einem Messer entfernen.
2. **Druckfäden entfernen:**
   * Vor allem im **Spaltbereich**, im **Spalt selbst**, in der **Kameraöffnung** und in den **Schraubenlöchern**.
   * **Magnettaschen** säubern, sodass nichts klappert.
   * Lose Fäden durch leichtes Aufklopfen des Gehäuses auf den Tisch entfernen.
3. **Nur bei vorherigem Brim — Rand schleifen:**
   * 300er Sandpapier mit festem Druck in eine Richtung abrollen (flacher Winkel zur Spektrometerwand).
   * Mit feuchtem Tuch säubern.
   * Ggf. mit 1000er Sandpapier nachschleifen.
4. **Abschließende Lichtwegprüfung:** Prüfen, ob der gesamte Lichtweg innen frei von Hindernissen ist.
5. **Magnete einlegen:**
   > [!IMPORTANT]
   > Alle Magnete müssen in **dieselbe Richtung gepolt** sein.
   * Jeweils die inneren Magnete mit den äußeren in ihre Taschen ziehen.
   * Äußere Magnete zunächst dranlassen, damit die inneren nicht verrutschen.
6. **Mutternhalter (`nut_holder.stl`):**
   * Muttern in den Halter einlegen.
   * Halterung in das Gehäuse einschieben.
7. **1/4″-Stativgewinde (`tripod_nut_cover.stl`):**
   * Stativmutternkappe innen anrauen, sodass sie nicht mehr spiegelt.
   * Stativmutter einsetzen.
   * Eine 1/4″-Schraube von unten einschrauben, bis sie die Mutter ganz heruntergezogen hat, und dann so weit zurückdrehen, dass sie nicht mehr übersteht.
   * Heißkleber am Rand in die Kappe füllen und auf die Mutter drücken.

---

## Schritt 2: Kamera (Logitech C270) vorbereiten

1. Kamera auspacken und per USB am PC auf Funktionsfähigkeit testen.
2. Gehäuse demontieren:
   * Halterung abschrauben.
   * Graue Kamerakappe abknipsen.
   * Kameradeckel abschrauben.
3. **Fokus voreinstellen:**
   * Oberste Zacke am Fokusring markieren (Kameralinse zeigt nach links).
   * Verklebung am Fokusring entfernen.
   * Fokusring **7 Zacken gegen den Uhrzeigersinn** drehen (Feineinstellung erfolgt in Schritt 5).
4. **Status-LED deaktivieren:** Grüne LED lichtdicht abkleben oder auslöten, damit kein Streulicht entsteht.

---

## Schritt 3: Optisches Gitter (DVD-Stück)

> [!WARNING]
> **Nicht auf die Unterseite der DVD fassen** — Fingerabdrücke beschädigen das Gitter dauerhaft.

1. **DVD-Stück schneiden:**
   * Am DVD-Rand **1,0 cm** markieren.
   * Zur Mitte hin **1,1 cm** einschneiden und parallel zum Rand zum Schnittende schneiden.
   * Schnitt so anpassen, dass das Gitter gerade so in den Schlitz passt (die Außenkante der DVD sollte weitestgehend erhalten bleiben).
2. **Gitter säubern (nur abtupfen, nicht reiben):**
   * Brennspiritus auf ein Taschentuch geben.
   * Gitter darauflegen, Tuch umklappen und **vorsichtig andrücken, nicht reiben**.
   * Vorgang einmal wiederholen.
3. **Gitter einlegen:**
   * Der **DVD-Rand zeigt nach außen** (zum weißen Lichtleiter).
   * Die **Schräge des DVD-Rands** zeigt ins Innere zur Kamera.

---

## Schritt 4: Optischen Spalt montieren

Der Spalt besteht aus zwei Hälften: der fest verschraubten linken Hälfte ([`aperture.stl`](case/aperture.stl)) und der magnetisch verstellbaren rechten Hälfte ([`aperture_magnet.stl`](case/aperture_magnet.stl)).

> [!IMPORTANT]
> Bei beiden Hälften darf die Rasierklinge **maximal 1 mm** vorne über die Plastikblende überstehen. Steht sie weiter über, lässt sich der Spalt nach Aufkleben des Deckels (`cap.stl`) nicht mehr vollständig schließen.

### 4.1 — Linke Spalthälfte (geschraubt: `aperture.stl`)

1. Rasierklinge an den **dünnen Ausbuchtungen** zurechtschneiden.
2. Doppelseitiges Klebeband auf ca. **0,5 × 2 cm** zuschneiden und auf die Klingenseite mit dem Markennamen kleben.
   * *Alternative:* Mit Zweikomponentenkleber wie bei der Magnetblende verkleben, kräftig andrücken und ohne weiteren Druck trocknen lassen.
3. [`aperture.stl`](case/aperture.stl) mittig auf die Klinge setzen:
   * Klinge ca. **1 mm** vorne überstehen lassen (nicht mehr).
   * Die Schraubenlöcher in der Plastikblende dürfen **nicht** von der Klinge verdeckt werden, da diese sonst später von der Schraube weggedrückt wird.
4. Schrauben in die Plastikblende eindrehen, bis das Schraubenende ca. **2 mm** übersteht.
5. Blende links am Gehäuse ansetzen, andrücken und festschrauben, bis leichter Widerstand zu spüren ist.
   * *Tipp:* Nicht zu fest anziehen (sonst biegt sich der Rand herunter) und nicht zu locker lassen (sonst steht die Blende ab).

### 4.2 — Rechte Spalthälfte (magnetisch verstellbar: `aperture_magnet.stl`)

#### Magnetblende vorbereiten
1. Unterseite von [`aperture_magnet.stl`](case/aperture_magnet.stl) leicht anrauen.
2. Rasierklingen an der **breiten Einbuchtung** zurechtschneiden.
3. Rasierklingen mit genügend Abstand auf eine Eisenplatte legen, damit sich die Magnete später nicht gegenseitig anziehen.
4. Klingen mit Ethanol säubern.
5. Zweikomponentenkleber anrühren (innerhalb von ca. 5 min verarbeiten; ideal für 5–8 Klingen gleichzeitig).
6. Kleber dünn und gleichmäßig auf die Klinge aufstreichen, dabei vorne **2–3 mm frei lassen**, damit beim Ausrichten kein Kleber überquillt.
7. Plastikblende aufsetzen, auf **1 mm** Klingenüberstand ausrichten (nicht mehr) und mit 2 Magneten fixieren (Magnete können leicht schräg in den Vertiefungen sitzen). Falls die Blende auf frischem Kleber verrutscht, kurz warten, bis der Kleber zäher ist.
8. Kleber vollständig aushärten lassen.

#### Magnetblende am Gehäuse anbringen
1. Übergequollenes Druckmaterial auf der Gleitfläche des Gehäuses entfernen.
2. Magnete in `aperture_magnet.stl` einsetzen, Schrauben durchschieben und Federn auffädeln.
3. **Schraubenköpfe einziehen:** Schraubenkopf leicht **erhitzen** und in die Halterung ziehen, damit er genug Platz hat und nicht weggedrückt wird. Der Schraubenkopf sollte möglichst nah an Blende und Halterung sitzen; die Schraube darf leicht von der Blende wegzeigen, damit sie an die Plastikführung gedrückt wird.
4. Sicherstellen, dass die Schraube **parallel zur Platte** sitzt; falls nicht, Schritt 3 wiederholen.
5. Magnetblende am Gehäuse anbringen und **Rändelmuttern** aufschrauben.
6. Prüfen, dass sich die Rasierklinge vorne nicht nach oben biegt (im Zweifel Blende zunächst ohne Schrauben testen).

---

## Schritt 5: Kamera einbauen & Fokus feinjustieren

1. *(Optional)* Spalt vorab mit einer kalibrierten Referenzkamera prüfen.
2. Vorbereitete Logitech C270 einsetzen, mit Spectral-Explorer verbinden und den **Fokus individuell auf das gedruckte Gehäuse feineinstellen** (der Abstand von Linse zu Spalt variiert leicht je nach Druck).
3. Sobald ein scharfes Spektrum mit klaren Fraunhofer- oder Leuchtstofflampen-Linien sichtbar ist, Deckel ([`cap.stl`](case/cap.stl)) und Abdeckplatten festkleben.
