# Spectral-Explorer Hardware Assembly Guide

> **Language:** English | [Deutsch](assembly_instructions.de.md)

This guide covers printing, preparing, and assembling the **Spectral-Explorer** 3D-printed spectrometer housing with a **Logitech C270** webcam and a **DVD diffraction grating**.

All 3D-printable `.stl` meshes, FreeCAD source files (`.FCStd`), and Slic3r print profiles (`.ini`) are in [`hardware/case/`](case/).

---

## Parts & Materials

### 3D-Printed Parts (`hardware/case/`)

| Part | Mesh | Notes |
| --- | --- | --- |
| Main housing | [`case.stl`](case/case.stl) | Print in matte/opaque dark filament (e.g., black) to minimize stray internal reflections |
| Top lid | [`cap.stl`](case/cap.stl) | Glued on after internal assembly and slit alignment |
| Front plate | [`plate_front.stl`](case/plate_front.stl) | Front cover plate |
| Right plate | [`plate_right.stl`](case/plate_right.stl) | Side cover plate |
| Nut holder | [`nut_holder.stl`](case/nut_holder.stl) | Holds mounting nuts inside the case |
| Tripod nut cap | [`tripod_nut_cover.stl`](case/tripod_nut_cover.stl) | Covers the 1/4″ tripod nut inside the case |
| Slit — Left half (fixed) | [`aperture.stl`](case/aperture.stl) | Screwed left razor-blade slit half |
| Slit — Right half (adjustable) | [`aperture_magnet.stl`](case/aperture_magnet.stl) | Magnetic adjustable right razor-blade slit half with knurled nuts ([`knurled_nut.FCStd`](case/knurled_nut.FCStd)) |

### Optional Cuvette Holder (`hardware/cuvette_holder/`)

* **Cuvette holder:** [`cuvette_holder.stl`](cuvette_holder/cuvette_holder.stl) (`.FCStd` + `.ini` included) — attaches to the front for liquid absorption/transmission measurements
* **Cuvette cap:** [`cuvette_cap.stl`](cuvette_holder/cuvette_cap.stl) — light-tight cap for the cuvette holder
* **Cuvette storage tray:** [`cuvette_store.stl`](cuvette_holder/cuvette_store.stl) — storage stand for cuvettes

### Off-the-Shelf Components & Tools

* **Camera:** Logitech C270 USB webcam
* **Optical grating:** 1× standard DVD-R / DVD+R (used as a diffraction grating)
* **Slit blades:** Double-edge razor blades (2 blades, or 1 split in half)
* **Fasteners & magnets:**
  * Mounting nuts for `nut_holder` and screws for the slit
  * 1× 1/4″ tripod nut (and a matching 1/4″ screw for pulling it into place)
  * Small magnets (for the magnetic right slit half) + 2× knurled nuts & springs
* **Adhesives & consumables:**
  * Two-component epoxy (and/or thin double-sided tape for Step 4.1)
  * Hot glue (for sealing the tripod nut cap)
  * Denatured alcohol / ethanol (*Brennspiritus*) and lint-free tissues
  * 300-grit and 1000-grit sandpaper
  * Flush cutters, craft knife, small screwdriver, soldering iron (or opaque tape)

> [!CAUTION]
> **Razor blades** are extremely sharp — cut and handle them carefully (e.g., on a flat surface or with pliers). **Step 4.2** also involves heating a screw head; use pliers or heat-resistant tools to avoid burns.

---

## Step 1: Prepare the 3D-Printed Case

1. **Remove brim (if printed with one):**
   * Snap the brim downward off the edges.
   * Trim any remaining jagged tabs with a craft knife.
2. **Clean out printing strings and loose filament:**
   * Inspect and clean the **slit area**, **slit channel**, **camera opening**, and **screw holes**.
   * Clear the **magnet pockets** so the magnets sit snugly and do not rattle.
   * Gently tap the housing against a table to dislodge any loose filament threads inside.
3. **Sand the rim (only if printed with a brim):**
   * Roll **300-grit sandpaper** firmly in one direction at a shallow angle along the case wall.
   * Wipe clean with a damp cloth.
   * Optionally smooth further with **1000-grit sandpaper**.
4. **Check the optical path:** Look through the housing to verify the light path from slit to grating to camera is completely unobstructed.
5. **Install the magnets:**
   > [!IMPORTANT]
   > All magnets must be oriented with the **same magnetic polarity**.
   * Place each inner magnet inside the housing and use an outer magnet on the outside wall to pull it firmly into its pocket.
   * Leave the outer magnets attached on the outside for now so the inner magnets stay seated.
6. **Install the nut holder:**
   * Insert the nuts into [`nut_holder.stl`](case/nut_holder.stl).
   * Slide the nut holder into its slot in the case.
7. **Install the 1/4″ tripod thread:**
   * Roughen the inside of [`tripod_nut_cover.stl`](case/tripod_nut_cover.stl) with sandpaper so it is matte and non-reflective.
   * Place the 1/4″ tripod nut into its recess in the case.
   * Thread a 1/4″ screw in from the bottom until it pulls the nut all the way down, then back the screw out just enough so it no longer protrudes above the nut.
   * Apply hot glue around the rim of the tripod nut cap and press it onto the nut to seal it in place.

---

## Step 2: Prepare the Logitech C270 Camera

1. Unpack the webcam and plug it into a PC via USB to verify it works.
2. Disassemble the housing:
   * Unscrew the mounting bracket/clip.
   * Snip off the gray front bezel/cap.
   * Unscrew and remove the front camera cover.
3. **Pre-adjust the manual focus:**
   * Hold the camera board with the **lens on the left** and mark the **top tooth** on the focus ring.
   * Carefully scrape off the factory glue dot locking the lens ring.
   * Rotate the focus ring **7 teeth counter-clockwise** as a starting point (you will fine-tune this in Step 5).
4. **Eliminate the status LED:** Cover the green status LED completely with opaque black tape—or desolder it—so it cannot cast stray light into the spectrum.

---

## Step 3: Cut and Install the DVD Diffraction Grating

> [!WARNING]
> **Never touch the recorded/underside surface of the DVD** with bare fingers—fingerprints permanently degrade the diffraction grating.

1. **Cut the grating piece:**
   * Mark a **1.0 cm** wide section along the outer rim of the DVD.
   * Cut **1.1 cm** inward toward the center, then cut parallel to the outer rim to form a small rectangle (~1.0 × 1.1 cm).
   * Trim the edges slightly if needed so the piece fits snugly into the grating slot while **preserving the DVD's outer edge** as much as possible.
2. **Clean the grating (blot, never wipe):**
   * Wet a clean tissue with denatured alcohol / ethanol (*Brennspiritus*).
   * Lay the DVD piece on the damp tissue, fold the tissue over it, and **press gently without rubbing**.
   * Repeat once with a fresh spot on the tissue.
3. **Insert the grating into the case:**
   * Orient it so the **DVD outer rim faces outward** (toward the white light guide).
   * The **beveled edge** of the DVD rim should face inward toward the camera.

---

## Step 4: Build and Mount the Optical Slit

The slit consists of two halves: the fixed/screwed left half ([`aperture.stl`](case/aperture.stl)) and the magnetic adjustable right half ([`aperture_magnet.stl`](case/aperture_magnet.stl)).

> [!IMPORTANT]
> For both halves, the razor blade edge must protrude **no more than 1 mm** past the plastic aperture. If it protrudes further, the slit cannot close completely once the top lid (`cap.stl`) is glued on.

### 4.1 — Left Slit Half (Screwed: `aperture.stl`)

1. Cut a razor blade to size at the **thin notches**.
2. Cut a ~**0.5 × 2 cm** strip of double-sided tape and stick it onto the side of the blade with the brand logo.
   * *Alternative:* Bond with two-component epoxy (see Step 4.2 below). Press firmly and let cure undisturbed.
3. Center [`aperture.stl`](case/aperture.stl) onto the blade:
   * Leave **~1 mm** of blade protruding at the front (no more).
   * Make sure the blade **does not cover the screw holes** in the plastic aperture, otherwise the mounting screws will push the blade out of alignment.
4. Pre-thread the screws into the plastic aperture until their tips protrude by **~2 mm**.
5. Hold the aperture against the left side of the case slit, press flat, and tighten the screws until you feel light resistance.
   * *Tip:* Do not over-tighten (which bends the edge downward) or leave too loose (which lets light leak underneath).

### 4.2 — Right Slit Half (Magnetic Adjustable: `aperture_magnet.stl`)

#### Prepare the Magnetic Aperture
1. Roughen the underside of [`aperture_magnet.stl`](case/aperture_magnet.stl) with sandpaper for better glue adhesion.
2. Cut the razor blades to size at the **wide notch**.
3. Place the cut blades on a flat steel/iron plate, spaced well apart so the magnets won't snap together.
4. Degrease the blades with ethanol.
5. Mix two-component epoxy (work within its ~5-minute pot life; you can batch 5–8 blades at once).
6. Spread a thin, even coat of epoxy over each blade, leaving **2–3 mm uncoated at the front cutting edge** so glue does not squeeze out during alignment.
7. Set `aperture_magnet.stl` onto the blade so the edge protrudes **1 mm** (no more), and clamp it down using 2 magnets (they may sit slightly angled in the recesses).
   * *Tip:* If the aperture slides on fresh epoxy, wait 1–2 minutes until the glue becomes tackier before setting it down.
8. Wait for the epoxy to cure completely.

#### Mount the Magnetic Aperture on the Case
1. Scrape off any excess printed plastic on the sliding surface of the case.
2. Insert the magnets into `aperture_magnet.stl`, push the adjustment screws through, and slide the springs onto the screws.
3. **Seat the screw heads:** Gently **heat** each screw head and pull it into its plastic holder so the head sits flush and is not pushed out. Keep the screw head as close to the aperture and holder as possible; the screw may angle slightly away from the aperture so it presses against the plastic guide.
4. Check that each screw sits **parallel to the plate**; if skewed, reheat and re-seat it.
5. Mount the magnetic aperture onto the case and thread on the **knurled nuts**.
6. Inspect the razor blade from the front to verify it does **not** bow upward. If you are unsure whether the slit edges are clean, test the aperture without screws first.

---

## Step 5: Install Camera & Final Focus Calibration

1. *(Optional)* Inspect the slit alignment using an already-calibrated reference camera.
2. Insert the prepared Logitech C270 into the camera bay, connect it to Spectral-Explorer, and **fine-tune the lens focus individually for your printed case** (due to small print tolerances, the exact lens-to-slit distance varies slightly from case to case).
3. Once a sharp spectrum with crisp Fraunhofer or CFL spectral lines is achieved, glue the lid ([`cap.stl`](case/cap.stl)) and side/front plates in place.
