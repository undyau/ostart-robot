#!/bin/bash

for (( ; ; ))
do
  if ! pgrep -x "gpsd" > /dev/null
  then
    service gpsd restart
  fi

  # $GPRMC,191749.247,V,,,,,0.17,0.00,031214,,,N*4C
  r=`gpspipe -R | head -10 | grep GPRMC | head -1 | awk -F"," '{print $10" "$2" "$3}'`
  # 031214 191822.247 A

  # Don't set time if dud time from gpspipe (ie GPS not initialised)
  if test "${r:18:1}" != "A"
  then
    sleep 30s
  else
    D="20${r:4:2}-${r:2:2}-${r:0:2} ${r:7:2}:${r:9:2}:${r:11:2}"
    T="${r:13:4}"
    T=".400"
    # I added prefix 20 to year (to get year with 4 digits ex: 2014)
    # set date in UTC format.... -u 
    # Fudge an additional second - by the time we have  the gps output
    # set the clock that's about  right.
    DD=`date -s "$D$T UTC + 1 second"`
    sleep 10m
 fi
done


