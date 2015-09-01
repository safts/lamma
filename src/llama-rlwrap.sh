#! /bin/sh
args=
for arg in "$@";
do
  args="$args '$arg'"
done
eval exec ./rlwrap-0.30/src/rlwrap -f ".inputrc" ./llama $args