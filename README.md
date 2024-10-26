## Intro

A simple tool to manip daily encounters in the Great Marsh, and also win a Master Ball in the Lottery Corner while you're at it.

## Usage

1) Input your information into the program to find a seed. Recommended starting values:
   - Min delay: 600
   - Max delay: 2000
   - Extra RNG frames: 100
2) Use any Gen 4 RNG manip software to find dates and times for that seed, and hit it
3) Verify your seed using the number of coin flips specified by the program
4) Advance the RNG further with Happiness Checker double taps
5) Create a new group by talking to the NPC directly south of the Jubilife TV Station
6) Save your game and then wait for a day to pass (or change your clock to 23:59 and wait one minute)
7) Check that your Lottery Corner number matches
8) Go to the Great Marsh and get your manipped Pokes. Daily Great Marsh Pokes appear in encounter slots 6 and 7.

## Compiling

```
gcc main.c mersenne_twister.c marsh_poke.c -o GreatMarshManipTool.exe -Ofast
```
