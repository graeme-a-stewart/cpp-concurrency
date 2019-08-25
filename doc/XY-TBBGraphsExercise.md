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
energy spike in a cluster of pixels, that should last for 5 seconds.

![The Fooble Detector](https://graemes.web.cern.ch/graemes/teaching/gridka/fooble-sketch.png)

## Fooble Detector Data Processing

Processing the data from the fooble detector for a time frame does require
some extra steps:

1. Read raw data for a frame.
2. Subtract the detector's pedastal values from each cell (the pedastal is
a constant value per cell, but varies across the detector).
3. Mask hot pixels, where we know that we get a bad signal (in our example
these pixels are fixed for the whole dataset).
4. Search for a possible fooble signal in a 3x3 subset of cells by calculating
the average signal strength:
    1. If the average signal in this cluster exceeds 150 this could be a fooble.

The potential fooble clusters then need to be searched in different time
frames to see if they appear in 5 consecutive frames.

## Thinking About Parallelism

The first step in designing our data processing pipeline is to think about
what opportunies we have for parallelism when processing:

- Think about opportunities for parallelism for each single step above
- Think about opportunities for parallelism between each of the steps
- Think about opportunities for parallelism between different time frames

The goal is always to exploit parallelism at multiple levels!

## Designing A Data Flow Pipeline

Once parallel opportunities have been identified, think about how these could
be expressed in TBB.

### Detector definition 
