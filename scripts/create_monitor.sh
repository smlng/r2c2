#!/bin/bash
if [[ $EUID -ne 0 ]]; then
  echo "You must be a root user" 2>&1
  exit 1
fi

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
  ip link del lowpan0 type lowpan
fi
if (ip link show monitor0 ); then
  ip link set monitor0 down
fi
iwpan dev wpan0 set pan_id $PANID
iwpan phy phy0 interface add monitor%d type monitor
ip link set monitor0 up
iwpan phy phy0 set channel 0 $CHANNEL
