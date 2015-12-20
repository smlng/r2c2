#!/bin/bash
[ -z "$1" ] && {
  echo "Missing PAN ID!"
  echo "USAGE: $0 <PAN ID> <CHANNEL>"
  exit 1
}

[ -z "$2" ] && {
  echo "Missing channel!"
  echo "USAGE: $0 <PAN ID> <CHANNEL>"
  exit 1
}
PANID=$1
CHANNEL=$2

ip link set wpan0 down
if (ip link show lowpan0 ) ; then
  ip link set lowpan0 down
fi
if (ip link show monitor0 ); then
  iwpan dev monitor0 del
fi
iwpan phy phy0 set channel 0 $CHANNEL
ip link add link wpan0 name lowpan0 type lowpan
iwpan dev wpan0 set pan_id $PANID
ip link set wpan0 up
ip link set lowpan0 up
