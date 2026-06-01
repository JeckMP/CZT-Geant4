# CZT-Geant4

Geant4 simulation framework developed for investigating cadmium zinc telluride (CZT) detectors for thermal and cold neutron imaging applications.

This project was developed as part of the PhD research of **Edcer Laguda** in Medical Physics at **McMaster University**, under the supervision of **Dr. Soo Hyun Byun** and **Dr. Troy Farncombe**.

## Research Objectives

The primary goal of this work is to investigate the feasibility of using CZT detectors for neutron imaging through Monte Carlo simulations. The repository includes studies on:

- Bare CZT neutron response
- Gadolinium (Gd) neutron converters
- Lithium Fluoride (LiF) neutron converters
- Boron Carbide (B4C) neutron converters
- Converter thickness optimization
- Thermal and cold neutron beam response
- Deposited-energy spectra
- Particle and process contributions
- Neutron capture mechanisms
- Detector performance characterization

## Software

- Geant4 11.x
- CMake
- C++17

## Repository Structure

```text
src/        Geant4 source files
include/    Header files
macros/     Geant4 macro files
```

## Build

```bash
mkdir build
cd build
cmake ..
make -j4
```

## Run

```bash
./neutron_czt_sim run.mac
```

## Acknowledgements

This work was conducted within the Medical Physics program at McMaster University.

Researcher:
- Edcer Laguda, MSc, PhD Candidate

Supervisors:
- Dr. Soo Hyun Byun
- Dr. Troy Farncombe

The author gratefully acknowledges the support, guidance, and mentorship provided by Dr. Byun and Dr. Farncombe throughout this research project.

## Disclaimer

This repository contains research code developed for academic purposes. The software is provided as-is and may continue to evolve as the research progresses.
