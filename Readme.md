# Spherically-Symmetric-SelfInteracting-Scalar-Solver (S5)

| arXiv | Zenodo DOI |
|:-----:|:---:|
|[![arXiv](https://img.shields.io/badge/arXiv-2301.xxxxx-orange.svg)](https://arXiv.org/abs/2301.xxxxx)|[![DOI](https://zenodo.org/badge/DOI/10.5281/zenodo.xxxx.svg)](https://doi.org/10.5281/zenodo.xxxx)|

This code allows for the calculation of the value of the field that solves a Klein-Gordon equation with a mass term and a four-point self interaction &lambda;&phi;<sup>4</sup> term with an arbitrary spherically symmetric source. The figures in the paper can be made via the functions in [src/Figures.cpp](src/Figures.cpp).

## Usage
The code runs whatever is called in `main()` in [src/main.cpp](src/main.cpp). To run, compile by running `make` and then execute `./main`. Then run `python` [py/phi0.py](py/phi0.py) or `python` [py/phi_m_lambda.py](py/phi_m_lambda.py) to generate pdfs.

The primary calculations are in [src/S5.cpp](src/S5.cpp). First, one needs to pick a density profile. Three are provided:
1. `HARD` is a hard sphere (set `g=1` to get the default case here).
2. `SUN` uses the solar density profile from the BS05(OP) Standard Solar Model (SSM) which must be read in via `Read_SSM()` in [src/main.cpp](src/main.cpp) before it can be used.
3. `EARTH` uses the Preliminary Reference Earth Model (PREM), both in units of g/cc.

Additional density profiles can be added in a straightforward fashion as either a function or read in from a file. Next, one needs to calculate &phi;(0) which is done via the `phi0_KG` function (KG refers to Klein-Gordon). This uses a guess from the `phi0_Guess` function and then tries to maximize the distance at which either &phi;'<0 or &phi;>0. This provides a good guess in many cases, but care may be required depending on the density profile and the parameters, especially by changing the window over which to scan, defined by the variables `phi0_lo` and `phi0_hi`.

Once &phi;(0) is calculated, one can easily calculate the value of the field for a range of radii up to `r_max` using the `KG` function. The radii and the field values are returned as arrays as `rs` and `phis`.

Code to create the figures in the paper is also provided in the [src/Figures.cpp](src/Figures.cpp) file which also acts as a useful example file.

## Units
- **Distance**: cm

## Dependencies
This depends on gsl which can be found in `libgsl-dev` on ubuntu.

## Bugs and Features
If any bugs are identified or any features suggested, please use the tools (issues, pull requests, etc.) on github, or contact the creator.

## Reference
If you use this code please reference **[arXiv:2005.xxxxx](https://arxiv.org/abs/2005.xxxxx)** and **[doi:10.5281/zenodo.3826887](https://doi.org/10.5281/zenodo.3826887)**. If you use the Sun's density profile, please reference **[arXiv:astro-ph/0412440](https://arxiv.org/abs/astro-ph/0412440)**, if you use the Earth's density profile, please reference **[DOI:10.1016/0031-9201(81)90046-7](https://doi.org/10.1016/0031-9201(81)90046-7)**.


