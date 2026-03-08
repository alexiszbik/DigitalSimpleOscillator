# Digital Simple Oscillator

## Description

Digital oscillator module for DIY synthesizers built with the **Daisy Patch Submodule**.
This module provides two oscillators with pitch modulation, hard sync, and glide.

---

## Controls

### CV Inputs

| Input | Function |
|------|---------|
| **CV_1** | Main pitch CV (rounded to the nearest integer) |
| **CV_2** | Pitch modulation CV |
| **CV_4** | Hard sync modulation CV |
| **CV_5** | Pitch drift (0 → +24 semitones) |
| **CV_6** | Hard Sync trigger |
| **CV_7** | Chord mode (~0 for sweet detune, +3, +4, +5)
---

### Knobs / ADC

| Control | Function |
|--------|---------|
| **ADC_9** | Oscillator 2 tuning (-12, -5, ~0 for sweet detune, +7, +12) |
| **ADC_10** | Glide time |
| **ADC_11** | Oscillator 1 waveform |
| **ADC_12** | Oscillator 2 waveform |

### GPIO
| Control | Function |
|--------|---------|
| **B8** | Toggle Button, change Hard sync control destination (Oscillator 1 or 2) |
| **D1** | Led for pitch change on Main CV |

---

## Features

- Dual digital oscillators  
- Hard sync with selectable destination  
- Glide control  
- Multiple waveform selection for each oscillator

---

## Flashing the Microcontroller

First, clone this repository **including all submodules**:

```bash
git clone --recurse-submodules https://github.com/alexiszbik/DigitalSimpleOscillator.git
```

To flash the Daisy Seed, please refer to the official Daisy documentation:

https://daisy.audio/

Go to Software → C++ → Tutorials and follow the instructions to install the toolchain and flash the firmware.

