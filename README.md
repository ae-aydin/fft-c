# fft-c

A C-based implementation of various Fast Fourier Transform (FFT) algorithms and their applications. 
Includes test cases for 1D and 2D FFT, as well as the calculation of the Fourier magnitude spectrum of a given image.

FFT Algorithms:
* **Cooley-Tukey radix-2** (power-of-two lengths)
* **Cooley-Tukey iterative radix-2** (power-of-two lengths)
* **Discrete Fourier Transform (DFT)**
* **Bluestein's algorithm** (for handling non-power-of-two lengths)

FFT Applications:
* **2D FFT**: Transforming images or 2D signals.
* **FFT Shift**: Shifting the DC component (low frequencies) to the center of the spectrum for better visualization.

## Setup

1. Clone the repository:
    ```bash
    git clone https://github.com/ae-aydin/fft-c.git
    cd fft-c
    ```
   
2. Build the project: Use gcc to compile the source files. A C compiler is required.  
    ```bash
    gcc "*.c" -o fft -lm
    ```
   
3. Run the project:
    ```bash
    fft [FFT1 | FFT2 | FFT_IMAGE] [algorithm | | input_file output_file]
    ```
    * **algorithm**: Choose from [RADIX_2 | ITER_RADIX_2 | DFT | BLUESTEIN].
    * **input_file**: Path of the image for calculating the Fourier magnitude spectrum.
    * **output_file**: Path to save the calculated Fourier magnitude spectrum of the image.
    
**All run cases:**
```bash
fft FFT1 RADIX_2 # Run test case for radix-2
fft FFT1 ITER_RADIX_2 # Run test case for iterative-radix-2
fft FFT1 DFT # Run test case for DFT
fft FFT1 BLUESTEIN # Run test case for Bluestein's algorithm
fft FFT2 # Run test case for FFT2D
fft FFT_IMAGE <input_file> <output_file> # calculate Fourier magnitude transform for given image
```
If you want to modify the test cases, you can change the constants in the `main.c` file.

## References

* [_stb_](https://github.com/nothings/stb), used for image I/O.
* [FFT](https://en.wikipedia.org/wiki/Fast_Fourier_transform)
* [DFT](https://en.wikipedia.org/wiki/Discrete_Fourier_transform)
* [Cooley-Tukey FFT](https://en.wikipedia.org/wiki/Cooley%E2%80%93Tukey_FFT_algorithm)
* [Bluestein's Algorithm](https://en.wikipedia.org/wiki/Chirp_Z-transform#Bluestein.27s_algorithm)
* L. Bluestein, "A linear filtering approach to the computation of discrete Fourier transform," in IEEE Transactions on Audio and Electroacoustics, vol. 18, no. 4, pp. 451-455, December 1970, doi: 10.1109/TAU.1970.1162132. 