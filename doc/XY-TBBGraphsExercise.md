# TBB Tutorial - Scientific Data Processing Exercise

In this final part of the TBB tutorial we shall build up a simple, but failrly
realistic, example of scientific data processing. The exercise is the
following:

- We have a detector that consists of a grid of pixels, 100x100.
 
  - The detector is designed to be sensitive to a new type of long lived
  particle, *the fooble*.
  
- The detector is read out each second and we have a number of timeframes
available to us in which there might be a fooble.

- Foobles are slow moving and the expected signal from a fooble is a large
energy spike in a cluster of pixels, that should last for at least 3 seconds.

![The Fooble Detector](https://graemes.web.cern.ch/graemes/teaching/gridka/fooble-sketch.png)

## Fooble Detector Data Processing

- Read RAW data from `data-input.dat`
  - The data is written in an extremely simple way, equivalent to
```cpp
struct {
    float timestamp;
    float std:array cells[100][100];
}
```
  i.e. a series of 10001 float values written in little endian binary.
  